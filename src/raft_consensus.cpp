#include <iostream>
#include <fstream>
#include <ctime>
#include <dirent.h>
#include "election.h"
#include "raft_consensus.h"
using namespace std;

static int IsElected[identify,term_];//记录每个成员在当前领导人任期号下是否投过票 
void Checklog();//比较日志消息新旧 

// ---------------FOLLOWER---------------

//在class ServerNode添加变量 
//public:
//    int ReturnNumber();
//    ServerNode();//默认构造函数
//    ServerNode(const ServerNode&);//默认拷贝构造函数 
//private:
//    int status;
//    int number;
 //   int HeartBeatMsg; //为1代表收到leader信号
//	  raft_entry Log;   //raft_entry类 存储日志信息 

int ServerNode::TransToCandidate()
{
    if (HeartBeatMsg){//时限内未收到heartbeat转变为candidate
        status=CANDIDATE;
        HeartBeatMsg=0;//刷新
        //term_++
        
        return 1;
    }
    
    else{
    	return 0;
	}
}

int ServerNode::RespondRequest(int identify,int candidate)
{   
    // 供VoteAssistant调用 
	if(identify->term_<candidate->term_){
		
		return 1;          //如果当前领导人任期号小于发送邀请的候选人相应的任期号 
		
	}                      //说明该成员已经过时，转变为该候选人的follower 
	else if(identify->term_==candidate->term_){
		
		if(IsElected[identify,identify->term_]==0&&Checklog()==1){
			//update election_timeout_；更新election timeout 
			
			IsElected[identify,identify->term_]==1;
			
			return 1;//如果当前领导人任期号等于发送邀请的候选人相应的任期号,并且对方日志消息不比本地日志旧 
		}            // 如果该成员在该任期号下没有投过票就投给该候选人 
		else
			return 0;//已经投过票就不再投		
	}
	else{
		
		return 0;//如果候选人任期号更小，则该候选人已经过时，不给他投票 
	}
}

int ServerNode::ReceiveAppenEntries(ServerNode &L)
{
    if(L.ReturnIdentity()==LEADER && L.SendAppendEntries()==1){
    	//确认leader已经发送心跳包 
    	
        election_timeout_=rand()%200+100;//更新election_timeout_
        HeartBeatMsg=1;//表示收到
        
        return 1;
        
    }
    else{
    	
        return 0;
        
    }
    
}

void ServerNode::ReplicateLog(ServerNode &L)
{
    if(HeartBeatMsg && L.ReturnIdentity()==LEADER){
        //写入日志
        //Log.data = L.Log.data
    }
}


// ---------------CANDIDATE---------------
void ServerNode::SendVoteRequest()
{

}

void ServerNode::TransToLeader()
{
	// 需要先生成一个投票助手实例，假设这个实例叫vote_assistant
	// if (vote_assistant.ElectionResult() == this->self_number) {
    //     this->identity_ = LEADER;
	//     this->heartbeat_timeout_ = rand%200();
	// }
}

// ---------------LEADER---------------
void ServerNode::ReceiveClientChange(string send, string receive, double amount)
{
    Log.Write(send + " " + receive + " " + to_string(amount));
}

void ServerNode::SendAppendEntries()
{
    for (int i = 0; i < sizeof(nodes) / sizeof(nodes[0]); i++) {
        if (nodes[i].identity == FOLLOWER) {
            //向nodes[i]发送心跳包
        }
    }
}

bool ServerNode::CommitEntry(int entry)
{
    int n = sizeof(nodes) / sizeof(nodes[0]);
    int majority_size = n % 2 ? n / 2 + 1: n / 2;
    if (entry >= majority_size)	return true;
    else  return false;
}
