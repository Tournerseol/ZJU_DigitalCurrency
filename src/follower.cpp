#include<iostream>
#include<string>
#include "raft_.h"

static int IsElected[identify,term_];//��¼ÿ����Ա�ڵ�ǰ�쵼�����ں����Ƿ�Ͷ��Ʊ 
void Checklog();//�Ƚ���־��Ϣ�¾� 

//��class ServerNode��ӱ��� 
//public:
//    int ReturnNumber();
//    ServerNode();//Ĭ�Ϲ��캯��
//    ServerNode(const ServerNode&);//Ĭ�Ͽ������캯�� 
//private:
//    int status;
//    int number;
 //   int HeartBeatMsg; //Ϊ1�����յ�leader�ź�
//	  raft_entry Log;   //raft_entry�� �洢��־��Ϣ 

int ServerNode::TransToCandidate()
{
    if (HeartBeatMsg){//ʱ����δ�յ�heartbeatת��Ϊcandidate
        status=CANDIDATE;
        HeartBeatMsg=0;//ˢ��
        //term_++
        
        return 1;
    }
    
    else{
    	return 0;
	}
}

int ServerNode::RespondRequest(int identify,int candidate){//��VoteAssistant���� 
	if(identify->term_<candidate->term_){
		
		return 1;          //�����ǰ�쵼�����ں�С�ڷ�������ĺ�ѡ����Ӧ�����ں� 
		
	}                      //˵���ó�Ա�Ѿ���ʱ��ת��Ϊ�ú�ѡ�˵�follower 
	else if(identify->term_==candidate->term_){
		
		if(IsElected[identify,identify->term_]==0&&Checklog()==1){
			//update election_timeout_������election timeout 
			
			IsElected[identify,identify->term_]==1;
			
			return 1;//�����ǰ�쵼�����ںŵ��ڷ�������ĺ�ѡ����Ӧ�����ں�,���ҶԷ���־��Ϣ���ȱ�����־�� 
		}            // ����ó�Ա�ڸ����ں���û��Ͷ��Ʊ��Ͷ���ú�ѡ�� 
		else
			return 0;//�Ѿ�Ͷ��Ʊ�Ͳ���Ͷ		
	}
	else{
		
		return 0;//�����ѡ�����ںŸ�С����ú�ѡ���Ѿ���ʱ��������ͶƱ 
	}
}

int ServerNode::ReceiveAppenEntries(ServerNode &L)
{
    if(L.ReturnIdentity()==LEADER && L.SendAppendEntries()==1){
    	//ȷ��leader�Ѿ����������� 
    	
        election_timeout_=rand()%200+100;//����election_timeout_
        HeartBeatMsg=1;//��ʾ�յ�
        
        return 1;
        
    }
    else{
    	
        return 0;
        
    }
    
}

void ServerNode::ReplicateLog(ServerNode &L)
{
    if(HeartBeatMsg && L.ReturnIdentity()==LEADER){
        //д����־
        //Log.data = L.Log.data
    }
}
