import os
from multiprocessing import Manager
from multiprocessing import Process, Lock
import time

g_value = 0

class Task():
    def __init__(self, num):
        self.task_num = num

def func(task_list, lock, push_finished):
    print os.getpid()
    while not push_finished.value:
        lock.acquire() 
        if len(task_list) == 0:
            lock.release()
            time.sleep(1)
            continue
        task = task_list.pop()
        lock.release()
        print "task num %d, g_value: %d" % (task.task_num, g_value)

if __name__ == "__main__":
    task_manager = Manager()
    task_list = task_manager.list()
    lock = task_manager.Lock()
    process_list = list()
    push_finished = task_manager.Value("push_finished", False)
    g_value = 10 # g_value will be inherited by subprocess
    for i in range(1, 4):
        process_list.append(Process(target=func, args=(task_list, lock, push_finished))) 
    for process in process_list:
        process.start()

    for i in range(1,100):
        lock.acquire()
        while len(task_list) > 10:
            lock.release()
            time.sleep(1) 
            lock.acquire()
        task_list += [ Task(i) ] 
        lock.release()

    push_finished.value = True 
    
    for process in process_list:
        process.join()
