import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim
import torch.utils.data
import numpy as np
import random
import argparse
from torch.optim.lr_scheduler import StepLR
import struct


FLOAT_SIZE = 5
base64_str = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"


def trans3(X, T):
    if len(X.shape) != 0:
        for x in X:
            trans3(x, T)
    else:
        (x,) = struct.unpack("I", struct.pack("f", X))
        T2 = []
        for i in range(FLOAT_SIZE):
            T2.append(base64_str[(x >> (32 - 6 * (i + 1))) & 63])
        T.append("".join(T2))


def trans2(X):
    T = []
    trans3(X, T)
    return ",".join(str(i) for i in X.shape) + ":" + str(FLOAT_SIZE) + ":" + "".join(T)

class TorchContext:
    def __init__(self):
        self.ids = {}
        self.count = 0
        self.total_used = {}
        self.script = []
        self.outputs = []
        self.shapes = {}
    def argument(self, a):
        a_id = id(a)
        assert a_id in self.ids
        ret = self.ids[a_id]
        self.total_used[ret] += 1
        return ret
    def whole_result(self, A):
        if not isinstance(A, tuple):
            A = (A, )
        for a in A:
            a_id = id(a)
            assert a_id in self.ids
            ret = self.ids[a_id]
            self.total_used[ret] += 1
            self.outputs.append(ret)
    def result(self, a):
        a_id = id(a)
        self.count += 1
        self.ids[a_id] = self.count
        ret = self.ids[a_id]
        self.total_used[ret] = 0
        self.shapes[ret] = a.shape
        return ret
    def use(self, name, kind, arg_ids, ret_id):
        self.script.append((name, kind, arg_ids, ret_id))
    def calc_shape_cost(self, before, after):
        assert len(before)==len(after)
        ret = 1
        for i in range(len(before)):
            if before[i] < after[i]:
                return None
            ret *= before[i]
        return ret
    def gen(self):
        whole_lines = []
        X = {"x": 1}
        rX = {1: "x"}
        last_shape = {"x": self.shapes[1]}
        used = [0] * self.count
        var_count = 1
        for name, kind, arg_ids, ret_id in self.script:
            var_names = [rX[arg_id] for arg_id in arg_ids]
            shape = self.shapes[ret_id]
            if kind=="BatchNorm2d" or kind=="ReLU":
                assert len(var_names)==1
                free_flag = False
                for arg_id in arg_ids:
                    used[arg_id] += 1
                    if used[arg_id] == self.total_used[arg_id]:
                        X.pop(rX[arg_id])
                        rX.pop(arg_id)
                        free_flag = True
                if free_flag:
                    ret_var_name = var_names[0]
                    assert self.calc_shape_cost(last_shape[ret_var_name], self.shapes[ret_id]) is not None
                else:
                    ret_var_name = None
                    best = None
                    for var_name in last_shape:
                        if var_name in X:
                            continue
                        cost = self.calc_shape_cost(last_shape[var_name], shape)
                        if ret_var_name is None or best is None and cost is not None or cost is not None and cost < best:
                            best = cost
                            ret_var_name = var_name
                    if ret_var_name is None:
                        var_count += 1
                        ret_var_name = "x%d" % (var_count, )
                        assert best is None
                    if best is None:
                        assert len(shape)==4
                        assert shape[0]==1
                        whole_lines.append("        %s[:%d][:%d][:%d] = %s[$1][$2][$3]" % (ret_var_name, shape[2], shape[3], shape[1], var_names[0]))
                        last_shape[ret_var_name] = self.shapes[ret_id]
                whole_lines.append("        %s(%s)" % (name if kind=="BatchNorm2d" else kind, ret_var_name))
            elif kind=="cat":
                free_flag = False
                for arg_id in arg_ids[:1]:
                    used[arg_id] += 1
                    if used[arg_id] == self.total_used[arg_id]:
                        X.pop(rX[arg_id])
                        rX.pop(arg_id)
                        free_flag = True
                lines = []
                if free_flag:
                    ret_var_name = var_names[0]
                    if self.calc_shape_cost(last_shape[ret_var_name], shape) is None:
                        last_shape[ret_var_name] = shape
                else:
                    ret_var_name = None
                    best = None
                    for var_name in last_shape:
                        if var_name in X:
                            continue
                        cost = self.calc_shape_cost(last_shape[var_name], shape)
                        if ret_var_name is None or best is None and cost is not None or cost is not None and cost < best:
                            best = cost
                            ret_var_name = var_name
                    if ret_var_name is None:
                        var_count += 1
                        ret_var_name = "x%d" % (var_count, )
                        assert best is None
                    if best is None:
                        assert len(shape)==4
                        assert shape[0]==1
                        lines.append("        %s[:%d][:%d][:%d] = %s[$1][$2][$3]" % (ret_var_name, shape[2], shape[3], self.shapes[arg_ids[0]][1], var_names[0]))
                        last_shape[ret_var_name] = shape
                sum_shape = self.shapes[arg_ids[0]][1]
                for arg_id in arg_ids[1:]:
                    lines.append("        %s[:%d][:%d][%d:%d] = %s[$1][$2][$3-%d]" % (ret_var_name, shape[2], shape[3], sum_shape, sum_shape+self.shapes[arg_id][1], rX[arg_id], sum_shape))
                    sum_shape += self.shapes[arg_id][1]
                for line in reversed(lines):
                    whole_lines.append(line)
                for arg_id in arg_ids[1:]:
                    used[arg_id] += 1
                    if used[arg_id] == self.total_used[arg_id]:
                        X.pop(rX[arg_id])
                        rX.pop(arg_id)
            else:
                ret_var_name = None
                best = None
                for var_name in last_shape:
                    if var_name in X:
                        continue
                    cost = self.calc_shape_cost(last_shape[var_name], shape)
                    if ret_var_name is None or best is None and cost is not None or cost is not None and cost < best:
                        best = cost
                        ret_var_name = var_name
                if ret_var_name is None:
                    var_count += 1
                    ret_var_name = "x%d" % (var_count, )
                    whole_lines.append("        %s: vector!(vector!(vector!(float)))" % (ret_var_name, ))
                if best is None:
                    assert len(shape)==4
                    assert shape[0]==1
                    whole_lines.append("        %s.resize(%d)" % (ret_var_name, shape[2]))
                    whole_lines.append("        %s[:%d].resize(%d)" % (ret_var_name, shape[2], shape[3]))
                    whole_lines.append("        %s[:%d][:%d].resize(%d)" % (ret_var_name, shape[2], shape[3], shape[1]))
                    last_shape[ret_var_name] = self.shapes[ret_id]
                whole_lines.append("        %s(%s, %s)" % (name if name is not None else kind, ", ".join(var_name for var_name in var_names), ret_var_name))
                for arg_id in arg_ids:
                    used[arg_id] += 1
                    if used[arg_id] == self.total_used[arg_id]:
                        X.pop(rX[arg_id])
                        rX.pop(arg_id)
            rX[ret_id] = ret_var_name
            X[ret_var_name] = ret_id
        #whole_lines.append("        x.resize(%s)" % (shape[2], ))
        #whole_lines.append("        x[:%s].resize(%s)" % (shape[2], shape[3]))
        #whole_lines.append("        x[:%s][:%s].resize(%s)" % (shape[2], shape[3], shape[1]))
        #if len(self.script) and ret_var_name!="x":
        #    if shape[1]!=1:
        #        whole_lines.append("        x[:%s][:%s][:%s] = %s[$1][$2][$3]" % (shape[2], shape[3], shape[1], ret_var_name))
        #    else:
        #        whole_lines.append("        x[:%s][:%s][0] = %s[$1][$2][0]" % (shape[2], shape[3], ret_var_name))
        define_args = []
        for ret_id in self.outputs:
            shape = self.shapes[ret_id]
            whole_lines.append("        %s.resize(%d)" % (rX[ret_id], shape[2]))
            whole_lines.append("        %s[:%d].resize(%d)" % (rX[ret_id], shape[2], shape[3]))
            whole_lines.append("        %s[:%d][:%d].resize(%d)" % (rX[ret_id], shape[2], shape[3], shape[1]))
            define_args.append(", %s:&vector!(vector!(vector!(float)))" % (rX[ret_id], ))
        print("    def __call__(x:&vector!(vector!(vector!(float)))%s):" % ("".join(define_args), ))
        for line in whole_lines:
            print(line)


context = TorchContext()
def cat(A, *args, **kargs):
    global obj_id_count
    arg_ids = [context.argument(a) for a in A]
    ret = torch.cat(A, *args, **kargs)
    ret_id = context.result(ret)
    context.use(None, "cat", arg_ids, ret_id)
    return ret


def hook(origin, module_type, name):
    def func(*args):
        arg_id = context.argument(args[0])
        ret = origin(*args)
        ret_id = context.result(ret)
        context.use(name, module_type, [arg_id], ret_id)
        return ret
    return func


def trans4(prefix, model, D):
    for name0 in model.__dict__["_modules"]:
        name = prefix + name0
        module = model.__dict__["_modules"][name0]
        module_type = None
        if isinstance(module, nn.BatchNorm2d):
            module_type = "BatchNorm2d"
        elif isinstance(module, nn.Conv2d):
            module_type = "Conv2d"
        elif isinstance(module, nn.ConvTranspose2d):
            module_type = "ConvTranspose2d"
        elif isinstance(module, nn.ReLU):
            module_type = "ReLU"
        else:
            trans4(name + "_", module, D)
        #if name in D:
        #    print(module_type, name, D[name])
        #else:
        #    print(module_type, name)
        if module_type:
            model.__dict__["_modules"][name0] = hook(module, module_type, name)

def trans(model_name, model, dummy_input, state_dict):
    global context
    context = TorchContext()
    context.result(dummy_input)
    context.argument(dummy_input)
    D = {}
    for K in state_dict:
        Ks = K.split(".")
        K2 = "_".join(Ks[:-1])
        if K2 not in D:
            D[K2] = (Ks[-2], {})
        D[K2][1][Ks[-1]] = state_dict[K]
    model.eval()
    trans4("", model, D)
    with torch.no_grad():
        whole_result = model(dummy_input)
        context.whole_result(whole_result)
    print("class %s:" % (model_name, ))
    for K in D:
        name, sd = D[K]
        if name.startswith("bn"):
            sz = sd["weight"].shape[0]
            bn = nn.BatchNorm2d(sz).eval()
            bn.load_state_dict(sd)
            with torch.no_grad():
                s0 = bn(torch.zeros((1, sz, 1, 1)))[0, :, 0, 0]
                s1 = bn(torch.ones((1, sz, 1, 1)))[0, :, 0, 0]
                s2 = bn(torch.zeros((1, sz, 1, 1)) + 2)[0, :, 0, 0]
            print("    %s: BatchNorm2d" % (K, ))
            print('    %s.init("%s", "%s")' % (K, trans2(s0), trans2(s1 - s0)))
            pass
        elif name.startswith("conv"):
            if K.startswith("dec") and name == "conv2":
                print("    %s: ConvTranspose2d" % (K, ))
                print('    %s.init("%s", "%s")' % (K, trans2(sd["bias"]), trans2(sd["weight"])))
            else:
                print("    %s: Conv2d" % (K, ))
                print('    %s.init("%s", "%s")' % (K, trans2(sd["bias"]), trans2(sd["weight"])))
    context.gen()

class Enc(nn.Module):
    def __init__(self, in_channels, inner_channels, out_channels):
        super().__init__()
        self.bn1 = nn.BatchNorm2d(in_channels)
        self.af1 = nn.ReLU()
        self.conv1 = nn.Conv2d(in_channels, inner_channels, 3, 1, 1)
        self.bn2 = nn.BatchNorm2d(inner_channels)
        self.af2 = nn.ReLU()
        self.conv2 = nn.Conv2d(inner_channels, out_channels, 4, 2, 1)

    def forward(self, x):
        x = self.bn1(x)
        x = self.af1(x)
        x = self.conv1(x)
        x = self.bn2(x)
        x = self.af2(x)
        x = self.conv2(x)
        return x


class Dec(nn.Module):
    def __init__(self, in_channels, inner_channels, out_channels):
        super().__init__()
        self.bn1 = nn.BatchNorm2d(in_channels)
        self.af1 = nn.ReLU()
        self.conv1 = nn.Conv2d(in_channels, inner_channels, 3, 1, 1)
        self.bn2 = nn.BatchNorm2d(inner_channels)
        self.af2 = nn.ReLU()
        self.conv2 = nn.ConvTranspose2d(inner_channels, out_channels, 4, 2, 1)

    def forward(self, x):
        x = self.bn1(x)
        x = self.af1(x)
        x = self.conv1(x)
        x = self.bn2(x)
        x = self.af2(x)
        x = self.conv2(x)
        return x


class Net(nn.Module):
    def __init__(self):
        super().__init__()
        self.conv1 = nn.Conv2d(3, 4, 3, 1, 1)
        self.enc1 = Enc(4, 4, 4)
        self.enc2 = Enc(4, 4, 4)
        self.enc3 = Enc(4, 4, 4)
        self.enc4 = Enc(4, 4, 4)
        self.enc5 = Enc(4, 4, 4)
        self.dec5 = Dec(4, 4, 4)
        self.dec4 = Dec(8, 4, 4)
        self.dec3 = Dec(8, 4, 4)
        self.dec2 = Dec(8, 4, 4)
        self.dec1 = Dec(8, 4, 4)
        self.bn2 = nn.BatchNorm2d(8)
        self.af2 = nn.ReLU()
        self.conv2 = nn.Conv2d(8, 1, 3, 1, 1)

    def forward(self, x0):
        x0 = self.conv1(x0)
        x1 = self.enc1(x0)
        x2 = self.enc2(x1)
        x3 = self.enc3(x2)
        x4 = self.enc4(x3)
        x = self.enc5(x4)
        x = self.dec5(x)
        x = self.dec4(cat((x, x4), dim=1))
        del x4
        x = self.dec3(cat((x, x3), dim=1))
        del x3
        x = self.dec2(cat((x, x2), dim=1))
        del x2
        x = self.dec1(cat((x, x1), dim=1))
        del x1
        x = cat((x, x0), dim=1)
        del x0
        x = self.bn2(x)
        x = self.af2(x)
        x = self.conv2(x)
        return x

trans("UNetC", Net(), torch.zeros((1, 3, 64, 64)), torch.load("modelC1.pt", map_location=torch.device("cpu")))
print()
trans("UNetD", Net(), torch.zeros((1, 3, 64, 64)), torch.load("modelD1B.pt", map_location=torch.device("cpu")))
