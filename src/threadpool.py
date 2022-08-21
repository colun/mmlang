import sys
import time
import threading
import psutil
import heapq

class ThreadPool:
    def entry_point(self):
        self.local.rank = 0
        self.local.count = 0
        while self.enable:
            run_func = None
            with self.task_lock:
                if self.tasks:
                    self.local.rank, run_id, run_func, run_args = heapq.heappop(self.tasks)
            if run_func is not None:
                try:
                    run_ret = False, run_func(*run_args)
                except:
                    run_ret = True, sys.exc_info()[1]
                with self.future_lock:
                    self.futures[run_id] = run_ret
            else:
                time.sleep(0.01)

    def __init__(self, threads=0):
        if threads<1:
            threads = max(1, psutil.cpu_count(logical=False) - 1)
        assert 1<=threads
        self.threads = threads
        self.capacity = threads * 8
        self.enable = True
        self.tasks = []
        self.futures = {}
        self.count = 0
        self.local = threading.local()
        self.local.rank = 0
        self.task_lock = threading.Lock()
        self.future_lock = threading.Lock()
        self.pool = [threading.Thread(target=self.entry_point, daemon=True) for i in range(1, threads)]
        for th in self.pool:
            th.start()

    def close(self):
        self.enable = False
        for th in self.pool:
            th.join()

    def submit(self, submit_func, *submit_args):
        assert self.enable
        assert submit_func is not None
        #run_func = None
        now_rank = self.local.rank
        with self.task_lock:
            self.count += 1
            submit_id = self.count
            heapq.heappush(self.tasks, (now_rank-1, submit_id, submit_func, submit_args))
        #    if self.capacity <= len(self.tasks):
        #        run_rank, run_id, run_func, run_args = heapq.heappop(self.tasks)
        #if run_func is not None:
        #    self.local.rank = run_rank
        #    try:
        #        run_ret = False, run_func(*run_args)
        #    except:
        #        run_ret = True, sys.exc_info()[1]
        #    self.local.rank = now_rank
        #    with self.future_lock:
        #        self.futures[run_id] = run_ret
        return submit_id

    def result(self, future_id):
        assert self.enable
        now_rank = self.local.rank
        ret = None
        while True:
            with self.future_lock:
                if future_id in self.futures:
                    ret = self.futures.pop(future_id)
            if ret is not None:
                if ret[0]:
                    raise ret[1]
                else:
                    return ret[1]
            run_func = None
            with self.task_lock:
                if self.tasks:
                    run_rank, run_id, run_func, run_args = heapq.heappop(self.tasks)
            if run_func is not None:
                self.local.rank = run_rank
                try:
                    run_ret = False, run_func(*run_args)
                except:
                    run_ret = True, sys.exc_info()[1]
                self.local.rank = now_rank
                with self.future_lock:
                    self.futures[run_id] = run_ret
            else:
                time.sleep(0.01)
