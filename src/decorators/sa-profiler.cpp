{{- include('set') }}
{{- include('sarand49.h') }}
{{- include('xmem.h') }}
void {{ NAME }}() {
    fprintf(stderr, "used --sa-profiler\n");
    std::vector<char*> $vec;
    std::set<char*> $target;
{%- if def('init') %}
    {{ init }}();
{%- endif %}
    xmem::lock();
    double $whole_st = getTime();
    // 探索空間が連結( https://github.com/hakomo/Simulated-Annealing-Techniques )の診断
{%- if not def('init') %}
    fprintf(stderr, "WARN: init関数が指定されていなければ、探索空間が連結( https://github.com/hakomo/Simulated-Annealing-Techniques )の診断を行うことが出来ません。\n");
{%- else %}
    for(int $i=0; $i<64; ++$i) {
        {{ init }}();
        xmem::trace_modify($vec);
        xmem::unlocklock();
        for(int $j=0; $j<$vec.size(); $j+=2) {
            char * $e = $vec[$j+1];
            for(char * $c=$vec[$j]; $c<$e; ++$c) {
                $target.insert($c);
            }
        }
    }
    assert(!$target.empty());
    char * $begin = *$target.begin();
    char * $end = *$target.rbegin() + 1;
    int $size = $end - $begin;
    char * $target_mem = (char*)malloc($size);
    char * $before_mem = (char*)malloc($size);
    size_t $target_diff = $target_mem - $begin;
    size_t $before_diff = $before_mem - $begin;
    {
        xmem::unlock();
        {{ init }}();
        memcpy($target_mem, $begin, $size);
        {{ init }}();
        memcpy($before_mem, $begin, $size);
        xmem::lock();
{%- if def('vis') %}
        {{ vis }}();
{%- endif %}
        setTime($whole_st);
        double $st = $whole_st;
        double $tt = __LIMIT__ - $st;
        double $rev_tt = 1.0 / $tt;
        double $t = $st;
        static const int $N = 10;
        int $i = 1;
        double $nxt = $i * $tt / $N;
        double $T0 = 8;//TODO
        double $T = $T0;
        int $timeCycle = __TIME_CYCLE__;
        int $comp = 0;
        int $bonu = 0;
        int $fail = 0;
        int $lastComp = 0;
        int $lastBonu = 0;
        int $lastFail = 0;
        while(true) {
            __NAME__$origin();
            __UPDATE__
            xmem::trace_modify($vec);
            int $dsc = 0;
            for(int $j=0; $j<$vec.size(); $j+=2) {
                char * $e = $vec[$j+1];
                for(char * $c=$vec[$j]; $c<$e; ++$c) {
                    if($target.find($c)!=$target.end() && *$c!=$c[$before_diff]) {
                        if(*$c==$c[$target_diff]) {
                            ++$dsc;
                        }
                        else if($c[$before_diff]==$c[$target_diff]) {
                            --$dsc;
                        }
                    }
                }
            }
            if(0<=$dsc) {
                for(int $j=0; $j<$vec.size(); $j+=2) {
                    char * $e = $vec[$j+1];
                    for(char * $c=$vec[$j]; $c<$e; ++$c) {
                        if($target.find($c)!=$target.end() && *$c!=$c[$before_diff]) {
                            $c[$before_diff] = *$c;
                        }
                    }
                }
                xmem::unlocklock();
                ++$comp;
            }
            else {
                if(!$T) {
                    double progress = ($t-$st) * $rev_tt;
                    double remain = 1 - progress;
                    double $Tr = remain;
                    $T = $T0 * $Tr;
                }
                if(0<=$dsc + $T * sarand49()) {
                    for(int $j=0; $j<$vec.size(); $j+=2) {
                        char * $e = $vec[$j+1];
                        for(char * $c=$vec[$j]; $c<$e; ++$c) {
                            if($target.find($c)!=$target.end() && *$c!=$c[$before_diff]) {
                                $c[$before_diff] = *$c;
                            }
                        }
                    }
                    xmem::unlocklock();
                    ++$bonu;
                }
                else {
                    xmem::undo();
                    ++$fail;
                }
            }
            if(__TIME_CYCLE__<=1 || !--$timeCycle) {
                $t = getTime();
                if($nxt<=$t) {
                    __VIS__
                    int $c = $comp-$lastComp;
                    int $b = $bonu-$lastBonu;
                    int $f = $fail-$lastFail;
                    int $sum = $c+$b+$f;
                    if($sum<1) {
                        $sum = 1;
                    }
                    double progress = ($i-0.5) / $N;
                    double remain = 1 - progress;
                    double $Tr = remain;
                    int $sc = 0;
                    for(char * $c : $target) {
                        if(*$c!=$c[$target_diff]) {
                            ++$sc;
                        }
                    }
                    fprintf(stderr, "sa(%2d%%): %5.2f%%, %5.2f%%, %5.2f%%, %d, T=%g, score=%d\n", 50 * ($i+$i-1) / $N, $c*100.0/$sum, $b*100.0/$sum, $f*100.0/$sum, $c+$b+$f, $T0 * $Tr, $sc);
                    if($N<=$i) {
                        int $c = $comp;
                        int $b = $bonu;
                        int $f = $fail;
                        int $sum = $c+$b+$f;
                        if($sum<1) {
                            $sum = 1;
                        }
                        fprintf(stderr, "sa(ALL): %5.2f%%, %5.2f%%, %5.2f%%, %d\n", $c*100.0/$sum, $b*100.0/$sum, $f*100.0/$sum, $c+$b+$f);
                        xmem::unlock();
                        for(char * $c : $target) {
                            *$c = $c[$target_diff];
                        }
                        xmem::lock();
                        __VIS__
                        break;
                    }
                    ++$i;
                    $nxt = $i * $tt / $N;
                    $lastComp = $comp;
                    $lastBonu = $bonu;
                    $lastFail = $fail;
                }
                if(__LIMIT__<=$t) {
                    break;
                }
                $T = 0;
                if(1<__TIME_CYCLE__) {
                    $timeCycle = __TIME_CYCLE__;
                }
            }
        }
    }
{%- endif %}
    xmem::unlock();
}
