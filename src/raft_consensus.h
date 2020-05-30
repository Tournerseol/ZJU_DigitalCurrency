#include <iostream>
#include <ctime>
#include "election.h"
using namespace std;

#ifndef RAFT_CONSENSUS_H_
#define RAFT_CONSENSUS_H_

class ServerNode
{
public:
    // 枚举身份类型，FOLLOWER=0, CANDIDATE=1, LEADER=2
    // 初始时刻所有节点默认为FOLLOWER身份
    enum identity
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

    // candidate特有功能

    // 
    void SendVoteRequest();

    // 收到大多数投票时
    void TransToLeader();

    // leader特有功能

private:
    // 记录任期号
    int term;
    // 判断节点是否超时
    bool IsTimeOut();
};


#endif