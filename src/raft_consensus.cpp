#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <windows.h>
#include "election.h"
#include "raft_consensus.h"
#include "wallet.h"
#include "main.cpp"
using namespace std;

VoteAssistant vote_assistant;

void Checklog();//比较日志消息新旧 

// 返回节点的当前身份
int ServerNode::ReturnIdentity()
{
    return this->ID;
}

//返回节点所处的任期
int ServerNode::ReturnTerm()
{
    return this->term_;
}

// ---------------FOLLOWER---------------

int ServerNode::TransToCandidate()
{
    if (heartbeat_msg){//时限内未收到heartbeat转变为candidate
        this->ID=CANDIDATE;
        heartbeat_msg=0;//刷新
        //term_++
        
        return 1;
    }
    
    else{
    	return 0;
	}
}

int ServerNode::RespondRequest(ServerNode &L, ServerNode &C)
{
	if(L.ReturnTerm()<C.ReturnTerm()){
		return 1;
		
	}	
	
	else if(L.ReturnTerm() == C.ReturnTerm()){
		if(isElected == 0){
			election_timeout_=rand()%200+100;
			isElected=1;
			return 1;
		}
		else
			return 0; 
	}
	else{
		return 0;
	}
} 

int ServerNode::ReceiveAppendEntries(ServerNode &L)
{
    if(L.ReturnIdentity()==LEADER){
    	//确认leader已经发送心跳包 
    	
        election_timeout_=rand()%200+100;//更新election_timeout_
        heartbeat_msg=1;//表示收到
        
        return 1;       
    }
    else{
        return 0;        
    }
    
}

void ServerNode::ReplicateLog(ServerNode &L)
{
    if(heartbeat_msg && L.ReturnIdentity()==LEADER){
        //写入日志
        //Log.data = L.Log.data
    }
}

void ServerNode::ResetMsg() {
	//身份为follower且heartbeat_msg=1的节点需调用该函数刷新 
    while(this->ID==FOLLOWER)
    {
        Sleep(heartbeat_timeout_+50);
        heartbeat_msg=0;
    }
}


// ---------------CANDIDATE---------------
void ServerNode::SendVoteRequest()
{
    int n; // n为目前节点个数
	for (int i = 0; i < n; i++) {
		if (i != this->self_number) {
			s[i].vote_request_msg = 1;
		}
	}
	// 向投票助手表明目前的candidate是自己
	vote_assistant.current_candidate = this->self_number;
}

void ServerNode::TransToLeader()
{
	int n = GetNumberOfNode();
    // 如果选举结果所返回的新leader的下标等于自身的下标，则变为leader
	if (vote_assistant.ElectionResult() == this->self_number) {
        this->ID = LEADER;
        // 更新自己的心跳超时时间
	    this->heartbeat_timeout_ = 1000;

        // 通知所有节点新的leader是自己
		for (int i = 0; i < n; i++) {
			s[i].current_leader = this->self_number;
			(s[i].term_)++;
		}
	}
}

// ---------------leader功能---------------

// 接收客户端发来的交易信息变动，并写入leader的日志
void ServerNode::ReceiveClientChange(string send, string receive, double amount)
{
	Log::Write(send + " " + receive + " " + to_string(amount));
}

// 向follower发送心跳包
void ServerNode::SendAppendEntries()
{
	while (1){
		Sleep(heartbeat_timeout_);
		for (int i = 0; i < sizeof(s) / sizeof(s[0]); i++) {
			if (s[i].ID == FOLLOWER) {
				s[i].heartbeat_msg = 1;
			}
		}
	}
}

// 当大多数节点收到变动信息后，leader反馈给客户端
bool ServerNode::CommitEntry(int entry)
{
	int n = GetNumberOfNode();
	int majority_size = n % 2 ? n / 2 + 1: n / 2;
	if (entry >= majority_size)	return true;
	else  return false;
}
