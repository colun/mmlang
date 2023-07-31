{{- include('get_time.h') }}
{{- include('sarand49.h') }}
{{- include('xmem.h') }}
void {{ NAME }}() {
    if(__local__{% if def('help') %} && {{help}}{% endif %}) {
        fprintf(stderr, "@sa(\n");
        fprintf(stderr, "  help:bool=True         ... ヘルプ表示の有無\n");
        fprintf(stderr, "  verbose:bool=True      ... 冗長表示の有無\n");
        fprintf(stderr, "  init:func              ... 初期盤面定義関数\n");
        fprintf(stderr, "  vis:func               ... ビジュアライザ関数\n");
        fprintf(stderr, "  score:func->float      ... 厳密スコア関数\n");
        fprintf(stderr, "  update:func            ... 状態更新関数\n");
        fprintf(stderr, "  timelimit:float        ... 制限時間[sec]\n");
        fprintf(stderr, "  dt:float=1             ... 制限時間がないときのリポート表示間隔[sec]\n");
        fprintf(stderr, "  T0:ret_type            ... 初期温度\n");
        fprintf(stderr, "  Tfin:ret_type          ... 最終温度\n");
        fprintf(stderr, "  Tnorm:0, 1, 2, float=1 ... 温度ノルム\n");
        fprintf(stderr, "  Trate:1, float=1       ... 温度減衰\n");
        fprintf(stderr, "  time_cycle:int=1       ... 時刻取得サイクル\n");
        fprintf(stderr, ")\n\n");
        fprintf(stderr, "ヒント: xmem系を使って状態遷移時に評価差分を返す関数を書いてください。\n\n");
        //fprintf(stderr, "補足: mmlang実行時に --sa-profiler を指定すると、SAプロファイラが動作します。\n\n");
    }
{%- if def('init') %}
    {{ init }}();
{%- endif %}
{%- if def('vis') %}
    if(__local__) {
        getTimeStop();
        {{ vis }}();
        getTimeStart();
    }
{%- endif %}
    xmem::lock();
    double $st = getTime();
    double $t = $st;
    double progress = 0.0;
{%- if def('timelimit') %}
    double $tt = ({{ timelimit }}) - $st;
    double $rev_tt = 1.0 / $tt;
    static const int $N = 100;
    int $i = 1;
    double $nxt = $i * $tt / $N;
    double $dt = $nxt - $st;
{%- else %}
    double $dt = {{ get('dt', 1) }};
    double $nxt = $st + $dt;
{%- endif %}
    double $best = 0.0;
    double $now_sc = 0.0;
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
        {{ RET_TYPE }} $dsc = {{ NAME }}$origin(
{%- for param in PARAMS -%}
{%- if loop.index0!=0 %}, {% endif -%}
{%- if param=='progress' -%}
progress
{%- endif -%}
{%- endfor -%}
        );
        if(0<=$dsc) {
{%- if def('update') %}
            xmem::unlock();
            {{ update }}();
            xmem::lock();
{%- else %}
            xmem::unlocklock();
{%- endif %}
            if($dsc==0) {
                ++$same;
            }
            else {
                ++$up;
                $now_sc += $dsc;
                if($best < $now_sc) {
                    $best = $now_sc;
                }
            }
        }
        else {
{%- if def('T0') %}
{%- if def('timelimit') %}
            if(!$T) {
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
{%- if def('Tfin') %}
                $T = (({{ T0 }}) - ({{ Tfin }})) * $Tr + ({{ Tfin }});
{%- else %}
                $T = ({{ T0 }}) * $Tr;
{%- endif %}
            }
{%- endif %}
            if(0<=$dsc + $T * sarand49()) {
{%- if def('update') %}
                xmem::unlock();
                {{ update }}();
                xmem::lock();
{%- else %}
                xmem::unlocklock();
{%- endif %}
                ++$down;
                $now_sc += $dsc;
            }
            else {
                xmem::undo();
                ++$fail;
            }
{%- else %}
            xmem::undo();
            ++$fail;
{%- endif %}
        }
{%- if def('time_cycle') %}
        if(!--$timeCycle) {
{%- else %}
        {
{%- endif %}
            $t = getTime();
            if($nxt<=$t) {
{%- if def('vis') %}
                if(__local__) {
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
{%- if def('Tfin') %}
                    double $T = (({{ T0 }}) - ({{ Tfin }})) * $Tr + ({{ Tfin }});
{%- else %}
                    double $T = ({{ T0 }}) * $Tr;
{%- endif %}
                    fprintf(stderr, "sa({% if def('timelimit') %}%2d%%{% else %}%3g{% endif %}):%6.2f%%%6.2f%%%6.2f%%%6.2f%%, %d, %.1fi/s{% if def('T0') %}, T=%g{% endif %}{% if def('score') %}, score=%g(%+g){% endif %}\n", {% if def('timelimit') %}50 * ($i+$i-1) / $N{% else %}$nxt - $st{% endif %}, $u*100.0/$denom, $s*100.0/$denom, $d*100.0/$denom, $f*100.0/$denom, $sum, $sum / $dt{% if def('T0') %}, $T{% endif %}{% if def('score') %}, (double)({{ score }}()), $best - $now_sc{% endif %});
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
                        fprintf(stderr, "sa(ALL):%6.2f%%%6.2f%%%6.2f%%%6.2f%%, %d, %.1fi/s\n", $u*100.0/$denom, $s*100.0/$denom, $d*100.0/$denom, $f*100.0/$denom, $sum, $sum / ($t-$st));
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
                $lastSame = $same;
                $lastDown = $down;
                $lastFail = $fail;
            }
{%- if def('timelimit') %}
            progress = ($t-$st) * $rev_tt;
{%- endif %}
{%- if def('T0') %}
            $T = 0;
{%- endif %}
{%- if def('time_cycle') %}
            $timeCycle = {{ time_cycle }};
{%- endif %}
        }
    }
    xmem::unlock();
}
