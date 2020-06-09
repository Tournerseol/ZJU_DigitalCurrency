#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <dirent.h>
#include <windows.h>
#include "election.h"
#include "raft_consensus.h"
using namespace std;

// ------------leader功能------------

// 接收客户端发来的交易信息变动，并写入leader的日志
void ServerNode::ReceiveClientChange(string send, string receive, double amount)
{
	Log::Write(send + " " + receive + " " + to_string(amount));
}

// 向follower发送心跳包
void ServerNode::SendAppendEntries()
{
	while (1){
		Sleep(HeartbeatTimeout);
		for (i = 0; i < sizeof(s) / sizeof(s[0]); i++) {
			if (s[i].identity == FOLLOWER) {
				s[i].heartbeat_msg = 1;
			}
		}
	}
}

// 当大多数节点收到变动信息后，leader反馈给客户端
bool ServerNode::CommitEntry(int entry)
{
	int n = sizeof(s) / sizeof(s[0]);
	int majority_size = n % 2 ? n / 2 + 1: n / 2;
	if (entry >= majority_size)	return true;
	else  return false;
}
