#include<iostream>
#include<string>
#include "raft_.h"

#define RAFT_STATE_LEADER 1
#define RAFT_STATE_CANDIDATE 2
#define RAFT_STATE_FOLLOWER 3
#define RAFT_NODE_VOTED_FOR_ME 1
#define RAFT_NODE_VOTING 1 << 1 //�ڵ���ѡ��Ȩ��
#define RAFT_NODE_HAS_SUFFICIENT_LOG 1 << 2  //�ڵ����㹻����־

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
	msg_requestvote_t rv;//����ͶƱ��Ϣ
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

/*Follower��ΪCandidateִ�еĺ���*/
void raft_become_candidate(raft_server_private_t* me)
{
	int i;

	/*������ǰ�����ںţ����Լ�ͶƱ�������Լ���״̬Ϊcandidate*/
	raft_set_current_term(me, raft_get_current_term(me) + 1);
	for (i = 0; i < me->num_nodes; i++)
		raft_node_vote_for_me(me->nodes[i], 0);
	raft_vote(me, me->node);
	me->current_leader = NULL;
	me->state = RAFT_STATE_CANDIDATE;

	/* ����ѡ�ٳ�ʱ��ʱ����Ϊ�˷�ֹ���Candidate����������һ�η���ͶƱ��ʱ�������ó����ֵ*/
	me->timeout_elapsed = rand() % me->election_timeout;

	/*��������ͶƱ�� RPC ���������з�����*/
	for (i = 0; i < me->num_nodes; i++)
		if (me->node != me->nodes[i] && raft_node_is_voting(me->nodes[i]))
			raft_send_requestvote(me, me->nodes[i]);
}

/*����ͶƱ����*/
int raft_recv_requestvote(raft_server_private_t* me, raft_node_private_t* node, msg_requestvote_t* vr, msg_requestvote_response_t *r)
{
	/*���������term > ����currentTerm, ��תΪFollowerģʽ*/
	if (raft_get_current_term(me) < vr->term)
	{
		raft_set_current_term(me, vr->term);
		me->state = RAFT_STATE_FOLLOWER;
	}

	/*�����ҪͶƱ����ظ�true������r->vote_granted = 1;*/
	if (__should_grant_vote(me, vr))
	{
		/*ͬ��ͶƱ--���ؼ�¼���ĸ�������Ͷ��Ʊ��������response�е�vote_grantedΪ1*/
		raft_vote_for_nodeid(me, vr->candidate_id);
		r->vote_granted = 1;

		/* �϶���һ��ѡ�� */
		me->current_leader = NULL;
		me->timeout_elapsed = 0;
	}
	else
	{
		r->vote_granted = 0;
	}

	/*���±��ر�������ںţ��������еı���һ��*/
	r->term = raft_get_current_term(me);
	return 0;
}

int raft_already_voted(raft_server_private_t* me)
{
	return (me)->voted_for != -1;
}

/*����Ƿ�����ͶƱ������*/
static int __should_grant_vote(raft_server_private_t* me, msg_requestvote_t* vr)
{
	/**�����е����ں�term�ȱ���termҪС������ͶƱ*/
	if (vr->term < raft_get_current_term(me))
		return 0;

	/*����Ѿ�Ͷ��Ʊ�ˣ�����false*/
	if (raft_already_voted(me))
		return 0;

	/* �����������������־��Ϣ�Ƿ�ȱ�����־��*/

	/* ��ȡ�������µ���־���� */
	int current_idx = raft_get_current_idx(me);

	/* ������־Ϊ�գ������е���־��Ϣ���Աȱ���Ҫ�£�����true */
	if (0 == current_idx)
		return 1;

	/* �������������־�е����ںű������е�last_log_termҪС���򷵻�true*/

	raft_entry_t* e = raft_get_entry_from_idx(me, current_idx);//���������֪����ôд������
	if (e->term < vr->last_log_term)
		return 1;

	/* ����������־�е����ں��������е�last_log_term��ȣ���Ƚ���־�����������Ƚϴ��˵����־�Ƚ���*/
	if (vr->last_log_term == e->term && current_idx <= vr->last_log_idx)
		return 1;

	/*������������־�е����ںű������е�last_log_termҪ���򷵻�false */
	return 0;
}

/*����ͶƱ�ָ�*/
int raft_recv_requestvote_response(raft_server_private_t* me, raft_node_private_t* node, msg_requestvote_response_t* r)
{
	/* ����Candidate��ֱ�ӷ��� */
	if (me->state!=RAFT_STATE_CANDIDATE)
	{
		return 0;
	}
	/* response�е����ںű��Լ��Ĵ�˵���Լ���term�Ѿ���ʱ��������תΪFollower */
	else if (raft_get_current_term(me) < r->term)
	{
		raft_set_current_term(me, r->term);
		me->state = RAFT_STATE_FOLLOWER;
		return 0;
	}
	/* response�е����ںű��Լ�С��˵���յ���һ����ʱ��response�����Լ��ɡ�*/
	else if (raft_get_current_term(me) != r->term)
	{
		return 0;
	}


	/* ����ͶƱ�� */
	if (1 == r->vote_granted)
	{
		/* ��¼���Լ�ͶƱ�ķ�������Ϣ */
		if (node)
			raft_node_vote_for_me(node, 1);
		int votes = raft_get_nvotes_for_me(me);
		/* ������Լ�ͶƱ�ķ�����������������һ�㣬���ΪLeader */
		if (raft_votes_is_majority(me->num_nodes, votes))
			me->state = RAFT_STATE_LEADER;
	}
	return 0;
}
