class MM$ThreadPool {
    std::vector<std::thread> pool;
    std::deque<std::function<void(int)> > tasks;
    std::mutex mtx;
    bool enable;
    int capacity;
public:
    MM$ThreadPool(int threads=0) {
        if(threads==0) {
            threads = std::max((int)1, (int)(std::thread::hardware_concurrency() >> 1));
        }
        assert(1<=threads);
        capacity = threads * 8;
        enable = true;
        for(int i=1; i<threads; ++i) {
            pool.push_back(std::thread([&, i] {
                while(true) {
                    std::function<void(int)> func;
                    bool enable_now = enable;
                    {
                        std::lock_guard<std::mutex> lock(mtx);
                        if(!tasks.empty()) {
                            func = tasks.front();
                            tasks.pop_front();
                        }
                    }
                    if(func) {
                        func(i);
                    }
                    else if(!enable_now) {
                        break;
                    }
                }
            }));
        }
    }
    ~MM$ThreadPool() {
        enable = false;
        while(true) {
            std::function<void(int)> func;
            {
                std::lock_guard<std::mutex> lock(mtx);
                if(!tasks.empty()) {
                    func = tasks.front();
                    tasks.pop_front();
                }
            }
            if(func) {
                func(0);
            }
            else {
                break;
            }
        }
        for(std::thread & th : pool) {
            th.join();
        }
    }
    void submit(const std::function<void(int)> & func) {
        std::function<void(int)> func2;
        {
            std::lock_guard<std::mutex> lock(mtx);
            tasks.push_back(func);
            if(capacity <= tasks.size()) {
                func2 = tasks.front();
                tasks.pop_front();
            }
        }
        if(func2) {
            func2(0);
        }
    }
};
