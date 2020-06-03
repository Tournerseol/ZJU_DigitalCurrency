#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define ID_length 4
#define start_money 100;

int number_of_wallet = 10;//规定钱包的数量，此处规定假定为10，后期将在删除和创建钱包时变动

using namespace std;

class Wallet {
public:
	Wallet();//构造函数，创建钱包
	//~Wallet();//析构函数
	double Readmoney();//查询余额，返回值为钱包余额
	bool Deal(char *id, double tran_money, Wallet w[]);//发起交易，参数ID为目标钱包地址，tran_money为转账数额，返回值为是否成功,w[]为钱包类数组
	char* ReadID();//查询ID，返回值为自身地址

private:
	char Name[4];//钱包名，方便称呼该钱包，通常与对象名相同
	char ID[ID_length+1];//钱包地址
	double money;//余额
	char *GenerateID();//用于初始化产生随机地址,后期可能代替以密钥代替
};

Wallet::Wallet() {
	strcpy(ID, GenerateID());
	money = start_money;
}

char* Wallet::GenerateID() {//产生四位随机数字作为ID
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

char* Wallet::ReadID() {
	return ID;
}

double Wallet::Readmoney() {
	return money;
}

bool Wallet::Deal(char* id, double tran_money,Wallet w[]) {
	if (money < tran_money) {
		cout << "余额不足，无法转账\n";
		return false;
	}
	money = money - tran_money;
	int i;
	for (i = 0; i < number_of_wallet; i++) {
		if (strcmp(w[i].ReadID(), id) == 0) {
			w[i].money += tran_money;
			break;
		}
	}
	if (i == number_of_wallet)
		return false;
	return true;
}

int main() {
	Wallet W[10];
	cout<<"钱包ID为："<<W[0].ReadID()<<"\n";
	cout << "余额为:" << W[0].Readmoney() << "\n";
	cout<< "钱包ID为：" << W[1].ReadID()<<"\n";
	cout << "余额为:" << W[1].Readmoney() << "\n";
	if (W[0].Deal(W[1].ReadID(), 30, W) == true) {
		cout << "转账成功\n";
		cout << "钱包ID为：" << W[0].ReadID() << "\n";
		cout << "余额为:" << W[0].Readmoney() << "\n";
		cout << "钱包ID为：" << W[1].ReadID() << "\n";
		cout << "余额为:" << W[1].Readmoney() << "\n";
	}
	else
		cout << "转账失败\n";
}