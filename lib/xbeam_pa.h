//pa ... proactive(先読み)
struct xbeam_pa$node {
    static fast_pool<xbeam_pa$node> pool;
    xbeam_pa$node * parent;
    char * input;
    void * patch;
    int depth;
    int ref;
#if __local__
    int rank;
#endif
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
    inline static xbeam_pa$node * create() {
        xbeam_pa$node * n = pool.pop();
        n->parent = 0;
        n->input = 0;
        n->patch = 0;
        n->depth = 0;
        n->ref = 1;
#if __local__
        n->rank = -1;
#endif
        return n;
    }
    inline static xbeam_pa$node * create(xbeam_pa$node * parent_, char * input_) {
        parent_->addRef();
        xbeam_pa$node * n = pool.pop();
        n->parent = parent_;
        n->input = input_;
        n->patch = 0;
        n->depth = parent_->depth + 1;
        n->ref = 1;
#if __local__
        n->rank = -1;
#endif
        return n;
    }
};
fast_pool<xbeam_pa$node> xbeam_pa$node::pool;
struct xbeam_pa {
    xmem myxmem;
    xbeam_pa$node * current_node;
    xbeam_pa$node * next_node = NULL;
    typedef std::pair<double, xbeam_pa$node*> ranking_item;
    typedef dpque<ranking_item> ty_ranking;
    typedef std::pair<ty_ranking *, xnodemem *> rankings_item;
    std::deque<rankings_item> rankings;
    std::vector<double> sum_rank_vec;
    double zipf_rate = 3;
    xnodemem * nmem = NULL;
#if __local__
    xbeam_pa$node * last_top_node = NULL;
#endif
    std::vector<ranking_item> reserved;
    char * input_mem = 0;
    double firstSc;
    double nextLimit;
    double wholeLimit;
    int remain_depth;
    int max_proactive;
    int max_beam_size;
    int infoAcceptCount;
    int infoDepthAcceptCount;
    int infoLoopCount;
    int infoDepthLoopCount;
    int infoDepth;
    double infoStartTime;
    double infoDepthStartTime;
    bool verboseFlag = true;
    inline xbeam_pa() {
    }
    inline void setVerbose(bool flag) {
        verboseFlag = flag;
    }
    inline void clear_ranking(ty_ranking * rnk) {
        for(const ranking_item & v : *rnk) {
            v.second->release();
        }
        rnk->clear();
    }
    inline int getRemainDepth() const {
        return remain_depth;
    }
    inline void setProactive(int proactive) {
        max_proactive = proactive;
    }
    inline void setZipfRate(double rate) {
        zipf_rate = rate;
    }
    inline void init(int max_depth_, double timeLimit=9.8, int max_beam_size_=2000) {
        infoAcceptCount = 0;
        infoDepthAcceptCount = 0;
        infoLoopCount = 0;
        infoDepthLoopCount = 0;
        infoDepth = 0;
        remain_depth = max_depth_;
        max_beam_size = max_beam_size_;
        max_proactive = 0;
        firstSc = 0;
        wholeLimit = timeLimit;
        {
            double t = getTime();
            nextLimit = (wholeLimit - t) / (remain_depth + 1) + t;
            infoDepthStartTime = infoStartTime = t;
        }
        assert(next_node==NULL);
#if __local__
        assert(last_top_node==NULL);
#endif
        assert(reserved.empty());
        current_node = xbeam_pa$node::create();
        for(rankings_item ri : rankings) {
            clear_ranking(ri.first);
            ri.second->free();
        }
        while(rankings.size()<2) {
            rankings.emplace_back(new ty_ranking(), new xnodemem());
        }
        sum_rank_vec.clear();
        xmem::lock();
    }
    inline void dest_(double t) {
        xmem::unlock();
        if(verboseFlag) {
#if __local__
            fprintf(stderr, "beam_pa(all): %.3f->%.3f(%.6fs) %d/%d ... %.1fa/s\n", infoStartTime, t, t-infoStartTime, infoAcceptCount, infoLoopCount, infoAcceptCount/(t-infoStartTime));
            if(last_top_node!=NULL) {
                xbeam_pa$node * node = last_top_node;
                std::map<int, int> rank_count;
                while(node) {
                    if(node->rank!=-1) {
                        ++rank_count[node->rank];
                    }
                    node = node->parent;
                }
                last_top_node->release();
                last_top_node = NULL;
                bool flag = false;
                fprintf(stderr, "beam ranks { ");
                for(std::pair<int, int> rc : rank_count) {
                    if(flag) {
                        fprintf(stderr, ", ");
                    }
                    fprintf(stderr, "%d: %d", rc.first, rc.second);
                    flag = true;
                }
                fprintf(stderr, " }\n\n");
            }
#else
            fprintf(stderr, "beam_pa(all): %.3f->%.3f(%.6fs) %d/%d ... %.1fa/s\n\n", infoStartTime, t, t-infoStartTime, infoAcceptCount, infoLoopCount, infoAcceptCount/(t-infoStartTime));
#endif
        }
        xbeam_pa$node * cn = current_node;
        while(current_node) {
            if(current_node->patch) {
                xmem::undo(current_node->patch);
            }
            current_node = current_node->parent;
        }
        cn->release();
    }
    inline void infoForNextDepth_(double t) {
        if(verboseFlag && 1<=infoLoopCount-infoDepthLoopCount) {
            fprintf(stderr, "beam_pa(%3d): %.3f->%.3f(%.6fs) %d/%d ... %.1fa/s ... %.3f\n", infoDepth, infoDepthStartTime, t, t-infoDepthStartTime, infoAcceptCount-infoDepthAcceptCount, infoLoopCount-infoDepthLoopCount, (infoAcceptCount-infoDepthAcceptCount)/(t-infoDepthStartTime), firstSc);
        }
        infoDepthStartTime = t;
        infoDepthAcceptCount = infoAcceptCount;
        infoDepthLoopCount = infoLoopCount;
        ++infoDepth;
    }
    inline void transit_(xbeam_pa$node * current_node, xbeam_pa$node * next_node) {
        assert(next_node);
        assert(next_node->patch==0);
        assert(next_node->parent!=0);
        assert(current_node!=next_node);
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
    inline void accept() {
        //acceptは呼び出し必須になりました！
        assert(next_node);
        ++infoAcceptCount;
        //前回のノードと候補ノードとの共通の最も近い親を探して、patchを使って盤面を移動する
        transit_(current_node, next_node);
        //前回ノードを解放して、候補ノードを現在ノードとする。
        current_node->release();
        current_node = next_node;
        next_node = NULL;
    }
    inline long long get_current_node() {
        assert(current_node);
        current_node->addRef();
        return (long long)(void*)current_node;
    }
    inline void release_node(long long node) {
        ((xbeam_pa$node *)(void *)node)->release();
    }
    inline long long parent_node(long long node) {
        return (long long)(void*)(((xbeam_pa$node *)(void *)node)->parent);
    }
    inline void undo_node(long long node) {
        xmem::undo(((xbeam_pa$node *)(void *)node)->patch);
    }
    inline void redo_node(long long node) {
        xmem::redo(((xbeam_pa$node *)(void *)node)->patch);
    }
    inline bool onloop() {
        if(next_node) {
            assert(reserved.empty());
            if(next_node->depth!=infoDepth) {
                xbeam_pa$node * node = current_node;
                assert(node);
                if(node->parent) {
                    while(node->patch==NULL) {
                        xmem::unlock();
                        if(node->parent) {
                            node->patch = myxmem.build_undo();
                            node = node->parent;
                        }
                        else {
                            node->patch = myxmem.build();
                            break;
                        }
                    }
                }
                else {
                    xmem::unlock();
                    assert(!node->patch);
                    node->patch = myxmem.build();
                }
                node->addRef();
                current_node->release();
                current_node = node;
            }
            next_node->release();
            next_node = NULL;
        }
        else {
            current_node->input = NULL;
            if(!reserved.empty()) {
                int current_depth = current_node->depth;
                assert(next_node==NULL);
                int proactive = current_depth-infoDepth;
                if(proactive < min(max_proactive, remain_depth)) {
                    sort(reserved.rbegin(), reserved.rend());
                    int sz = reserved.size();
                    while(sum_rank_vec.size()<reserved.size()) {
                        double before = sum_rank_vec.size()==0 ? 0.0 : sum_rank_vec.back();
                        sum_rank_vec.emplace_back(before + pow(sum_rank_vec.size()+1, -zipf_rate));
                    }
                    double rnd = drand49() * sum_rank_vec[reserved.size()-1];
                    int idx = 0;
                    while(idx<reserved.size()-1 && sum_rank_vec[idx] < rnd) {
                        ++idx;
                    }
                    int rank=0;
                    for(ranking_item v : reserved) {
#if __local__
                        v.second->rank = rank;
#endif
                        if(idx==rank) {
                            next_node = v.second;
                        }
                        else {
                            v.second->release();
                        }
                        ++rank;
                    }
                    reserved.clear();
                    assert(next_node!=NULL);
                    input_mem = next_node->input;
                    assert(input_mem);
                    while(rankings.size()<3+proactive) {
                        rankings.emplace_back(new ty_ranking(), new xnodemem());
                    }
                    xmem::lock();
                    ++infoLoopCount;
                    return true;
                }
#if __local__
                sort(reserved.rbegin(), reserved.rend());
                int rank=0;
                for(ranking_item v : reserved) {
                    v.second->rank = rank;
                    v.second->release();
                    ++rank;
                }
#else
                for(ranking_item v : reserved) {
                    v.second->release();
                }
#endif
                reserved.clear();
            }
            xbeam_pa$node * node = current_node;
            assert(node);
            if(node->parent) {
                node->patch = myxmem.build_undo();
                node = node->parent;
                while(node->patch==NULL) {
                    xmem::unlock();
                    if(node->parent) {
                        node->patch = myxmem.build_undo();
                        node = node->parent;
                    }
                    else {
                        node->patch = myxmem.build();
                        break;
                    }
                }
            }
            else {
                node->patch = myxmem.build();
            }
            node->addRef();
            current_node->release();
            current_node = node;
        }
        double t = getTime();
        while(true) {
            if(t<nextLimit) {
                while(!rankings[0].first->empty()) {
                    //最初に、ビームの中から次の候補を取り出す。
                    next_node = rankings[0].first->large().second;
                    rankings[0].first->pop_large();
                    //以降のnextInt等は、候補ノードから情報を読み出す。
                    input_mem = next_node->input;
                    if(input_mem) {
                        ++infoLoopCount;
                        return true;
                    }
                    next_node->release();
                }
                next_node = NULL;
            }
            infoForNextDepth_(t);
            clear_ranking(rankings[0].first);
            rankings[0].second->free();
            if(0<remain_depth) {
                nextLimit = (wholeLimit - t) / remain_depth + t;
                --remain_depth;
                rankings.push_back(rankings.front());
                rankings.pop_front();
            }
            if(rankings[0].first->empty()) {
                dest_(t);
                return false;
            }
#if __local__
            if(last_top_node!=NULL) {
                last_top_node->release();
            }
            last_top_node = rankings[0].first->large().second;
            last_top_node->addRef();
#endif
            firstSc = rankings[0].first->large().first;
        }
    }
    inline bool reserve(double score=0) {
        //予約ノードを作って、ビームに突っ込む
        //以降のnextInt等は、予約ノードに書き込む。
        int ranking_idx = 1 + current_node->depth - infoDepth;
        if(max_beam_size<=rankings[ranking_idx].first->size()) {
            if(score<=rankings[ranking_idx].first->small().first) {
                return false;
            }
            rankings[ranking_idx].first->small().second->release();
            rankings[ranking_idx].first->pop_small();
        }
        nmem = rankings[ranking_idx].second;
        xbeam_pa$node * node = xbeam_pa$node::create(current_node, nmem->alloc());
        rankings[ranking_idx].first->emplace(score, node);
        node->addRef();
        reserved.emplace_back(score, node);
        return true;
    }
    inline bool reschedule(double score) {
        int ranking_idx = next_node->depth - infoDepth;
        if(ranking_idx) {
            if(rankings[ranking_idx].first->size() < max_beam_size) {
                return false;
            }
            return score < rankings[ranking_idx].first->small().first;
        }
        if(rankings[0].first->empty()) {
            return false;
        }
        if(rankings[0].first->large().first<=score) {
            return false;
        }
        assert(next_node);
        next_node->addRef();
        rankings[0].first->emplace(score, next_node);
        return true;
    }
    //以下、シリアライズ系
    inline xnodemem * getNodeMem() {
        return nmem;
    }
    inline void nextDouble(double value) {
        nmem->next(value);
    }
    inline void nextFloat(float value) {
        nmem->next(value);
    }
    inline void nextInt64(long long value) {
        nmem->next(value);
    }
    inline void nextInt(int value) {
        nmem->next(value);
    }
    inline void nextShort(short value) {
        nmem->next(value);
    }
    inline void nextChar(char value) {
        nmem->next(value);
    }
    inline char * getInputMem() {
        return input_mem;
    }
    inline double nextDouble() {
        double ret = *(double*)input_mem;
        input_mem += sizeof(ret);
        return ret;
    }
    inline float nextFloat() {
        float ret = *(float*)input_mem;
        input_mem += sizeof(ret);
        return ret;
    }
    inline long long nextInt64() {
        long long ret = *(long long*)input_mem;
        input_mem += sizeof(ret);
        return ret;
    }
    inline int nextInt() {
        int ret = *(int*)input_mem;
        input_mem += sizeof(ret);
        return ret;
    }
    inline short nextShort() {
        short ret = *(short*)input_mem;
        input_mem += sizeof(ret);
        return ret;
    }
    inline char nextChar() {
        char ret = *(char*)input_mem;
        input_mem += sizeof(ret);
        return ret;
    }
};
