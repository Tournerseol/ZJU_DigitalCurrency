#include <iostream>
#include "raft_consensus.h"
#include "election.h"
#include "wallet.h"
#include "main.cpp"
using namespace std;

bool VoteAssistant::AreAllVotesIn()
{
    if (this->CountYesVotes() + this->CountNoVotes() == num_voters) {
        return true;
    } else {
        return false;
    }
}

bool VoteAssistant::IsDecided()
{
    // 当所有投出的票数等于应投票数时，表明选举结果已经决定
    if (GetTotalVotesCounted() == GetTotalExpectedVotes()) {
        return true;
    } else {
        return false;
    }
}

int VoteAssistant::CountYesVotes()
{
    int n = GetNumberOfNode();
    int candidate = this->current_candidate;
    int count = 0;

    for (int i = 0; i < n; i++) {
        if (s[i].ID == ServerNode::FOLLOWER) {
            if (s[i].RespondRequest(s[i], s[candidate]) == 1)
                count++;
        }
    }

    return count;
}

int VoteAssistant::CountNoVotes()
{
    int n = GetNumberOfNode();
    int candidate = this->current_candidate;
    int count = 0;

    for (int i = 0; i < n; i++) {
        if (s[i].ID == ServerNode::FOLLOWER) {
            if (s[i].RespondRequest(s[i], s[candidate]) == 0)
                count++;
        }
    }

    return count; 
}

int VoteAssistant::GetTotalVotesCounted()
{
    return this->CountYesVotes() + this->CountNoVotes();
}

int VoteAssistant::GetTotalExpectedVotes()
{
    return GetNumberOfNode() - 1;
}

int VoteAssistant::ElectionResult()
{
    // 返回值，记录新leader的下标
    int new_leader;
    if (this->CountYesVotes() >= majority_size) {
        new_leader = current_candidate;
    } else {
        // 表示竞选失败
        new_leader = -1;
    }
    
    return new_leader;
}
