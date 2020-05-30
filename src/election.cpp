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

int VoteAssistant::GetTotalVotesCounted()
{
    return yes_votes + no_votes;
}

int VoteAssistant::GetTotalExpectedVotes()
{

}