#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include"wallet.h" 



using namespace std;

int main() {
	
	//���������Ǯ������
	cin >> number_of_wallet;
	
	//������̬��Ǯ����������
	Wallet* W = new Wallet[number_of_wallet];
	
	//������Ӧ�Ķ�̬ServerNode��������
	ServerNode* s = new ServerNode[number_of_wallet];

	cout<<"Ǯ��IDΪ��"<<W[0].ReadID()<<"\n";
	cout << "���Ϊ:" << W[0].Readmoney() << "\n";
	cout<< "Ǯ��IDΪ��" << W[1].ReadID()<<"\n";
	cout << "���Ϊ:" << W[1].Readmoney() << "\n";
	if (W[0].Deal(W[1].ReadID(), 30, W) == true) {
		cout << "ת�˳ɹ�\n";
		cout << "Ǯ��IDΪ��" << W[0].ReadID() << "\n";
		cout << "���Ϊ:" << W[0].Readmoney() << "\n";
		cout << "Ǯ��IDΪ��" << W[1].ReadID() << "\n";
		cout << "���Ϊ:" << W[1].Readmoney() << "\n";
	}
	else
		cout << "ת��ʧ��\n";
}
