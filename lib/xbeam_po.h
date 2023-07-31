struct xbeam_po$node {
    static fast_pool<xbeam_po$node> pool;
    xbeam_po$node * parent;
    char * input;
    void * patch;
    int score_diff_count;
    double score_diff_sum;
    double score_diff2_sum;
    double score_bonus;
    int depth;
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
            xmem::free(patch);//myxmem.free(patch);
            pool.push(this);
        }
        else {
            --ref;
        }
    }
    inline double getInnerScore() const {
        return *(double*)(input + sizeof(char*));
    }
    inline double getScore0() const {
        return *(double*)(input + sizeof(char*) + sizeof(double));
    }
    inline double getScore() const {
        return getInnerScore() + score_bonus;
    }
    inline bool change() {
        assert(input);
        input = *(char**)input;
        if(!input) {
            return false;
        }
        assert(1<=score_diff_count);
        return true;
    }
    inline bool reschedule(double score) {
        assert(input);
        *(double*)(input + sizeof(char*)) = score;
        char * input2 = *(char**)input;
        if(!input2 || *(double*)(input2 + sizeof(char*))<=score) {
            return false;
        }
        char * input1 = input;
        input = input2;
        while(true) {
            char * input3 = *(char**)input2;
            if(!input3 || *(double*)(input3 + sizeof(char*))<=score) {
                *(char**)input2 = input1;
                *(char**)input1 = input3;
                assert(1<=score_diff_count);
                return true;
            }
            input2 = input3;
        }
    }
    inline void update(double score, double avg_bonus_rate, double stdev_bonus_rate, double ucb_bonus_rate) {
        score_bonus = 0;
        ++score_diff_count;
        if(avg_bonus_rate || stdev_bonus_rate) {
            double score_diff = score - getScore0();
            score_diff_sum += score_diff;
            double score_diff_avg = score_diff_sum / score_diff_count;
            if(avg_bonus_rate) {
                score_bonus += avg_bonus_rate * score_diff_avg;
            }
            if(stdev_bonus_rate) {
                score_diff2_sum += score_diff * score_diff;
                double score_diff_stdev = std::sqrt((score_diff2_sum - score_diff * score_diff_avg) / score_diff_count);
                score_bonus += stdev_bonus_rate * score_diff_stdev;
            }
        }
        if(ucb_bonus_rate) {
            score_bonus += ucb_bonus_rate / std::sqrt(score_diff_count);
        }
    }
    inline static xbeam_po$node * create() {
        xbeam_po$node * n = pool.pop();
        n->parent = 0;
        n->input = 0;
        n->patch = 0;
        n->score_diff_count = 0;
        n->score_diff_sum = 0;
        n->score_diff2_sum = 0;
        n->score_bonus = 0;
        n->depth = -1;
        n->ref = 1;
        return n;
    }
    inline static xbeam_po$node * create_release(xbeam_po$node * parent_, char * input) {
        //parent_->addRef();
        xbeam_po$node * n = pool.pop();
        n->parent = parent_;
        n->input = input;
        n->patch = 0;
        n->score_diff_count = 0;
        n->score_diff_sum = 0;
        n->score_diff2_sum = 0;
        n->score_bonus = 0;
        n->depth = parent_->depth + 1;
        n->ref = 1;
        return n;
    }
};
fast_pool<xbeam_po$node> xbeam_po$node::pool;
struct xbeam_po {
    xmem myxmem;
    xbeam_po$node * current_node;
    xbeam_po$node * next_node = NULL;
    typedef std::pair<double, xbeam_po$node*> ranking_item;
    typedef dpque<ranking_item> ty_ranking;
    std::vector<ty_ranking> rankings;
    std::vector<std::pair<double, char*> > reserves;
    double current_score;
    char * input_mem = 0;
    xnodemem nmem;
    double nextLimit;
    double wholeLimit;
    int remain_depth;
    int max_beam_size;
    int infoAcceptCount;
    int infoDepthAcceptCount;
    int infoLoopCount;
    int infoDepthLoopCount;
    int infoLoopCount2;
    int infoDepthLoopCount2;
    int infoDepth;
    int skip_count;
    double infoStartTime;
    double infoDepthStartTime;
    bool verboseFlag = true;
    double avg_bonus_rate = 0.0;
    double stdev_bonus_rate = 0.0;
    double ucb_bonus_rate = 0.0;
    xbeam_po() {
    }
    void setVerbose(bool flag) {
        verboseFlag = flag;
    }
    void setBonusRates(double avg_bonus_rate, double stdev_bonus_rate, double ucb_bonus_rate) {
        this->avg_bonus_rate = avg_bonus_rate;
        this->stdev_bonus_rate = stdev_bonus_rate;
        this->ucb_bonus_rate = ucb_bonus_rate;
    }
    void clear_ranking(int i) {
        for(const ranking_item & v : rankings[i]) {
            v.second->release();
        }
        rankings[i].clear();
    }
    int getRemainDepth() const {
        return remain_depth;
    }
    void init(int max_depth_, double timeLimit=9.8, int max_beam_size_=2000) {
        infoAcceptCount = 0;
        infoDepthAcceptCount = 0;
        infoLoopCount = 0;
        infoDepthLoopCount = 0;
        infoLoopCount2 = 0;
        infoDepthLoopCount2 = 0;
        infoDepth = 0;
        skip_count = 0;
        remain_depth = max_depth_;
        max_beam_size = max_beam_size_;
        wholeLimit = timeLimit;
        {
            double t = getTime();
            nextLimit = (wholeLimit - t) / (remain_depth + 1) + t;
            infoDepthStartTime = infoStartTime = t;
        }
        reserves.clear();
        nmem.free();
        for(int i=0; i<rankings.size(); ++i) {
            clear_ranking(i);
        }
        rankings.resize(remain_depth + 1);
        xmem::lock();
        assert(next_node==NULL);
        current_node = xbeam_po$node::create();
        current_node->patch = myxmem.build_undo();
    }
    void dest_(double t) {
        xmem::unlock();
        if(verboseFlag) {
            fprintf(stderr, "beam_po(all): %.3f->%.3f(%.6fs) %d/%d(%d) ... %.1fa/s\n\n", infoStartTime, t, t-infoStartTime, infoAcceptCount, infoLoopCount, infoLoopCount+infoLoopCount2, infoAcceptCount/(t-infoStartTime));
        }
        xbeam_po$node * cn = current_node;
        while(current_node) {
            if(current_node->patch) {
                xmem::undo(current_node->patch);
            }
            current_node = current_node->parent;
        }
        cn->release();
    }
    void infoForNextDepth_(double t) {
        if(verboseFlag && 1<=infoLoopCount-infoDepthLoopCount) {
            fprintf(stderr, "beam_po(%3d): %.3f->%.3f(%.6fs) %d/%d(%d) ... %.1fa/s\n", infoDepth, infoDepthStartTime, t, t-infoDepthStartTime, infoAcceptCount-infoDepthAcceptCount, infoLoopCount-infoDepthLoopCount, infoLoopCount+infoLoopCount2-infoDepthLoopCount-infoDepthLoopCount2, (infoAcceptCount-infoDepthAcceptCount)/(t-infoDepthStartTime));
        }
        infoDepthStartTime = t;
        infoDepthAcceptCount = infoAcceptCount;
        infoDepthLoopCount = infoLoopCount;
        infoDepthLoopCount2 = infoLoopCount2;
        ++infoDepth;
    }
    void transit_(xbeam_po$node * current_node, xbeam_po$node * next_node) {
        assert(next_node);
        assert(next_node->patch);
        assert(next_node->parent!=0);
        assert(current_node!=next_node);
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
    void accept() {
        //acceptは呼び出し必須になりました！
        assert(next_node);
        if(next_node->patch) {
            ++infoAcceptCount;
        }
        //前回のノードと候補ノードとの共通の最も近い親を探して、patchを使って盤面を移動する
        if(current_node!=next_node) {
            transit_(current_node, next_node);
        }
        //前回ノードを解放して、候補ノードを元に新しく作ったものを現在ノードとする。
        current_node->release();
        current_node = next_node;
        next_node = NULL;
        current_score = current_node->getScore0();
        if(skip_count) {
            ++skip_count;
        }
    }
    long long get_current_node() {
        assert(current_node);
        current_node->addRef();
        return (long long)(void*)current_node;
    }
    void release_node(long long node) {
        ((xbeam_po$node *)(void *)node)->release();
    }
    long long parent_node(long long node) {
        return (long long)(void*)(((xbeam_po$node *)(void *)node)->parent);
    }
    void undo_node(long long node) {
        xmem::undo(((xbeam_po$node *)(void *)node)->patch);
    }
    void redo_node(void * node) {
        xmem::redo(((xbeam_po$node *)(void *)node)->patch);
    }
    bool onloop() {
        if(next_node) {
            if(next_node==(xbeam_po$node*)-1) {
            }
            else if(next_node->change()) {
                assert(1<=next_node->score_diff_count);
                assert(next_node->depth<rankings.size());
                assert(next_node->input);
                rankings[next_node->depth].emplace(next_node->getScore(), next_node);
            }
            else {
                next_node->release();
            }
            next_node = NULL;
        }
        else if(!reserves.empty()) {
            std::sort(reserves.begin(), reserves.end());
            char * inputs = NULL;
            for(std::pair<double, char*> & r : reserves) {
                *(char**)r.second = inputs;
                inputs = r.second;
            }
            reserves.clear();
            current_node = xbeam_po$node::create_release(current_node, inputs);
            xmem::lock();
            assert(next_node==NULL);
            next_node = current_node;
            current_node->addRef();
            assert(next_node->input);
            input_mem = next_node->input + (sizeof(char*) + sizeof(double) + sizeof(double));
            ++infoLoopCount2;
            return true;
        }
        else {
            xmem::undo();
            xbeam_po$node * node = current_node;
            assert(node);
            while(node->patch==NULL) {
                xmem::unlock();
                if(3<=skip_count) {
                    --skip_count;
                    xmem::undo();
                }
                else {
                    node->patch = myxmem.build_undo();
                    node->update(current_score, avg_bonus_rate, stdev_bonus_rate, ucb_bonus_rate);
                    if(node->change()) {
                        assert(node->score_diff_count==1);
                        assert(node->depth<rankings.size());
                        double score = node->getScore();
                        ty_ranking & ranking = rankings[node->depth];
                        if(ranking.size()<max_beam_size || ranking.small().first<score) {
                            if(max_beam_size<=ranking.size()) {
                                ranking.small().second->release();
                                ranking.pop_small();
                            }
                            node->addRef();
                            assert(node->input);
                            ranking.emplace(score, node);
                        }
                    }
                }
                node = node->parent;
                assert(node);
            }
            assert(node);
            node->addRef();
            current_node->release();
            current_node = node;
            if(node->parent) {
                node->update(current_score, avg_bonus_rate, stdev_bonus_rate, ucb_bonus_rate);
                if(node->change()) {
                    assert(2<=node->score_diff_count);
                    assert(node->depth<rankings.size());
                    double score = node->getScore();
                    ty_ranking & ranking = rankings[node->depth];
                    if(ranking.size()<max_beam_size || ranking.small().first<score) {
                        node->addRef();
                        assert(node->input);
                        ranking.emplace(score, node);
                    }
                }
            }
        }
        double t = getTime();
        if(nextLimit<=t || rankings[infoDepth].empty() || max_beam_size<=infoAcceptCount-infoDepthAcceptCount) {
            clear_ranking(infoDepth);
            while(rankings[infoDepth].empty()) {
                infoForNextDepth_(t);
                if(remain_depth<=0) {
                    dest_(t);
                    return false;
                }
                --remain_depth;
            }
            nextLimit = (wholeLimit - t) / (remain_depth+1) + t;
        }
        //最初に、ビームの中から次の候補を取り出す。
        next_node = rankings[infoDepth].large().second;
        rankings[infoDepth].pop_large();
        //以降のnextInt等は、候補ノードから情報を読み出す。
        assert(next_node->input);
        input_mem = next_node->input + (sizeof(char*) + sizeof(double) + sizeof(double));
        skip_count = 0;
        ++infoLoopCount;
        //trueを返す。
        return true;
    }
    bool reserve(double score=0, double score0=DBL_MAX) {
        if((int)rankings.size()<=current_node->depth+1) {
            return false;
        }
        if(score0==DBL_MAX) {
            score0 = score;
        }
        reserves.emplace_back(score, nmem.alloc());
        nmem.skip(sizeof(char*));
        nmem.next(score);
        nmem.next(score0);
        return true;
    }
    bool skip() {
        if(!next_node->patch) {
            if(!skip_count) {
                skip_count = 1;
            }
            return false;
        }
        return true;
    }
    bool reschedule(double score) {
        assert(next_node);
        if(!next_node->patch) {
            return false;
        }
        ty_ranking & ranking = rankings[next_node->depth];
        if(!next_node->reschedule(score)) {
            if(ranking.empty()) {
                return false;
            }
            if(ranking.large().first<=score+next_node->score_bonus) {
                return false;
            }
        }
        assert(next_node);
        assert(next_node->input);
        ranking.emplace(next_node->getScore(), next_node);
        next_node = (xbeam_po$node*)-1;
        return true;
    }
    //以下、シリアライズ系
    void nextDouble(double value) {
        nmem.next(value);
    }
    void nextFloat(float value) {
        nmem.next(value);
    }
    void nextInt64(long long value) {
        nmem.next(value);
    }
    void nextInt(int value) {
        nmem.next(value);
    }
    void nextShort(short value) {
        nmem.next(value);
    }
    void nextChar(char value) {
        nmem.next(value);
    }
    double nextDouble() {
        double ret = *(double*)input_mem;
        input_mem += sizeof(ret);
        return ret;
    }
    float nextFloat() {
        float ret = *(float*)input_mem;
        input_mem += sizeof(ret);
        return ret;
    }
    long long nextInt64() {
        long long ret = *(long long*)input_mem;
        input_mem += sizeof(ret);
        return ret;
    }
    int nextInt() {
        int ret = *(int*)input_mem;
        input_mem += sizeof(ret);
        return ret;
    }
    short nextShort() {
        short ret = *(short*)input_mem;
        input_mem += sizeof(ret);
        return ret;
    }
    char nextChar() {
        char ret = *(char*)input_mem;
        input_mem += sizeof(ret);
        return ret;
    }
};
