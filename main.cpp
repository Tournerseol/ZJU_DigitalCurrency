#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <fstream>
#include <ctime>
//#include <dirent.h>
#include"wallet.h" 
//#include "election.h"
//#include "raft_consensus.h"

using namespace std;

//int number_of_wallet=0;		//ȫ�ֱ���number_of_wallet����˻�����

int main_menu();
int user_menu();
int GetNumberOfNode();
int Generate_rand(int label);
#pragma warning(disable:4996)
int main(){
	char aim_id[10];
	char login_id[10];
	double tran_money;//���׽��
	int i,k;	
	char ch;
	int search_flag=0;
	int back_to_mainmenu=0;
	int present;//��ǰ��¼�˻����±�


	//�ȿ��ٿռ�
	Wallet* W = new Wallet[10];
	//ServerNode* s = new ServerNode[10];
	//Log* L = new Log[10];

	while(1){
		switch(main_menu()){		//�������˵�����
			case 1:		//ע���˻�����ʼ������
				
				//Ǯ������ ��ʼ��Ϣ��ʾ
				cout<<"ע��ɹ�!Ǯ��IDΪ:"<<W[number_of_wallet].ReadID()<<endl;
				cout<<"���Ϊ:"<<W[number_of_wallet].Readmoney()<<endl;
				
				/*
				//��㲿�� ���Ա��ʼ��			!!ע��:�˴���current_leaderʵ������private���ͣ���ʱ��public����
				if(number_of_wallet==0)	//��ע���Ϊ��һ���˻�
					s[number_of_wallet].current_leader = -1;
				else
					s[number_of_wallet].current_leader = s[0].current_leader;

				s[number_of_wallet].self_number=number_of_wallet;
				s[number_of_wallet].election_timeout_ = Generate_rand(number_of_wallet);

				//��־����	����ӳ�ʼ������
				//...
				*/
				number_of_wallet=number_of_wallet+1;	//ÿ�����ע�����ʼ�����˻�����+1

				break;
			case 2:		//��¼�˻�
				{cout << "�������˻�id:" <<endl;
				cin >> login_id;
				//����Ŀ���ַ��Ӧ�������±�i 
				for(i=0;i<number_of_wallet;i++){
					if(strcmp(W[i].ReadID(),login_id)==0){
						search_flag=1;
						break;
					}	
				}
					//��¼�������֤����...
					//�����¼�����˻�W[present],Ҳ�ǽ��׷���
					if(search_flag==1){
						cout<<"��¼�ɹ�!"<<endl;
						present=i; 
						search_flag=0;
						while(1){
							switch(user_menu()){		//�����û������˵�����
								case 1://����ѯ
									cout << "����Ǯ�����Ϊ:" << W[present].Readmoney() << endl;
									break;
								case 2://������
									cout << "������Ŀ��Ǯ��id:" <<endl;
									cin >> aim_id;
									cout << "������ת�˽��:" << endl;
									cin >> tran_money;
									if (W[present].Deal(aim_id,tran_money,W)== true) {	//���׳ɹ�
										cout << "ת�˳ɹ�" << endl;
										cout << "Ǯ��IDΪ��" << W[present].ReadID() << endl;
										cout << "���Ϊ:" << W[present].Readmoney() << endl;

										//����Ŀ���ַ��Ӧ�������±�k
										for(k=0;k<number_of_wallet;k++){
											if(strcmp(W[k].ReadID(),aim_id)==0)
												break;
										}
										cout << "Ǯ��IDΪ��" << aim_id << endl;
										cout << "���Ϊ:" << W[k].Readmoney() << endl;

										/*
										//������Ϣ�䶯�����֪leader		!!ע��:�˴���current_leaderʵ������private���ͣ���ʱ��public����
										int temp=s[0].current_leader;
										s[temp].ReceiveClientChange(W[present].ReadID(),aim_id,tran_money);	
										*/
									}
									else
										cout << "ת��ʧ��!" << endl;
									break;
								case 3:		//ע���˻����ص����˵�,���ע����ǰ��¼��Ǯ��W[present]����
									delete W;
									number_of_wallet=number_of_wallet-1;	//ÿע��һ�Σ��˻�����-1
									cout << "�˻���ע��!" << endl;
									back_to_mainmenu=1;
									break;

								case 4:		//�˳��˻����ص����˵�
									back_to_mainmenu=1;
									break;
							}
							if(back_to_mainmenu==1){	//�����յ��������˵�ָ��ʱ����whileѭ��
								back_to_mainmenu=0;
								break;
							}
						}
					}
					else
						cout<<"�û��Ų�����!"<<endl;
				}
				break;
			case 3:exit(0);
				break; 
		}
	}
}

int main_menu(){
	int choice;

	cout<<"�����������Ӧ��ѡ��:"<<endl;
	cout<<"[1]ע���˻�"<<endl<<"[2]��¼�˻�"<<endl<<"[3]�˳�����"<<endl;
	cout<<"�ڴ���������ѡ��:";
	
	while(1){	//�ж������Ƿ���Ч
		cin>>choice;
		if(choice>=1&&choice<=4)
			break;
		cout<<"��Ч����,������:"<<endl;
	}
	return choice;
}
int user_menu(){
	int choice;

	cout<<"�����������Ӧ��ѡ��:"<<endl;
	cout<<"[1]����ѯ"<<endl<<"[2]������"<<endl<<"[3]ע���˻�"<<endl<<"[4]�˳��˻�"<<endl;
	cout<<"�ڴ���������ѡ��:";
	
	while(1){	//�ж������Ƿ���Ч
		cin>>choice;
		if(choice>=1&&choice<=4)
			break;
		cout<<"��Ч����,������:"<<endl;
	}
	return choice;
}
int GetNumberOfNode(){	//���ص�ǰ�˻�����
	return number_of_wallet;
}
int Generate_rand(int label){	//���ݽ���±귵��������һ�������
	srand(time(NULL));
	int i;
	int a[200];//�����������У���������200�������
	for(i=0; i<=199; ++i) 
		a[i]=i+100;
	for(i=199; i>=1; --i) 
		swap(a[i], a[rand()%i]);

	return a[label];
}
