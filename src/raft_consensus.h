#include <iostream>
#include <fstream>
#include <ctime>
#include <dirent.h>
#include "election.h"
using namespace std;

// 一些声明：
// (1) 因为函数的具体实现还没有确定，所以传参和返回类型我都没有规
// 定好，需要自己再加工
// (2) 有些变量后面加了下划线的原因是，初始化节点的时候结构函数传
// 参可能会用到一模一样的名字，加下划线是一个通用的解决方法
#ifndef RAFT_CONSENSUS_H_
#define RAFT_CONSENSUS_H_

class ServerNode
{
public:
    // 枚举身份类型，FOLLOWER=0, CANDIDATE=1, LEADER=2
    // 初始时刻所有节点默认为FOLLOWER身份
    enum identity_
    {
        FOLLOWER,
        CANDIDATE,
        LEADER
    };
    identity_ ID;
    // ---------------共有功能---------------

    // 返回节点当前的身份，0代表Follower这样以此类推
    int ReturnIdentity();
    
    //返回当前任期
    int ReturnTerm(); 

    // ---------------follower功能---------------
    //（在所有特有功能执行前，可以先验证一下身份判断是否有操作
    // 权限）

    // 当节点超时，或没有收到心跳包时，转变身份为candidate
    int TransToCandidate();

    // 接收candidate发来的投票请求并决定赞成还是反对，若赞成
    // 则要更新election timeout
    // 投票的结果由投票助手记录
    int RespondRequest(int identify,int candidate);

    // 接收leader发来的心跳包，接收到后更新election timeout
    int ReceiveAppendEntries(ServerNode &L);

    // 当收到leader发来的带有交易变动信息的心跳包后，更新日志
    void ReplicateLog(ServerNode &L);

    // ---------------candidate功能---------------

    // 向其他节点发送投票请求（记得在最后更新timeout！）
    void SendVoteRequest();

    // 收到大多数投票时（是否竞选成功由投票助手告知），转变身
    // 份为leader
    void TransToLeader();

    // ---------------leader功能---------------

    // 接收客户端发来的交易信息变动，并写入leader的日志
    void ReceiveClientChange(string send, string receive, double amount);

    // 向follower发送心跳包
    void SendAppendEntries();

    // 当大多数节点收到变动信息后，leader反馈给客户端
    bool CommitEntry(int entry);

private:
    // 记录当前leader的任期号
    int term_;

    // 定义选举超时时间，初始化时生成一个100-300ms随机时间
    int election_timeout_;

    // 定义心跳超时时间，仅在Leader身份时使用
    int heartbeat_timeout_;

    // 定义是否收到心跳包的标志
    int heartbeat_msg;
    
    //定义是否在当前任期进行投票(仅follower使用) 
	int isElected;

    // 记录自己的数组下标
    int self_number;

    // 记录当前Leader的编号
    int current_leader;

    // 判断节点是否超时
    bool IsTimeOut();
};

// 这个类是用来作为日志的，目前有一个写日志的函数，参考的是
// https://www.cnblogs.com/zhhh/p/9470255.html
// 可能后续还需要添加一些别的功能。初步的设想是在创建一个服务器
// 节点的同时创建一个Log类的实例对象
class Log
{
public:
    static void Write(string log)
    {
        try
        {
            ofstream ofs;
            time_t t = time(0);
            char tmp[64];
            strftime(tmp, sizeof(tmp), "[%Y-%m-%d %X]", localtime(&t));
            // 具体打开什么文件之后还得再改
            ofs.open("D:\\PipeLog.log", ofstream::app);

            ofs << tmp << " - ";
            ofs.write(log.c_str(), log.size());
            ofs << endl;
            ofs.close();
        }
        catch(...)
        {

        }
    }
};

#endif
