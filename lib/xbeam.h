struct xbeam$node {
    static fast_pool<xbeam$node> pool;
    xbeam$node * parent;
    char * input;
    void * patch;
    int depth;
    int ref;
    void addRef() {
        ++ref;
    }
    void release() {
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
    static xbeam$node * create() {
        xbeam$node * n = pool.pop();
        n->parent = 0;
        n->input = 0;
        n->patch = 0;
        n->depth = 0;
        n->ref = 1;
        return n;
    }
    static xbeam$node * create(xbeam$node * parent_, char * input_) {
        parent_->addRef();
        xbeam$node * n = pool.pop();
        n->parent = parent_;
        n->input = input_;
        n->patch = 0;
        n->depth = parent_->depth + 1;
        n->ref = 1;
        return n;
    }
};
fast_pool<xbeam$node> xbeam$node::pool;
struct xbeam {
    xmem myxmem;
    xbeam$node * current_node;
    xbeam$node * next_node = NULL;
    typedef std::pair<double, xbeam$node*> ranking_item;
    typedef dpque<ranking_item> ty_ranking;
    ty_ranking * current_ranking = NULL;
    ty_ranking * next_ranking = NULL;
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
    int infoDepth;
    double infoStartTime;
    double infoDepthStartTime;
    bool verboseFlag = true;
    xbeam() {
    }
    void setVerbose(bool flag) {
        verboseFlag = flag;
    }
    void clear_ranking(ty_ranking * rnk) {
        for(const ranking_item & v : *rnk) {
            v.second->release();
        }
        rnk->clear();
    }
    int getRemainDepth() const {
        return remain_depth;
    }
    void init(int max_depth_, double timeLimit=9.8, int max_beam_size_=2000) {
        infoAcceptCount = 0;
        infoDepthAcceptCount = 0;
        infoLoopCount = 0;
        infoDepthLoopCount = 0;
        infoDepth = 0;
        remain_depth = max_depth_;
        max_beam_size = max_beam_size_;
        wholeLimit = timeLimit;
        {
            double t = getTime();
            nextLimit = (wholeLimit - t) / (remain_depth + 1) + t;
            infoDepthStartTime = infoStartTime = t;
        }
        nmem.free2();
        nmem.free2();
        assert(next_node==NULL);
        current_node = xbeam$node::create();
        if(current_ranking) {
            clear_ranking(current_ranking);
        }
        else {
            current_ranking = new ty_ranking();
        }
        if(next_ranking) {
            clear_ranking(next_ranking);
        }
        else {
            next_ranking = new ty_ranking();
        }
        xmem::lock();
    }
    void dest_(double t) {
        xmem::unlock();
        if(verboseFlag) {
            fprintf(stderr, "xbeam(all): %.3f->%.3f(%.6fs) %d/%d ... %.1fa/s\n\n", infoStartTime, t, t-infoStartTime, infoAcceptCount, infoLoopCount, infoAcceptCount/(t-infoStartTime));
        }
        xbeam$node * cn = current_node;
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
            fprintf(stderr, "xbeam(%3d): %.3f->%.3f(%.6fs) %d/%d ... %.1fa/s\n", infoDepth, infoDepthStartTime, t, t-infoDepthStartTime, infoAcceptCount-infoDepthAcceptCount, infoLoopCount-infoDepthLoopCount, (infoAcceptCount-infoDepthAcceptCount)/(t-infoDepthStartTime));
        }
        infoDepthStartTime = t;
        infoDepthAcceptCount = infoAcceptCount;
        infoDepthLoopCount = infoLoopCount;
        ++infoDepth;
    }
    void transit_(xbeam$node * current_node, xbeam$node * next_node) {
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
    void accept() {
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
    long long get_current_node() {
        assert(current_node);
        current_node->addRef();
        return (long long)(void*)current_node;
    }
    void release_node(long long node) {
        ((xbeam$node *)(void *)node)->release();
    }
    long long parent_node(long long node) {
        return (long long)(void*)(((xbeam$node *)(void *)node)->parent);
    }
    void undo_node(long long node) {
        xmem::undo(((xbeam$node *)(void *)node)->patch);
    }
    void redo_node(void * node) {
        xmem::redo(((xbeam$node *)(void *)node)->patch);
    }
    bool onloop() {
        if(next_node) {
            next_node->release();
            next_node = NULL;
        }
        else {
            if(current_node->parent) {
                current_node->patch = myxmem.build_undo();
                xbeam$node * node = current_node->parent;
                node->addRef();
                current_node->release();
                current_node = node;
            }
            else {
                current_node->patch = myxmem.build();
            }
        }
        double t = getTime();
        if(nextLimit<=t || current_ranking->empty()) {
            infoForNextDepth_(t);
            if(remain_depth<=0) {
                dest_(t);
                return false;
            }
            nextLimit = (wholeLimit - t) / remain_depth + t;
            --remain_depth;
            clear_ranking(current_ranking);
            std::swap(current_ranking, next_ranking);
            nmem.free2();
            if(current_ranking->empty()) {
                dest_(t);
                return false;
            }
        }
        //最初に、ビームの中から次の候補を取り出す。
        next_node = current_ranking->large().second;
        current_ranking->pop_large();
        //以降のnextInt等は、候補ノードから情報を読み出す。
        input_mem = next_node->input;

        ++infoLoopCount;
        //trueを返す。
        return true;
    }
    bool reserve(double score=0) {
        //予約ノードを作って、ビームに突っ込む
        //以降のnextInt等は、予約ノードに書き込む。
        if(max_beam_size<=next_ranking->size()) {
            if(score<=next_ranking->small().first) {
                return false;
            }
            next_ranking->small().second->release();
            next_ranking->pop_small();
        }
        //TODO: ハッシュの扱いをどうする？→ハッシュはnextIntで突っ込んでおけば大丈夫。
        next_ranking->emplace(score, xbeam$node::create(current_node, nmem.alloc()));
        return true;
    }
    bool reschedule(double score) {
        if(current_ranking->empty()) {
            return false;
        }
        if(current_ranking->large().first<=score) {
            return false;
        }
        assert(next_node);
        next_node->addRef();
        current_ranking->emplace(score, next_node);
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
