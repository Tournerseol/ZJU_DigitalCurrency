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
int number_of_wallet=0;		//ȫ�ֱ���number_of_wallet����˻�����

class Wallet {
public:
	//����������������Ǯ���������ܡ��������������� 
	//���캯��������Ǯ��
	Wallet();
	//��������������������ѯ���� ���������������� 
	//����ֵΪǮ�����
	double Readmoney();
	
	//�������������������׷����� ���������������� 
	//����IDΪĿ��Ǯ����ַ��tran_moneyΪת������,w[]ΪǮ����������
	//����ֵΪ�Ƿ�ɹ�
	bool Deal(char *id, double tran_money, Wallet w[]);
	
	//����������������ID��ȡ���� ���������������� 
	// ����ֵΪ�����ַ
	char* ReadID();
	
	//��������
	//~Wallet();
	
private:
	//Ǯ����
	//����ƺ���Ǯ����ͨ�����������ͬ
	char Name[4];
	
	//Ǯ����ַ
	char ID[ID_length+1];
	
	//Ǯ�����
	double money;
	
	//���ID 
	//���ڳ�ʼ�����������ַ,���ڿ��ܴ�������Կ
	char *GenerateID();
};

//�������������������캯������������������ 
Wallet::Wallet() {
	strcpy(ID, GenerateID());
	money = start_money;
}

//����������������ID���ɡ��������������� 
//������λ���������ΪID
char* Wallet::GenerateID() {
	int i;
	static int k = 0;//��̬�������ڱ�֤Ǯ������ID��ͬ
	char str[ID_length+1];
	srand(time(NULL));
	for (i = 0; i < ID_length; i++) {
		str[i] = '0' + k;
		k = (k + rand()) % 10;
	}
	str[i] = '\0';
	return str;
}

//����������������ID��ȡ���� ���������������� 
char* Wallet::ReadID() {
	return ID;
}

//��������������������ѯ���� ���������������� 
double Wallet::Readmoney() {
	return money;
}

//�������������������׷����� ����������������
bool Wallet::Deal(char* id, double tran_money,Wallet w[]) {
	if (money < tran_money) {
		cout << "���㣬�޷�ת��"<<endl;
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
		cout << "Ŀ��id�����ڣ��޷�ת��"<<endl;
		return false;
	}
		
	money = money - tran_money;
	return true;
}
#endif
