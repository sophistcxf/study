/*!
 * 测试消息队列
 */

#include <iostream>
using namespace std;

#include <unistd.h>
#include <sys/msg.h>

#include "util.hpp"

int get_msg(const char* path, key_t& key)
{
    key = ftok(path, 0);
    if (key < 0) {
        std::cerr << "ftok failed, path: " << path << std::endl;
        return -1;
    }
    int id = msgget(key, IPC_CREAT);
    if (id < 0) {
        std::cerr << "msgget failed, key: " << key << std::endl;
        return -1;
    }
    return id;
}

void recs_msg(int msgid)
{
    while (true) {
        mymesg mesg;
        memset(&mesg, 0, sizeof(mesg));
        msgrcv(msgid, &mesg, sizeof(mesg.mtext), 0, 0);
        print_mesg(&mesg);
    }
}

int main(int argc, char* argv[])
{
    char* path = getcwd(NULL, 0);
    std::cout << "path: " << path << std::endl;
    key_t key;
    int id = get_msg(path, key);
    if (id < 0) {
        std::cerr << "get msg queue failed" << std::endl;
        return -1;
    }
    std::cout << "get msg queue successfully, id: " << id << " key: " << key <<
    " path: " << path << std::endl;
    recs_msg(id);
    return 0;
}
