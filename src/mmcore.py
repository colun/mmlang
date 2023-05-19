import os
import re
from . import lib
from . import mmbase

SRC_DIR = os.path.dirname(__file__)
ROOT_DIR = os.path.dirname(SRC_DIR)
LIB_DIR = os.path.join(ROOT_DIR, 'lib')


def restruct(obj, new_class, *args, **kargs):
    if '__del__' in dir(obj):
        obj.__del__()
    obj.__class__ = new_class
    obj.__dict__.clear()
    obj.__init__(*args, **kargs)


def typeToStr(ty):
    if ty is None:
        return 'int'
    assert not isinstance(ty, tuple)
    if isinstance(ty, tuple):
        return ('tuple<%s>' % (', '.join([typeToStr(t) for t in ty]), )).replace('>>', '> >')
    return str(ty)


def typeFromLib(name):
    if name is not None:
        if isinstance(name, tuple):
            return Template('tuple', [typeFromLib(n) for n in name])
        m = re.match(r'([^<]+)<(.+)>$', name)
        if m:
            return Template(m.group(1), [typeFromLib(m.group(2))])
    return name


def castToStr(expr, ty):
    ty2 = expr.getType()
    if ty == ty2:
        return str(expr)
    else:
        return '(%s)(%s)' % (typeToStr(ty), expr)


def toParenthesisStr(expr):
    if (isinstance(expr, str) or isinstance(expr, node_parenthesis) or isinstance(expr, const_node) or isinstance(expr, node_var)
            or isinstance(expr, tuple_node) or isinstance(expr, node_ref) or isinstance(expr, call_node) or isinstance(expr, post_op1_node)):
        return str(expr)
    return '(%s)' % (expr, )


class Template:

    def __init__(self, name, args):
        self.name = name
        self.args = args

    def __str__(self):
        return ('%s<%s>' % (self.name, ', '.join(typeToStr(arg) for arg in self.args))).replace('>>', '> >')

    def __eq__(self, o):
        if not isinstance(o, Template) or self.name != o.name or len(self.args) != len(o.args):
            return False
        for a, b in zip(self.args, o.args):
            if not(a == b):
                return False
        return True

    def itemType(self):
        if self.name in ['vector', 'deque', 'list', 'stack', 'queue', 'set', 'multiset', 'xvector']:
            return self.args[0]
        return None


def testToStr(expr):
    ty = expr.getType()
    if isinstance(ty, Template) and ty.name in ['vector', 'deque', 'list', 'stack', 'queue', 'priority_queue', 'PriorityQueue', 'fast_pque', 'fast_pque_vk', 'set_pque', 'xpque', 'set', 'multiset']:
        return '!%s.empty()' % (expr, )
    return str(expr)


def itemType(ty):
    if isinstance(ty, Template):
        return ty.itemType()
    if ty == 'string':
        return 'char'
    print('itemType', ty)
    TODO


class base_node:

    def __str__(self):
        if hasattr(self, 'bindStr'):
            assert self.bindStr is not None
            assert isinstance(self.bindStr, str)
            return self.bindStr
        return self.toStr()

    def reserveBind(self, name=None):
        assert not hasattr(self, 'bindStr') or self.bindStr is None
        assert name is None or isinstance(name, str)
        if hasattr(self, 'bindName'):
            assert name is None or self.bindName is None
            if self.bindName is None:
                self.bindName = name
        else:
            self.bindName = name
        self.bindStr = None

    def visitExecBind(self):
        if hasattr(self, 'bindName') and self.bindStr is None:
            self.bindStr = context.nameInstant(self, self.bindName)

    def isAggregateFunc(self):
        return False

    def visitUseBind(self):
        if hasattr(self, 'bindStr') and self.bindStr is not None:
            def func():
                context.instantVar(self, self.bindStr)
            return 'aggregate' if self.isAggregateFunc() else 'instant', func

    def visit(self, method, beforeMethod=None, childrenMethods=None, reduce=None):
        if beforeMethod is not None and hasattr(self, beforeMethod):
            getattr(self, beforeMethod)()
        children = []
        if childrenMethods is None:
            childrenMethods = []
        elif not isinstance(childrenMethods, list):
            childrenMethods = [childrenMethods]
        for childrenMethod in childrenMethods:
            if hasattr(self, childrenMethod):
                children = getattr(self, childrenMethod)()
                break
        else:
            children = self.getChildNodes()
        rets = []
        for child in children:
            if isinstance(child, base_node):
                rets.append(child.visit(method, beforeMethod, childrenMethods, reduce))
        ret = None
        if method is not None and hasattr(self, method):
            ret = getattr(self, method)()
        if reduce is not None:
            return reduce(ret, rets)

    def visitInit(self):
        self.parent = None

    def visitSetParent(self):
        for child in self.getChildNodes():
            if isinstance(child, base_node):
                child.parent = self

    def printResize(self, size, name):
        return []

    def isConstExpr(self):
        return False


class node_if_suffix(base_node):

    def __init__(self, meta, node_type, body, test):
        self.meta = meta
        self.node_type = node_type
        self.body = body
        self.test = test

    def getChildNodes(self):
        return [self.body, self.test]

    def getType(self):
        return self.body.getType()


def isComprehensionType(ty0, ty1):
    if ty1 is None or ty0 == ty1:
        return True
    assert not isinstance(ty0, tuple)
    assert not isinstance(ty1, tuple)
    if isinstance(ty0, Template):
        return True
    if ty0 != 'auto' and ty1 == 'auto':
        return True
    if ty0 in ['double', 'float64'] and ty1 in ['double', 'float', 'float64', 'float32', 'unsigned long long', 'long long', 'int64', 'size_t', 'int', 'short', 'char', 'bool']:
        return True
    if ty0 in ['float', 'float32'] and ty1 in ['float', 'float32', 'unsigned long long', 'long long', 'int64', 'size_t', 'int', 'short', 'char', 'bool']:
        return True
    if ty0 in ['__int128', 'int128'] and ty1 in ['__int128', 'int128', 'unsigned long long', 'uint64', 'long long', 'int64', 'size_t', 'int', 'short', 'char', 'bool']:
        return True
    if ty0 in ['unsigned long long', 'uint64', 'long long', 'int64', 'size_t'] and ty1 in ['unsigned long long', 'uint64', 'long long', 'int64', 'size_t', 'int', 'short', 'char', 'bool']:
        return True
    if ty0 == 'int' and ty1 in ['int', 'short', 'char', 'bool']:
        return True
    if ty0 == 'short' and ty1 in ['short', 'char', 'bool']:
        return True
    if ty0 == 'char' and ty1 in ['char', 'bool']:
        return True
    return False


def comprehensionType(ty0, ty1):
    # if isinstance(ty0, tuple) and isinstance(ty1, tuple) and len(ty0) == len(ty1):
    if isinstance(ty0, Template) and ty0.name == 'tuple' and isinstance(ty1, Template) and ty1.name == 'tuple' and len(ty0.args) == len(ty1.args):
        return Template('tuple', [comprehensionType(u, v) for u, v in zip(ty0.args, ty1.args)])
        # return tuple(comprehensionType(u, v) for u, v in zip(ty0, ty1))
    if isComprehensionType(ty0, ty1):
        return ty0
    elif isComprehensionType(ty1, ty0):
        return ty1
    else:
        print(ty0, ty1, type(ty0), type(ty1))
        TODO


def hintType(name, ty0, ty1, op):
    # TODO: add containertypes
    if isinstance(ty0, Template) and isinstance(ty1, Template) and ty0.name == ty1.name and len(ty0.args) == 1 and len(ty1.args) == 1 and ty0.name in ['vector', 'deque']:
        return Template(ty0.name, [hintType('%s[]' % name, ty0.args[0], ty1.args[0], op)])
    if op in ['+=', '-='] and ty0 in ['set', 'multiset']:
        return Template(ty0, [ty1])
    elif op in ['=', '+=', '-=', '|=', '&=', '^=', '/=', '<?=', '>?='] or op in ['*='] and ty1 in ['double']:
        return comprehensionType(ty0, ty1)
    elif op in ['*='] and ty1 in ['int']:
        return ty0
    else:
        print(name, ty0, ty1, op)
        TODO


def getMinMaxValueExpr(ty, upper_flag):
    LOWER = {
        'char': '-128',
        'short': '-32768',
        'int': '-2147483648',
        'long long': '-9223372036854775808LL',
        'double': '-DBL_MAX',
    }
    UPPER = {
        'char': '127',
        'short': '32767',
        'int': '2147483647',
        'long long': '9223372036854775807LL',
        'double': 'DBL_MAX',
    }
    if str(ty) in ['double']:
        context.appendInclude('cfloat')
    if isinstance(ty, Template) and ty.name=='tuple':
        return '%s(%s)' % (ty, ', '.join([getMinMaxValueExpr(arg, upper_flag) for arg in ty.args]))
    return UPPER[str(ty)] if upper_flag else LOWER[str(ty)]


def toStrBoundArg(containerTy, args, upper_flag):
    itemTy = itemType(containerTy)
    while len(args) == 1 and isinstance(args[0], tuple_node) and args[0].node_type == 'tuple':
        args = args[0].elements
    tysArgs = tuple(arg.getType() for arg in args)
    assert not isinstance(itemTy, tuple)
    if not (isinstance(itemTy, Template) and itemTy.name == 'tuple'):
        assert len(args) <= 1
        return str(args[0]) if len(args) == 1 else D[itemTy]
    if len(args) == 1 and tysArgs[0] == itemTy:
        return args[0]
    if len(args) == 1 and isinstance(tysArgs[0], Template) and tysArgs[0].name == 'tuple':
        exprs = []
        for i in range(len(itemTy.args)):
            if i < len(tysArgs[0].args):
                exprs.append('get<%s>(%s)' % (i, args[0]))
            else:
                exprs.append(getMinMaxValueExpr(itemTy.args[i], upper_flag))
        return '%s(%s)' % (typeToStr(itemTy), ', '.join(exprs))
    exprs = []
    for i in range(len(itemTy.args)):
        if i < len(tysArgs):
            exprs.append(str(args[i]))
        else:
            exprs.append(getMinMaxValueExpr(itemTy.args[i], upper_flag))
    return '%s(%s)' % (typeToStr(itemTy), ', '.join(exprs))


use_profiler = False
use_pybind11 = False


class CodeContext:

    def __init__(self):
        self._indent = 0
        self.tabed = False

        self.used_includes = ['cassert', 'vector', 'deque', 'string', 'map', 'queue', 'stack', 'algorithm', 'type.h']
        self.breaks = []
        self.breaks_used = []
        self.break_label_index = 0
        self.blanked = True
        self.print_lines = []

    def getPrintLines(self):
        return self.print_lines

    def getUsedIncludes(self):
        return self.used_includes

    def print0(self, text):
        if not self.tabed:
            self.print_lines.append('    ' * self._indent)
            self.tabed = True
        self.print_lines.append(text)
        self.blanked = False

    def print(self, text=''):
        if not self.tabed:
            self.print_lines.append('    ' * self._indent)
        self.print_lines.append(text)
        self.print_lines.append("\n")
        self.tabed = False
        self.blanked = False

    def printBlank(self):
        if not self.blanked:
            self.print_lines.append("\n")
        self.blanked = True

    def indent(self):
        self._indent += 1

    def dedent(self):
        self._indent -= 1

    def appendInclude(self, inc, top=False):
        if inc is None or inc in self.used_includes:
            return
        if inc in lib.dependency:
            for inc2 in lib.dependency[inc]:
                self.appendInclude(inc2)
        if top:
            self.used_includes.insert(0, inc)
        else:
            self.used_includes.append(inc)

    def visitInclude(self, name):
        if name in lib.func_definition:
            self.appendInclude(lib.func_definition[name][1])

    def visitTypeInclude(self, name):
        if name in lib.class_definition:
            self.appendInclude(lib.class_definition[name][1])

    def get_break(self):
        assert self.breaks[-1], 'break可能な場所でbreakしてください'
        self.breaks_used[-1] += 1
        return self.breaks[-1]

    def get_continue(self):
        if self.breaks[-1]:
            return 'goto %sc' % (self.breaks[-1], )
        else:
            return 'return'

    def get_break_used(self):
        return self.breaks_used[-1]

    def push_banbreak(self):
        self.breaks.append(None)
        self.breaks_used.append(0)

    def push_break(self):
        self.break_label_index += 1
        ret = '$L%s' % (self.break_label_index, )
        self.breaks.append(ret)
        self.breaks_used.append(0)
        return ret

    def pop_break(self):
        self.breaks.pop()
        self.breaks_used.pop()


class Context:

    def __init__(self, parent=None, scopeLevel=0):
        self.code_context = CodeContext() if parent is None else parent.code_context
        self.is_main = (parent is None) if scopeLevel==0 else parent.is_main
        self.pybind11_funcs = [] if parent is None else None
        self.instant = []
        self.instant_l_count = 0
        self.instant_c_count = 0
        self.parent = parent
        self.scopeLevel = scopeLevel
        self.func_types = {}
        self.definitions = {}
        self.temporaryDefinitions = {}
        self.removeBook = []
        self.removeBookStack = []
        self.instant_symbols_stack = []
        if parent is None:
            self.symbols, self.cpp_symbols = mmbase.initSymbols()
        else:
            self.symbols, self.cpp_symbols = {}, set()

    def getSymbol(self, name):
        assert isinstance(name, str)
        return name
        for instant_symbols in self.instant_symbols_stack:
            if name in instant_symbols:
                return self.instant_symbols[name]
        if name in self.symbols:
            return self.symbols[name]
        if self.parent is not None:
            return self.parent.getSymbol(name)
        assert False
        return name

    def generateCppName(self, name):
        for i in range(1, 10000):
            name2 = name if i==1 else f'{name}$${i}'
            if name2 not in self.cpp_symbols:
                self.cpp_symbols.add(name2)
                return name2
        else:
            assert False

    def getScope0(self):
        if self.scopeLevel==0:
            return self
        return self.parent.getScope0()

    def getScope1(self):
        if self.scopeLevel<=1:
            return self
        return self.parent.getScope1()

    def getScope2(self):
        if self.scopeLevel<=2:
            return self
        return self.parent.getScope2()

    def nameLoop(self, name):
        assert name is None or isinstance(name, str)
        self.instant_l_count += 1
        if name is None:
            name = '$%d' % (self.instant_l_count, )
        else:
            self.defineTemporaryDefinition(name, 'int')
        return name

    def reserveLoop(self, slice, name):
        assert isinstance(name, str)
        self.instant.append(('loop', name, slice))

    def nameInstant(self, expr, name=None):
        assert name is None or isinstance(name, str)
        if name is None:
            self.instant_c_count += 1
            name = '$c%d' % (self.instant_c_count, )
        else:
            self.defineTemporaryDefinition(name, expr.getType())
        return name

    def instantVar(self, expr, name):
        assert isinstance(name, str)
        self.instant.append(('instant', name, expr))

    def getInstant(self):
        ret = self.instant
        self.instant = []
        self.instant_l_count = 0
        self.instant_c_count = 0
        return ret

    def declareFunc(self, name, ty):
        #print('FUNC', name, ty)#TODO
        self.func_types[name] = ty

    def getReturnType(self, name):
        if name in self.func_types:
            return self.func_types[name]
        if self.parent is not None:
            return self.parent.getReturnType(name)
        if name in lib.func_definition:
            return typeFromLib(lib.func_definition[name][0])
        assert False, "function '%s' is not defined" % name

    def getMethodReturnType(self, ty, name):
        if isinstance(ty, Template):
            ty_name = ty.name
        else:
            assert isinstance(ty, str)
            ty_name = ty
        if ty_name not in lib.class_definition:
            print(context.definitions)
        method_dic = lib.class_definition[ty_name][0]
        ret_type = method_dic[name]
        m = re.match(r'\$(\d+)$', ret_type)
        if m:
            n = int(m.group(1)) - 1
            assert isinstance(ty, Template)
            ret_type = ty.args[n]
        return ret_type

    def hasMethod(self, ty, name):
        if isinstance(ty, Template):
            ty_name = ty.name
        else:
            assert isinstance(ty, str)
            ty_name = ty
        if ty_name not in lib.class_definition:
            return False
        return name in lib.class_definition[ty_name][0]

    def isDefined(self, name):
        assert isinstance(name, str)
        if name in self.definitions:
            return True
        if self.parent is not None:
            return self.parent.isDefined(name)
        return False

    def getDefinition(self, name):
        if name in self.definitions:
            return self.definitions[name]
        elif self.parent is not None:
            return self.parent.getDefinition(name)
        assert False

    def definesParam(self, name, ty):
        assert isinstance(name, str)
        assert name not in self.definitions
        self.definitions[name] = [ty, 1, 0, False, None]
        self.symbols[name] = self.generateCppName(name)

    def defines(self, name):
        assert isinstance(name, str)
        if not self.isDefined(name):
            self.definitions[name] = [None, 0, 1, True, None]
            self.symbols[name] = self.generateCppName(name)

    def definesFor(self, name):
        assert isinstance(name, str)
        assert name not in self.symbols
        self.symbols[name] = self.generateCppName(name)

    def definesWithoutHint(self, name):
        assert isinstance(name, str)
        if name in self.definitions:
            print(name, self.definitions)
            assert name not in self.definitions
        self.definitions[name] = [None, 0, 1, True, None]
        self.symbols[name] = self.generateCppName(name)

    def definesWithHint(self, name, ty):
        assert isinstance(name, str)
        if name in self.definitions:
            print(name, self.definitions)
            assert name not in self.definitions
        self.definitions[name] = [ty, 0, 1, False, None]
        self.symbols[name] = self.generateCppName(name)

    def hintType(self, name, ty, op):
        if self.isDefined(name):
            dObj = self.getDefinition(name)
            if dObj[4] is None:
                dObj[4] = ty if isinstance(ty, base_node) else False
            else:
                dObj[4] = False
            if not dObj[3]:
                return
            ty1 = ty.getType() if isinstance(ty, base_node) else ty
            dObj[0] = hintType(name, dObj[0], ty1, op)

    def countMutate(self, name):
        if self.isDefined(name):
            dObj = self.getDefinition(name)
            dObj[1] += 1

    def setImmutable(self, name):
        if self.isDefined(name):
            dObj = self.getDefinition(name)
            dObj[2] = 2

    def getDefinitions(self):
        return [(key, self.definitions[key][0], self.definitions[key][1], self.definitions[key][2], self.definitions[key][4]) for key in self.definitions]

    def defineTemporaryDefinition(self, name, ty):
        if name not in self.temporaryDefinitions:
            self.temporaryDefinitions[name] = []
        self.temporaryDefinitions[name].append(ty)
        self.removeBook.append(name)

    def enterTemporaryDefinition(self):
        self.removeBookStack.append(self.removeBook)
        self.removeBook = []

    def leaveTemporaryDefinition(self):
        self.releaseTemporaryDefinition()
        assert len(self.removeBook) == 0
        self.removeBook = self.removeBookStack.pop()

    def releaseTemporaryDefinition(self):
        for name in self.removeBook:
            self.temporaryDefinitions[name].pop()
        self.removeBook = []
        self.instant_symbols_stack = []

    def pushInstantSymbols(self):
        self.instant_symbols_stack.append({})

    def popInstantSymbols(self):
        self.instant_symbols_stack.pop()

    def registerInstantSymbol(self, name, info):
        assert name not in self.instant_symbols_stack[-1]
        self.instant_symbols_stack[-1][name] = info

    def getType(self, name):
        if name in self.temporaryDefinitions and 1 <= len(self.temporaryDefinitions[name]):
            return self.temporaryDefinitions[name][-1]
        if name in self.definitions:
            ty = self.definitions[name][0]
            if not isinstance(ty, base_node):
                return ty
            self.definitions[name][0] = None
            ret = ty.getType()
            self.definitions[name][0] = ty
            return ret
        if self.parent is not None:
            return self.parent.getType(name)
        return 'int'
        assert False

    def getPrintLines(self, *args, **kargs):
        return self.code_context.getPrintLines(*args, **kargs)

    def getUsedIncludes(self, *args, **kargs):
        return self.code_context.getUsedIncludes(*args, **kargs)

    def print0(self, *args, **kargs):
        return self.code_context.print0(*args, **kargs)

    def print(self, *args, **kargs):
        return self.code_context.print(*args, **kargs)

    def printBlank(self, *args, **kargs):
        return self.code_context.printBlank(*args, **kargs)

    def indent(self, *args, **kargs):
        return self.code_context.indent(*args, **kargs)

    def dedent(self, *args, **kargs):
        return self.code_context.dedent(*args, **kargs)

    def appendInclude(self, *args, **kargs):
        return self.code_context.appendInclude(*args, **kargs)

    def visitInclude(self, *args, **kargs):
        return self.code_context.visitInclude(*args, **kargs)

    def visitTypeInclude(self, *args, **kargs):
        return self.code_context.visitTypeInclude(*args, **kargs)

    def get_break(self, *args, **kargs):
        return self.code_context.get_break(*args, **kargs)

    def get_continue(self, *args, **kargs):
        return self.code_context.get_continue(*args, **kargs)

    def get_break_used(self, *args, **kargs):
        return self.code_context.get_break_used(*args, **kargs)

    def push_break(self, *args, **kargs):
        return self.code_context.push_break(*args, **kargs)

    def push_banbreak(self, *args, **kargs):
        return self.code_context.push_banbreak(*args, **kargs)

    def pop_break(self, *args, **kargs):
        return self.code_context.pop_break(*args, **kargs)

    def add_pybind11(self, func):
        if self.parent is not None:
            self.parent.add_pybind11(func)
        else:
            self.pybind11_funcs.append(func)


context = None


def expand_minus(x):
    minus_flag = False
    while isinstance(x, pre_op1_node) and x.node_type == 'pre_minus':
        minus_flag = not minus_flag
        x = x.child_node
    return x, minus_flag


def compress(src):
    ret = []
    prefix = ''
    nospace_chara = ',)]};*?{([=&:-+|<.'
    for line in src.split("\n"):
        pos = line.find('//')
        if 0 <= pos:
            line = line[:pos]
        if re.match(r'\s*\#', line):
            ret.append("\n")
            ret.append(line)
            prefix = "\n"
        elif line:
            ret.append(prefix)
            ret.append(line)
            prefix = ' '  # '' if line[-1] in nospace_chara else ' '
    if prefix == "\n":
        ret.append(prefix)
    src = ''.join(ret)
    ret = []
    for line in src.split("\n"):
        str_mode = False
        line_char_count = 0
        tokens = []
        for token in line.split('"'):
            if str_mode:
                tokens.append(token)
                line_char_count += len(token) + 1
                count = 0
                while count < len(token) and token[-1 - count] == "\\":
                    count += 1
                str_mode = count % 2 == 1
            else:
                words = []
                prefix = ''
                pre_word_is_quote = False
                for word in token.split(' '):
                    if word:
                        if pre_word_is_quote and word[0] == "'":
                            words.append(' ')
                            line_char_count += 1
                        elif 250 <= line_char_count:
                            words.append("\n")
                            line_char_count = 0
                        elif word[0] not in nospace_chara:
                            words.append(prefix)
                            line_char_count += len(prefix)
                        words.append(word)
                        line_char_count += len(word)
                        prefix = '' if word[-1] in nospace_chara else ' '
                        pre_word_is_quote = word[-1] == "'"
                tokens.append(''.join(words))
                str_mode = True
                line_char_count += 1
        assert str_mode
        ret.append('"'.join(tokens))
    return "\n".join(ret)


class node_multi_stmt(base_node):

    def __init__(self, meta, node_type, *stmts):
        self.meta = meta
        self.node_type = node_type
        self.stmts = stmts


def visitTopologicalUseBind(node):
    def reduce(main, subs):
        rets = []
        for sub in subs:
            sub_m, sub_children = sub
            if sub_m is None:
                rets.extend(sub_children)
            else:
                rets.append(sub)
        return (main, rets)

    def visit(obj):
        posts = []
        for sub in obj[1]:
            ret = visit(sub)
            if ret:
                posts.append(ret)
        if 1 <= len(posts) or obj[0] is not None and obj[0][0] == 'aggregate':
            return (obj[0], posts)
        elif obj[0] is not None:
            obj[0][1]()
        return False

    def visit2(obj):
        for sub in obj[1]:
            visit2(sub)
        if obj[0] is not None:
            obj[0][1]()
    ret = node.visit('visitUseBind', childrenMethods=['getChildNodesForBind', 'getChildNodesWithoutSuite'], reduce=reduce)
    ret = visit(ret)
    if ret:
        visit2(ret)


constexpr_types = ['bool', 'char', 'short', 'int', 'long long', 'double']


class group_node(base_node):

    def __init__(self, meta, node_type, *stmts):
        self.meta = meta
        self.node_type = node_type
        self.stmts = []
        for stmt in stmts:
            if isinstance(stmt, node_multi_stmt):
                for s in stmt.stmts:
                    self.stmts.append(s)
            else:
                self.stmts.append(stmt)

    def toStr(self):
        return str([str(stmt) for stmt in self.stmts])

    def getChildNodes(self):
        return self.stmts

    def printDefines(self):
        global context
        preContext = context
        context = self.context
        for name, ty, count, param_flag, node in context.getDefinitions():
            if isinstance(node, base_node) and count == 1 and node.isConstExpr() and typeToStr(ty) in constexpr_types:
                context.print('constexpr %s %s = %s;' % (typeToStr(ty), name, node))
                context.setImmutable(name)
                continue
            if param_flag == 1:
                context.print('%s %s;' % (typeToStr(ty), name))
        context = preContext

    def start(self, source_name, profiler_flag, pybind11_flag):
        global context
        global use_profiler
        global use_pybind11
        context = Context(scopeLevel=0)
        use_profiler = profiler_flag
        use_pybind11 = pybind11_flag
        if use_profiler:
            context.appendInclude('profiler.h')
        self.visit('visitInit')
        self.visit('visitSetParent')
        self.visit('visitRestruct')
        self.visit('visitInclude')
        assert self.node_type == 'start'
        self.defines()
        self.visit('visitDeclare')
        self.redefines()
        self.printDefines()
        context.printBlank()
        self.visit('visitDeclare2')
        context.printBlank()
        self.visit('visitDefineFunc')
        context.printBlank()
        if use_pybind11:
            context.appendInclude('pybind11/pybind11.h')
            context.appendInclude('pybind11/stl.h')
            context.print('PYBIND11_MODULE(%s, mm$pybind11) {' % (source_name, ))
        else:
            context.print('int main(int mm$argc, const char * * mm$argv) {')
        context.indent()
        if use_pybind11:
            for func_name in context.pybind11_funcs:
                context.print('mm$pybind11.def("%s", &%s);' % (func_name, func_name))
        else:
            context.appendInclude('init_params.h')
            context.print('mm$initParams(mm$argc, mm$argv);')
        self.suite()
        if not use_pybind11:
            context.print('return 0;')
        context.dedent()
        context.print('}')
        for inc in context.getUsedIncludes():
            if not os.path.exists('%s/%s' % (LIB_DIR, inc)):
                print('#include <%s>' % (inc, ))
        print()
        print('using namespace std;')
        print()
        print('// library code :')
        print()
        src_list = []
        for inc in context.getUsedIncludes():
            if os.path.exists('%s/%s' % (LIB_DIR, inc)):
                src = open('%s/%s' % (LIB_DIR, inc)).read().rstrip() + "\n"
                if inc in lib.nocompress:
                    if len(src_list):
                        print(compress("\n".join(src_list)).rstrip())
                        src_list = []
                    print(src)
                else:
                    src_list.append(src)
        if len(src_list):
            print(compress("\n".join(src_list)).rstrip())
            src_list = []
        print()
        print('// generated code ( by mmlang ... https://github.com/colun/mmlang ) :')
        print()
        print(''.join(context.getPrintLines()), end='')
        context = None

    def defines(self):
        global context
        self.context = context
        for child in self.getChildNodes():
            if isinstance(child, node_if_stmt) or isinstance(child, node_while_stmt):
                for condition in child.getChildNodesWithoutSuite():
                    condition.visit('visitBindSymbol')
                for suite in child.getSuiteChildNodes():
                    parentContext = context
                    context = Context(parentContext, scopeLevel=1)
                    suite.defines()
                    context = parentContext
            elif isinstance(child, node_for_stmt):
                for decorator in child.decorators:
                    decorator.visit('visitBindSymbol')
                child.container.visit('visitBindSymbol')
                parentContext = context
                context = Context(parentContext, scopeLevel=1)
                child.temporary.definesFor()
                child.suite.defines()
                if child.else_suite is not None:
                    context = Context(parentContext, scopeLevel=1)
                    child.else_suite.defines()
                context = parentContext
            elif isinstance(child, node_funcdef):
                child.visit('visitBindSymbol', childrenMethods='getChildNodesWithoutSuite')
                parentContext = context
                context = Context(parentContext, scopeLevel=0)
                child.defines()
                context = parentContext
            elif isinstance(child, op2_node) and child.node_type == 'assign':
                # 値の代入を確認（スコープの確認！）
                args_len = (len(child.args) - 1) // 2
                if child.mod:
                    child.mod.visit('visitBindSymbol')
                child.args[-1].visit('visitBindSymbol')
                for i in range(args_len - 1, -1, -1):
                    child.args[i * 2].defines()
            elif isinstance(child, node_varhint):
                child.defines()
            elif isinstance(child, node_tuple) and child.node_type == 'tuple' and sum(not isinstance(e, node_varhint) for e in child.elements) == 0:
                for child2 in child.elements:
                    assert isinstance(child2, node_varhint)
                    child2.defines()
            else:
                child.visit('visitBindSymbol')

    def redefines(self):
        global context
        parentContext = context
        context = self.context
        for child in self.getChildNodes():
            if isinstance(child, node_if_stmt) or isinstance(child, node_for_stmt) or isinstance(child, node_while_stmt) or isinstance(child, node_funcdef):
                for suite in child.getSuiteChildNodes():
                    suite.redefines()
            elif isinstance(child, node_funcdef):
                child.suite.redefines()
            elif isinstance(child, op2_node) and child.node_type == 'assign':
                # 値の再代入を確認（+=や*=なども含めて）（型に対してやや緩やかになるため）
                args_len = (len(child.args) - 1) // 2
                flag = True
                for i in range(args_len - 1, -1, -1):
                    if flag:
                        child.args[i * 2].hintType(child.args[args_len * 2], child.args[i * 2 + 1])
                        if child.args[i * 2 + 1] != '=':
                            flag = False
                    child.args[i * 2].countMutate()
            elif (isinstance(child, pre_op1_node) or isinstance(child, post_op1_node)) and child.node_type in ['pre_inc', 'pre_dec', 'post_inc', 'post_dec']:
                child.child_node.countMutate()
        context = parentContext

    def printContinueLabel(self, label):
        context.print('if(false) {')
        context.indent()
        context.print('%sc: continue;' % (label, ))
        context.dedent()
        context.print('}')

    def closeNest(self, nest_count):
        for i in range(nest_count):
            context.dedent()
            context.print('}')

    def processInstant(self, child=None):
        instant = context.getInstant()
        nest_count = 0
        nest_flag = False
        for kind, name, info in instant:
            if kind == 'instant':
                if not nest_flag:
                    nest_flag = True
                    nest_count += 1
                    context.print('{')
                    context.indent()
                expr = info
                if expr.isAggregateFunc():
                    arg = expr.args[0]
                    test = None
                    if isinstance(arg, node_if_suffix):
                        test = arg.test
                        arg = arg.body
                    ty = arg.getType()
                    init_value = '0'
                    func_name = str(expr.func)
                    if func_name == 'min':
                        init_value = getMinMaxValueExpr(ty, True)
                    elif func_name == 'max':
                        init_value = getMinMaxValueExpr(ty, False)
                    context.registerInstantSymbol(name, name)
                    if func_name in ['median', 'count_uniques']:
                        context.print('std::vector<%s> %s$c;' % (ty, name))
                    elif func_name == 'count_trues':
                        context.print('int %s = 0;' % (name, ))
                    elif func_name == 'count_changes':
                        context.print('%s %s$c = 0;' % (ty, name))
                        context.print('int %s = 0;' % (name, ))
                    elif func_name == 'any':
                        context.print('bool %s = false;' % (name, ))
                    elif func_name == 'all':
                        context.print('bool %s = true;' % (name, ))
                    elif func_name in ['sum', 'mean']:
                        context.print('%s %s = %s;' % ('int' if ty == 'bool' else ty, name, init_value))
                    else:
                        context.print('%s %s = %s;' % (ty, name, init_value))
                    if func_name == 'mean':
                        context.print('int %s$cnt = 0;' % (name, ))
                    visitTopologicalUseBind(arg)
                    context.pushInstantSymbols()
                    nest_count2 = self.processInstant()
                    if test is not None:
                        context.print0('if(%s) ' % (test, ))
                    if func_name == 'min':
                        context.print('if(%s<%s) %s = %s;' % (arg, name, name, arg))
                    elif func_name == 'max':
                        context.print('if(%s<%s) %s = %s;' % (name, arg, name, arg))
                    elif func_name == 'count_trues':
                        context.print('if(%s) ++%s;' % (arg, name))
                    elif func_name == 'count_changes':
                        context.print('{ %s %s$d = %s; if(!%s || %s$c!=%s$d) { ++%s; %s$c = %s$d; } }' % (ty, name, arg, name, name, name, name, name, name))
                    elif func_name == 'any':
                        context.print('if(%s) { %s = true; break; }' % (arg, name))
                    elif func_name == 'all':
                        context.print('if(!(%s)) { %s = false; break; }' % (arg, name))
                    elif func_name == 'sum':
                        context.print('%s += %s;' % (name, arg))
                    elif func_name == 'mean':
                        context.print('{ %s += %s; ++%s$cnt; }' % (name, arg, name))
                    elif func_name in ['median', 'count_uniques']:
                        context.print('%s$c.push_back(%s);' % (name, arg))
                    self.closeNest(nest_count2)
                    context.popInstantSymbols()
                    if func_name == 'mean':
                        context.print('assert(%s$cnt);' % (name, ))
                        context.print('%s /= %s$cnt;' % (name, name))
                    elif func_name == 'median':
                        context.print('assert(!%s$c.empty());' % (name, ))
                        context.appendInclude('algorithm')
                        context.print('std::nth_element(%s$c.begin(), %s$c.begin() + (%s$c.size()>>1), %s$c.end());' % (name, name, name, name))
                        context.print('%s %s = %s$c[%s$c.size()>>1];' % (ty, name, name, name))
                        context.print('if(!(%s$c.size()&1)) {' % (name, ))
                        context.indent()
                        context.print('%s += *std::max_element(%s$c.begin(), %s$c.begin() + (%s$c.size()>>1));' % (name, name, name, name))
                        context.print('%s /= 2;' % (name, ))
                        context.dedent()
                        context.print('}')
                    elif func_name == 'count_uniques':
                        context.appendInclude('algorithm')
                        context.print('int %s;' % (name, ))
                        context.print('if(%s$c.empty()) {' % (name, ))
                        context.indent()
                        context.print('%s = 0;' % (name, ))
                        context.dedent()
                        context.print('}')
                        context.print('else {')
                        context.indent()
                        context.print('%s = 1;' % (name, ))
                        context.print('std::sort(%s$c.begin(), %s$c.end());' % (name, name))
                        context.print('for(int %s$i=1; %s$i<%s$c.size(); ++%s$i) {' % (name, name, name, name))
                        context.indent()
                        context.print('if(%s$c[%s$i-1]!=%s$c[%s$i]) ++%s;' % (name, name, name, name, name))
                        context.dedent()
                        context.print('}')
                        context.dedent()
                        context.print('}')
                elif isinstance(expr, call_node) and expr.node_type == 'funccall' and str(expr.func) == 'move':
                    context.print('auto %s = %s;' %
                                  (name, expr.toStr()))
                else:
                    context.print('const auto & %s = %s;' %
                                  (name, expr.toStr()))
            elif kind == 'loop':
                begin, end, diff, condition, container = info
                name = str(name)
                if begin is None and diff is None and end is not None:
                    ty = end.getType()
                    if ty == 'string' or isinstance(ty, Template) and ty.name in ['vector', 'deque', 'list', 'set', 'multiset']:
                        nest_flag = False
                        context.print0(
                            'for(auto & %s : %s) ' % (name, end))
                        if condition is not None:
                            context.print0('if(%s) ' % (condition, ))
                        continue
                resize_stmts = []
                if child is not None and isinstance(child, op2_node) and child.node_type == 'assign':
                    for i in range(len(child.args) - 2, 0, -2):
                        if child.args[i] == '=' and (end is not None or container is not None):
                            resize_stmts.extend(
                                child.args[i - 1].printResize('%s.size()' % (container, ) if end is None else end, name))
                if len(resize_stmts):
                    if not nest_flag:
                        nest_count += 1
                        context.print('{')
                        context.indent()
                    for resize_stmt in resize_stmts:
                        context.print('%s;' % (resize_stmt, ))
                nest_flag = False
                defs = {}
                if begin is not None and type(begin) not in [node_var, node_ref, const_node]:
                    defs['$b'] = str(begin)
                    begin = '$b'
                if end is not None and type(end) not in [node_var, node_ref, const_node]:
                    defs['$e0'] = str(end)
                    end = '$e0'
                if diff is None:
                    diff = '1'
                sign = False
                while isinstance(diff, pre_op1_node) and diff.node_type in ['pre_plus', 'pre_minus']:
                    sign = sign if diff.node_type == 'pre_plus' else not sign
                    diff = diff.child_node
                sign = '-' if sign else '+'
                if str(diff) == '1':
                    diff = '%s%s%s' % (sign, sign, name)
                elif type(diff) not in [node_var, node_ref, const_node]:
                    diff = '%s%s=%s' % (name, sign, diff)
                else:
                    defs['$d'] = '-%s' % (diff,
                                          ) if sign == '-' else str(diff)
                    diff = '%s+=$d' % (name, )
                    sign = None
                if begin is not None:
                    if container is not None:
                        begin = '(0<=%s ? %s : (int)%s.size()+%s)' % (begin,
                                                                      begin, container, begin)
                elif sign == '+':
                    begin = '0'
                elif sign == '-':
                    assert container is not None
                    begin = '(int)%s.size()-1' % (container, )
                else:
                    begin = '(0<=$d ? 0 : (int)%s.size()-1)'
                if end is not None:
                    if container is not None:
                        end = '(0<=%s ? %s : (int)%s.size()+%s)' % (end,
                                                                    end, container, end)
                elif sign == '+':
                    assert container is not None
                    end = '(int)%s.size()' % (container, )
                elif sign == '-':
                    end = '-1'
                else:
                    end = '(0<=$d ? (int)%s.size() : -1)'
                order = ['$b', '$e0', '$d', name, '$e']
                if end == '$e0':
                    defs['$e'] = defs['$e0']
                    defs.pop('$e0')
                    order = ['$e'] + order
                else:
                    defs['$e'] = str(end)
                if begin == '$b':
                    defs[name] = defs['$b']
                    defs.pop('$b')
                    order = [name] + order
                else:
                    defs[name] = str(begin)
                def_str = ', '.join('%s=%s' % (k, defs.pop(k))
                                    for k in order if k in defs)
                #def_str = []
                # for k in order:
                #    if k in defs:
                #        def_str.append('%s=%s' % (k, defs.pop(k)))
                if sign == '+':
                    context.print0('for(int %s; %s<$e; %s) ' %
                                   (def_str, name, diff))
                elif sign == '-':
                    context.print0('for(int %s; $e<%s; %s) ' %
                                   (def_str, name, diff))
                else:
                    context.print0('for(int %s; 0<=$d ? %s<$e : $e<%s; %s) ' % (
                        def_str, name, name, diff))
                if condition is not None:
                    context.print0('if(%s) ' % (condition, ))
                context.registerInstantSymbol(name, name)
            else:
                TODO
        return nest_count

    def suiteWithDefines(self):
        self.printDefines()
        self.suite()

    def suite(self):
        global context
        preContext = context
        context = self.context
        context.enterTemporaryDefinition()
        for child in self.getChildNodes():
            context.releaseTemporaryDefinition()
            if isinstance(child, node_funcdef):
                continue
            if isinstance(child, node_varhint):
                continue
            if isinstance(child, node_tuple) and child.node_type == 'tuple' and sum(not isinstance(e, node_varhint) for e in child.elements) == 0:
                continue
            if use_profiler:
                context.print('MM$P(%s);' % (child.meta['line'], ))
            # ノードを訪れて、:と@と複数比較を展開する
            child.visit('visitReserveBind', childrenMethods='getChildNodesWithoutSuite')
            child.visit('visitExecBind', childrenMethods='getChildNodesWithoutSuite')
            visitTopologicalUseBind(child)
            # :がある場合は、その数だけforを作る
            # @または複数比較がある場合は、{を出力してindent
            # @または複数比較がある場合は、一時変数計算を行う
            context.pushInstantSymbols()
            nest_count = self.processInstant(child=child)
            done = False
            if isinstance(child, node_if_stmt):
                context.print('if(%s) {' % (testToStr(child.if_data[0]), ))
                context.indent()
                child.if_data[1].suiteWithDefines()
                if use_profiler:
                    context.print('MM$P(%s);' % (child.meta['line'], ))
                context.dedent()
                context.print('}')
                for elif_row in child.elif_data:
                    context.print('else if(%s) {' % (
                        testToStr(elif_row[0]), ))
                    context.indent()
                    elif_row[1].suiteWithDefines()
                    if use_profiler:
                        context.print('MM$P(%s);' % (child.meta['line'], ))
                    context.dedent()
                    context.print('}')
                if child.else_suite is not None:
                    context.print('else {')
                    context.indent()
                    child.else_suite.suiteWithDefines()
                    if use_profiler:
                        context.print('MM$P(%s);' % (child.meta['line'], ))
                    context.dedent()
                    context.print('}')
                done = True
            elif isinstance(child, node_for_stmt):
                expr = child.container
                ty = expr.getType()
                temporary = child.temporary
                if child.hasParallel():
                    context.appendInclude('thread_pool.h')
                    context.print('{')
                    context.indent()
                    context.print('MM$ThreadPool $tp%s;' % (child.getParallelConstructorCall(), ))
                if ty in ['unsigned long long', 'uint64', 'long long', 'int64', 'size_t', 'int', 'short', 'char', 'bool']:
                    temporary, minus_flag = expand_minus(temporary)
                    assert isinstance(temporary, node_var), 'コンテナではなく整数値で暗黙のforを回すのに、ループ変数が単独変数ではありません'
                    if not minus_flag:
                        context.print('for(int %s=0; %s<%s; ++%s) {' % (temporary, temporary, expr, temporary))
                    else:
                        context.print('for(int %s=0; -%s<%s; --%s) {' % (temporary, temporary, expr, temporary))
                else:
                    itemTy = itemType(ty)
                    if itemTy is None:
                        itemTy = 'auto'
                    temporaries = []
                    temporary_is_tuple = isinstance(temporary, tuple_node) and temporary.node_type == 'tuple'
                    if temporary_is_tuple:
                        if itemTy == 'auto':
                            for element in child.temporary.elements:
                                element, minus_flag = expand_minus(element)
                                assert isinstance(element, node_var)
                                temporaries.append((element.name, 'auto', minus_flag))
                        else:
                            assert isinstance(itemTy, Template) and itemTy.name == 'tuple'
                            assert len(temporary.elements) == len(itemTy.args)
                            for element, iTy in zip(child.temporary.elements, itemTy.args if isinstance(itemTy, Template) else itemTy):
                                element, minus_flag = expand_minus(element)
                                assert isinstance(element, node_var)
                                temporaries.append((element.name, iTy, minus_flag))
                        temporary = '$fr'
                    else:
                        temporary, minus_flag = expand_minus(temporary)
                        assert isinstance(temporary, node_var)
                        temporary = child.temporary.name
                        temporaries.append((temporary, itemTy, minus_flag))
                        if minus_flag:
                            temporary = '$fr'
                    for name, iTy, minus_flag in temporaries:
                        context.defineTemporaryDefinition(name, iTy)
                    if isinstance(ty, Template) and ty.name in ['stack', 'queue', 'priority_queue', 'PriorityQueue', 'fast_pque', 'fast_pque_vk', 'set_pque', 'xpque']:
                        D = {
                            'queue': 'front',
                            'priority_queue': 'top',
                            'PriorityQueue': 'top',
                            'fast_pque': 'top',
                            'fast_pque_vk': 'top',
                            'set_pque': 'top',
                            'xpque': 'top',
                            'stack': 'top',
                        }
                        context.print0('while(!(%s).empty()) { %s %s = (%s).%s(); (%s).pop();' % (
                            child.container,
                            typeToStr(itemTy),
                            temporary,
                            child.container,
                            D[ty.name],
                            child.container,
                        ))
                    else:
                        context.print0('for(const %s & %s : %s) {' % (typeToStr(itemTy), temporary, expr))
                    if temporary == '$fr':
                        for i, (name, iTy, minus_flag) in enumerate(temporaries):
                            if temporary_is_tuple:
                                context.print0(' const %s & %s = %sget<%s>($fr);' % (iTy, name, '-' if minus_flag else '', i))
                            else:
                                context.print0(' const %s & %s = %s$fr;' % (iTy, name, '-' if minus_flag else ''))
                    context.print('')
                context.indent()
                if child.hasParallel():
                    context.print('$tp.submit([&, %s](int %s) {' % (temporary, child.getParallelWorkerIdVarName()))
                    context.indent()
                    label = context.push_banbreak()
                else:
                    label = context.push_break()
                    self.printContinueLabel(label)
                child.suite.suiteWithDefines()
                if context.get_break_used() == 0:
                    label = None
                context.pop_break()
                if use_profiler:
                    context.print('MM$P(%s);' % (child.meta['line'], ))
                if child.hasParallel():
                    context.dedent()
                    context.print('});')
                context.dedent()
                context.print('}')
                if child.else_suite is not None:
                    context.print('{')
                    context.indent()
                    child.else_suite.suiteWithDefines()
                    if use_profiler:
                        context.print('MM$P(%s);' % (child.meta['line'], ))
                    context.dedent()
                    context.print('}')
                if label is not None:
                    context.print('%s:;' % (label, ))
                if child.hasParallel():
                    context.dedent()
                    context.print('}')
                done = True
            elif isinstance(child, node_while_stmt):
                context.print('while(%s) {' % (child.conditions, ))
                context.indent()
                label = context.push_break()
                self.printContinueLabel(label)
                child.suite.suiteWithDefines()
                if context.get_break_used() == 0:
                    label = None
                context.pop_break()
                if use_profiler:
                    context.print('MM$P(%s);' % (child.meta['line'], ))
                context.dedent()
                context.print('}')
                if child.else_suite is not None:
                    context.print('{')
                    context.indent()
                    child.else_suite.suiteWithDefines()
                    if use_profiler:
                        context.print('MM$P(%s);' % (child.meta['line'], ))
                    context.dedent()
                    context.print('}')
                if label is not None:
                    context.print('%s:;' % (label, ))
                done = True
            if not done:
                code = str(child)
                if 1 <= len(code):
                    context.print(code + ';')
            # ノードの内容を出力する
            # @または複数比較がある場合は、dedentして}を出力
            self.closeNest(nest_count)
            context.popInstantSymbols()
        context.leaveTemporaryDefinition()
        context = preContext


node_start = group_node
node_suite = group_node


class node_funcdef(base_node):

    def __init__(self, meta, node_type, *args):
        self.meta = meta
        self.node_type = node_type
        self.decorators = []
        self.name = None
        self.suite = args[-1]
        self.parameters = ()
        self.return_type = None
        for arg in args[:-1]:
            if isinstance(arg, node_decorator):
                self.decorators.append(arg)
            elif self.name is None:
                assert isinstance(arg, node_var)
                self.name = arg.name
            elif isinstance(arg, tuple_node) and arg.node_type == 'parameters':
                self.parameters = arg.elements
            else:
                self.return_type = arg
        #print(len(self.decorators), self.name, tuple(str(param) for param in self.parameters), self.return_type)

    def toStr(self):
        TODO

    def defines(self):
        for param in self.parameters:
            param.defines()
        self.suite.defines()

    def getChildNodesWithoutSuite(self):
        ret = self.decorators + list(self.parameters)
        if self.return_type is not None:
            ret += [self.return_type]
        return ret

    def getSuiteChildNodes(self):
        return [self.suite]

    def getChildNodes(self):
        return self.getChildNodesWithoutSuite() + self.getSuiteChildNodes()

    def visitDeclare(self):
        context.declareFunc(
            self.name, None if self.return_type is None else str(self.return_type))

    def visitDeclare2(self):
        context.print('%s %s(%s);' % ('void' if self.return_type is None else self.return_type,
                                      self.name, ', '.join(str(param) for param in self.parameters)))

    def getDecorators(self):
        ret = {}
        for decorator in self.decorators:
            values = []
            keyvalues = {}
            for arg in decorator.args:
                if isinstance(arg, node_argvalue):
                    keyvalues[arg.name] = arg.value
                else:
                    values.append(arg)
            if decorator.name not in ret:
                ret[decorator.name] = []
            ret[decorator.name].append((values, keyvalues))
        return ret

    def visitDefineFunc(self):
        global context
        parentContext = context
        context = self.suite.context

        func_name = self.name
        decorators = self.getDecorators()
        pybind11 = decorators.get('pybind11', [None])[0]
        memo = decorators.get('memo', [None])[0]
        beam = decorators.get('beam', [None])[0]
        beam_po = decorators.get('beam_po', [None])[0]
        sa_dac = decorators.get('sa_dac', [None])[0]
        penalty_hashes = {kv['varname'].name: str(kv['function']) if 'function' in kv else None for (v, kv) in decorators.get('penalty_hash', []) if isinstance(kv.get('varname'), node_var)}
        xheu_type = None
        xheu = None
        if pybind11 is not None:
            context.add_pybind11(func_name)
        if beam is not None:
            xheu, xheu_type = beam, 'beam'
        elif beam_po is not None:
            xheu, xheu_type = beam_po, 'beam_po'
        elif sa_dac is not None:
            xheu, xheu_type = sa_dac, 'sa_dac'
        if memo is not None or xheu is not None:
            func_name = '%s$origin' % (self.name, )
            if xheu_type == 'beam':
                assert self.return_type is None
            if xheu_type == 'beam_po':
                assert self.return_type is None
            if xheu_type == 'sa_dac':
                assert str(self.return_type) == 'float'

        context.print('%s %s(%s) {' % ('void' if self.return_type is None else self.return_type,
                                       func_name, ', '.join(str(param) for param in self.parameters)))
        context.indent()
        if use_profiler:
            context.print('MM$P$Func MM$P$func;')
        self.suite.printDefines()
        self.suite.suite()
        context.dedent()
        context.print('}')
        if memo is not None:
            cycle = memo[1].get('cycle', None)
            size = memo[1].get('size', None)
            default_value = memo[1].get('default', None)
            if default_value is None:
                default_value = '(unsigned %s)-1 >> 1' % (self.return_type, )
            if size is not None:
                while isinstance(size, node_parenthesis):
                    size = size.child_node
                if isinstance(size, tuple_node):
                    size = size.elements
                else:
                    size = [size]
                assert len(self.parameters) == len(size)
                context.print('vector<%s> %s$memo;' %
                              (self.return_type, self.name))
            else:
                context.print('%s %s$memo;' % (typeToStr(Template('map', [Template('tuple', [param.ty for param in self.parameters]), self.return_type])), self.name, ))
            context.print('%s %s(%s) {' % ('void' if self.return_type is None else self.return_type, self.name, ', '.join(str(param) for param in self.parameters)))
            context.indent()
            if size is not None:
                context.print('if(%s$memo.empty()) {' % (self.name, ))
                context.indent()
                context.print('%s$memo.resize(%s, %s);' % (self.name, ' * '.join(toParenthesisStr(s) for s in size), toParenthesisStr(default_value)))
                context.dedent()
                context.print('}')
                context.print('auto & ref = %s$memo[%s];' % (self.name, ' + '.join('%s%s' % (self.parameters[i].name, ''.join('*%s' % (s, ) for s in size[i + 1:])) for i in range(len(size)))))
                context.print('if(ref == %s) {' % (
                    toParenthesisStr(default_value), ))
                context.indent()
                if cycle is not None:
                    context.print('ref = %s;' % (cycle, ))
                context.print('ref = %s$origin(%s);' % (
                    self.name, ', '.join(param.name for param in self.parameters)))
                context.dedent()
                context.print('}')
                context.print('return ref;')
            else:
                context.print('auto it = %s$memo.find(%s(%s));' % (self.name, typeToStr(Template('tuple', [param.ty for param in self.parameters])), ', '.join(param.name for param in self.parameters)))
                context.print('if(it!=%s$memo.end()) {' % (self.name, ))
                context.indent()
                context.print('return it->second;')
                context.dedent()
                context.print('}')
                if cycle is not None:
                    context.print('%s$memo[%s(%s)] = %s;' % (self.name, typeToStr(Template('tuple', [param.ty for param in self.parameters])), ', '.join(param.name for param in self.parameters), cycle))
                context.print('auto ret = %s$origin(%s);' % (
                    self.name, ', '.join(param.name for param in self.parameters)))
                context.print('%s$memo[%s(%s)] = ret;' % (self.name, typeToStr(Template('tuple', [param.ty for param in self.parameters])), ', '.join(param.name for param in self.parameters)))
                context.print('return ret;')
            context.dedent()
            context.print('}')
        if xheu is not None:
            maxdepth = xheu[1].get('maxdepth', None)
            maxwidth = xheu[1].get('maxwidth', None)
            timelimit = xheu[1].get('timelimit', None)
            verbose = xheu[1].get('verbose', None)
            avg_bonus_rate = xheu[1].get('avg_bonus_rate', None)
            stdev_bonus_rate = xheu[1].get('stdev_bonus_rate', None)
            ucb_bonus_rate = xheu[1].get('ucb_bonus_rate', None)
            visitclear = xheu[1].get('visitclear', None)
            if xheu_type == 'beam':
                context.appendInclude('xbeam.h')
                context.print('xbeam %s$so;' % (self.name, ))
            if xheu_type == 'beam_po':
                context.appendInclude('xbeam_po.h')
                context.print('xbeam_po %s$so;' % (self.name, ))
            elif xheu_type == 'sa_dac':
                context.appendInclude('xsa_dac.h')
                context.print('xsa_dac %s$so;' % (self.name, ))
            clear_with_inits = []
            for param in self.parameters:
                if param.name in penalty_hashes:
                    clear_with_inits.append('%s$%s$counter' % (self.name, param.name))
                    if param.ty in ['uint64', 'int64']:
                        context.appendInclude('fast_weak_counter64.h')
                        context.print('fast_weak_counter64<> %s$%s$counter;' % (self.name, param.name))
                    else:
                        context.appendInclude('fast_weak_counter32.h')
                        context.print('fast_weak_counter32<> %s$%s$counter;' % (self.name, param.name))
                elif param.name == 'hash':
                    clear_with_inits.append('%s$visited' % (self.name, ))
                    if param.ty in ['uint64', 'int64']:
                        context.appendInclude('fast_weak_set64.h')
                        context.print('fast_weak_set64<> %s$visited;' % (self.name, ))
                    else:
                        context.appendInclude('fast_weak_set32.h')
                        context.print('fast_weak_set32<> %s$visited;' % (self.name, ))
                    if visitclear is not None:
                        context.print('int %s$remain_depth;' % (self.name, ))
            context.print('void %s$loop() {' % (self.name, ))
            context.indent()
            context.print('while(%s$so.onloop()) {' % (self.name, ))
            context.indent()
            paramTypeMethodDic = {
                'short': 'nextShort',
                'int': 'nextInt',
                'int64': 'nextInt64',
                'float': 'nextFloat',
                'bool': 'nextBool',
            }
            penalty_hash_flag = False
            hash_flag = False
            for param in self.parameters:
                context.print('auto %s = %s$so.%s();' % (param.name, self.name, paramTypeMethodDic[str(param.ty)]))
                if param.name in penalty_hashes:
                    penalty_hash_flag = True
                elif param.name == 'hash':
                    hash_flag = True
            if penalty_hash_flag and 'score' in [param.name for param in self.parameters]:
                context.print('double $score = score;')
                context.print('int $cnt;')
                for param in self.parameters:
                    if param.name in penalty_hashes:
                        context.print('$cnt = %s$%s$counter.get(%s);' % (self.name, param.name, param.name))
                        if penalty_hashes[param.name] is not None:
                            context.print('if($cnt) $score -= %s($cnt);' % (penalty_hashes[param.name], ))
                        else:
                            context.print('if($cnt) $score -= $cnt;')
                context.print('if(%s$so.reschedule($score)) continue;' % (self.name, ))
            if hash_flag:
                if visitclear is not None:
                    context.print('if(%s$remain_depth!=%s$so.getRemainDepth()) {' % (self.name, self.name))
                    context.indent()
                    context.print('%s$remain_depth = %s$so.getRemainDepth();' % (self.name, self.name))
                    context.print('if(%s) %s$visited.clear();' % (visitclear, self.name))
                    context.dedent()
                    context.print('}')
                if xheu_type == 'beam_po':
                    context.print('if(%s$visited.testWithSet(%s) && %s$so.skip()) continue;' % (self.name, param.name, self.name))
                else:
                    context.print('if(%s$visited.testWithSet(%s)) continue;' % (self.name, param.name))
            if penalty_hash_flag and 'score' in [param.name for param in self.parameters]:
                for param in self.parameters:
                    if param.name in penalty_hashes:
                        context.print('%s$%s$counter.count(%s);' % (self.name, param.name, param.name))
            context.print('%s$so.accept();' % (self.name, ))
            context.print('%s$origin(%s);' % (
                self.name, ', '.join(param.name for param in self.parameters)))
            context.dedent()
            context.print('}')
            context.dedent()
            context.print('}')
            context.print('bool %s$lock = false;' % (self.name, ))
            context.print('%s %s(%s) {' % ('void' if self.return_type is None else self.return_type,
                                           self.name, ', '.join(str(param) for param in self.parameters)))
            context.indent()
            context.print('if(%s$lock) {' % (self.name, ))
            context.indent()
            if xheu_type == 'beam_po' and 'score0' in [param.name for param in self.parameters]:
                context.print('if(%s$so.reserve(%s, score0)) {' % (self.name, 'score' if 'score' in [param.name for param in self.parameters] else 'score0'))
            else:
                context.print('if(%s$so.reserve(%s)) {' % (self.name, 'score' if 'score' in [param.name for param in self.parameters] else '0'))
            context.indent()
            for param in self.parameters:
                context.print('%s$so.%s(%s);' % (self.name, paramTypeMethodDic[str(param.ty)], param.name))
            context.dedent()
            context.print('}')
            context.dedent()
            context.print('}')
            context.print('else {')
            context.indent()
            context.print('%s$lock = true;' % (self.name, ))
            for clear_with_init in clear_with_inits:
                context.print('%s.clear();' % (clear_with_init, ))
            context.print('%s$so.init(%s, %s, %s);' % (self.name, maxdepth, timelimit, maxwidth))
            if visitclear is not None:
                context.print('%s$remain_depth = (%s)+1;' % (self.name, maxdepth))
            if verbose is not None:
                context.print('%s$so.setVerbose(%s);' % (self.name, verbose))
            if xheu_type == 'beam_po' and (avg_bonus_rate is not None or stdev_bonus_rate is not None or ucb_bonus_rate is not None):
                context.print('%s$so.setBonusRates(%s, %s, %s);' % (self.name, avg_bonus_rate or 0, stdev_bonus_rate or 0, ucb_bonus_rate or 0))
            context.print('%s$origin(%s);' % (
                self.name, ', '.join(param.name for param in self.parameters)))
            context.print('%s$loop();' % (self.name, ))
            context.print('%s$lock = false;' % (self.name, ))
            context.dedent()
            context.print('}')
            context.dedent()
            context.print('}')
        context.printBlank()
        context = parentContext


class node_varhint(base_node):

    def __init__(self, meta, node_type, varname, typehint):
        assert node_type == 'varhint'
        assert isinstance(varname, node_var)
        self.meta = meta
        self.node_type = node_type
        self.varname = varname
        self.typehint = typehint

    def toStr(self):
        return self.varname.toStr()

    def getChildNodes(self):
        if self.typehint is not None:
            return [self.varname, self.typehint]
        else:
            return [self.varname]

    def getReturnType(self):
        return self.varname.getReturnType()

    def reserveBind(self, name=None):
        pass

    def defines(self):
        if self.typehint is not None:
            self.typehint.visit('visitBindSymbol')
            ty = self.typehint
            if isinstance(ty, call_node) and ty.node_type == 'template':
                ty = ty.getReturnType()
            else:
                ty = ty.name
            context.definesWithHint(self.varname.name, ty)
        else:
            context.definesWithoutHint(self.varname.name)
        self.varname.visit('visitBindSymbol')

    def hintType(self, ty, op):
        self.varname.hintType(ty, op)

    def countMutate(self):
        self.varname.countMutate()

    def getType(self):
        return self.varname.getType()

    def visitInclude(self):
        if self.typehint is not None:
            ty = self.typehint
            if isinstance(ty, call_node) and ty.node_type == 'template':
                ty = ty.getReturnType()
            else:
                ty = ty.name
            while True:
                if isinstance(ty, Template):
                    context.visitTypeInclude(ty.name)
                    if context.hasMethod(ty, '__getitem__'):
                        ty = context.getMethodReturnType(ty, '__getitem__')
                        continue
                else:
                    context.visitTypeInclude(ty)
                break


class loop_node(base_node):

    def __init__(self, meta, node_type, slice):
        self.meta = meta
        self.slice = slice

    def getChildNodes(self):
        return self.slice

    def getChildNodesForBind(self):
        return []

    def visitRestruct(self):
        target = self
        if isinstance(target.parent, tuple_node) and target.parent.node_type == 'tuple':
            target = target.parent
        if isinstance(target.parent, op2_node):
            if not target.parent.isAssignedObject(target):
                return
            target = target.parent
        if not isinstance(target.parent, group_node):
            return
        assert isinstance(self.slice[0], base_node)
        assert self.slice[1] is None or isinstance(self.slice[1], base_node)
        restruct(self, node_varhint, self.meta, 'varhint', self.slice[0], self.slice[1])

    def visitExecBind(self):
        self.bindStr = context.nameLoop(self.bindName)

    def visitUseBind(self):
        slice = [i for i in self.slice] + [None]
        if isinstance(self.parent, call_node) and self.parent.node_type == 'getitem':
            slice[4] = self.parent.func

        def func():
            context.reserveLoop(slice, self.bindStr)
        return 'loop', func

    def getType(self):
        if self.slice[0] is None and self.slice[2] is None and self.slice[1] is not None:
            ty = self.slice[1].getType()
            if isinstance(ty, Template) and ty.name in ['vector', 'deque', 'list', 'set', 'multiset']:
                return ty.itemType()
        return 'int'


def node_instant(meta, node_type, *args):
    whole_name = None
    name = None
    index = 0
    slice = [None] * 4
    for arg in args:
        if arg == '@':
            assert slice[index] is not None
            assert isinstance(slice[index], node_var)
            if index == 0 and whole_name is None:
                whole_name = slice[index].name
            else:
                assert name is None
                name = slice[index].name
            slice[index] = None
        elif arg == ':':
            assert index in [0, 1, 2]
            assert name is None
            index += 1
        else:
            assert slice[index] is None
            slice[index] = arg
            if name is not None:
                arg.reserveBind(name)
                name = None
    assert name is None
    if index == 0:
        ret = slice[0]
        if whole_name is not None:
            ret.reserveBind(whole_name)
    else:
        ret = loop_node(meta, node_type, slice)
        ret.reserveBind(whole_name)
    return ret


class node_var(base_node):

    def __init__(self, meta, node_type, name):
        assert isinstance(name, str)
        self.meta = meta
        self.name = name
        self.dest_variable = '__unknown__'

    def toStr(self):
        return str(self.dest_variable)

    def getChildNodes(self):
        return []

    def getReturnType(self):
        if self.name in ['min', 'max'] and isinstance(self.parent, call_node) and self.parent.args is not None:
            if len(self.parent.args) == 1:
                ty = self.parent.args[0].getType()
                if isinstance(ty, Template) and ty.name in ['set', 'multiset']:
                    return ty.itemType()
                return ty
            ty = None
            for args in self.parent.args:
                ty = comprehensionType(ty, args.getType())
            return ty
        if self.name == 'abs' and isinstance(self.parent, call_node) and self.parent.args is not None and len(self.parent.args) == 1:
            return self.parent.args[0].getType()
        return context.getReturnType(self.name)

    def reserveBind(self, name=None):
        pass

    def defines(self):
        context.defines(self.name)
        self.visit('visitBindSymbol')

    def definesFor(self):
        context.definesFor(self.name)
        self.visit('visitBindSymbol')

    def hintType(self, ty, op):
        context.hintType(self.name, ty, op)

    def countMutate(self):
        context.countMutate(self.name)

    def getType(self):
        return context.getType(self.name)

    def visitInclude(self):
        context.visitInclude(self.name)

    def visitBindSymbol(self):
        self.dest_variable = context.getSymbol(self.name)


class node_ref(base_node):

    def __init__(self, meta, node_type, name):
        self.meta = meta
        self.name = name
        self.dest_variable = '__unknown__'

    def toStr(self):
        return str(self.dest_variable)

    def getChildNodes(self):
        return []

    def getType(self):
        return 'int'

    def visitBindSymbol(self):
        self.dest_variable = context.getSymbol(self.name)


def escape(value):
    value = value.replace("\\", "\\\\")
    value = value.replace("\r", "\\r")
    value = value.replace("\n", "\\n")
    value = value.replace("'", "\\'")
    value = value.replace('"', '\\"')
    return value


class const_node(base_node):

    def __init__(self, meta, node_type, *args):
        self.meta = meta
        self.node_type = node_type
        if node_type == 'const_true':
            self.value_str = 'true'
            self.value_type = 'bool'
        elif node_type == 'const_false':
            self.value_str = 'false'
            self.value_type = 'bool'
        elif node_type == 'string':
            self.value_str = '"%s"' % (
                escape(''.join(eval(arg) for arg in args)), )
            self.value_type = 'string'
        elif node_type == 'char':
            self.value_str = "'%s'" % (
                escape(''.join(eval(arg) for arg in args)), )
            self.value_type = 'char'
        elif node_type == 'float':
            self.value_str = '(long long)%s' % (
                args[0][:-2], ) if args[0].lower().endswith('ll') else args[0]
            self.value_type = 'long long' if args[
                0].lower().endswith('ll') else 'double'
        else:
            assert len(args) == 1
            s = args[0]
            if s.startswith('0o') or s.startswith('0b'):
                if s.endswith('ll'):
                    self.value_str = hex(eval(s[:-2])) + 'll'
                else:
                    self.value_str = hex(eval(s))
            else:
                self.value_str = args[0]
            self.value_type = 'long long' if self.value_str.lower().endswith('ll') else 'int'

    def reserveBind(self, name=None):
        pass

    def toStr(self):
        return self.value_str

    def getChildNodes(self):
        return []

    def getType(self):
        return self.value_type

    def isConstExpr(self):
        return self.value_type in constexpr_types


node_string = const_node
node_char = const_node
node_dec_number = const_node
node_float = const_node
node_const_true = const_node
node_const_false = const_node


class node_decorator(base_node):

    def __init__(self, meta, node_type, name, args=None):
        assert isinstance(name, node_var)
        self.meta = meta
        self.name = name.name
        if isinstance(args, tuple_node) and args.node_type == 'arguments':
            args = args.elements
        else:
            args = [args]
        self.args = args

    def toStr(self):
        TODO

    def getChildNodes(self):
        return self.args


class node_argvalue(base_node):

    def __init__(self, meta, node_type, name, value):
        assert isinstance(name, node_var)
        self.meta = meta
        self.name = name.name
        self.value = value

    def toStr(self):
        TODO

    def getChildNodes(self):
        assert self.value is not None
        return [self.value]


class node_typedparam(base_node):

    def __init__(self, meta, node_type, name, ty=None):
        assert isinstance(name, node_var)
        self.meta = meta
        self.name = name.name
        self.referenced = isinstance(ty, node_referencedtype)
        if self.referenced:
            ty = ty.ty
        self.ty = ty

    def toStr(self):
        ty = 'int' if self.ty is None else (self.ty.getReturnType() if isinstance(self.ty, call_node) and self.ty.node_type == 'template' else str(self.ty))
        return '%s%s%s' % (ty, ' & ' if self.referenced else ' ', self.name)

    def defines(self):
        ty = 'int' if self.ty is None else (self.ty.getReturnType() if isinstance(self.ty, call_node) and self.ty.node_type == 'template' else str(self.ty))
        context.definesParam(self.name, ty)

    def getChildNodes(self):
        if self.ty is not None:
            return [self.ty]
        return []


class node_referencedtype(base_node):

    def __init__(self, meta, node_type, ty):
        self.meta = meta
        self.ty = ty


class call_node(base_node):

    def __init__(self, meta, node_type, func, args=None):
        self.meta = meta
        self.node_type = node_type
        self.func = func
        if args is None:
            self.args = ()
        elif isinstance(args, tuple_node) and args.node_type == 'tuple':
            self.args = args.elements
        else:
            self.args = (args, )
        assert isinstance(self.args, tuple)

    def visitInclude(self):
        if self.node_type == 'funccall' and isinstance(self.func, node_getattr) and self.func.member in ['where', 'eq_to_end', 'eq_to_rend'] and 1 <= len(self.args):
            context.visitInclude('_slice')
        if self.node_type == 'template':
            context.visitTypeInclude(self.func.name)

    def toStr(self):
        if self.node_type == 'getitem':
            ty = self.func.getType()
            if isinstance(ty, Template) and ty.name == 'tuple':
                assert len(self.args) == 1
                return 'get<%s>(%s)' % (self.args[0], self.func)
            return '%s[%s]' % (self.func, ', '.join(str(arg) for arg in self.args))
        if self.node_type == 'funccall' and len(self.args) == 0 and isinstance(self.func, node_getattr) and self.func.member in ['sort', 'rsort', 'reverse', 'shuffle', 'pop', 'shift']:
            ty = self.func.obj.getType()
            if ty == 'string' or isinstance(ty, Template) and ty.name in ['vector', 'deque']:
                if self.func.member == 'sort':
                    return 'sort(%s.begin(), %s.end())' % (self.func.obj, self.func.obj)
                if self.func.member == 'rsort':
                    return 'sort(%s.rbegin(), %s.rend())' % (self.func.obj, self.func.obj)
                if self.func.member == 'reverse':
                    return 'reverse(%s.begin(), %s.end())' % (self.func.obj, self.func.obj)
                if self.func.member == 'shuffle':
                    context.visitInclude('lrand49')
                    return 'for(int $1=0, $e=%s.size(); $1<$e; ++$1) swap(%s[$1], %s[lrand49($e-$1)+$1])' % (self.func.obj, self.func.obj, self.func.obj)
                if self.func.member == 'pop':
                    return '({ auto ret = %s.back(); %s.pop_back(); ret; })' % (self.func.obj, self.func.obj, )
                if self.func.member == 'shift' and ty.name == 'deque':
                    return '({ auto ret = %s.front(); %s.pop_front(); ret; })' % (self.func.obj, self.func.obj, )
        if self.node_type == 'funccall' and len(self.args) == 1 and isinstance(self.func, node_getattr) and self.func.member in ['shuffle']:
            ty = self.func.obj.getType()
            if ty == 'string' or isinstance(ty, Template) and ty.name in ['vector', 'deque']:
                if self.func.member == 'shuffle':
                    context.visitInclude('lrand49')
                    return 'for(int $1=0, $e2=%s.size(), $e=min((int)(%s), $e2); $1<$e; ++$1) swap(%s[$1], %s[lrand49($e2-$1)+$1])' % (self.func.obj, self.args[0], self.func.obj, self.func.obj)
        if self.node_type == 'funccall' and len(self.args) == 2 and isinstance(self.func, node_getattr) and self.func.member in ['shuffle']:
            ty = self.func.obj.getType()
            if ty == 'string' or isinstance(ty, Template) and ty.name in ['vector', 'deque']:
                if self.func.member == 'shuffle':
                    context.visitInclude('lrand49')
                    return 'for(int $1=%s, $e2=%s.size(), $e=min((int)(%s), $e2); $1<$e; ++$1) swap(%s[$1], %s[lrand49($e2-$1)+$1])' % (self.args[0], self.func.obj, self.args[1], self.func.obj, self.func.obj)
        if self.node_type == 'funccall' and isinstance(self.func, node_getattr) and self.func.member in ['lower_bound', 'upper_bound', 'bound', 'where', 'eq_to_end', 'eq_to_rend', 'min', 'max']:
            ty = self.func.obj.getType()
            if isinstance(ty, Template) and ty.name in ['vector', 'deque', 'xvector']:
                if self.func.member in ['lower_bound', 'upper_bound']:
                    if 1 <= len(self.args):
                        args = toStrBoundArg(ty, self.args, self.func.member == 'upper_bound')
                        return '(int)(%s(%s.begin(), %s.end(), %s) - %s.begin())' % (self.func.member, self.func.obj, self.func.obj, args, self.func.obj)
                    else:
                        return '(int)%s.size()' % (self.func.member, ) if self.func.member == 'upper_bound' else '0'
                if self.func.member == 'where' and 1 <= len(self.args):
                    return '_slice(lower_bound(%s.begin(), %s.end(), %s), upper_bound(%s.begin(), %s.end(), %s))' % (
                        self.func.obj, self.func.obj, toStrBoundArg(ty, self.args, False), self.func.obj, self.func.obj, toStrBoundArg(ty, self.args, True))
                if self.func.member == 'bound' and 1 <= len(self.args):
                    return 'range(lower_bound(%s.begin(), %s.end(), %s)-%s.begin(), upper_bound(%s.begin(), %s.end(), %s)-%s.begin())' % (
                        self.func.obj, self.func.obj, toStrBoundArg(ty, self.args, False), self.func.obj, self.func.obj, self.func.obj, toStrBoundArg(ty, self.args, True), self.func.obj)
                if self.func.member == 'eq_to_end':
                    return '_slice(std::lower_bound(%s.begin(), %s.end(), %s), %s.end())' % (self.func.obj, self.func.obj, toStrBoundArg(ty, self.args, False), self.func.obj)
                if self.func.member == 'eq_to_rend':
                    return '_slice((decltype(%s.rbegin()))std::upper_bound(%s.begin(), %s.end(), %s), %s.rend())' % (self.func.obj, self.func.obj, self.func.obj, toStrBoundArg(ty, self.args, True), self.func.obj)
            if isinstance(ty, Template) and ty.name in ['set', 'multiset']:
                if self.func.member == 'where':
                    return '_slice(%s.lower_bound(%s), %s.upper_bound(%s))' % (self.func.obj, toStrBoundArg(ty, self.args, False), self.func.obj, toStrBoundArg(ty, self.args, True))
                if self.func.member == 'eq_to_end':
                    return '_slice(%s.lower_bound(%s), %s.end())' % (self.func.obj, toStrBoundArg(ty, self.args, False), self.func.obj)
                if self.func.member == 'eq_to_rend':
                    return '_slice((decltype(%s.rbegin()))%s.upper_bound(%s), %s.rend())' % (self.func.obj, self.func.obj, toStrBoundArg(ty, self.args, True), self.func.obj)
                if self.func.member in ['min', 'max']:
                    return ('*%s.begin()' if self.func.member == 'min' else '*%s.rbegin()') % (self.func.obj, )
        if self.node_type == 'funccall' and isinstance(self.func, node_var) and self.func.name in ['int', 'int64', 'uint64', 'int128', 'float'] and len(self.args) == 1:
            D = {
                'int': '(int)',
                'uint64': '(unsigned long long)',
                'int64': '(long long)',
                'int128': '(__int128)',
                'float': '(double)',
            }
            return '%s(%s)' % (D[self.func.name], self.args[0])
        if self.node_type == 'funccall' and isinstance(self.func, node_var) and self.func.name in ['set', 'multiset']:
            if isinstance(self.parent, op2_node) and self.parent.node_type == 'assign' and self.parent.args[-1] == self:
                return '%s(%s)' % (self.parent.args[-3].getType(), ', '.join(str(arg) for arg in self.args))
        if self.node_type == 'funccall' and isinstance(self.func, node_var) and self.func.name in ['min', 'max'] and len(self.args) != 0:
            assert 2 <= len(self.args)
            ty = None
            for arg in self.args:
                ty = comprehensionType(ty, arg.getType())
            ret = '(%s)(%s)' % (ty, self.args[0])
            for arg in self.args[1:]:
                ret = '%s(%s, (%s)(%s))' % (self.func, ret, ty, arg)
            return ret
        if self.node_type == 'template':
            ret = '%s%s%s%s' % (self.func, call_dic[self.node_type][
                                0], ', '.join(str(arg) for arg in self.args), call_dic[self.node_type][1])
            ret = ret.replace('>>', '> >')
        else:
            ret = '%s%s%s%s' % (self.func, call_dic[self.node_type][
                                0], ', '.join(str(arg) for arg in self.args), call_dic[self.node_type][1])
        return ret

    def getType(self):
        if self.node_type == 'funccall':
            if isinstance(self.func, node_var) and self.func.name in ['min', 'max', 'sum', 'mean', 'median', 'move'] and len(self.args) == 1:
                return self.args[0].getType()
            if isinstance(self.func, node_var) and self.func.name in ['count_trues', 'count_uniques', 'count_changes'] and len(self.args) == 1:
                return 'int'
            if isinstance(self.func, node_var) and self.func.name in ['any', 'all'] and len(self.args) == 1:
                return 'bool'
            if isinstance(self.func, node_getattr):
                if self.func.member in ['where', 'eq_to_end', 'eq_to_rend']:
                    return self.func.obj.getType()
                if self.func.member == 'bound':
                    return Template('vector', ['int'])
            return self.func.getReturnType()
        elif self.node_type == 'getitem':
            assert len(self.args) == 1
            ty = self.func.getType()
            if isinstance(ty, Template) and ty.name in ['vector', 'deque', 'BIT'] and 1 <= len(ty.args):
                return ty.args[0]
            if isinstance(ty, Template) and ty.name in ['map'] and 2 <= len(ty.args):
                return ty.args[1]
            if ty == 'string':
                return 'char'
            if isinstance(ty, Template) and ty.name == 'tuple':
                assert isinstance(self.args[0], const_node)
                return ty.args[int(self.args[0].toStr())]
            return context.getMethodReturnType(ty, '__getitem__')
        elif self.node_type == 'template':
            TODO

    def getReturnType(self):
        assert self.node_type == 'template'
        D = {
            'uint64': 'unsigned long long',
            'int64': 'long long',
            'float': 'double',
            'float32': 'float',
            'float64': 'double',
        }
        args = []
        for arg in self.args:
            if isinstance(arg, call_node) and arg.node_type == 'template':
                args.append(arg.getReturnType())
            elif arg is not None:
                arg = str(arg)
                arg = D.get(arg, arg)
                args.append(arg)
            else:
                TODO
        return Template(self.func.name, args)

    def defines(self):
        assert self.node_type == 'getitem'
        for arg in self.args:
            arg.visit('visitBindSymbol')
        self.func.defines()

    def hintType(self, ty, op):
        assert self.node_type == 'getitem'
        assert len(self.args) == 1
        if isinstance(ty, base_node):
            ty = ty.getType()
        ty_key = self.args[0].getType()
        if ty_key == 'string':
            ty = Template('map', ['string', ty])
        else:
            ty = Template('vector', [ty])
        self.func.hintType(ty, op)

    def countMutate(self):
        assert self.node_type == 'getitem'
        self.func.countMutate()

    def getChildNodes(self):
        return [self.func] + list(self.args)

    def getChildNodesForBind(self):
        if self.node_type == 'funccall' and isinstance(self.func, node_var) and self.func.name in ['min', 'max', 'sum', 'count_trues', 'count_uniques', 'count_changes', 'any', 'all', 'mean', 'median'] and len(self.args) == 1:
            return [self.func]
        else:
            return [self.func] + list(self.args)

    def visitReserveBind(self):
        if self.node_type == 'funccall' and isinstance(self.func, node_getattr):
            if self.func.member in ['bound', 'where']:
                for arg in self.args:
                    arg.reserveBind()
                self.func.obj.reserveBind()
            elif self.func.member in ['eq_to_end', 'eq_to_rend']:
                self.func.obj.reserveBind()
        if self.node_type == 'funccall' and isinstance(self.func, node_var) and self.func.name in ['min', 'max', 'sum', 'count_trues', 'count_uniques', 'count_changes', 'any', 'all', 'mean', 'median'] and len(self.args) == 1:
            self.reserveBind()
            if self.func.name in ['min', 'max']:
                self.args[0].reserveBind()

    def printResize(self, size, name):
        if self.node_type == 'getitem' and len(self.args) == 1:
            if (isinstance(self.args[0], loop_node) or isinstance(self.args[0], node_ref) or isinstance(self.args[0], node_var)) and str(self.args[0]) == name:
                ty = self.func.getType()
                if isinstance(ty, Template) and ty.name in ['vector']:
                    return ['%s.resize(max((int)%s.size(), (int)(%s)))' % (self.func, self.func, size)]
            return self.func.printResize(size, name)

    def isAggregateFunc(self):
        return self.node_type == 'funccall' and str(self.func) in ['min', 'max', 'sum', 'count_trues', 'count_uniques', 'count_changes', 'any', 'all', 'mean', 'median'] and len(self.args) == 1


call_dic = {
    'funccall': ('(', ')'),
    'template': ('<', '>'),
    'getitem': ('[', ']'),
}
node_template = call_node
node_getitem = call_node
node_funccall = call_node


class node_getattr(base_node):

    def __init__(self, meta, node_type, obj, member):
        assert isinstance(member, node_var)
        self.meta = meta
        self.obj = obj
        self.member = member.name

    def toStr(self):
        return '%s.%s' % (self.obj, self.member)

    def getChildNodes(self):
        return [self.obj, self.member]

    def getReturnType(self):
        ty = self.obj.getType()

        if self.member in ['pop', 'shift', 'top', 'back', 'front']:
            if isinstance(ty, Template) and ty.name in ['vector', 'deque', 'queue', 'priority_queue', 'PriorityQueue', 'fast_pque', 'set_pque', 'xpque', 'stack'] and 1 <= len(ty.args):
                return ty.args[0]
        if self.member in ['lower_bound', 'upper_bound']:
            return 'int'
        if self.member in ['substr']:
            return 'string'
        if isinstance(ty, Template):
            return context.getMethodReturnType(ty, self.member)
        return 'int'


class tuple_node(base_node):

    def __init__(self, meta, node_type, *elements):
        if node_type == 'noinstant_tuple':
            node_type = 'tuple'
        self.meta = meta
        self.node_type = node_type
        if node_type == 'list':
            assert len(elements) == 1
            assert isinstance(elements[0], tuple_node)
            elements = elements[0].getChildNodes()
        self.elements = elements

    def toStr(self):
        if self.node_type == 'list':
            children_type = None
            for child in self.elements:
                children_type = comprehensionType(
                    children_type, child.getType())
            assert children_type is not None
            return 'vector<%s>({%s})' % (typeToStr(children_type), ', '.join(str(e) for e in self.elements))
        else:
            assert self.node_type == 'tuple'
            tuple_expr = ', '.join(str(e) for e in self.elements)
            if isinstance(self.parent, op2_node) and self.parent.node_type == 'assign' or isinstance(self.parent, node_parenthesis):
                if isinstance(self.parent, node_parenthesis) or self.parent.args[-1] == self:
                    prefix = typeToStr(self.getType())
                else:
                    prefix = 'tie'
                return '%s(%s)' % (prefix, tuple_expr, )
            return tuple_expr

    def visitReserveBind(self):
        if isinstance(self.parent, op2_node) and self.parent.node_type == 'assign' and self.parent.args[-1] == self or isinstance(self.parent, group_node):
            L = []
            for i in range(len(self.elements)):
                if not isinstance(self.elements[i], node_var) and not isinstance(self.elements[i], node_ref):
                    L.append(i)
            if 2 <= len(L):
                for i in L:
                    self.elements[i].reserveBind()

    def getType(self):
        if self.node_type == 'list':
            children_type = None
            for child in self.elements:
                child_type = child.getType()
                if child_type is not None:
                    if children_type is None:
                        children_type = child_type
                    elif children_type != child_type:
                        TODO
            assert children_type is not None
            return Template('vector', [children_type])
        else:
            assert self.node_type == 'tuple'
            return Template('tuple', [element.getType() for element in self.elements])

    def defines(self):
        assert self.node_type == 'tuple'
        for element in self.elements:
            element.defines()

    def definesFor(self):
        assert self.node_type == 'tuple'
        for element in self.elements:
            element.definesFor()

    def hintType(self, ty, op):
        assert self.node_type == 'tuple'
        assert op == '='
        if isinstance(ty, tuple_node) and ty.node_type == 'tuple':
            assert len(ty.elements) == len(self.elements)
            for t, e in zip(ty.elements, self.elements):
                e.hintType(t, op)
        else:
            if isinstance(ty, base_node):
                ty = ty.getType()
            if not (isinstance(ty, Template) and ty.name == 'tuple'):
                print('hintType', ty)
            assert isinstance(ty, Template) and ty.name == 'tuple'
            assert len(self.elements) == len(ty.args)
            for e, t in zip(self.elements, ty.args):
                e.hintType(t, op)

    def countMutate(self):
        assert self.node_type == 'tuple'
        for element in self.elements:
            element.countMutate()

    def getChildNodes(self):
        return self.elements

    def printResize(self, size, name):
        ret = []
        for element in self.elements:
            ret.extend(element.printResize(size, name))
        return ret


node_tuple = tuple_node
node_noinstant_tuple = tuple_node
node_list = tuple_node
node_parameters = tuple_node
node_arguments = tuple_node


class node_mod(base_node):

    def __init__(self, meta, node_type, child):
        self.meta = meta
        self.child = child

    def toStr(self):
        return str(self.child)

    def getChildNodes(self):
        return [self.child]


class op2_node(base_node):

    def __init__(self, meta, node_type, *args):
        self.meta = meta
        mod = None
        if node_type == 'assign' and len(args) % 2 == 0:
            mod = args[-1]
            args = args[:-1]
        assert len(args) % 2 == 1
        self.node_type = node_type
        self.args = args
        self.mod = mod
        for i, arg in enumerate(args):
            if i % 2 == 0:
                assert not isinstance(arg, str)
            else:
                assert isinstance(arg, str)

    def isAssignedObject(self, obj):
        if self.node_type != 'assign':
            return False
        for i, arg in enumerate(self.args):
            if not isinstance(arg, str) and arg == obj:
                return i == 0 or i+1 != len(self.args)
            if isinstance(arg, str) and arg != '=':
                return False
        return False

    def visitReserveBind(self):
        if self.node_type == 'comparison' and 5 <= len(self.args):
            for i in range(2, len(self.args) - 2, 2):
                if not isinstance(self.args[i], node_var):
                    self.args[i].reserveBind()
        if self.node_type == 'comparison' and len(self.args) == 3 and self.args[1] in ['in', 'not in']:
            for i in [0, 2]:
                if not isinstance(self.args[i], node_var):
                    self.args[i].reserveBind()

    def toStr(self):
        if self.node_type == 'comparison' and 5 <= len(self.args):
            return '(%s)' % ' && '.join('%s %s %s' % (self.args[i], self.args[i + 1], self.args[i + 2]) for i in range(0, len(self.args) - 2, 2))
        if self.node_type == 'comparison' and len(self.args) == 3 and self.args[1] in ['in', 'not in']:
            ty = self.args[2].getType()
            if isinstance(ty, Template) and ty.name in ['set', 'multiset']:
                return '%s.find(%s)%s%s.end()' % (self.args[2], self.args[0], '==' if self.args[1] == 'not in' else '!=', self.args[2])
            if isinstance(ty, Template) and ty.name in ['vector']:
                return 'find(%s.begin(), %s.end(), %s)%s%s.end()' % (self.args[2], self.args[2], self.args[0], '==' if self.args[1] == 'not in' else '!=', self.args[2])
        ty = self.args[0].getType()
        if self.node_type == 'assign' and isinstance(ty, Template) and ty.name in ['set', 'multiset'] and self.args[1] in ['+=', '-=']:
            assert len(self.args) == 3
            if self.args[1] == '+=':
                return '%s.insert(%s)' % (self.args[0], self.args[2])
            else:
                return '%s.erase(%s.find(%s))' % (self.args[0], self.args[0], self.args[2])
        if self.node_type == 'assign' and self.mod is not None:
            assert len(self.args) == 3
            if self.args[1] == '=':
                return '%s = mod(%s, %s)' % (self.args[0], self.args[2], self.mod)
            elif self.args[1] == '+=':
                return '%s = mod((%s) + (%s), %s)' % (self.args[0], self.args[0], self.args[2], self.mod)
            elif self.args[1] == '-=':
                return '%s = mod((%s) - (%s), %s)' % (self.args[0], self.args[0], self.args[2], self.mod)
            elif self.args[1] == '*=':
                return '%s = mod_mul(%s, %s, %s)' % (self.args[0], self.args[0], self.args[2], self.mod)
            elif self.args[1] == '/=':
                return '%s = mod_div(%s, %s, %s)' % (self.args[0], self.args[0], self.args[2], self.mod)
        if self.node_type == 'assign' and self.args[1] in ['<?=', '>?=']:
            assert len(self.args) == 3
            if self.args[1] == '<?=':
                return '%s = min(%s, %s)' % (self.args[0], castToStr(self.args[0], ty), castToStr(self.args[2], ty))
            else:
                return '%s = max(%s, %s)' % (self.args[0], castToStr(self.args[0], ty), castToStr(self.args[2], ty))
        if self.node_type == 'assign' and len(self.args) == 3 and self.args[1] == '=' and (isinstance(self.args[0], node_var) or isinstance(self.args[0], node_varhint)):
            definition = context.getDefinition(self.args[0].varname.name if isinstance(self.args[0], node_varhint) else self.args[0].name)
            if definition is not None and definition[2] == 2:
                return ''
        return ' '.join(str(arg) for arg in self.args)

    def getType(self):
        if self.node_type == 'comparison':
            return 'bool'
        ty = None
        for i in range(0, len(self.args), 2):
            ty = comprehensionType(ty, self.args[i].getType())
        return ty

    def visitInclude(self):
        if self.node_type == 'assign' and self.mod is not None:
            assert len(self.args) == 3
            if self.args[1] == '=':
                context.visitInclude('mod')
            elif self.args[1] == '+=':
                context.visitInclude('mod')
            elif self.args[1] == '-=':
                context.visitInclude('mod')
            elif self.args[1] == '*=':
                context.visitInclude('mod_mul')
            elif self.args[1] == '/=':
                context.visitInclude('mod_div')

    def getChildNodes(self):
        return list(self.args) + [self.mod]


node_arith = op2_node
node_mul = op2_node
node_shift = op2_node
node_comparison = op2_node
node_assign = op2_node


class op2f_node(base_node):

    def __init__(self, meta, node_type, *args):
        self.meta = meta
        self.node_type = node_type
        self.args = args
        self.op = [node_type]

    def toStr(self):
        return op2f_dic[self.node_type].join(str(s) for s in self.args)

    def getChildNodes(self):
        return self.args

    def getType(self):
        if self.node_type.startswith('logical_'):
            return 'bool'
        ty = None
        for arg in self.args:
            ty = comprehensionType(ty, arg.getType())
        return ty


op2f_dic = {
    'logical_or': ' || ',
    'logical_and': ' && ',
    'bit_or': ' | ',
    'bit_xor': ' ^ ',
    'bit_and': ' & ',
}
node_logical_or = op2f_node
node_logical_and = op2f_node
node_bit_or = op2f_node
node_bit_xor = op2f_node
node_bit_and = op2f_node


class pre_op1_node(base_node):

    def __init__(self, meta, node_type, child_node):
        self.meta = meta
        self.node_type = node_type
        self.child_node = child_node
        self.op = pre_op1_dic[node_type]

    def toStr(self):
        return '%s%s' % (self.op, self.child_node)

    def getChildNodes(self):
        return [self.child_node]

    def getType(self):
        return self.child_node.getType()

    def defines(self):
        assert self.op == '-'
        self.child_node.defines()

    def definesFor(self):
        assert self.op == '-'
        self.child_node.definesFor()

    def hintType(self, ty, op):
        assert self.op == '-'
        self.child_node.hintType(ty, op)

    def countMutate(self):
        assert self.op == '-'
        self.child_node.countMutate()


pre_op1_dic = {
    'logical_not': '!',
    'pre_inc': '++',
    'pre_dec': '--',
    'pre_plus': '+',
    'pre_minus': '-',
    'bit_not': '~',
}
node_logical_not = pre_op1_node
node_pre_inc = pre_op1_node
node_pre_dec = pre_op1_node
node_pre_plus = pre_op1_node
node_pre_minus = pre_op1_node
node_bit_not = pre_op1_node


class post_op1_node(base_node):

    def __init__(self, meta, node_type, child_node):
        self.meta = meta
        self.node_type = node_type
        self.child_node = child_node
        self.op = post_op1_dic[node_type]

    def toStr(self):
        return '%s%s' % (self.child_node, self.op)

    def getChildNodes(self):
        return [self.child_node]


post_op1_dic = {
    'post_inc': '++',
    'post_dec': '--',
}
node_post_inc = post_op1_node
node_post_dec = post_op1_node


class node_if_stmt(base_node):

    def __init__(self, meta, node_type, if_conditions, if_suite, *args):
        self.meta = meta
        self.if_data = (if_conditions, if_suite)
        self.elif_data = [(args[i * 2], args[i * 2 + 1])
                          for i in range(len(args) // 2)]
        self.else_suite = args[-1] if len(args) % 2 == 1 else None
        assert isinstance(if_suite, node_suite)
        for elif_conditions, elif_suite in self.elif_data:
            assert isinstance(elif_suite, node_suite)
        assert self.else_suite is None or isinstance(
            self.else_suite, node_suite)

    def toStr(self):
        TODO

    def getChildNodes(self):
        ret = list(self.if_data)
        for ei in self.elif_data:
            ret += ei
        if self.else_suite is not None:
            ret += [self.else_suite]
        return ret

    def getChildNodesWithoutSuite(self):
        ret = [self.if_data[0]]
        for ei in self.elif_data:
            ret.append(ei[0])
        return ret

    def getSuiteChildNodes(self):
        ret = [self.if_data[1]]
        for ei in self.elif_data:
            ret.append(ei[1])
        if self.else_suite is not None:
            ret += [self.else_suite]
        return ret


class node_assert_stmt(base_node):

    def __init__(self, meta, node_type, conditions, message=None):
        self.meta = meta
        self.conditions = conditions
        self.message = message

    def toStr(self):
        return 'assert(%s)' % (self.conditions, )

    def getChildNodes(self):
        return [self.conditions, self.message]


class node_test(base_node):

    def __init__(self, meta, node_type, conditions, true_value, false_value):
        self.meta = meta
        self.conditions = conditions
        self.true_value = true_value
        self.false_value = false_value

    def getType(self):
        return comprehensionType(self.true_value.getType(), self.false_value.getType())

    def toStr(self):
        return '%s ? %s : %s' % (self.conditions, self.true_value, self.false_value)

    def getChildNodes(self):
        return [self.conditions, self.true_value, self.false_value]


class node_parenthesis(base_node):

    def __init__(self, meta, node_type, child_node):
        self.meta = meta
        self.child_node = child_node

    def toStr(self):
        if isinstance(self.child_node, tuple_node) and self.child_node.node_type == 'tuple':
            return str(self.child_node)
        return '(%s)' % str(self.child_node)

    def getChildNodes(self):
        return [self.child_node]

    def getType(self):
        return self.child_node.getType()


class node_while_stmt(base_node):

    def __init__(self, meta, node_type, conditions, suite, else_suite=None):
        self.meta = meta
        self.conditions = conditions
        self.suite = suite
        self.else_suite = else_suite

    def toStr(self):
        TODO

    def getChildNodes(self):
        ret = [self.conditions, self.suite]
        if self.else_suite is not None:
            ret.append(self.else_suite)
        return ret

    def getChildNodesWithoutSuite(self):
        return [self.conditions]

    def getSuiteChildNodes(self):
        ret = [self.suite]
        if self.else_suite is not None:
            ret.append(self.else_suite)
        return ret


class node_for_stmt(base_node):

    def __init__(self, meta, node_type, *args):
        args = list(args)
        self.meta = meta
        self.decorators = []
        while isinstance(args[0], node_decorator):
            assert args[0].name in 'parallel', 'for文は@parallel以外のデコレータに対応していません'
            self.decorators.append(args.pop(0))
        self.temporary = args.pop(0)
        self.container = args.pop(0)
        self.suite = args.pop(0)
        self.else_suite = args.pop(0) if args else None

    def hasParallel(self):
        for decorator in self.decorators:
            assert isinstance(decorator, node_decorator)
            if decorator.name == 'parallel':
                return True
        return False

    def getParallelConstructorCall(self):
        for decorator in self.decorators:
            assert isinstance(decorator, node_decorator)
            if decorator.name == 'parallel':
                for arg in decorator.args:
                    if isinstance(arg, node_argvalue) and arg.name == 'workers':
                        return '(%s)' % (arg.value, )
        return ''

    def getParallelWorkerIdVarName(self):
        for decorator in self.decorators:
            assert isinstance(decorator, node_decorator)
            if decorator.name == 'parallel':
                for arg in decorator.args:
                    if isinstance(arg, node_argvalue) and arg.name == 'worker_id':
                        return arg.value
        return '$worker_id'

    def getChildNodes(self):
        return self.getChildNodesWithoutSuite() + self.getSuiteChildNodes()

    def getChildNodesWithoutSuite(self):
        return self.decorators + [self.temporary, self.container]

    def getSuiteChildNodes(self):
        ret = [self.suite]
        if self.else_suite is not None:
            ret.append(self.else_suite)
        return ret

    def visitReserveBind(self):
        if isinstance(self.container, call_node) and self.container.node_type == 'funccall' and isinstance(self.container.func, node_var) and self.container.func.name == 'move':
            self.container.reserveBind()


class node_break_stmt(base_node):

    def __init__(self, meta, node_type):
        self.meta = meta

    def toStr(self):
        return 'goto %s' % context.get_break()

    def getChildNodes(self):
        return []


class node_continue_stmt(base_node):

    def __init__(self, meta, node_type):
        self.meta = meta

    def toStr(self):
        return context.get_continue()

    def getChildNodes(self):
        return []


class node_return_stmt(base_node):

    def __init__(self, meta, node_type, expr=None):
        self.meta = meta
        self.expr = expr

    def toStr(self):
        if self.expr is not None:
            return 'return %s' % (self.expr, )
        if context.is_main and not use_pybind11:
            return 'return 0'
        return 'return'

    def getChildNodes(self):
        if self.expr is not None:
            return [self.expr]
        return []


def symbol_node(meta, node_type, *op):
    return ' '.join(op)


node_assign_op = symbol_node
node_comp_op = symbol_node
node_arith_op = symbol_node
node_mul_op = symbol_node
node_shift_op = symbol_node
node_instant_op = symbol_node


class node_pass_stmt(base_node):

    def __init__(self, meta, node_type):
        self.meta = meta

    def toStr(self):
        return ''

    def getChildNodes(self):
        return []
