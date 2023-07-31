{{- include('cfloat') -}}
{{- include('get_time.h') -}}
{{- include('fast_pool.h') -}}
{{- include('dpque.h') -}}
{{- include('xmem.h') -}}
{{- include('xnodemem.h') -}}
{{- include('drand49.h') -}}

struct {{ NAME }}$candidate;

struct {{ NAME }}$node {
    static fast_pool<{{ NAME }}$node> pool;
    {{ NAME }}$node * parent;
    void * patch;
    int depth;
#if __local__
    int rank;
#endif
    int ref;
    inline void addRef() {
        ++ref;
    }
    inline void release() {
        assert(0<ref);
        if(ref==1) {
            if(parent) {
                parent->release();
            }
            xmem::free(patch);//{{ NAME }}$myxmem.free(patch);
            pool.push(this);
        }
        else {
            --ref;
        }
    }
    inline static {{ NAME }}$node * create() {
        {{ NAME }}$node * n = pool.pop();
        n->parent = 0;
        n->patch = 0;
        n->depth = 0;
#if __local__
        n->rank = -1;
#endif
        n->ref = 1;
        return n;
    }
    inline static {{ NAME }}$node * create({{ NAME }}$node * parent_) {
        parent_->addRef();
        {{ NAME }}$node * n = pool.pop();
        n->parent = parent_;
        n->patch = 0;
        n->depth = parent_->depth + 1;
#if __local__
        n->rank = -1;
#endif
        n->ref = 1;
        return n;
    }
};
fast_pool<{{ NAME }}$node> {{ NAME }}$node::pool;

struct {{ NAME }}$candidate {
    static fast_pool<{{ NAME }}$candidate> $pool;
    {{ NAME }}$node * $parent;
{%- for param in PARAMS %}
    {{ P_TYPE(param) }} {{ param }};
{%- endfor %}
#if __local__
    int $rank;
#endif
    inline static {{ NAME }}$candidate * $create({{ NAME }}$node * $parent_) {
        assert($parent_);
        $parent_->addRef();
        {{ NAME }}$candidate * ret = $pool.pop();
        ret->$parent = $parent_;
#if __local__
        ret->$rank = -1;
#endif
        return ret;
    }
    inline void $release() {
        $parent->release();
        $pool.push(this);
    }
};
fast_pool<{{ NAME }}$candidate> {{ NAME }}$candidate::$pool;

struct {{ NAME }}$order {
    static fast_pool<{{ NAME }}$order> pool;
    {{ NAME }}$candidate * input;
    {{ NAME }}$node * node;
    {{ NAME }}$order * link;
    double score;
    inline static {{ NAME }}$order * create(double score_, {{ NAME }}$candidate * input_, {{ NAME }}$node * node_) {
        {{ NAME }}$order * ret = pool.pop();
        ret->input = input_;
        ret->node = node_;
        ret->link = NULL;
        ret->score = score_;
        return ret;
    }
    inline void release() {
        pool.push(this);
    }
};
fast_pool<{{ NAME }}$order> {{ NAME }}$order::pool;

xmem {{ NAME }}$myxmem;
typedef std::pair<double, {{ NAME }}$candidate*> {{ NAME }}$ranking_item;
typedef dpque<{{ NAME }}$ranking_item> {{ NAME }}$ty_ranking;
struct {{ NAME }}$generation {
    {{ NAME }}$ty_ranking * ranking;
    int ra_count;
    {{ NAME }}$generation() : ranking(NULL), ra_count(0) {}
    void init() {
        if(ranking==NULL) {
            ranking = new {{ NAME }}$ty_ranking();
        }
        else {
            for(const {{ NAME }}$ranking_item & v : *ranking) {
                v.second->$release();
            }
            ranking->clear();
        }
        ra_count = 0;
    }
    void dest() {
        if(ranking) {
            for(const {{ NAME }}$ranking_item & v : *ranking) {
                v.second->$release();
            }
            delete ranking;
            ranking = NULL;
        }
    }
};

{{ NAME }}$node * {{ NAME }}$current_node;
std::deque<{{ NAME }}$generation> {{ NAME }}$generations;
int {{ NAME }}$generation_size;
std::vector<double> {{ NAME }}$sum_rank_vec;
std::vector<{{ NAME }}$ranking_item> {{ NAME }}$reserved;
std::vector<{{ NAME }}$order *> {{ NAME }}$orders;
std::vector<tuple<{{ NAME }}$node *, double, {{ NAME }}$order *> > {{ NAME }}$orders_work;
std::vector<tuple<{{ NAME }}$node *, double, {{ NAME }}$order *> > {{ NAME }}$orders_work2;
std::vector<void*> {{ NAME }}$transit_vec;
int {{ NAME }}$maxdepth;
int {{ NAME }}$maxwidth;
int {{ NAME }}$nowDepth;
bool {{ NAME }}$verboseFlag = true;
bool {{ NAME }}$lock = false;
{%- for param in PARAMS %}
    {%- if param in PENALTY_HASHES %}
        {%- if P_TYPE[param] in ['int64', 'uint64'] %}
            {{- include('fast_weak_counter64.h') }}
fast_weak_counter64<> {{ NAME }}${{ param }}$counter;
        {%- else %}
            {{- include('fast_weak_counter32.h') }}
fast_weak_counter32<> {{ NAME }}${{ param }}$counter;
        {%- endif %}
    {%- elif param=='hash' %}
        {%- if P_TYPE[param] in ['int64', 'uint64'] %}
            {{- include('fast_weak_set64.h') }}
fast_weak_set64<> {{ NAME }}$visited;
        {%- else %}
            {{- include('fast_weak_set32.h') }}
fast_weak_set32<> {{ NAME }}$visited;
        {%- endif %}
    {%- endif %}
{%- endfor %}
void {{ NAME }}({%- for param in PARAMS -%}{% if loop.index0!=0 %}, {% endif %}{{ P_TYPE(param) }} $param${{ param }}{%- endfor -%}) {
    if({{ NAME }}$lock) {
        int $r_idx = 1 + {{ NAME }}$current_node->depth - {{ NAME }}$nowDepth;
        if({{ NAME }}$generation_size<=$r_idx) {
            return;
        }
{%- if not 'score' in PARAMS %}
        static const double $param$score = 0.0;
{%- endif %}
        if({{ NAME }}$maxwidth<={{ NAME }}$generations[$r_idx].ranking->size() && $param$score<={{ NAME }}$generations[$r_idx].ranking->small().first) {
            return;
        }
        {{ NAME }}$candidate * $candidate = {{ NAME }}$candidate::$create({{ NAME }}$current_node);
        assert($candidate->$parent);
{%- for param in PARAMS %}
        $candidate->{{ param }} = $param${{ param }};
{%- endfor %}
        {{ NAME }}$reserved.emplace_back($param$score, $candidate);
    }
    else {
        {{ NAME }}$lock = true;
        if(__local__{% if def('help') %} && {{help}}{% endif %}) {
            fprintf(stderr, "@beam(\n");
            fprintf(stderr, "  help:bool=True          ... ヘルプ表示の有無\n");
            fprintf(stderr, "  verbose:bool=True       ... 冗長表示の有無\n");
            fprintf(stderr, "  init:()->void           ... 初期盤面定義関数\n");
            fprintf(stderr, "  vis:()->void            ... ビジュアライザ関数\n");
            fprintf(stderr, "  score:()->float         ... 厳密スコア関数\n");
            fprintf(stderr, "  timelimit:float=1.95    ... 制限時間[sec]\n");
            fprintf(stderr, "  maxdepth:int            ... 最大の深さ\n");
            fprintf(stderr, "  maxwidth:int=1000       ... 最大のビーム幅\n");
            fprintf(stderr, "  ra_depth:int=0          ... 乱択の深さ\n");
            fprintf(stderr, "  ra_zipf:float=3         ... 乱択の選択確率を、順位の何乗に反比例させるか？\n");
            fprintf(stderr, ")\n\n");
            fprintf(stderr, "@penalty_hash(\n");
            fprintf(stderr, "  varname                 ... 監視対象のハッシュ引数名\n");
            fprintf(stderr, "  function:(float)->float ... 先読み時の選択確率を、順位の何乗に反比例するか？\n");
            fprintf(stderr, ")\n\n");
            fprintf(stderr, "ヒント: xmem系を使って状態遷移した上で、複数の遷移候補を再帰呼び出しする関数を書いてください。\n\n");
{%- if not def('maxdepth') %}
            fprintf(stderr, "警告: maxdepthが指定されていません。\n\n");
{%- endif %}
        }
        {{ NAME }}$maxdepth = {{ get('maxdepth', 1000) }};
        {{ NAME }}$maxwidth = {{ get('maxwidth', 1000) }};
        double $wholeLimit = {{ get('timelimit', 1.95) }};
        double $st = getTime();
        double $t = $st;
        assert({{ NAME }}$reserved.empty());
        int $ra_max_depth = {{ get('ra_depth', 0) }};
        double $ra_zipf = {{ get('ra_zipf', 3) }};
        if({{ NAME }}$generations.size() < $ra_max_depth + 2) {
            {{ NAME }}$generations.resize($ra_max_depth + 2);
        }
        for({{ NAME }}$generation & $gen : {{ NAME }}$generations) {
            $gen.init();
        }
        {{ NAME }}$sum_rank_vec.clear();
{%- if def('verbose') %}
        {{ NAME }}$verboseFlag = {{ verbose }};
{%- endif %}
{%- set flags = namespace(has_penalty_hash=False, has_hash=False) %}
{%- for param in PARAMS %}
    {%- if param in PENALTY_HASHES %}
        {%- set flags.has_penalty_hash = True %}
        {{ NAME }}${{ param }}$counter.clear();
    {%- elif param=='hash' %}
        {%- set flags.has_hash = True %}
        {%- if P_TYPE[param] in ['int64', 'uint64'] %}
        {{ NAME }}$visited.clear();
        {%- else %}
        {{ NAME }}$visited.clear();
        {%- endif %}
    {%- endif %}
{%- endfor %}
        xmem::lock();
        {{ NAME }}$nowDepth = 0;
        {{ NAME }}$generation_size = min(1 + {{ NAME }}$maxdepth - {{ NAME }}$nowDepth, 2+$ra_max_depth);
        {{ NAME }}$current_node = {{ NAME }}$node::create();
        {{ NAME }}$origin({%- for param in PARAMS -%}{% if loop.index0!=0 %}, {% endif %}$param${{ param }}{%- endfor -%});
{%- macro accept_or_continue(ra_flag) %}
        {
    {%- if ra_flag %}
        {%- if flags.has_hash %}
            if({{ NAME }}$visited.testWithSet($candidate->hash)) continue;
        {%- endif %}
        {%- if flags.has_penalty_hash %}
           {%- for param in PARAMS %}
                {%- if param in PENALTY_HASHES %}
            {{ NAME }}${{ param }}$counter.count($candidate->{{ param }});
                {%- endif %}
            {%- endfor %}
        {%- endif %}
    {%- else %}
            assert(0==1 + $candidate->$parent->depth - {{ NAME }}$nowDepth);
        {%- if flags.has_penalty_hash %}
            double $score = {% if 'score' in PARAMS %}$candidate->score{% else %}0{% endif %};
            int $cnt;
            {%- for param in PARAMS %}
                {%- if param in PENALTY_HASHES %}
            $cnt = {{ NAME }}${{ param }}$counter.get($candidate->{{ param }});
                    {%- if PENALTY_HASHES[param] is none %}
            if($cnt) $score -= $cnt;
                    {%- else %}
            if($cnt) $score -= {{ PENALTY_HASHES[param] }}($cnt);
                    {%- endif %}
                {%- endif %}
            {%- endfor %}
            //TODO: rescheduleを展開
            {%- if ra_flag %}
            if({{ NAME }}$maxwidth<={{ NAME }}$generations[0].ranking->size() && $score < {{ NAME }}$generations[0].ranking->small().first) {
                $candidate->$release();
                continue;
            }
            {%- else %}
            if(!{{ NAME }}$generations[0].ranking->empty() && $score < {{ NAME }}$generations[0].ranking->large().first) {
                assert($candidate);
                assert($candidate->$parent);
                {{ NAME }}$generations[0].ranking->emplace($score, $candidate);
                continue;
            }
            {%- endif %}
        {%- endif %}
        {%- if flags.has_hash %}
            if({{ NAME }}$visited.testWithSet($candidate->hash)) continue;
        {%- endif %}
        {%- if flags.has_penalty_hash %}
           {%- for param in PARAMS %}
                {%- if param in PENALTY_HASHES %}
            {{ NAME }}${{ param }}$counter.count($candidate->{{ param }});
                {%- endif %}
            {%- endfor %}
        {%- endif %}
    {%- endif %}
        }
{%- endmacro %}
{%- macro process_reserved() %}
        while(!{{ NAME }}$reserved.empty()) {
            int $r_idx = 1 + {{ NAME }}$current_node->depth - {{ NAME }}$nowDepth;
            assert($r_idx < {{ NAME }}$generation_size);
            assert({{ NAME }}$current_node->depth < {{ NAME }}$maxdepth);
            {{ NAME }}$candidate * $candidate = NULL;
            {
                assert($r_idx < {{ NAME }}$generations.size());
                sort({{ NAME }}$reserved.rbegin(), {{ NAME }}$reserved.rend());
                int $idx;
                if($r_idx+1 < {{ NAME }}$generation_size) {
                    int sz = {{ NAME }}$reserved.size();
                    while({{ NAME }}$sum_rank_vec.size()<{{ NAME }}$reserved.size()) {
                        double $before = {{ NAME }}$sum_rank_vec.size()==0 ? 0.0 : {{ NAME }}$sum_rank_vec.back();
                        {{ NAME }}$sum_rank_vec.emplace_back($before + pow({{ NAME }}$sum_rank_vec.size()+1, -$ra_zipf));
                    }
                    double $rnd = drand49() * {{ NAME }}$sum_rank_vec[{{ NAME }}$reserved.size()-1];
                    $idx = 0;
                    while($idx<{{ NAME }}$reserved.size()-1 && {{ NAME }}$sum_rank_vec[$idx] < $rnd) {
                        ++$idx;
                    }
                }
                else {
                    $idx = -1;
                }
                for(int $rank=0; $rank<{{ NAME }}$reserved.size(); ++$rank) {
                    if($idx==$rank) {
                        $candidate = {{ NAME }}$reserved[$rank].second;
                        assert($candidate->$parent);
                    }
                    else if({{ NAME }}$maxwidth<={{ NAME }}$generations[$r_idx].ranking->size()) {
                        if({{ NAME }}$reserved[$rank].first<={{ NAME }}$generations[$r_idx].ranking->small().first) {
                            {{ NAME }}$reserved[$rank].second->$release();
                            ++$rank;
                            while($rank<{{ NAME }}$reserved.size()) {
                                if($idx==$rank) {
                                    $candidate = {{ NAME }}$reserved[$rank].second;
                                    assert($candidate->$parent);
                                }
                                else {
                                    {{ NAME }}$reserved[$rank].second->$release();
                                }
                                ++$rank;
                            }
                            break;
                        }
                        {{ NAME }}$generations[$r_idx].ranking->small().second->$release();
                        assert({{ NAME }}$reserved[$rank].second->$parent);
                        {{ NAME }}$generations[$r_idx].ranking->swap_small({{ NAME }}$reserved[$rank]);
                    }
                    else {
                        assert({{ NAME }}$reserved[$rank].second->$parent);
                        {{ NAME }}$generations[$r_idx].ranking->push({{ NAME }}$reserved[$rank]);
                    }
#if __local__
                    {{ NAME }}$reserved[$rank].second->$rank = $rank;
#endif
                }
                {{ NAME }}$reserved.clear();
            }
            if($candidate) {
                xmem::lock();
                {{- accept_or_continue(True).replace('\n', '\n        ') }}
                //先読みの際はノード移動が不要のはず。
                assert({{ NAME }}$current_node==$candidate->$parent);
                {{ NAME }}$node * $node = {{ NAME }}$node::create($candidate->$parent);
#if __local__
                $node->rank = $candidate->$rank;
#endif
                {{ NAME }}$current_node->release();
                {{ NAME }}$current_node = $node;
                ++{{ NAME }}$generations[$r_idx].ra_count;
                {{ NAME }}$origin(
    {%- for param in PARAMS -%}
                    {% if loop.index0!=0 %}, {% endif %}$candidate->{{ param }}
    {%- endfor -%}
                );
                $candidate->$release();
            }
        }
        assert(!{{ NAME }}$current_node->patch);
        if({{ NAME }}$current_node->parent) {
            {{ NAME }}$node * $node = {{ NAME }}$current_node;
            $node->patch = {{ NAME }}$myxmem.build_undo();
            $node = $node->parent;
            while(!$node->patch) {
                xmem::unlock();
                if($node->parent) {
                    $node->patch = {{ NAME }}$myxmem.build_undo();
                    $node = $node->parent;
                }
                else {
                    $node->patch = {{ NAME }}$myxmem.build();
                    break;
                }
            }
            $node->addRef();
            {{ NAME }}$current_node->release();
            {{ NAME }}$current_node = $node;
        }
        else {
            {{ NAME }}$current_node->patch = {{ NAME }}$myxmem.build();
        }
{%- endmacro %}
        int $totalLoopCount = 0;
        int $totalAcceptCount = 0;
        int $totalRaCount = 0;
        {{- process_reserved() }}
        {{ NAME }}$node * $lastTopNode = {{ NAME }}$current_node;
        $lastTopNode->addRef();
        $t = getTime();
        double $st2 = $t;
        for({{ NAME }}$nowDepth=1; {{ NAME }}$nowDepth<={{ NAME }}$maxdepth; ++{{ NAME }}$nowDepth) {
            {{ NAME }}$generations[0].init();
            {{ NAME }}$generations.push_back({{ NAME }}$generations.front());
            {{ NAME }}$generations.pop_front();
            if({{ NAME }}$generations[0].ranking->empty()) {
                if({{ NAME }}$generations[0].ra_count) {
                    fprintf(stderr, "beam(%3d) ... skip\n", {{ NAME }}$nowDepth);
                    continue;
                }
                break;
            }
            int $depthLoopCount = 0;
            int $depthAcceptCount = 0;
            {{ NAME }}$generation_size = min(1 + {{ NAME }}$maxdepth - {{ NAME }}$nowDepth, 2+$ra_max_depth);
            double $depthStartTime = $t;
            double $firstSc = {{ NAME }}$generations[0].ranking->large().first;
            if($totalAcceptCount < 10) {
                double $depthLimit = ($wholeLimit - $t) / (1 + {{ NAME }}$maxdepth - {{ NAME }}$nowDepth) + $t;
                assert({{ NAME }}$orders.empty());
                while(!{{ NAME }}$generations[0].ranking->empty() && $depthAcceptCount<{{ NAME }}$maxwidth) {
                    ++$depthLoopCount;
                    {{ NAME }}$candidate * $candidate = {{ NAME }}$generations[0].ranking->large().second;
                    assert($candidate->$parent);
                    {{ NAME }}$generations[0].ranking->pop_large();
                    {{- accept_or_continue(False).replace('\n', '\n        ') }}
                    //current_nodeからcandidate->$parentまで移動
                    {{ NAME }}$node * $from = {{ NAME }}$current_node;
                    {{ NAME }}$node * $to = $candidate->$parent;
                    assert($from);
                    assert($to);
                    assert({{ NAME }}$transit_vec.empty());
                    while($from!=$to) {
                        assert($from);
                        assert($to);
                        if($from->depth<$to->depth) {
                            assert($to->patch);
                            {{ NAME }}$transit_vec.emplace_back($to->patch);
                            $to = $to->parent;
                        }
                        else {
                            assert($from->patch);
                            xmem::undo($from->patch);
                            $from = $from->parent;
                        }
                    }
                    while(!{{ NAME }}$transit_vec.empty()) {
                        xmem::redo({{ NAME }}$transit_vec.back());
                        {{ NAME }}$transit_vec.pop_back();
                    }
                    {
                        {{ NAME }}$node * $node = {{ NAME }}$node::create($candidate->$parent);
#if __local__
                        $node->rank = $candidate->$rank;
#endif
                        ++$depthAcceptCount;
                        if($depthAcceptCount==1) {
                            $lastTopNode->release();
                            $lastTopNode = $node;
                            $lastTopNode->addRef();
                        }
                        {{ NAME }}$current_node->release();
                        {{ NAME }}$current_node = $node;
                    }
                    {{ NAME }}$origin(
    {%- for param in PARAMS -%}
                        {% if loop.index0!=0 %}, {% endif %}$candidate->{{ param }}
    {%- endfor -%}
                    );
                    $candidate->$release();
                    {{- process_reserved().replace('\n', '\n        ') }}
                    $t = getTime();
                    if($depthLimit<$t) {
                        break;
                    }
                }
            }
            else {
                int $width = max(1, (int)min((double){{ NAME }}$maxwidth, (($totalAcceptCount * ($wholeLimit - $t)) / (($t - $st2) * (1 + {{ NAME }}$maxdepth - {{ NAME }}$nowDepth)))));
                assert({{ NAME }}$orders.empty());
                while(!{{ NAME }}$generations[0].ranking->empty() && $depthAcceptCount<$width) {
                    ++$depthLoopCount;
                    {{ NAME }}$candidate * $candidate = {{ NAME }}$generations[0].ranking->large().second;
                    assert($candidate->$parent);
                    {{ NAME }}$generations[0].ranking->pop_large();
                    {{- accept_or_continue(False).replace('\n', '\n        ') }}
                    {{ NAME }}$node * $node = {{ NAME }}$node::create($candidate->$parent);
#if __local__
                    $node->rank = $candidate->$rank;
#endif
                    {{ NAME }}$orders.emplace_back({{ NAME }}$order::create({% if 'score' in PARAMS %}$candidate->score{% else %}0{% endif %}, $candidate, $node));
                    ++$depthAcceptCount;
                }
                if({{ NAME }}$orders.empty()) {
                    fprintf(stderr, "beam(%3d) ... skip2(error?)\n", {{ NAME }}$nowDepth);
                    continue;
                }
                $lastTopNode->release();
                $lastTopNode = {{ NAME }}$orders[0]->node;
                $lastTopNode->addRef();
                {
                    assert({{ NAME }}$orders_work.empty());
                    for({{ NAME }}$order * $order : {{ NAME }}$orders) {
                        {{ NAME }}$orders_work.emplace_back($order->node->parent, $order->score, $order);
                    }
                    {{ NAME }}$order * $start_order = {{ NAME }}$order::create(0, NULL, NULL);
                    int $reserved_depth = {{ NAME }}$current_node->depth;
                    while(2<={{ NAME }}$orders_work.size() || $reserved_depth!=-1) {
                        if(get<0>({{ NAME }}$orders_work[0])->depth==$reserved_depth) {
                            {{ NAME }}$orders_work.emplace_back({{ NAME }}$current_node, -DBL_MAX, $start_order);
                            $reserved_depth = -1;
                        }
                        std::sort({{ NAME }}$orders_work.begin(), {{ NAME }}$orders_work.end());
                        {{ NAME }}$orders_work.swap({{ NAME }}$orders_work2);
                        assert({{ NAME }}$orders_work.empty());
                        int j = 0;
                        {{ NAME }}$order * $last = get<2>({{ NAME }}$orders_work2[0]);
                        for(int i=1; ; ++i) {
                            if({{ NAME }}$orders_work2.size()<=i || get<0>({{ NAME }}$orders_work2[j])!=get<0>({{ NAME }}$orders_work2[i])) {
                                {{ NAME }}$orders_work.emplace_back(get<0>({{ NAME }}$orders_work2[j])->parent, get<2>({{ NAME }}$orders_work2[j])==$start_order ? -DBL_MAX : get<1>({{ NAME }}$orders_work2[i-1]), get<2>({{ NAME }}$orders_work2[j]));
                                if({{ NAME }}$orders_work2.size()<=i) {
                                    break;
                                }
                                $last = get<2>({{ NAME }}$orders_work2[i]);
                                j = i;
                            }
                            else {
                                while($last->link) {
                                    $last = $last->link;
                                }
                                $last->link = get<2>({{ NAME }}$orders_work2[i]);
                            }
                        }
                        {{ NAME }}$orders_work2.clear();
                    }
                    {{ NAME }}$orders.clear();
                    {
                        assert({{ NAME }}$orders_work.size()==1);
                        assert($reserved_depth==-1);
                        assert(get<2>({{ NAME }}$orders_work[0])==$start_order);
                        {{ NAME }}$order * $order = $start_order->link;
                        while($order) {
                            {{ NAME }}$orders.emplace_back($order);
                            $order = $order->link;
                        }
                    }
                    {{ NAME }}$orders_work.clear();
                    $start_order->release();
                }
                for({{ NAME }}$order * $order : {{ NAME }}$orders) {
                    //current_nodeからcandidate->$parentまで移動
                    {{ NAME }}$node * $from = {{ NAME }}$current_node;
                    {{ NAME }}$node * $to = $order->node->parent;
                    assert($from);
                    assert($to);
                    assert({{ NAME }}$transit_vec.empty());
                    while($from!=$to) {
                        assert($from);
                        assert($to);
                        if($from->depth<$to->depth) {
                            assert($to->patch);
                            {{ NAME }}$transit_vec.emplace_back($to->patch);
                            $to = $to->parent;
                        }
                        else {
                            assert($from->patch);
                            xmem::undo($from->patch);
                            $from = $from->parent;
                        }
                    }
                    while(!{{ NAME }}$transit_vec.empty()) {
                        xmem::redo({{ NAME }}$transit_vec.back());
                        {{ NAME }}$transit_vec.pop_back();
                    }
                    {{ NAME }}$current_node->release();
                    {{ NAME }}$current_node = $order->node;
                    assert($order->input);
                    {{ NAME }}$candidate * $candidate = $order->input;
                    {{ NAME }}$origin(
    {%- for param in PARAMS -%}
                        {% if loop.index0!=0 %}, {% endif %}$candidate->{{ param }}
    {%- endfor -%}
                    );
                    $candidate->$release();
                    $order->release();
                    {{- process_reserved().replace('\n', '\n        ') }}
                }
                {{ NAME }}$orders.clear();
                $t = getTime();
            }
            if({{ NAME }}$verboseFlag) {
                fprintf(stderr, "beam(%3d): %.3f->%.3f(%.6fs) (%d+)%d/%d ... %.1fa/s ... %.3f\n", {{ NAME }}$nowDepth, $depthStartTime, $t, $t-$depthStartTime, {{ NAME }}$generations[0].ra_count, $depthAcceptCount, $depthLoopCount, $depthAcceptCount/($t-$depthStartTime), $firstSc);
            }
            $totalLoopCount += $depthLoopCount;
            $totalAcceptCount += $depthAcceptCount;
            $totalRaCount += {{ NAME }}$generations[0].ra_count;
        }
        $t = getTime();
        if({{ NAME }}$verboseFlag) {
            fprintf(stderr, "beam(all): %.3f->%.3f(%.6fs) (%d+)%d/%d ... %.1fa/s\n", $st, $t, $t-$st, $totalRaCount, $totalAcceptCount, $totalLoopCount, $totalAcceptCount/($t-$st));
#if __local__
            {{ NAME }}$node * $node = $lastTopNode;
            std::map<int, int> $rank_count;
            while($node) {
                if($node->rank!=-1) {
                    ++$rank_count[$node->rank];
                }
                $node = $node->parent;
            }
            bool $flag = false;
            fprintf(stderr, "beam ranks { ");
            for(std::pair<int, int> $rc : $rank_count) {
                if($flag) {
                    fprintf(stderr, ", ");
                }
                fprintf(stderr, "%d: %d", $rc.first, $rc.second);
                $flag = true;
            }
            fprintf(stderr, " }\n\n");
#endif
            fprintf(stderr, "\n");
        }
{%- if def('vis') %}
        if(__local__) {
            getTimeStop();
            {{ NAME }}$node * $node0 = {{ NAME }}$current_node;
            while({{ NAME }}$current_node) {
                assert({{ NAME }}$current_node->patch);
                xmem::undo({{ NAME }}$current_node->patch);
                {{ NAME }}$current_node = {{ NAME }}$current_node->parent;
            }
            assert(!{{ NAME }}$current_node);
            {{ NAME }}$node * $node = $lastTopNode;
            vector<{{ NAME }}$node *> $nodes;
            while($node) {
                $nodes.emplace_back($node);
                $node = $node->parent;
            }
            assert(!$node);
            {{ vis }}();
            while(!$nodes.empty()) {
                assert($nodes.back()->patch);
                xmem::redo($nodes.back()->patch);
                $nodes.pop_back();
                {{ vis }}();
            }
            {{ NAME }}$current_node = $lastTopNode;
            $node0->release();
            getTimeStart();
        }
{%- else %}
        $lastTopNode->release();
{%- endif %}
        {
            {{ NAME }}$node * $node = {{ NAME }}$current_node;
            while({{ NAME }}$current_node) {
                assert({{ NAME }}$current_node->patch);
                xmem::undo({{ NAME }}$current_node->patch);
                {{ NAME }}$current_node = {{ NAME }}$current_node->parent;
            }
            $node->release();
        }
        {{ NAME }}$lock = false;
    }
}
