#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <dirent.h>
#include "election.h"
#include "raft_consensus.h"
using namespace std;

// ------------leader功能------------

// 接收客户端发来的交易信息变动，并写入leader的日志
void ReceiveClientChange(string send, string receive, double amount)
{
	Log.Write(send + " " + receive + " " + to_string(amount));
}

// 向follower发送心跳包
void SendAppendEntries()
{
	for (i = 0; i < sizeof(nodes) / sizeof(nodes[0]); i++) {
		if (nodes[i].identity == FOLLOWER) {
			//向nodes[i]发送心跳包
		}
	}
}

// 当大多数节点收到变动信息后，leader反馈给客户端
bool CommitEntry(int entry)
{
	int n = sizeof(nodes) / sizeof(nodes[0]);
	int majority_size = n % 2 ? n / 2 + 1: n / 2;
	if (entry >= majority_size)	return true;
	else  return false;
}
