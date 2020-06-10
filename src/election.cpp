#include <iostream>
#include "raft_consensus.h"
#include "election.h"
using namespace std;

bool VoteAssistant::RegisterVote()
{

}

bool VoteAssistant::AreAllVotesIn()
{
    if (yes_votes + no_votes == num_voters) {
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

int VoteAssistant::GetTotalVotesCounted()
{
    return yes_votes + no_votes;
}

int VoteAssistant::GetTotalExpectedVotes()
{
    // **这个功能目前无法具体实现，需要一个统计有多少节点（账户）
    // 的函数，现假设这个函数的名字叫做GetNumberOfNode

    // return GetNumberOfNode() - 1;
}

int VoteAssistant::ElectionResult()
{
    // 返回值，记录新leader的下标
    int new_leader;
    if (yes_votes >= majority_size) {
        new_leader = current_candidate;
    } else {
        // 表示竞选失败
        new_leader = -1;
    }
    
    return new_leader;
}
