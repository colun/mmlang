struct EulerTourTree$UVHash {
    size_t operator()(const std::pair<int, int> & o) const {
        return o.first * 6364136223846793005ULL +  o.second;
    }
};

struct EulerTourTree {
private:
    struct node {
        static fast_pool<node> pool;
        node * children[2];
        node * parent;
        int l, r;
        int size;
        void update() {
            size = (l==r ? 1 : 0);
            if(children[0]) {
                size += children[0]->size;
            }
            if(children[1]) {
                size += children[1]->size;
            }
        }
        void init(int l, int r) {
            children[0] = NULL;
            children[1] = NULL;
            parent = NULL;
            this->l = l;
            this->r = r;
            update();
        }
        node * root() {
            node * target = this;
            while(node * next = target->parent) {
                target = next;
            }
            return target;
        }
        void rotate(bool parent_hand) {
            assert(parent);
            assert(parent->children[parent_hand]==this);
            node * old_parent = parent;
            parent = old_parent->parent;
            node * move_child = children[!parent_hand];
            if(old_parent->children[parent_hand] = move_child) {
                move_child->parent = old_parent;
            }
            children[!parent_hand] = old_parent;
            old_parent->parent = this;
            if(parent) {
                assert(parent->children[0]==old_parent || parent->children[1]==old_parent);
                parent->children[parent->children[1]==old_parent] = this;
            }
        }
        /*
        void debug_print2(int indent) {
            if(children[0]) {
                assert(children[0]->parent==this);
                children[0]->debug_print2(indent+1);
            }
            for(int i=0; i<indent; ++i) {
                fputs("  ", stderr);
            }
            fprintf(stderr, "%p(%d %d %p): %p, %p\n", this, l, r, parent, children[0], children[1]);
            if(children[1]) {
                assert(children[1]->parent==this);
                children[1]->debug_print2(indent+1);
            }
        }
        void debug_print1() {
            if(parent) {
                fprintf(stderr, "-- %p(%d %d %p): %p %p\n", this, l, r, parent, children[0], children[1]);
                if(!(parent->children[0]==this || parent->children[1]==this)) {
                    fprintf(stderr, "assert %p(%d %d %p): %p %p\n", parent, parent->l, parent->r, parent->parent, parent->children[0], parent->children[1]);
                }
                assert(parent->children[0]==this || parent->children[1]==this);
                parent->debug_print1();
            }
            else {
                debug_print2(0);
            }
        }
        void debug_print(const char * p) {
            fprintf(stderr, "[DEBUG] %s %p(%d %d %p): %p %p\n", p, this, l, r, parent, children[0], children[1]);
            debug_print1();
        }
        */
        void splay() {
            //TODO: rotateを展開して、もっと効率的にする
            while(node * now_parent = parent) {
                assert(now_parent->children[0]==this || now_parent->children[1]==this);
                assert(children[0]==NULL || children[0]->parent==this);
                assert(children[1]==NULL || children[1]->parent==this);
                if(node * grand_parent = now_parent->parent) {
                    bool grand_parent_hand = (grand_parent->children[1]==now_parent);
                    if(now_parent->children[grand_parent_hand]==this) {
                        // zig zig ステップ
                        rotate(grand_parent_hand);//自分・父→子・祖父→父のupdate
                        now_parent->update();
                        rotate(grand_parent_hand);//自分・父(←祖父)→子(・祖父(←曽祖父)→父)のupdate
                        grand_parent->update();
                        if(!parent) {
                            update();
                        }
                    }
                    else {
                        // zig zag ステップ
                        now_parent->rotate(grand_parent_hand);//父→祖父・祖父→父(・曽祖父)のupdate
                        rotate(grand_parent_hand);//自分・父(←祖父)→子(・祖父(←父)→父)のupdate
                        grand_parent->update();
                        assert(parent==now_parent);
                    }
                }
                else {
                    // zig ステップ
                    rotate(now_parent->children[1]==this);
                    now_parent->update();
                    update();
                }
            }
        }
        node * add_right(node * child) {
            assert(this);
            assert(child);
            node * to = this;
            while(node * next = to->parent) {
                to = next;
            }
            while(node * next = to->children[1]) {
                to = next;
            }
            assert(to->children[1]==NULL);
            to->splay();
            assert(to->children[1]==NULL);
            assert(child->parent==NULL);
            child->parent = to;
            assert(to->children[1]==NULL);
            to->children[1] = child;
            to->update();
            return to;
        }
        node * to_left() {
            splay();
            node * left = children[0];
            children[0] = NULL;
            if(left) {
                left->parent = NULL;
                return add_right(left);
            }
            return this;
        }
        std::pair<node *, node *> split() {
            splay();
            node * left = children[0];
            if(left) {
                children[0] = NULL;
                left->parent = NULL;
            }
            node * right = children[1];
            if(right) {
                children[1] = NULL;
                right->parent = NULL;
            }
            return std::pair<node *, node *>(left, right);
        }
    };
    bool is_connected(node * u, node * v) {
        assert(u);
        assert(v);
        v->splay();
        u->splay();
        assert(u->root()==u);
        return u==v->root();
    }
    std::unordered_map<std::pair<int, int>, node *, EulerTourTree$UVHash> nodes;
    node * get_node(int u, int v) {
        auto & ref = nodes[std::pair<int, int>(u, v)];
        if(ref==NULL) {
            ref = node::pool.pop();
            ref->init(u, v);
        }
        return ref;
    }
    node * get_node_with_assert(int u, int v) {
        assert(nodes.find(std::pair<int, int>(u, v))!=nodes.end());
        return nodes[std::pair<int, int>(u, v)];
    }
    void remove_node(int u, int v, node * node_uv) {
        nodes.erase(std::pair<int, int>(u, v));
        node::pool.push(node_uv);
    }
public:
    EulerTourTree() {
    }
    void clear() {
        for(const auto & n : nodes) {
            node::pool.push(n.second);
        }
        nodes.clear();
    }
    ~EulerTourTree() {
        clear();
    }
    bool is_connected(int u, int v) {
        return is_connected(get_node(u, u), get_node(v, v));
    }
    bool is_direct_connected(int u, int v) {
        return nodes.find(std::pair<int, int>(u, v))!=nodes.end();
    }
    void link(int u, int v) {
        assert(!is_connected(u, v));//いきなりとんでもないものを要求: 既に繋がっていたら強制終了
        get_node(u, u)->to_left()->add_right(get_node(u, v))->add_right(get_node(v, v)->to_left())->add_right(get_node(v, u));
    }
    void cut(int u, int v) {
        assert(is_direct_connected(u, v));//いきなりとんでもないものを要求: 直接繋がっていなかったら強制終了
        node * node_uv = get_node_with_assert(u, v);
        node * node_vu = get_node_with_assert(v, u);
        const std::pair<node *, node *> & ret1 = node_uv->split();
        bool first_inclusive = (ret1.first ? is_connected(node_vu, ret1.first) : false);
        const std::pair<node *, node *> & ret2 = node_vu->split();
        if(first_inclusive) {
            if(ret2.first && ret1.second) {
                ret2.first->add_right(ret1.second);
            }
        }
        else {
            if(ret1.first && ret2.second) {
                ret1.first->add_right(ret2.second);
            }
        }
        remove_node(u, v, node_uv);
        remove_node(v, u, node_vu);
    }
    int size(int u) {
        return get_node(u, u)->root()->size;
    }
};
fast_pool<EulerTourTree::node> EulerTourTree::node::pool;
