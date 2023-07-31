{{- define('MRAND49$RESERVE') }}
{{- include('get_time.h') }}
{{- include('lrand49.h') }}
{{- include('sarand49.h') }}
void {{ NAME }}() {
    if(__local__{% if def('help') %} && {{help}}{% endif %}) {
        fprintf(stderr, "@ra(\n");
        fprintf(stderr, "  help:bool=True         ... ヘルプ表示の有無\n");
        fprintf(stderr, "  verbose:bool=True      ... 冗長表示の有無\n");
        fprintf(stderr, "  vis:func               ... ビジュアライザ関数\n");
        fprintf(stderr, "  timelimit:float        ... 制限時間[sec]\n");
        fprintf(stderr, "  dt:float=1             ... 制限時間がないときのリポート表示間隔[sec]\n");
        fprintf(stderr, "  T0:ret_type            ... 初期温度\n");
        fprintf(stderr, "  Tfin:ret_type          ... 最終温度\n");
        fprintf(stderr, "  Tnorm:0, 1, 2, float=1 ... 温度ノルム\n");
        fprintf(stderr, "  Trate:1, float=1       ... 温度減衰\n");
        fprintf(stderr, "  time_cycle:int=1       ... 時刻取得サイクル\n");
        fprintf(stderr, ")\n\n");
        fprintf(stderr, "ヒント: rand49系を使って乱択を行い評価値を返す関数を書いてください。\n\n");
    }
    double $st = getTime();
    double $t = $st;
{%- if def('timelimit') %}
    double $tt = ({{ timelimit }}) - $st;
    double $rev_tt = 1.0 / $tt;
    static const int $N = 10;
    int $i = 1;
    double $nxt = $i * $tt / $N;
    double $dt = $nxt - $st;
    double $progress = 0.0;
{%- else %}
    double $dt = {{ get('dt', 1) }};
    double $nxt = $st + $dt;
{%- endif %}
    mrand49$reserved.clear();
    mrand49$cursor = 0;
    {{ RET_TYPE }} $now_sc = {{ NAME }}$origin();
    mrand49$cursor = -1;
    if(mrand49$reserved.empty()) {
        return;
    }
    {{ RET_TYPE }} $best = $now_sc;
    std::vector<int> $best_seq = mrand49$reserved;
    std::vector<int> $now_seq = mrand49$reserved;
{%- if def('vis') %}
    if(__local__) {
        getTimeStop();
        {{ vis }}();
        getTimeStart();
    }
{%- endif %}
{%- if def('T0') %}
    double $T = {{ T0 }};
{%- endif %}
{%- if def('time_cycle') %}
    int $timeCycle = {{ time_cycle }};
{%- endif %}
    int $up = 0;
    int $same = 0;
    int $down = 0;
    int $fail = 0;
    int $lastUp = 0;
    int $lastSame = 0;
    int $lastDown = 0;
    int $lastFail = 0;
{%- if def('verbose') %}
    if({{ verbose }}) {
{%- else %}
    if(__local__) {
{%- endif %}
        fprintf(stderr, "           up    same   down   fail , iterations\n");
    }
    while(true) {
{%- if def('timelimit') %}
        int $fixed = (int)($progress * ($now_seq.size()-1));
        mrand49$reserved.assign($now_seq.begin(), $now_seq.begin() + ($fixed + lrand49($now_seq.size() - $fixed)));
{%- else %}
        mrand49$reserved.assign($now_seq.begin(), $now_seq.begin() + lrand49($now_seq.size()));
{%- endif %}
        mrand49$cursor = 0;
        {{ RET_TYPE }} $sc = {{ NAME }}$origin();
        mrand49$cursor = -1;
        if($now_sc<=$sc) {
            if($now_sc==$sc) {
                ++$same;
            }
            else {
                ++$up;
                $now_sc = $sc;
                $now_seq = mrand49$reserved;
                if($best < $sc) {
                    $best = $sc;
                    $best_seq = mrand49$reserved;
                }
            }
        }
        else {
{%- if def('T0') %}
{%- if def('timelimit') %}
            if(!$T) {
                double remain = 1 - $progress;
                double $Tr = {% if (not def('Tnorm')) or Tnorm=="1" -%}
                    remain
{%- elif Tnorm=="2" -%}
                    remain * remain
{%- elif Tnorm=="0" -%}
                    1.0
{%- else -%}
                    pow(remain, {{ Tnorm }})
{%- endif -%}
{%- if def('Trate') and Trate!="1" %} * pow({{ Trate }}, $progress){% endif -%}
                ;
{%- if def('Tfin') %}
                $T = (({{ T0 }}) - ({{ Tfin }})) * $Tr + ({{ Tfin }});
{%- else %}
                $T = ({{ T0 }}) * $Tr;
{%- endif %}
            }
{%- endif %}
            if($now_sc<=$sc + $T * sarand49()) {
                ++$down;
                $now_sc = $sc;
                $now_seq = mrand49$reserved;
            }
            else {
                ++$fail;
            }
{%- else %}
            ++$fail;
{%- endif %}
        }
{%- if def('time_cycle') %}
        if(!--$timeCycle) {
{%- else %}
        {
{%- endif %}
            $t = getTime();
{%- if def('timelimit') %}
            $progress = ($t-$st) * $rev_tt;
{%- endif %}
            if($nxt<=$t) {
{%- if def('vis') %}
                if(__local__) {
                    mrand49$reserved = $now_seq;
                    mrand49$cursor = 0;
                    {{ RET_TYPE }} $sc = {{ NAME }}$origin();
                    mrand49$cursor = -1;
                    getTimeStop();
                    {{ vis }}();
                    getTimeStart();
                }
{%- endif %}
{%- if def('verbose') %}
                if({{ verbose }}) {
{%- else %}
                if(__local__) {
{%- endif %}
                    int $u = $up-$lastUp;
                    int $s = $same-$lastSame;
                    int $d = $down-$lastDown;
                    int $f = $fail-$lastFail;
                    int $sum = $u+$s+$d+$f;
                    int $denom = std::max($sum, 1);
{%- if def('timelimit') %}
                    double progress = ($i-0.5) / $N;
{%- else %}
                    double progress = 0.0;
{%- endif %}
                    double remain = 1 - progress;
                    double $Tr = {% if (not def('Tnorm')) or Tnorm=="1" -%}
                        remain
{%- elif Tnorm=="2" -%}
                        remain * remain
{%- elif Tnorm=="0" -%}
                        1.0
{%- else -%}
                        pow(remain, {{ Tnorm }})
{%- endif -%}
{%- if def('Trate') and Trate!="1" %} * pow({{ Trate }}, progress){% endif -%}
                    ;
{%- if def('T0') and def('Tfin') %}
                    double $T = (({{ T0 }}) - ({{ Tfin }})) * $Tr + ({{ Tfin }});
{%- elif def('T0') %}
                    double $T = ({{ T0 }}) * $Tr;
{%- endif %}
                    fprintf(stderr, "rand({% if def('timelimit') %}%2d%%{% else %}%3g{% endif %}):%6.2f%%%6.2f%%%6.2f%%%6.2f%%, %d, %.1fi/s{% if def('T0') %}, T=%g{% endif %}, score=%g(%+g)\n", {% if def('timelimit') %}50 * ($i+$i-1) / $N{% else %}$nxt - $st{% endif %}, $u*100.0/$denom, $s*100.0/$denom, $d*100.0/$denom, $f*100.0/$denom, $sum, $sum / $dt{% if def('T0') %}, $T{% endif %}, (double)$now_sc, (double)($best - $now_sc));
                }
{%- if def('timelimit') %}
                if($N<=$i) {
{%- if def('verbose') %}
                    if({{ verbose }}) {
{%- else %}
                    if(__local__) {
{%- endif %}
                        int $u = $up;
                        int $s = $same;
                        int $d = $down;
                        int $f = $fail;
                        int $sum = $u+$s+$d+$f;
                        int $denom = std::max($sum, 1);
                        fprintf(stderr, "sa(ALL):%6.2f%%%6.2f%%%6.2f%%%6.2f%%, %d\n", $u*100.0/$denom, $s*100.0/$denom, $d*100.0/$denom, $f*100.0/$denom, $sum);
                    }
                    break;
                }
                ++$i;
                $nxt = $i * $tt / $N;
                $dt = $nxt - $t;
{%- else %}
                $nxt += $dt;
{%- endif %}
                $lastUp = $up;
                $lastDown = $down;
                $lastFail = $fail;
            }
{%- if def('T0') %}
            $T = 0;
{%- endif %}
{%- if def('time_cycle') %}
            $timeCycle = {{ time_cycle }};
{%- endif %}
        }
    }
    mrand49$reserved = $best_seq;
    mrand49$cursor = 0;
    {{ RET_TYPE }} $sc = {{ NAME }}$origin();
    mrand49$cursor = -1;
{%- if def('vis') %}
    if(__local__) {
        getTimeStop();
        {{ vis }}();
        getTimeStart();
    }
{%- endif %}
}
