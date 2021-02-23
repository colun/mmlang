import os
import sys
import shutil
import subprocess
import urllib.request
import lark
import lark.indenter
import json
import mmcore

SRC_DIR = os.path.dirname(__file__)
ROOT_DIR = os.path.dirname(SRC_DIR)
WORK_DIR = os.path.join(ROOT_DIR, '.work')

grammer = open('%s/mmlang.lark' % (SRC_DIR, )).read()

class MyIndenter(lark.indenter.Indenter):
    NL_type, INDENT_type, DEDENT_type, tab_len = '_NEWLINE', '_INDENT', '_DEDENT', 8
    OPEN_PAREN_types = ['LPAR', 'LSQB', 'LBRACE', 'LESS']
    CLOSE_PAREN_types = ['RPAR', 'RSQB', 'RBRACE', 'GREATER']

parser = lark.Lark(grammer, parser='lalr', postlex=MyIndenter())

def expand(node):
    if isinstance(node, lark.Token):
        return node.value
    else:
        assert isinstance(node, lark.Tree)
        return mmcore.__dict__['node_' + node.data](node.data, *(expand(child) for child in node.children))


def compile_raw(target_path):
    try:
        program = open(target_path).read()
        print('//#define NDEBUG')
        print('#ifdef mmlang_source_%s_lines' % (len(program.rstrip().split('\n')), ))
        print()
        print(program.rstrip())
        print()
        print('#endif')
        print()
        tree = parser.parse(program)
        tree = expand(tree)
        tree.start()
    except Exception as e:
        sys.excepthook(*(sys.exc_info()))

def compile_file(origin_outs, target_path, output_path):
    os.makedirs(os.path.dirname(output_path), exist_ok=True)
    with open(output_path, 'w') as f:
        sys.stdout, sys.stderr = f, f
        compile_raw(target_path)
        sys.stdout, sys.stderr = origin_outs


def compile_folder(origin_outs, target_path, output_path):
    assert os.path.isdir(target_path)
    for name in os.listdir(target_path):
        path = os.path.join(target_path, name)
        if os.path.isdir(path):
            compile_folder(origin_outs, path, os.path.join(output_path, name))
        elif name.endswith('.m2'):
            compile_file(origin_outs, path, os.path.join(output_path, name[:-3] + '.cpp'))


def main():
    target_path = None
    output_path = None
    run_flag = False
    arg_state = None
    for arg in sys.argv[1:]:
        if arg in ['-run', '--run']:
            run_flag = True
            continue
        if arg in ['-o', '-output', '--output']:
            arg_state = 'output'
            continue
        if arg_state == 'output':
            arg_state = None
            output_path = arg
            continue
        assert target_path is None, '対象ファイルまたはフォルダは1つだけ指定可能です'
        target_path = arg
    assert arg_state is None, '引数オプション%sの後に必要なパラメータが設定されていません'
    if target_path is None:
        print('USAGE: mm [--output OUTPUT] [--run] TARGET')
        print('  --output OUTPUT : output file or folder(.cpp files)')
        print('  --run           : run mode')
        print('  TARGET          : target file or folder(.m2 files)')
        exit()

    origin_outs = sys.stdout, sys.stderr
    if os.path.isdir(target_path):
        assert output_path is not None, '対象がフォルダの場合には、--output指定が必須となります'
        assert not run_flag, '対象がフォルダの場合には、--run指定はできません'
        assert not os.path.exists(output_path) or os.path.isdir(output_path), '対象がフォルダの場合には、--output指定もフォルダとしてください'
        compile_folder(origin_outs, target_path, output_path)
    else:
        if output_path is not None and os.path.isdir(output_path):
            name, ext = os.path.splitext(os.path.basename(target_path))
            output_path = os.path.join(output_path, name + '.cpp')
        stdout_flag = output_path is None
        if stdout_flag:
            output_path = os.path.join(WORK_DIR, 'output', 'output.cpp')
        compile_file(origin_outs, target_path, output_path)
        if run_flag:
            if os.path.exists('a.out'):
                os.remove('a.out')
            subprocess.run(['g++', '-std=gnu++1y', '-DGV_ENABLE', '-O2', output_path])
            if os.path.exists('a.out'):
                stdout_flag = False
                if os.path.exists('result.gv'):
                    os.remove('result.gv')
                subprocess.run(['./a.out'])
                if os.path.exists('result.gv'):
                    gvc_path = os.path.join(WORK_DIR, 'gvc', 'gvc.jar')
                    if not os.path.exists(gvc_path):
                        os.makedirs(os.path.dirname(gvc_path), exist_ok=True)
                        open(gvc_path, 'wb').write(urllib.request.urlopen('https://github.com/colun/gvc/raw/master/gvc.jar').read())
                    subprocess.run(['java', '-jar', gvc_path, 'result.gv'])
        if stdout_flag:
            print(open(output_path, 'r').read())


if __name__ == '__main__':
    main()
