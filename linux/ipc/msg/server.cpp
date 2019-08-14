/*!
 * 测试消息队列
 */

#include <iostream>
using namespace std;

#include <errno.h>
#include <unistd.h>
#include <sys/msg.h>

#include "util.hpp"

extern int errno;

int create_msg(const char* path, key_t& key)
{
    key = ftok(path, 0);
    if (key < 0) {
        std::cerr << "ftok failed, path: " << path << std::endl;
        return -1;
    }
    int id = msgget(key, IPC_CREAT|IPC_W|IPC_M|IPC_R);
    if (id < 0) {
        std::cerr << "msgget failed, key: " << key << " errno: " << errno << std::endl;
        return -1;
    }
    return id;
}

void send_msg(int msq_id)
{
    pid_t pid = getpid();
    msqid_ds ds;
    int max_mtype = 10;
    int msg_cnt = 5;
    while(true) {
        for (int mtype = 0; mtype < max_mtype; ++mtype) {
            for (int msg_idx = 0; msg_idx < msg_cnt; ++msg_idx) {
                int rlt = msgctl(msq_id, IPC_STAT, &ds);
                if (rlt < 0) {
                    std::cout << "msgctl failed, msq_id: " << msq_id << " errno: " << errno << std::endl;
                    break;
                }
                print_msgid_ds(&ds);
                mymesg mesg;
                memset(mesg.mtext, 0, sizeof(mesg.mtext));
                mesg.mtype = mtype;
                snprintf(mesg.mtext, sizeof(mesg.mtext), "This is %d, mtype %d, msg idx %d\n", pid, mtype, msg_idx);
                msgsnd(msq_id, &mesg, strlen(mesg.mtext), 0); 
                sleep(1);
            }
        }
    }
}

int main(int argc, char* argv[])
{
    char* path = getcwd(NULL, 0);
    std::cout << "path: " << path << std::endl;
    key_t key;
    int id = create_msg(path, key);
    if (id < 0) {
        std::cerr << "create msg queue failed" << std::endl;
        return -1;
    }
    std::cout << "create msg queue successfully, id: " << id << std::endl;

    send_msg(id);
    
    return 0;
}
