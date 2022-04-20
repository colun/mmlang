struct xsa_dac$node;
struct xsa_dac$option;
struct xsa_dac$sub;

struct xsa_dac$node {
    xsa_dac$node * parent;
    void * patch;
    int depth;
    int ref;
    int option_count;
    int selected;
    int selection_count;//TODO: 値を入れてないし、そもそもnodeなのかoptionなのかも未決定。
    inline xsa_dac$option & operator[](int i) {
        assert(i<option_count);
        return *(xsa_dac$option *)((char*)this - ((int*)(this + 1))[i]);
    }
};
struct xsa_dac$option {
    long long hash;
    double expect_score;
    double observed_score;
    int sub_count;
    bool visited;
    inline xsa_dac$sub & operator[](int i) {
        assert(i<sub_count);
        return ((xsa_dac$sub*)(this+1))[i];
    }
};
struct xsa_dac$sub {
    void * input;
    xsa_dac$node * node;
};
//分割統治(divide-and-conquer)問題解決用の焼きなまし風ソルバ
struct xsa_dac {
    xsa_dac$node * root_node;
    xsa_dac$node * mutate_node;
    xsa_dac$node * current_node;
    xsa_dac$sub * current_sub;
    vector<xsa_dac$option *> creating_options;
    fast_weak_map64<unsigned int> selected_table;
    fast_weak_map64<double> observed_table;
    vector<xsa_dac$sub*> visit_stack;
    vector<xsa_dac$node*> visiting_stack;
    vector<tuple<double, int> > work_probs;
    xmem myxmem;
    xnodemem node_mem;
    xnodemem input_mem;
    int sub_count;
    bool verboseFlag = true;
    int infoAcceptCount;
    double timeLimit;
    double revT0;
    double revT;//TODO: 計算されていない
    unsigned int transitions = 0;
    xsa_dac() {
    }
    void init(double timeLimit=9.8, double T0=1.0) {
        this->timeLimit = timeLimit;
        revT0 = 1.0 / T0;
        node_mem.free();
        input_mem.free();
        //root_node = NULL;
        //mutate_node = NULL;
        current_node = NULL;
        current_sub = NULL;
        creating_options.clear();
        sub_count = 0;
        infoAcceptCount = 0;
        transitions = 0;
        node_mem.alloc();
    }
    void setVerbose(bool flag) {
        verboseFlag = flag;
    }
    void visit_(xsa_dac$node & node) {
        xsa_dac$option & option = node[node.selected];
        assert(!option.visited);
        for(int i=0; i<option.sub_count; ++i) {
            visit_stack.push_back(&option[i]);
        }
        visiting_stack.push_back(node);
    }
    void transit_(xsa_dac$node * current_node, xsa_dac$node * next_node) {
        //TODO: xbeamから持ってきただけ。おそらく直しが必要。最初のparent持ってくるところとかね。
        assert(next_node);
        assert(next_node->patch==0);
        assert(next_node->parent!=0);
        next_node = next_node->parent;
        std::vector<void*> vec;
        while(current_node!=next_node) {
            assert(current_node);
            assert(next_node);
            if(current_node->depth<next_node->depth) {
                assert(next_node->patch);
                vec.emplace_back(next_node->patch);
                next_node = next_node->parent;
            }
            else {
                assert(current_node->patch);
                xmem::undo(current_node->patch);
                current_node = current_node->parent;
            }
        }
        while(!vec.empty()) {
            xmem::redo(vec.back());
            vec.pop_back();
        }
    }
    void update_(xsa_dac$node * node) {
        assert(node);
        while(true) {
            xsa_dac$option & option = (*node)[node->selected];
            assert(option.visited);
            int sub_count = option.sub_count;
            int selection_count = (2<=node->option_count ? 1 : 0);
            double score = 0;
            for(int i=0; i<sub_count; ++i) {
                xsa_dac$node & node2 = *option[i].node;
                selection_count += node2.selection_count;
                score += node2[node2.selected].observed_score;
            }
            node->selection_count = selection_count;
            option.observed_score = score;
            observed_table.set(option.hash, score - option.expect_score);
            node = node->parent;
            if(!node) {
                break;
            }
        }
    }
    void mutate_(xsa_dac$node & node, int selected) {
        node.selected = selected;
        if(node[node.selected].visited) {
            update_(&node);
        }
        else {
            visit_(node);
        }
    }
    bool onloop() {
        {
            int option_count = creating_options.size();
            xsa_dac$node * p = (xsa_dac$node*)node_mem.now();
            node_mem.skip(sizeof(xsa_dac$node));
            p->parent = current_node;
            p->patch = myxmem.build();
            p->option_count = option_count;
            if(creating_options.size()) {
                for(xsa_dac$option * p2 : creating_options) {
                    node_mem.next((int)((char*)p - (char*)p2));
                }
                int selected = -1;
                {
                    unsigned int max_v = 0;
                    unsigned int v;
                    for(int i=0; i<option_count; ++i) {
                        if(selected_table.testget(creating_options[i]->hash, v)) {
                            if(max_v < v) {
                                max_v = v;
                                selected = i;
                            }
                        }
                    }
                }
                if(selected==-1) {
                    work_probs.resize(option_count);
                    int observed_count = 0;
                    double correction_sum = 0;
                    for(int i=0; i<option_count; ++i) {
                        xsa_dac$option & option = creating_options[i];
                        if(option.visited) {
                            work_probs[i] = tuple<double, int>(option.observed_score, 0);
                            ++observed_count;
                            correction_sum += option.observed_score - option.expect_score;
                        }
                        else {
                            double observed_score_diff;
                            if(observed_table.testget(option.hash, observed_score_diff)) {
                                work_probs[i] = tuple<double, int>(option.expect_score + observed_score_diff, 0);
                                ++observed_count;
                                correction_sum += observed_score_diff;
                            }
                            else {
                                work_probs[i] = tuple<double, int>(option.expect_score, 1);
                            }
                        }
                    }
                    double correction = observed_count ? correction_sum / observed_count : 0;
                    double max_score = -DBL_MAX;
                    for(int i=0; i<option_count; ++i) {
                        if(get<1>(work_probs[i])) {
                            get<0>(work_probs[i]) += correction;
                        }
                        get<1>(work_probs[i]) = i;
                        max_score = max(max_score, get<0>(work_probs[i]));
                    }
                    for(int i=0; i<option_count; ++i) {
                        double score = get<0>(work_probs[i]);
                        double prob = exp((score - max_score) * revT);
                        get<0>(work_probs[i]) = prob;
                    }
                    std::sort(work_probs.begin(), work_probs.end());
                    for(int i=1; i<option_count; ++i) {
                        get<0>(work_probs[i]) += get<0>(work_probs[i-1]);
                    }
                    double p = drand49() * get<0>(work_probs.back());
                    selected = get<1>(work_probs[option_count-1]);
                    for(int i=0; i<option_count-1; ++i) {
                        if(p<get<0>(work_probs[i])) {
                            selected = get<1>(work_probs[i]);
                            break;
                        }
                    }
                    selected_table.set(creating_options[selected]->hash, ++transitions);
                }
                assert(selected!=-1);
                p->selected = selected;
                visit_(*p);//TODO: 葉ノードのまとめのときって、visited==trueじゃないっけ？
                creating_options.clear();
            }
            else {
                p->selected = -1;
            }
            if(current_sub) {
                current_sub->node = p;
            }
            else {
                mutate_node = root_node = p;
            }
            current_node = p;
        }
        if(visit_stack.empty()) {
            while(!visiting_stack.empty()) {
                xsa_dac$node & node = *visiting_stack.back();
                visiting_stack.pop_back();
                assert(!node[node.selected].visited);
                node[node.selected].visited = true;
                update_(&node);
            }
            if(root_node->selection_count==0) {
                //TODO: onleafの準備をする
                return false;
            }
            //次回訪問予約スタックが空の間
            while(visit_stack.empty()) {
                if(timeLimit<=getTime()) {
                    //TODO: onleafの準備をする
                    return false;
                }
                assert(mutate_node);
                assert(mutate_node->selection_count!=0);
                Label0:;
                int selection_id = lrand49(mutate_node->selection_count);
                while(true) {
                    {
                        int option_count = mutate_node->option_count;
                        if(option_count<=1) {
                            assert(mutate_node->selected==0);
                        }
                        else {
                            --selection_id;
                            int selected;
                            if(selection_id!=-1) { // 現在の選択を確認する
                                selected = mutate_node->selected;
                                unsigned int max_v = 0;
                                unsigned int v;
                                for(int i=0; i<option_count; ++i) {
                                    if(selected_table.testget((*mutate_node)[i].hash, v)) {
                                        if(max_v < v) {
                                            max_v = v;
                                            selected = i;
                                        }
                                    }
                                }
                            }
                            else { // 選択が無い場合、あるいはselection_idがヒットした場合は、ランダムに選ぶ
                                work_probs.resize(option_count);
                                int observed_count = 0;
                                double correction_sum = 0;
                                for(int i=0; i<option_count; ++i) {
                                    xsa_dac$option & option = (*mutate_node)[i];
                                    if(option.visited) {
                                        work_probs[i] = tuple<double, int>(option.observed_score, 0);
                                        ++observed_count;
                                        correction_sum += option.observed_score - option.expect_score;
                                    }
                                    else {
                                        double observed_score_diff;
                                        if(observed_table.testget(option.hash, observed_score_diff)) {
                                            work_probs[i] = tuple<double, int>(option.expect_score + observed_score_diff, 0);
                                            ++observed_count;
                                            correction_sum += observed_score_diff;
                                        }
                                        else {
                                            work_probs[i] = tuple<double, int>(option.expect_score, 1);
                                        }
                                    }
                                }
                                double correction = observed_count ? correction_sum / observed_count : 0;
                                double max_score = -DBL_MAX;
                                for(int i=0; i<option_count; ++i) {
                                    if(get<1>(work_probs[i])) {
                                        get<0>(work_probs[i]) += correction;
                                    }
                                    get<1>(work_probs[i]) = i;
                                    max_score = max(max_score, get<0>(work_probs[i]));
                                }
                                for(int i=0; i<option_count; ++i) {
                                    double score = get<0>(work_probs[i]);
                                    double prob = exp((score - max_score) * revT);
                                    get<0>(work_probs[i]) = prob;
                                }
                                std::sort(work_probs.begin(), work_probs.end());
                                for(int i=1; i<option_count; ++i) {
                                    get<0>(work_probs[i]) += get<0>(work_probs[i-1]);
                                }
                                double p = drand49() * get<0>(work_probs.back());
                                selected = get<1>(work_probs[option_count-1]);
                                for(int i=0; i<option_count-1; ++i) {
                                    if(p<get<0>(work_probs[i])) {
                                        selected = get<1>(work_probs[i]);
                                        break;
                                    }
                                }
                                selected_table.set((*mutate_node)[selected].hash, ++transitions);
                            }
                            assert(selected!=-1);
                            if(mutate_node->selected!=selected) {
                                //テーブルが書き換わっていた場合は遷移処理。
                                mutate_(*mutate_node, selected);
                                if(!visit_stack.empty()) {
                                    goto Label1;
                                }
                                if(mutate_node->selection_count<2) {
                                    mutate_node = root_node;
                                    goto Label0;
                                }
                                else {
                                    selection_id = lrand49(mutate_node->selection_count - 1);
                                }
                            }
                            if(selection_id==-1) {
                                mutate_node = root_node;
                                goto Label0;
                            }
                        }
                    }
                    xsa_dac$option & option = (*mutate_node)[mutate_node->selected];
                    int sub_count = option.sub_count;
                    int i;
                    for(i=0; i<sub_count-1; ++i) {
                        int selection_count = option[i].node->selection_count;
                        if(selection_id < selection_count) {
                            break;
                        }
                        selection_id -= selection_count;
                    }
                    mutate_node = option[i].node;
                }
            }
            Label1:;
        }
        xsa_dac$sub * next_sub = visit_stack.back();
        assert(next_sub);
        ++infoAcceptCount;
        transit_(current_node, next_node);
        current_node = next_node;
        next_node = NULL;
        node_mem.alloc();
        return true;
    }
    bool onleaf() {
        //TODO: 選択済みの各葉ノードを回る間はtrueを返す
    }
    void release() {
        //TODO: rootノードに遷移する
        //TODO: その他、必要な解放があれば、行う。（ないかも？）
    }
    void reportScore(double score) {
        xsa_dac$option * p = (xsa_dac$option*)node_mem.now();
        node_mem.skip(sizeof(xsa_dac$option));
        p->hash = 0;
        p->expect_score = score;
        p->observed_score = score;
        p->sub_count = 0;
        p->visited = true;
        creating_options.push_back(p);
    }
    void reserve(long long hash, double score=0, bool head=true) {
        if(head) {
            xsa_dac$option * p = (xsa_dac$option*)node_mem.now();
            node_mem.skip(sizeof(xsa_dac$option));
            p->hash = hash;
            p->expect_score = score;
            p->observed_score = score;
            p->sub_count = 0;
            p->visited = false;
            creating_options.push_back(p);
        }
        else {
            creating_options.back()->hash ^= hash;
            creating_options.back()->expect_score += score;
            creating_options.back()->observed_score += score;
        }
        {
            xsa_dac$sub * p = (xsa_dac$sub*)node_mem.now();
            node_mem.skip(sizeof(xsa_dac$sub));
            p->input = input_mem.alloc();
            p->node = NULL;
            ++creating_options.back()->sub_count;
        }
    }
    void nextDouble(double value) {
        input_mem.next(value);
    }
    void nextFloat(float value) {
        input_mem.next(value);
    }
    void nextInt64(long long value) {
        input_mem.next(value);
    }
    void nextInt(int value) {
        input_mem.next(value);
    }
    void nextShort(short value) {
        input_mem.next(value);
    }
    void nextChar(char value) {
        input_mem.next(value);
    }
    double nextDouble() {
    }
    float nextFloat() {
    }
    long long nextInt64() {
    }
    int nextInt() {
    }
    short nextShort() {
    }
    char nextChar() {
    }
};
