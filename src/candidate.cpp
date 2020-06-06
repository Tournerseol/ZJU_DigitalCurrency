#include<iostream>
#include<string>
#include "raft_.h"

#define RAFT_STATE_LEADER 1
#define RAFT_STATE_CANDIDATE 2
#define RAFT_STATE_FOLLOWER 3
#define RAFT_NODE_VOTED_FOR_ME 1
#define RAFT_NODE_VOTING 1 << 1 //节点有选举权限
#define RAFT_NODE_HAS_SUFFICIENT_LOG 1 << 2  //节点有足够的日志

void raft_set_current_term(raft_server_private_t* me, const int term)
{
	if (me->current_term < term)
	{
		me->current_term = term;
		me->voted_for = -1;
	}
}
int raft_get_current_term(raft_server_private_t* me)
{
	return (me)->current_term;
}

void raft_node_vote_for_me(raft_node_private_t* me, const int vote)
{

	if (vote)
		me->flags |= RAFT_NODE_VOTED_FOR_ME;
	else
		me->flags &= ~RAFT_NODE_VOTED_FOR_ME;
}

void raft_vote(raft_server_private_t* me, raft_node_private_t* node)
{
	raft_vote_for_nodeid(me, node ? raft_node_get_id(node) : -1);
}

int raft_node_get_id(raft_node_private_t* me)
{
	return me->id;
}

void raft_vote_for_nodeid(raft_server_private_t* me, const int nodeid)
{
	me->voted_for = nodeid;
}

int raft_node_is_voting(raft_node_private_t* me)
{
	return (me->flags & RAFT_NODE_VOTING) != 0;
}

int raft_send_requestvote(raft_server_private_t* me, raft_node_private_t* node)
{
	msg_requestvote_t rv;//请求投票消息
	return 0;
}

int raft_get_current_idx(raft_server_private_t* me)
{
	return me->last_applied_idx;
}

int raft_votes_is_majority(const int num_nodes, const int nvotes)
{
	if (num_nodes < nvotes)
		return 0;
	int half = num_nodes / 2;
	return half + 1 <= nvotes;
}

int raft_get_nvotes_for_me(raft_server_private_t* me)
{
	int i, votes;

	for (i = 0, votes = 0; i < me->num_nodes; i++)
	{ 
		if (me->node != me->nodes[i] && raft_node_is_voting(me->nodes[i]))
		{
			if (raft_node_has_vote_for_me(me->nodes[i]))
				votes += 1;
		}
	}
	if (me->voted_for == raft_get_nodeid(me))
		votes += 1;

	return votes;
}

int raft_node_has_vote_for_me(raft_node_private_t* me)
{
	return (me->flags & RAFT_NODE_VOTED_FOR_ME) != 0;
}
int raft_get_nodeid(raft_server_private_t* me)
{
	if (!me->node)
		return -1;
	return raft_node_get_id(me->node);
}

/*Follower成为Candidate执行的函数*/
void raft_become_candidate(raft_server_private_t* me)
{
	int i;

	/*自增当前的任期号；给自己投票，设置自己的状态为candidate*/
	raft_set_current_term(me, raft_get_current_term(me) + 1);
	for (i = 0; i < me->num_nodes; i++)
		raft_node_vote_for_me(me->nodes[i], 0);
	raft_vote(me, me->node);
	me->current_leader = NULL;
	me->state = RAFT_STATE_CANDIDATE;

	/* 重置选举超时计时器。为了防止多个Candidate竞争，将下一次发起投票的时间间隔设置成随机值*/
	me->timeout_elapsed = rand() % me->election_timeout;

	/*发送请求投票的 RPC 给其他所有服务器*/
	for (i = 0; i < me->num_nodes; i++)
		if (me->node != me->nodes[i] && raft_node_is_voting(me->nodes[i]))
			raft_send_requestvote(me, me->nodes[i]);
}

/*处理投票请求*/
int raft_recv_requestvote(raft_server_private_t* me, raft_node_private_t* node, msg_requestvote_t* vr, msg_requestvote_response_t *r)
{
	/*如果请求中term > 本地currentTerm, 则转为Follower模式*/
	if (raft_get_current_term(me) < vr->term)
	{
		raft_set_current_term(me, vr->term);
		me->state = RAFT_STATE_FOLLOWER;
	}

	/*如果需要投票，则回复true，即将r->vote_granted = 1;*/
	if (__should_grant_vote(me, vr))
	{
		/*同意投票--本地记录给哪个服务器投了票，并设置response中的vote_granted为1*/
		raft_vote_for_nodeid(me, vr->candidate_id);
		r->vote_granted = 1;

		/* 肯定有一个选举 */
		me->current_leader = NULL;
		me->timeout_elapsed = 0;
	}
	else
	{
		r->vote_granted = 0;
	}

	/*更新本地保存的任期号，与请求中的保持一致*/
	r->term = raft_get_current_term(me);
	return 0;
}

int raft_already_voted(raft_server_private_t* me)
{
	return (me)->voted_for != -1;
}

/*检查是否满足投票的条件*/
static int __should_grant_vote(raft_server_private_t* me, msg_requestvote_t* vr)
{
	/**请求中的任期号term比本地term要小，不给投票*/
	if (vr->term < raft_get_current_term(me))
		return 0;

	/*如果已经投过票了，返回false*/
	if (raft_already_voted(me))
		return 0;

	/* 下面代码检查请求中日志信息是否比本地日志新*/

	/* 获取本地最新的日志索引 */
	int current_idx = raft_get_current_idx(me);

	/* 本地日志为空，请求中的日志信息绝对比本地要新，返回true */
	if (0 == current_idx)
		return 1;

	/* 如果本地最新日志中的任期号比请求中的last_log_term要小，则返回true*/

	raft_entry_t* e = raft_get_entry_from_idx(me, current_idx);//这个函数不知道怎么写。。。
	if (e->term < vr->last_log_term)
		return 1;

	/* 本地最新日志中的任期号与请求中的last_log_term相等，则比较日志索引，索引比较大的说明日志比较新*/
	if (vr->last_log_term == e->term && current_idx <= vr->last_log_idx)
		return 1;

	/*果本地最新日志中的任期号比请求中的last_log_term要大，则返回false */
	return 0;
}

/*处理投票恢复*/
int raft_recv_requestvote_response(raft_server_private_t* me, raft_node_private_t* node, msg_requestvote_response_t* r)
{
	/* 不是Candidate，直接返回 */
	if (me->state!=RAFT_STATE_CANDIDATE)
	{
		return 0;
	}
	/* response中的任期号比自己的大，说明自己的term已经过时，无条件转为Follower */
	else if (raft_get_current_term(me) < r->term)
	{
		raft_set_current_term(me, r->term);
		me->state = RAFT_STATE_FOLLOWER;
		return 0;
	}
	/* response中的任期号比自己小，说明收到了一个过时的response，忽略即可。*/
	else if (raft_get_current_term(me) != r->term)
	{
		return 0;
	}


	/* 给我投票了 */
	if (1 == r->vote_granted)
	{
		/* 记录给自己投票的服务器信息 */
		if (node)
			raft_node_vote_for_me(node, 1);
		int votes = raft_get_nvotes_for_me(me);
		/* 如果给自己投票的服务器超过了总数的一般，则成为Leader */
		if (raft_votes_is_majority(me->num_nodes, votes))
			me->state = RAFT_STATE_LEADER;
	}
	return 0;
}
