#include <iostream>
#include <sys/msg.h>    /* msqid_ds */
#include <sys/types.h>
#include <sys/ipc.h>

static void print_msgid_ds(const msqid_ds* ds)
{
    char sep = '\t';
    std::cout << "msgqnum_t " << ds->msg_qnum << sep
              << "msg_msglen_t " << ds->msg_qbytes << sep
              << "msg_lspid " << ds->msg_lspid << sep
              << "msg_lrpid " << ds->msg_lrpid << sep
              << "msg_stime " << ds->msg_stime << sep
              << "msg_rtime " << ds->msg_rtime << sep
              << "msg_ctime " << ds->msg_ctime << sep
              << std::endl;
}

struct mymesg
{
    long mtype;         /* 消息类型 */
    char mtext[512];    /* message data */
};

static void print_mesg(const mymesg* mesg)
{
    std::cout << "mtype: " << mesg->mtype << " msg: " << mesg->mtext << std::endl;
}
