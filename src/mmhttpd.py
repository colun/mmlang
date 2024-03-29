import time
import math
import os.path
import re
import html

def calcScore(now, best):
    if now<1e-9:
        return 0
    #return now / best
    return best / now

def isBetter(now, best):
    if now<1e-9:
        return False
    #return best < now
    return now < best

def main(callback=None):
    start_time = time.time()
    import sqlite3
    db = sqlite3.connect('mm.sqlite3')
    cur = db.cursor()
    cur.execute('select run_id, name, source, created_at from runs')
    CREATED_AT = {}
    NAME = {}
    SRC = {}
    for run_id, name, source, created_at in cur.fetchall():
        NAME[run_id] = name
        SRC[run_id] = source
        CREATED_AT[run_id] = created_at
    cur.execute('select run_id, test_id, sec, stdout, stderr from results order by run_id, test_id')
    R = {}
    for run_id, test_id, sec, text_stdout, text_stderr in cur.fetchall():
        if run_id not in R:
            R[run_id] = 0
        R[run_id] += 1
    pattern = re.compile(r'(\w+) *[=:] *([\d\.]+)')
    T = {}
    S = {}
    TIME = {}
    cur.execute('select run_id, test_id, sec, stdout, stderr from results order by run_id, test_id')
    for run_id, test_id, sec, text_stdout, text_stderr in cur.fetchall():
        if R[run_id] != 100:
            continue
        if run_id not in S:
            S[run_id] = {}
        if test_id not in T:
            T[test_id] = {}
        if run_id not in TIME:
            TIME[run_id] = [sec, sec, sec]
        else:
            TIME[run_id][0] = min(TIME[run_id][0], sec)
            TIME[run_id][1] += sec
            TIME[run_id][2] = max(TIME[run_id][2], sec)
        S[run_id][test_id] = -1
        for text in (text_stdout, text_stderr):
            for line in text.split("\n"):
                m = pattern.match(line)
                if m:
                    if m.group(1).lower()=='score':
                        S[run_id][test_id] = float(m.group(2))
                    else:
                        T[test_id][m.group(1)] = float(m.group(2))
    BEST = {}
    BEST_COUNT = {}
    for run_id in S:
        for test_id in S[run_id]:
            if test_id not in BEST or isBetter(S[run_id][test_id], BEST[test_id]):
                BEST[test_id] = S[run_id][test_id]
                BEST_COUNT[test_id] = 1
            elif BEST[test_id] == S[run_id][test_id]:
                BEST_COUNT[test_id] += 1
    T2 = {}
    for test_id in T:
        for name in T[test_id]:
            if name not in T2:
                T2[name] = []
            T2[name].append(T[test_id][name])
    T2 = {name: sorted(T2[name]) for name in T2}
    print(T2)
    def splitKind(values, split_n=3):
        target = len(values) / split_n
        DP = [[None]*len(values) for i in range(split_n)]
        DP[0][0] = [0, []]
        for o in range(split_n-1):
            for i in range(1, len(values)):
                best = [None, None]
                if values[i-1]!=values[i]:
                    for j in range(i):
                        dp = DP[o][j]
                        if dp is not None:
                            sc = dp[0] + math.log2(i-j)
                            if best[0] is None or best[0] < sc:
                                best[0] = sc
                                best[1] = dp[1] + [i]
                if best[0] is not None:
                    DP[o+1][i] = best
        best = [None, None]
        for j in range(len(values)):
            dp = DP[split_n-1][j]
            if dp is not None:
                sc = dp[0] + math.log2(len(values)-j)
                if best[0] is None or best[0] < sc:
                    best[0] = sc
                    best[1] = dp[1]
        if best[0] is None:
            return None
        ret = []
        for o in range(split_n-1):
            best_i = best[1][o]
            for j in range(10):
                sep2 = ('{:.%df}' % (j, )).format((values[best_i-1]+values[best_i])/2)
                sep2_f = float(sep2)
                if values[best_i-1] < sep2_f < values[best_i]:
                    break
            ret.append(sep2)
        return tuple(ret)
    T3 = {name: splitKind(T2[name], 3) for name in T2}
    print(T3)
    T5 = {name: splitKind(T2[name], 5) for name in T2}
    print(T5)
    import http.server
    import urllib.parse
    class MyHandler(http.server.BaseHTTPRequestHandler):
        def getSource(self, query):
            self.send_response(200)
            self.send_header('Content-Type', 'text/plain; charset=utf-8')
            self.end_headers()
            run_id = int(query.get('id', [])[0])
            self.wfile.write(SRC[run_id].encode())
        def getDetail(self, query):
            self.send_response(200)
            self.send_header('Content-Type', 'text/html; charset=utf-8')
            self.end_headers()
            param = query.get('PARAM', [])
            run_id = int(query.get('id', [])[0])
            query.pop('id')
            htmls = []
            htmls.append('<html>')
            htmls.append('<head>')
            htmls.append('<title>MM Analytics</title>')
            htmls.append('</head>')
            htmls.append('<body>')
            htmls.append(f'<h3>Name: {html.escape(f"{NAME[run_id]}")}</h3>')
            htmls.append(f'<a href="/?{urllib.parse.urlencode(query, True)}">[TOP]</a>')
            htmls.append(f'<a href="/source?id={run_id}">[SOURCE]</a>')
            htmls.append('<hr />')
            if 2<=len(param):
                sum_score = [0]*9
                sum2_score = [0]*9
                count_score = [0]*9
                bests = [0]*9
                uniques = [0]*9
                fails = [0]*9
                for test_id in S[run_id]:
                    kind = 4
                    if T[test_id][param[0]]<float(T3[param[0]][0]):
                        kind -= 1
                    elif float(T3[param[0]][1])<T[test_id][param[0]]:
                        kind += 1
                    if T[test_id][param[1]]<float(T3[param[1]][0]):
                        kind -= 3
                    elif float(T3[param[1]][1])<T[test_id][param[1]]:
                        kind += 3
                    if 0 < S[run_id][test_id]:
                        sc1 = calcScore(S[run_id][test_id], BEST[test_id])
                        sum_score[kind] += sc1
                        sum2_score[kind] += sc1*sc1
                    else:
                        fails[kind] += 1
                    count_score[kind] += 1
                    if BEST[test_id] == S[run_id][test_id]:
                        bests[kind] += 1
                        if BEST_COUNT[test_id]==1:
                            uniques[kind] += 1
                #for kind in range(3):
                #    score = '{:.3f}'.format(100 * sum_score[kind] / count_score[kind])
                #    htmls.append(f'<td align="right">{score}</td><td align="right">{bests[kind]}</td><td align="right">{uniques[kind]}</td><td align="right">{fails[kind]}</td>')
                htmls.append('<table border="1">')
                htmls.append(f'<tr><td rowspan="2"></td><th colspan="6">{T3[param[0]][0]}&gt;</th><th colspan="6">{param[0]}</th><th colspan="6">&gt;{T3[param[0]][1]}</th></tr>')
                htmls.append('<tr>')
                for i in range(3):
                    htmls.append('<th>Score</th><th>Std</th><th>Bests</th><th>Uniqs</th><th>Fails</th><th>Tests</th>')
                htmls.append('</tr>')
                labels = [f'{T3[param[1]][0]}&gt;', f'{param[1]}', f'&gt;{T3[param[1]][1]}']
                for y in range(3):
                    htmls.append(f'<tr><th>{labels[y]}</th>')
                    for x in range(3):
                        kind = y * 3 + x
                        avg_score = sum_score[kind] / count_score[kind]
                        score = '{:.3f}'.format(100 * avg_score)
                        std_score = '{:.3f}'.format(100 * math.sqrt((sum2_score[kind] - sum_score[kind]*avg_score) / count_score[kind]))
                        htmls.append(f'<td align="right">{score}</td><td align="right">{std_score}</td><td align="right">{bests[kind]}</td><td align="right">{uniques[kind]}</td><td align="right">{fails[kind]}</td><td align="right">{count_score[kind]}</td>')
                    htmls.append('</tr>')
                htmls.append('</table>')
            htmls.append('</body>')
            htmls.append('</html>')
            self.wfile.write("\n".join(htmls).encode())
        def getIndex(self, query):
            if 'id' in query or 'name' in query:
                if 'id' in query and 'name' in query:
                    cur.execute('update runs set name = ? where run_id = ?', (query['name'][-1], int(query['id'][-1])))
                    NAME[int(query['id'][-1])] = query['name'][-1]
                    db.commit()
                query.pop('id')
                query.pop('name')
                self.send_response(302)
                self.send_header('Location', '/?' + urllib.parse.urlencode(query, True))
                self.end_headers()
                return
            self.send_response(200)
            self.send_header('Content-Type', 'text/html; charset=utf-8')
            self.end_headers()
            param = query.get('PARAM', [])
            seeds = query.get('SEEDS', [])
            htmls = []
            htmls.append('<html>')
            htmls.append('<head>')
            htmls.append('<title>MM Analytics</title>')
            htmls.append('</head>')
            htmls.append('<body>')
            htmls.append('''
<script>
function change_name(id, value) {
    var new_value = window.prompt(id + "'s name =", value);
    if(new_value===null) {
        return false;
    }
    var href = window.location.href;
    if(0<=href.indexOf("?")) {
        href = href + "&";
    }
    else {
        href = href + "?";
    }
    window.location.href = href + new URLSearchParams({id: id, name: new_value}).toString();
}
</script>
<style>
td, th {
    background-color: #DDEEFF;
    height: 1.5em;
    margin: 0.1em;
    padding: 0;
}
.top1 th {
    position: sticky;
    top: 0.1em;
}
.top2 th {
    position: sticky;
    top: 1.8em;
}
.left {
    position: sticky;
    left: 0;
}
</style>
''')
            for name in T3:
                if T3[name] is None or T5[name] is None:
                    continue
                if name not in param:
                    htmls.append(f'<p>_ <a href="/?{urllib.parse.urlencode({**query, "PARAM": param + [name]}, True)}">{name}: {T3[name][0]}, {T3[name][1]}</a></p>')
                else:
                    param2 = list(param)
                    param2.remove(name)
                    htmls.append(f'<p>v <a href="/?{urllib.parse.urlencode({**query, "PARAM": param2}, True)}">{name}: {T3[name][0]}, {T3[name][1]}</a></p>')
            if "ON" not in seeds:
                htmls.append(f'<p>_ <a href="/?{urllib.parse.urlencode({**query, "SEEDS": ["ON"]}, True)}">SEEDS</a></p>')
            else:
                htmls.append(f'<p>v <a href="/?{urllib.parse.urlencode({**query, "SEEDS": []}, True)}">SEEDS</a></p>')
            htmls.append('<table border="1" bordercolor="#000000">')
            htmls.append('<tr class="top1"><th rowspan="2">ID</th><th rowspan="2">CREATED_AT</th><th rowspan="2" class="left" style="z-index: 1;">NAME</th><th colspan="3">Time</th><th colspan="6">Whole</th>')
            for name in param:
                testcases = [[] for i in range(5)]
                best_scores = [[] for i in range(5)]
                for test_id in  BEST:
                    kind = 2
                    if T[test_id][name]<float(T5[name][0]):
                        kind = 0
                    elif T[test_id][name]<float(T5[name][1]):
                        kind = 1
                    elif T[test_id][name]<float(T5[name][2]):
                        kind = 2
                    elif T[test_id][name]<float(T5[name][3]):
                        kind = 3
                    else:
                        kind = 4
                    testcases[kind].append(str(test_id))
                    best_scores[kind].append(str(BEST[test_id]))
                htmls.append(f'<th colspan="2"><a href="javascript: alert(\'{", ".join(testcases[0])}\\n{", ".join(best_scores[0])}\');">{T5[name][0]}&gt;</a></th>')
                htmls.append(f'<th colspan="2"><a href="javascript: alert(\'{", ".join(testcases[1])}\\n{", ".join(best_scores[1])}\');">{T5[name][1]}&gt;</a></th>')
                htmls.append(f'<th colspan="2"><a href="javascript: alert(\'{", ".join(testcases[2])}\\n{", ".join(best_scores[2])}\');">{name}</a></th>')
                htmls.append(f'<th colspan="2"><a href="javascript: alert(\'{", ".join(testcases[3])}\\n{", ".join(best_scores[3])}\');">&gt;{T5[name][2]}</a></th>')
                htmls.append(f'<th colspan="2"><a href="javascript: alert(\'{", ".join(testcases[4])}\\n{", ".join(best_scores[4])}\');">&gt;{T5[name][3]}</a></th>')
            if "ON" in seeds:
                htmls.append(f'<th colspan="{len(BEST)}">SEEDS</th>')
            htmls.append('</tr>')
            htmls.append('<tr class="top2">')
            htmls.append('<th>MIN</th><th>AVG</th><th>MAX</th>')
            htmls.append('<th>Score</th><th>Std</th><th>Bests</th><th>Uniqs</th><th>Fails</th><th>Tests</th>')
            for name in param:
                htmls.append('<th>Score(Std)</th><th>Bests, Uniqs, Fails / Tests</th>')
                htmls.append('<th>Score(Std)</th><th>Bests, Uniqs, Fails / Tests</th>')
                htmls.append('<th>Score(Std)</th><th>Bests, Uniqs, Fails / Tests</th>')
                htmls.append('<th>Score(Std)</th><th>Bests, Uniqs, Fails / Tests</th>')
                htmls.append('<th>Score(Std)</th><th>Bests, Uniqs, Fails / Tests</th>')
            if "ON" in seeds:
                for test_id in BEST:
                    htmls.append(f'<th>{test_id}</th>')
            htmls.append('</tr>')
            for run_id in reversed(list(S.keys())):
                sum_score = 0
                sum2_score = 0
                count_score = 0
                bests = 0
                uniques = 0
                fails = 0
                for test_id in S[run_id]:
                    if 0 < S[run_id][test_id]:
                        sc1 = calcScore(S[run_id][test_id], BEST[test_id])
                        sum_score += sc1
                        sum2_score += sc1*sc1
                    else:
                        fails += 1
                    count_score += 1
                    if BEST[test_id] == S[run_id][test_id]:
                        bests += 1
                        if BEST_COUNT[test_id]==1:
                            uniques += 1
                avg_score = sum_score / count_score
                score = '{:.3f}'.format(100 * avg_score)
                std_score = '{:.3f}'.format(100 * math.sqrt((sum2_score - sum_score*avg_score) / count_score))
                sec_min = '{:.3f}'.format(TIME[run_id][0])
                sec_avg = '{:.3f}'.format(TIME[run_id][1] / count_score)
                sec_max = '{:.3f}'.format(TIME[run_id][2])
                name = NAME[run_id]
                if name=="":
                    name = "None"
                htmls.append(f'<tr><td><a href="/detail?{urllib.parse.urlencode({**query, "id": run_id}, True)}">{run_id}</a></td><td>{CREATED_AT[run_id]}</td><td class="left"><a href="javascript: change_name({run_id}, &quot;{urllib.parse.quote(f"{NAME[run_id]}")}&quot;)">{html.escape(f"{name}")}</a></td><td align="right">{sec_min}</td><td align="right">{sec_avg}</td><td align="right">{sec_max}</td><td align="right">{score}</td><td align="right">{std_score}</td><td align="right">{bests}</td><td align="right">{uniques}</td><td align="right">{fails}</td><td align="right">{count_score}</td>')
                for name in param:
                    sum_score = [0]*5
                    sum2_score = [0]*5
                    count_score = [0]*5
                    bests = [0]*5
                    uniques = [0]*5
                    fails = [0]*5
                    for test_id in S[run_id]:
                        kind = 2
                        if T[test_id][name]<float(T5[name][0]):
                            kind = 0
                        elif T[test_id][name]<float(T5[name][1]):
                            kind = 1
                        elif T[test_id][name]<float(T5[name][2]):
                            kind = 2
                        elif T[test_id][name]<float(T5[name][3]):
                            kind = 3
                        else:
                            kind = 4
                        if 0 < S[run_id][test_id]:
                            sc1 = calcScore(S[run_id][test_id], BEST[test_id])
                            sum_score[kind] += sc1
                            sum2_score[kind] += sc1*sc1
                        else:
                            fails[kind] += 1
                        count_score[kind] += 1
                        if BEST[test_id] == S[run_id][test_id]:
                            bests[kind] += 1
                            if BEST_COUNT[test_id]==1:
                                uniques[kind] += 1
                    for kind in range(5):
                        avg_score = sum_score[kind] / count_score[kind]
                        score = '{:.3f}'.format(100 * avg_score)
                        std_score = '{:.3f}'.format(100 * math.sqrt((sum2_score[kind] - sum_score[kind]*avg_score) / count_score[kind]))
                        htmls.append(f'<td align="right">{score} ({std_score})</td><td align="right">{bests[kind]}, {uniques[kind]}, {fails[kind]} / {count_score[kind]}</td>')
                if "ON" in seeds:
                    for test_id in BEST:
                        if 0 < S[run_id][test_id]:
                            sc1 = '{:.3f}'.format(calcScore(S[run_id][test_id], BEST[test_id]))
                        else:
                            sc1 = 0
                        htmls.append(f'<td>{sc1} ({S[run_id][test_id]})</td>')
                htmls.append(f'</tr>')
            htmls.append('</table>')
            htmls.append('</body>')
            htmls.append('</html>')
            self.wfile.write("\n".join(htmls).encode())
        def do_GET(self):
            path, qs = (self.path.split('?') + [''])[:2]
            query = urllib.parse.parse_qs(qs)
            #query = {q: (query[q]+[''])[-1] for q in query}
            if path=='/':
                return self.getIndex(query)
            if path=='/detail':
                return self.getDetail(query)
            elif path=='/source':
                return self.getSource(query)
            elif path=='/favicon.ico':
                self.send_response(200)
                self.send_header('Content-Type', 'image/x-icon')
                self.end_headers()
                self.wfile.write(open(os.path.join(os.path.dirname(__file__), 'favicon.ico'), 'rb').read())
            else:
                self.send_response(200)
                self.send_header('Content-Type', 'text/html; charset=utf-8')
                self.end_headers()
                htmls = []
                htmls.append('<html>')
                htmls.append('<body>')
                htmls.append(self.path)
                htmls.append(f'{query}')
                htmls.append('</body>')
                htmls.append('</html>')
                self.wfile.write("\n".join(htmls).encode())
    with http.server.HTTPServer(('', 8080), MyHandler) as server:
        end_time = time.time()
        print('start httpd ...', end_time - start_time, "sec")
        if callback is not None:
            callback()
        server.serve_forever()
