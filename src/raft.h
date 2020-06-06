#pragma once
class msg_requestvote_t
{
public:
	/* ��ǰ���ںţ�ͨ�����ںŵĴ�С������Candidate����Leader */
	int term;

	/* ��ѡ�ߵ�id */
	int candidate_id;

	/* ��ѡ�߱��ر��������һ����־��index */
	int last_log_idx;

	/* ��ѡ�߱��ر��������һ����־�����ں�*/
	int last_log_term;
} ;

class msg_requestvote_response_t
{
public:
	/* node�����ںţ�Candidate����ͶƱ�����node�����ں��������Լ������ں� */
	int term;

	/* ͶƱ��������node��CandidateͶƱ��Ϊtrue */
	int vote_granted;
} ;


class msg_appendentries_t
{
	/* Leader��ǰ�����ں� */
	int term;

	/* ������־��ǰһ����־��index������Followerȷ����Leader����־��ȫһ�� */
	int prev_log_idx;

	/* ������־��ǰһ����־�����ں�term */
	int prev_log_term;

	/* leader��ǰ�Ѿ�ȷ���ύ��״̬��FSM����־����index������ζ��FollowerҲ���԰�ȫ�ؽ�������index��ǰ����־�ύ */
	int leader_commit;

	/* ���������־��ϢЯ������־��������ʵ�������ֻ��һ�� */
	int n_entries;

};


class msg_appendentries_response_t
{
	/* ��ǰ���ں� */
	int term;

	/* node�ɹ������־ʱ����ture����prev_log_index��prev_log_term���ȶԳɹ������򷵻�false */
	int success;

	/* ���������ֶβ���Raft�����й涨���ֶ�:
	/* �����Ż���־׷�ӹ��̣��Լ�����־��׷�ӡ�Raftԭ���е�׷�ӹ�����һ��ֻ��׷��һ����־*/

	/** ���������־����󱾵ص������־���� */
	int current_idx;

	/** �������־�����н��ܵĵ�һ����־���� */
	int first_idx;
} ;

class raft_server_private_t
{
public:
	/* ���з������ȽϹ̶���״̬: */

	/* ���������һ��֪�������ںţ���ʼ��Ϊ 0������������ */
	int current_term;

	/* ��¼�ڵ�ǰ�����ڸ��ĸ�CandidateͶ��Ʊ */
	int voted_for;

	/* ��־��Ŀ����ÿһ����Ŀ����һ���û�״̬��ִ�е�ָ����յ�ʱ�����ں� */
	void* log;

	/* �䶯�Ƚ�Ƶ���ı���: */

	/* ��֪�������Ѿ����ύ����־��Ŀ������ֵ */
	int commit_idx;

	/* ���Ӧ�õ�״̬������־��Ŀ����ֵ����ʼ��Ϊ 0������������*/
	int last_applied_idx;

	/* ����״̬��follower/leader/candidate */
	int state;

	/* ��ʱ�������ں���ÿ��ִ��ʱ�������ֵ */
	int timeout_elapsed;

	raft_node_private_t* nodes;
	int num_nodes;

	int election_timeout;
	int request_timeout;

	/* ����Leader����Ϣ��û��LeaderʱΪNULL */
	raft_node_private_t* current_leader;

	/* �Լ�����Ϣ */
	raft_node_private_t* node;

	/* ��raftʵ��ÿ��ֻ����һ�������������ø��ģ��ñ�����¼raft server
	 * �Ƿ����ڽ������ø���*/
	int voting_cfg_change_log_idx;
} ;

class raft_node_private_t
{
public:
	int next_idx; /*����ÿһ������������Ҫ���͸�������һ����־��Ŀ������ֵ����ʼ��Ϊ�쵼���������ֵ��һ��*/

	int match_idx; /*����ÿһ�����������Ѿ����Ƹ�������־���������ֵ*/

	int flags; /*������ȡֵ�������Ĺ�ϵ 1:�û����и���ͶƱ 2:�û�����ͶƱȨ����û�и���������ͶƱ��  3: �û��������µ���־*/

	int id; /*������Ӧ��idֵ�����ÿ̨������ȫ�ֶ���Ψһ��*/
} ;


class  raft_entry_t
{
public:
	/* the entry's term at the point it was created */
	unsigned int term;//���ں�

	/* the entry's unique ID */
	unsigned int id;//��־Ψһid

	/* type of entry */
	int type;//��־����

	raft_entry_data_t data;//��־����
};

//��־���ݽṹ��
class raft_entry_data_t
{
public:
	void *buf;

	unsigned int len;
} ;

