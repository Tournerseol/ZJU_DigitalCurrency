#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string.h>
//#include"raft_consensus.h"
#define ID_length 4
#define start_money 100;
#pragma warning(disable:4996)
using namespace std;

#ifndef WALLET_H_
#define WALLET_H_
int number_of_wallet=0;		//全局变量number_of_wallet存放账户数量

class Wallet {
public:
	//————————钱包创建功能———————— 
	//构造函数，创建钱包
	Wallet();
	//————————余额查询功能 ———————— 
	//返回值为钱包余额
	double Readmoney();
	
	//————————交易发起功能 ———————— 
	//参数ID为目标钱包地址，tran_money为转账数额,w[]为钱包对象数组
	//返回值为是否成功
	bool Deal(char *id, double tran_money, Wallet w[]);
	
	//————————ID读取功能 ———————— 
	// 返回值为自身地址
	char* ReadID();
	
	//析构函数
	//~Wallet();
	
private:
	//钱包名
	//方便称呼该钱包，通常与对象名相同
	char Name[4];
	
	//钱包地址
	char ID[ID_length+1];
	
	//钱包余额
	double money;
	
	//随机ID 
	//用于初始化产生随机地址,后期可能代替以密钥
	char *GenerateID();
};

//————————构造函数———————— 
Wallet::Wallet() {
	strcpy(ID, GenerateID());
	money = start_money;
}

//————————ID生成———————— 
//产生四位随机数字作为ID
char* Wallet::GenerateID() {
	int i;
	static int k = 0;//静态变量用于保证钱包生成ID不同
	char str[ID_length+1];
	srand(time(NULL));
	for (i = 0; i < ID_length; i++) {
		str[i] = '0' + k;
		k = (k + rand()) % 10;
	}
	str[i] = '\0';
	return str;
}

//————————ID读取功能 ———————— 
char* Wallet::ReadID() {
	return ID;
}

//————————余额查询功能 ———————— 
double Wallet::Readmoney() {
	return money;
}

//————————交易发起功能 ————————
bool Wallet::Deal(char* id, double tran_money,Wallet w[]) {
	if (money < tran_money) {
		cout << "余额不足，无法转账"<<endl;
		return false;
	}

	int i;
	for (i = 0; i < number_of_wallet; i++) {
		if (strcmp(w[i].ReadID(), id) == 0) {
			w[i].money += tran_money;
			break;
		}
	}
	if (i == number_of_wallet){
		cout << "目标id不存在，无法转账"<<endl;
		return false;
	}
		
	money = money - tran_money;
	return true;
}
#endif
