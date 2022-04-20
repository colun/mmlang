#!/usr/bin/env python3
import os
import sys
import traceback
import shutil
import subprocess
import urllib.request
import lark
import lark.indenter
import json
import mmcore
import concurrent.futures
import re
import colorama
import hashlib
import copy
import time

SRC_DIR = os.path.dirname(__file__)
ROOT_DIR = os.path.dirname(SRC_DIR)
WORK_DIR = os.path.join(ROOT_DIR, '.work')

grammer = open('%s/mmlang.lark' % (SRC_DIR, )).read()

class MyIndenter(lark.indenter.Indenter):
    NL_type, INDENT_type, DEDENT_type, tab_len = '_NEWLINE', '_INDENT', '_DEDENT', 8
    OPEN_PAREN_types = ['LPAR', 'LSQB', 'LBRACE', 'LESS']
    CLOSE_PAREN_types = ['RPAR', 'RSQB', 'RBRACE', 'GREATER']

parser = lark.Lark(grammer, parser='lalr', postlex=MyIndenter(), propagate_positions=True)

def expand(node):
    if isinstance(node, lark.Token):
        return node.value
    else:
        assert isinstance(node, lark.Tree)
        return mmcore.__dict__['node_' + node.data](node.meta.__dict__, node.data, *(expand(child) for child in node.children))


def compile_raw(args):
    try:
        print('/** mmlang_source_%s_lines' % (len(args.source.rstrip().split('\n')), ))
        print()
        print(args.source.rstrip())
        print()
        print('*/')
        print()
        print('#ifndef MM$DBG')
        print('#define NDEBUG')
        print('#endif')
        print('#ifdef MM$LOCAL')
        print('#define __local__ true')
        print('#else')
        print('#define __local__ false')
        print('#endif')
        print('#ifdef MM$TEST')
        print('#define __test__ true')
        print('#else')
        print('#define __test__ false')
        print('#endif')
        print()
        tree = parser.parse(args.source)
        tree = expand(tree)
        tree.start(args.profiler)
    except Exception as e:
        info = sys.exc_info()
        #sys.excepthook(*info)
        print('Traceback (most recent call last):')
        tb = info[2]
        while True:
            traceback.print_tb(tb, 1)
            if hasattr(tb, 'tb_frame') and hasattr(tb.tb_frame, 'f_locals') and 'self' in tb.tb_frame.f_locals:
                self_obj = tb.tb_frame.f_locals['self']
                if isinstance(self_obj, mmcore.base_node) and hasattr(self_obj, 'meta'):
                    #print(type(self_obj), self_obj.meta)
                    print(args.source[self_obj.meta['start_pos']:self_obj.meta['end_pos']])
                    #break
            print()
            if hasattr(tb, 'tb_next'):
                tb = tb.tb_next
            else:
                break
        print('%s: %s' % (info[0].__name__, info[1]))


def test_atcoder(args):
    args.stdout_flag = False
    m = re.search(r'# *atcoder *: *(https://[\-\w\./]*)', args.source)
    url, contest_id, task_id = None, None, None
    if m:
        url = m.group(1)
        m = re.match(r'https://([\-\w]+).contest.atcoder.jp/tasks/([\-\w]+)', url)
        if m:
            contest_id, task_id = m.group(1), m.group(2)
        m = re.match(r'https://atcoder.jp/contests/([\-\w]+)/tasks/([\-\w]+)', url)
        if m:
            contest_id, task_id = m.group(1), m.group(2)
    else:
        m = re.search(r'# *atcoder *: *([\-0-9A-Za-z]+)(_[0-9A-Za-z]+)', args.source)
        if m:
            contest_id, task_id = m.group(1), m.group(2)
    if contest_id is not None:
        ac_workspace = os.path.join(WORK_DIR, 'atcoder')
        contest_dir = os.path.join(ac_workspace, contest_id)
        if not os.path.exists(contest_dir):
            subprocess.run(['atcoder-tools', 'gen', '--workspace', ac_workspace, contest_id])
        for name in os.listdir(contest_dir):
            task_dir = os.path.join(contest_dir, name)
            if not os.path.isdir(task_dir):
                continue
            metadata_path = os.path.join(task_dir, 'metadata.json')
            if not os.path.exists(metadata_path):
                continue
            with open(metadata_path, 'rt') as f:
                metadata = json.load(f)
            if url is not None:
                if metadata['problem']['problem_id']!=task_id:
                    continue
            else:
                if metadata['problem']['problem_id'][-len(task_id):]!=task_id:
                    continue
                url = f'https://atcoder.jp/contests/{metadata["problem"]["contest"]["contest_id"]}/tasks/{metadata["problem"]["problem_id"]}'
            assert metadata['code_filename']=='main.cpp'
            open(os.path.join(task_dir, 'main.cpp'), 'w').write(open(args.output_path, 'r').read())
            print(f' === {args.target_path} ( {url} ) ===')
            submission_url = None
            ret_kind = -1
            ret = subprocess.run(['atcoder-tools', 'compile', '--compile-command', 'g++ -std=gnu++14 -DMM\\$DBG -DMM\\$LOCAL -DMM\\$TEST -O2 main.cpp -o main'], cwd=task_dir)
            if ret.returncode==0:
                ret_kind = 0
                if args.ac_test:
                    ret = subprocess.run(['atcoder-tools', 'test'], cwd=task_dir, stderr=subprocess.PIPE)
                elif args.ac_submit:
                    ret = subprocess.run(['atcoder-tools', 'submit'] + (['-u'] if args.ac_submit2 else []), cwd=task_dir, stderr=subprocess.PIPE)
                print(ret.stderr.decode())
                m = re.search(r'https://[\.\-/\w]+/submissions/[\.\-/\w]+', ret.stderr.decode())
                if m:
                    submission_url = m.group(0)
                if ret.returncode==0:
                    ret_kind = 1
            args.ac_results.append((args.target_path, url, ret_kind, submission_url))


def download(url, dst):
    if os.path.exists(dst):
        return
    cache = os.path.join(WORK_DIR, 'download_cache', hashlib.sha1(url.encode()).hexdigest())
    if not os.path.exists(cache):
        os.makedirs(os.path.join(WORK_DIR, 'download_cache'), exist_ok=True)
        import requests
        with open(cache, 'wb') as f:
            f.write(requests.get(url).content)
    with open(cache, 'rb') as f:
        with open(dst, 'wb') as f2:
            f2.write(f.read())

def downloads(args):
    for m in re.finditer(r'# *download *: *(https?://[^ \n]+) +([^ \n]+)', args.source):
        download(m.group(1), os.path.join(args.cwd, m.group(2)))


def build(args):
    if hasattr(args, 'build_success'):
        return args.build_success
    args.aout = os.path.join(args.cwd, 'a.out')
    if os.path.exists(args.aout):
        os.remove(args.aout)
    options = []
    options.append('-DMM$LOCAL')
    if args.debug:
        options.append('-DMM$DBG')
    if args.vis:
        options.append('-DMM$VIS')
    if args.test:
        options.append('-DMM$TEST')
    if args.profiler:
        options.append('-pthread')
    subprocess.run(['g++', '-std=gnu++14'] + options + ['-O2', args.output_path, '-o', args.aout])
    args.build_success = os.path.exists(args.aout)
    if args.build_success:
        args.stdout_flag = False
        downloads(args)
    return args.build_success


def test(args):
    m = re.search(r'# *command *: *(.+)', args.source)
    command = m.group(1).replace('{$AOUT}', './a.out') if m else None
    m = re.search(r'# *in_dir *: *(.+)', args.source)
    in_dir = m.group(1) if m else None
    tests = None
    if in_dir or not command:
        tests = {}
        if in_dir is None:
            in_dir = 'in'
        if os.path.isdir(in_dir):
            for fn in os.listdir(in_dir):
                if fn.endswith('.txt') and fn[:-4].isnumeric():
                    tests[int(fn[:-4])] = os.path.join(in_dir, fn)
        assert 1<=len(tests)
    if args.test_seed is not None:
        if os.path.exists('result.gv'):
            os.remove('result.gv')
        seed = tests[args.test_seed] if tests else args.test_seed
        if command:
            subprocess.run(command.replace('{$SEED}', seed).split(' '), cwd=args.cwd)
        else:
            subprocess.run(['./a.out'], stdin=open(tests[args.test_seed]), cwd=args.cwd)
        if os.path.exists('result.gv'):
            gvc_path = os.path.join(WORK_DIR, 'gvc', 'gvc.jar')
            download('https://github.com/colun/gvc/raw/master/gvc.jar', gvc_path)
            subprocess.run(['java', '-jar', gvc_path, 'result.gv'])
    else:
        if tests is None:
            test_size = int(args.test_size) if args.test_size is not None else 100
            tests = {i: str(test_size + i) for i in range(test_size)}
        max_workers = args.workers
        if max_workers is None:
            import psutil
            max_workers = max(1, psutil.cpu_count(logical=False) - 1)
        import sqlite3
        import datetime
        created_at = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
        db = sqlite3.connect('mm.sqlite3')
        cur = db.cursor()
        cur.execute('create table if not exists runs(run_id integer not null primary key autoincrement, name text null, source text null, created_at text)')
        cur.execute('create table if not exists scores(run_id integer, test_id integer, score real)')
        cur.execute('create index if not exists scores_run on scores(run_id, test_id)')
        cur.execute('create index if not exists scores_test on scores(test_id, run_id)')
        cur.execute('create table if not exists results(run_id integer, test_id integer, sec real, stdout text, stderr text, gv text null)')
        cur.execute('create index if not exists results_run on results(run_id, test_id)')
        cur.execute('insert into runs(name, source, created_at) values (?, ?, ?)', (args.test_name, args.source, created_at))
        run_id = cur.lastrowid
        print(f'run_id: {run_id}, created_at: {created_at}')
        executor = concurrent.futures.ThreadPoolExecutor(max_workers=max_workers)
        def exec_test(test_id, in_path):
            score = 0
            start_time = time.time()
            if command:
                completed = subprocess.run(command.replace('{$SEED}', in_path).split(' '), cwd=args.cwd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, encoding='utf-8')
            else:
                completed = subprocess.run(['./a.out'], stdin=open(in_path), cwd=args.cwd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, encoding='utf-8')
            sec = time.time() - start_time
            for line in completed.stderr.split("\n") + completed.stdout.split("\n"):
                m = re.match(r'^[Ss]core *[:=] *([\d\.]+)', line)
                if m:
                    score = float(m.group(1))
            return test_id, score, sec, completed.stdout, completed.stderr
        test_ids = sorted(tests.keys())
        if args.test_size is not None:
            test_ids = test_ids[:args.test_size]
        tasks = [executor.submit(exec_test, test_id, tests[test_id]) for test_id in test_ids]
        scores = []
        for task in tasks:
            test_id, score, sec, result_stdout, result_stderr = task.result()
            scores.append(score)
            print('%d/%d\r' % (len(scores), len(tasks)), end='')
            cur.execute('insert into scores(run_id, test_id, score) values (?, ?, ?)', (run_id, test_id, score))
            cur.execute('insert into results(run_id, test_id, sec, stdout, stderr, gv) values (?, ?, ?, ?, ?, ?)', (run_id, test_id, sec, result_stdout, result_stderr, None))
        print('score: %f %s' % (sum(scores) / len(scores), scores))
        db.commit()
        db.close()


def run(args):
    if os.path.exists('result.gv'):
        os.remove('result.gv')
    import datetime
    start_time = datetime.datetime.now()
    subprocess.run([args.aout])
    end_time = datetime.datetime.now()
    spend_time = str(end_time - start_time)
    if spend_time.startswith('0:'):
        spend_time = spend_time[2:]
        if spend_time.startswith('00:'):
            spend_time = spend_time[3:] + ' sec'
        if spend_time.startswith('0'):
            spend_time = spend_time[1:]
    print(f'time: {spend_time}')
    if os.path.exists('result.gv'):
        gvc_path = os.path.join(WORK_DIR, 'gvc', 'gvc.jar')
        download('https://github.com/colun/gvc/raw/master/gvc.jar', gvc_path)
        subprocess.run(['java', '-jar', gvc_path, 'result.gv'])


def compile_file(origin_outs, target_path, output_path, args):
    args = copy.copy(args)
    args.output_path = output_path
    args.target_path = target_path
    args.source = open(target_path).read()
    if args.folder_mode:
        args.cwd = os.path.join(WORK_DIR, 'work', hashlib.sha1(args.target_path.encode()).hexdigest())
        os.makedirs(args.cwd, exist_ok=True)
        if args.output_path is None:
            args.output_path = os.path.join(args.cwd, 'output.cpp')
    else:
        args.cwd = '.'
        if args.output_path is None:
            args.output_path = os.path.join(WORK_DIR, 'output', 'output.cpp')
    dir_path = os.path.dirname(args.output_path)
    if dir_path:
        os.makedirs(dir_path, exist_ok=True)
    with open(args.output_path, 'w') as f:
        sys.stdout, sys.stderr = f, f
        compile_raw(args)
        sys.stdout, sys.stderr = origin_outs
    if args.ac_test or args.ac_submit:
        test_atcoder(args)
    if args.build or args.run or args.test:
        if build(args):
            if args.test:
                test(args)
            elif args.run:
                run(args)
    if args.stdout_flag:
        print(open(args.output_path, 'r').read())


def compile_folder(origin_outs, target_path, output_path, args):
    assert os.path.isdir(target_path)
    for name in sorted(os.listdir(target_path)):
        path = os.path.join(target_path, name)
        if os.path.isdir(path):
            compile_folder(origin_outs, path, None if output_path is None else os.path.join(output_path, name), args)
        elif name.endswith('.m2'):
            compile_file(origin_outs, path, None if output_path is None else os.path.join(output_path, name[:-3] + '.cpp'), args)


def codegen_atcoder(output_path, client, problem):
    import atcodertools.fmtprediction.predict_format
    import atcodertools.fmtprediction.models
    def gen_input_code(ty):
        if ty is atcodertools.fmtprediction.models.type.Type.int:
            return 'inputInt()'
        elif ty is atcodertools.fmtprediction.models.type.Type.str:
            return 'inputWord()'
        else:
            print(ty)
            assert False, '未対応のタイプ'
    codes = []
    single_patterns = []
    def gen_single_patterns():
        nonlocal single_patterns
        if len(single_patterns):
            codes.append(f'%s = %s' % (', '.join([p[0] for p in single_patterns]), ', '.join([p[1] for p in single_patterns])))
            single_patterns = []
    contest_id = problem.get_contest().contest_id
    task_suffix = problem.problem_id.split('_')[-1]
    codes.append(f'# atcoder: {contest_id}_{task_suffix} ( {problem.get_url()} )')
    content = client.download_problem_content(problem)
    result = atcodertools.fmtprediction.predict_format.predict_format(content)
    for seq in result.format.sequence:
        if isinstance(seq, atcodertools.fmtprediction.models.format.SingularPattern):
            assert seq.var.first_index is None
            assert seq.var.second_index is None
            single_patterns.append((seq.var.name, gen_input_code(seq.var.type)))
        else:
            gen_single_patterns()
            if isinstance(seq, atcodertools.fmtprediction.models.format.TwoDimensionalPattern):
                if seq.var.second_index is not None:
                    assert seq.var.first_index is not None
                    codes.append(f'{seq.var.name}[:{seq.var.first_index.get_length()}][:{seq.var.second_index.get_length()}] = {gen_input_code(seq.var.type)}')
                else:
                    assert False, '未対応の2次元パターン'
            elif isinstance(seq, atcodertools.fmtprediction.models.format.ParallelPattern):
                names = []
                inputs = []
                for i, var in enumerate(seq.vars):
                    if i==0:
                        names.append(f'{var.name}[:{seq.loop_index.get_length()}]')
                    else:
                        names.append(f'{var.name}[$1]')
                    inputs.append(gen_input_code(var.type))
                codes.append('%s = %s' % (', '.join(names), ', '.join(inputs)))
            else:
                print(seq)
                assert False, '未対応のパターン'
    gen_single_patterns()
    codes.append('')
    with open(output_path, 'wt') as f:
        f.write("\n".join(codes))


def gen_atcoder(target_path):
    target_id, target_ext = os.path.splitext(os.path.basename(target_path))
    if target_ext=='':
        contest_id = target_id
        problem_suffix = None
    else:
        target_parts = target_id.split('_')
        assert len(target_parts)==2, '問題IDは1つの_を含む必要があります'
        contest_id, problem_suffix = target_parts
    print(f'contest_id: {contest_id}')
    import atcodertools.tools.codegen
    import atcodertools.client.atcoder
    contest = atcodertools.client.atcoder.Contest(contest_id=contest_id)
    client = atcodertools.client.atcoder.AtCoderClient()
    problem_list = client.download_problem_list(contest)
    for problem in problem_list:
        problem_parts = problem.problem_id.split('_')
        if problem_suffix is None or problem_suffix==problem_parts[-1]:
            print(f'gen  ... {problem.problem_id} ( {problem.get_url()} )')
            if target_ext=='':
                os.makedirs(target_path, exist_ok=True)
                output_path = os.path.join(target_path, f'{problem.problem_id}.m2')
            else:
                output_path = target_path
            codegen_atcoder(output_path, client, problem)
        else:
            print(f'skip ... {problem.problem_id} ( {problem.get_url()} )')


def main():
    import argparse
    parser = argparse.ArgumentParser(description='MM Language 2.0.0')
    parser.add_argument('target', metavar='SOURCE', help='source code file/folder')
    arggroup = parser.add_argument_group('build / run / test')
    arggroup.add_argument('--output', metavar='OUTPUT', help='output file/folder')
    arggroup.add_argument('--build', action='store_true', help='compile')
    arggroup.add_argument('--run', action='store_true', help='compile + execute')
    arggroup.add_argument('--test', action='store_true', help='compile + test using `in` folder')
    arggroup.add_argument('--test-size', metavar='N', type=int, default=None, help='use N testcases in `in` folder')
    arggroup.add_argument('--test-seed', metavar='N', type=int, default=None, help='use only N-nd testcase in `in` folder')
    arggroup.add_argument('--test1', dest='test_seed', action='store_const', const=1, help='use only first testcase in `in` folder')
    arggroup.add_argument('--test-name', metavar='NAME', help='name the test NAME')
    arggroup.add_argument('--workers', metavar='N', type=int, default=None, help='count of worker processes')
    arggroup = parser.add_argument_group('build options')
    arggroup.add_argument('--profiler', action='store_true', help='profiler on')
    arggroup.add_argument('--ndebug', dest='debug', action='store_false', help='debug off')
    arggroup.add_argument('--novis', dest='vis', action='store_false', help='visualizer off')
    arggroup.add_argument('--release', dest='release', action='store_true', help='debug off, visualizer off')
    arggroup = parser.add_argument_group('using atcoder-tools')
    arggroup.add_argument('--ac-gen', action='store_true', help='generate code using atcoder-tools')
    arggroup.add_argument('--ac-test', action='store_true', help='test using atcoder-tools')
    arggroup.add_argument('--ac-submit', action='store_true', help='submit using atcoder-tools')
    arggroup.add_argument('--ac-submit-folder', action='store_true', help='folder submit using atcoder-tools')
    arggroup.add_argument('--ac-submit2', action='store_true', help='unlock safety submit using atcoder-tools')
    args = parser.parse_args()

    args.ac_results = []
    args.stdout_flag = None
    if args.test_size is not None or args.test_seed is not None:
        args.test = True
    if args.ac_submit_folder or args.ac_submit2:
        args.ac_submit = True

    origin_outs = sys.stdout, sys.stderr
    if args.ac_gen:
        assert not os.path.exists(args.target), '対象が既に存在しているため、--ac-genは使えません'
        gen_atcoder(args.target)
    elif os.path.isdir(args.target):
        assert args.ac_test or args.ac_submit or args.output is not None, '対象がフォルダの場合には、--output, --ac-testのいずれかの指定が必須となります'
        assert not args.build, '対象がフォルダの場合には、--build指定はできません'
        assert not args.run, '対象がフォルダの場合には、--run指定はできません'
        assert not args.test, '対象がフォルダの場合には、--test指定はできません'
        assert args.output is None or not os.path.exists(args.output) or os.path.isdir(args.output), '--outputの指定先は、存在しないかフォルダである必要があります'
        assert not args.ac_submit or args.ac_submit_folder, '対象がフォルダの場合の--ac-submitは、代わりに--ac-submit-folderを使用してください'
        args.folder_mode = True
        compile_folder(origin_outs, args.target, args.output, args)

        def getSuccessString(success):
            if success==-1:
                return colorama.Style.BRIGHT + colorama.Fore.RED + 'ERROR' + colorama.Style.RESET_ALL
            elif success==0:
                return colorama.Style.BRIGHT + colorama.Fore.YELLOW + 'FAILURE' + colorama.Style.RESET_ALL
            elif success==1:
                return colorama.Style.BRIGHT + colorama.Fore.GREEN + 'SUCCESS' + colorama.Style.RESET_ALL
            else:
                assert False
        for target_path, task_url, success, submission_url in args.ac_results:
            print('%s ( %s ) ... %s%s' % (target_path, task_url,  getSuccessString(success), '' if submission_url is None else f' {submission_url}'))
    else:
        args.folder_mode = False
        output_path = args.output
        if output_path is not None and os.path.isdir(args.output):
            name, ext = os.path.splitext(os.path.basename(args.target))
            output_path = os.path.join(output_path, name + '.cpp')
        args.stdout_flag = output_path is None
        compile_file(origin_outs, args.target, output_path, args)


if __name__ == '__main__':
    main()
