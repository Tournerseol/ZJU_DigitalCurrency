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

//int number_of_wallet=0;		//全局变量number_of_wallet存放账户数量

int main_menu();
int user_menu();
int GetNumberOfNode();
int Generate_rand(int label);
#pragma warning(disable:4996)
int main(){
	char aim_id[10];
	char login_id[10];
	double tran_money;//交易金额
	int i,k;	
	char ch;
	int search_flag=0;
	int back_to_mainmenu=0;
	int present;//当前登录账户的下标


	//先开辟空间
	Wallet* W = new Wallet[10];
	//ServerNode* s = new ServerNode[10];
	//Log* L = new Log[10];

	while(1){
		switch(main_menu()){		//进入主菜单界面
			case 1:		//注册账户并初始化参数
				
				//钱包部分 初始信息显示
				cout<<"注册成功!钱包ID为:"<<W[number_of_wallet].ReadID()<<endl;
				cout<<"余额为:"<<W[number_of_wallet].Readmoney()<<endl;
				
				/*
				//结点部分 类成员初始化			!!注意:此处的current_leader实际上是private类型，暂时按public处理
				if(number_of_wallet==0)	//若注册的为第一批账户
					s[number_of_wallet].current_leader = -1;
				else
					s[number_of_wallet].current_leader = s[0].current_leader;

				s[number_of_wallet].self_number=number_of_wallet;
				s[number_of_wallet].election_timeout_ = Generate_rand(number_of_wallet);

				//日志部分	可添加初始化操作
				//...
				*/
				number_of_wallet=number_of_wallet+1;	//每次完成注册与初始化后，账户数量+1

				break;
			case 2:		//登录账户
				{cout << "请输入账户id:" <<endl;
				cin >> login_id;
				//查找目标地址对应的数组下标i 
				for(i=0;i<number_of_wallet;i++){
					if(strcmp(W[i].ReadID(),login_id)==0){
						search_flag=1;
						break;
					}	
				}
					//登录及身份验证操作...
					//假设登录的是账户W[present],也是交易发起方
					if(search_flag==1){
						cout<<"登录成功!"<<endl;
						present=i; 
						search_flag=0;
						while(1){
							switch(user_menu()){		//进入用户操作菜单界面
								case 1://余额查询
									cout << "您的钱包余额为:" << W[present].Readmoney() << endl;
									break;
								case 2://发起交易
									cout << "请输入目标钱包id:" <<endl;
									cin >> aim_id;
									cout << "请输入转账金额:" << endl;
									cin >> tran_money;
									if (W[present].Deal(aim_id,tran_money,W)== true) {	//交易成功
										cout << "转账成功" << endl;
										cout << "钱包ID为：" << W[present].ReadID() << endl;
										cout << "余额为:" << W[present].Readmoney() << endl;

										//查找目标地址对应的数组下标k
										for(k=0;k<number_of_wallet;k++){
											if(strcmp(W[k].ReadID(),aim_id)==0)
												break;
										}
										cout << "钱包ID为：" << aim_id << endl;
										cout << "余额为:" << W[k].Readmoney() << endl;

										/*
										//交易信息变动，需告知leader		!!注意:此处的current_leader实际上是private类型，暂时按public处理
										int temp=s[0].current_leader;
										s[temp].ReceiveClientChange(W[present].ReadID(),aim_id,tran_money);	
										*/
									}
									else
										cout << "转账失败!" << endl;
									break;
								case 3:		//注销账户并回到主菜单,如何注销当前登录的钱包W[present]？？
									delete W;
									number_of_wallet=number_of_wallet-1;	//每注销一次，账户数量-1
									cout << "账户已注销!" << endl;
									back_to_mainmenu=1;
									break;

								case 4:		//退出账户并回到主菜单
									back_to_mainmenu=1;
									break;
							}
							if(back_to_mainmenu==1){	//当接收到返回主菜单指令时跳出while循环
								back_to_mainmenu=0;
								break;
							}
						}
					}
					else
						cout<<"用户号不存在!"<<endl;
				}
				break;
			case 3:exit(0);
				break; 
		}
	}
}

int main_menu(){
	int choice;

	cout<<"请输入操作对应的选项:"<<endl;
	cout<<"[1]注册账户"<<endl<<"[2]登录账户"<<endl<<"[3]退出程序"<<endl;
	cout<<"在此输入您的选择:";
	
	while(1){	//判断输入是否有效
		cin>>choice;
		if(choice>=1&&choice<=4)
			break;
		cout<<"无效输入,请重试:"<<endl;
	}
	return choice;
}
int user_menu(){
	int choice;

	cout<<"请输入操作对应的选项:"<<endl;
	cout<<"[1]余额查询"<<endl<<"[2]发起交易"<<endl<<"[3]注销账户"<<endl<<"[4]退出账户"<<endl;
	cout<<"在此输入您的选择:";
	
	while(1){	//判断输入是否有效
		cin>>choice;
		if(choice>=1&&choice<=4)
			break;
		cout<<"无效输入,请重试:"<<endl;
	}
	return choice;
}
int GetNumberOfNode(){	//返回当前账户数量
	return number_of_wallet;
}
int Generate_rand(int label){	//根据结点下标返回序列中一个随机数
	srand(time(NULL));
	int i;
	int a[200];//存放随机数序列，事先生成200个随机数
	for(i=0; i<=199; ++i) 
		a[i]=i+100;
	for(i=199; i>=1; --i) 
		swap(a[i], a[rand()%i]);

	return a[label];
}
