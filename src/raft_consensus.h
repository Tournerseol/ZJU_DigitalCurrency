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
    // ------------共有功能------------

    // ------------follower功能------------
    //（在所有特有功能执行前，可以先验证一下身份判断是否有操作
    // 权限）

    // 当节点超时，或没有收到心跳包时，转变身份为candidate
    void TransToCandidate();

    // 接收candidate发来的投票请求并决定赞成还是反对
    // 投票的结果由投票助手记录
    void RespondRequest();

    // ------------candidate功能------------

    // 向其他节点发送投票请求
    void SendVoteRequest();

    // 收到大多数投票时（是否竞选成功由投票助手告知），转变身
    // 份为leader
    void TransToLeader();

    // ------------leader功能------------

private:
    // 记录任期号
    int term_;
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
    static void Write(std::string log)
    {
        try
        {
            std::ofstream ofs;
            time_t t = time(0);
            char tmp[64];
            strftime(tmp, sizeof(tmp), "[%Y-%m-%d %X]", localtime(&t));
            // 具体打开什么文件之后还得再改
            ofs.open("D:\\PipeLog.log", std::ofstream::app);

            ofs << tmp << " - ";
            ofs.write(log.c_str(), log.size());
            ofs << std::endl;
            ofs.close();
        }
        catch(...)
        {

        }
    }
};

#endif
