#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include"wallet.h" 



using namespace std;

int main() {
	
	//输入所需的钱包数量
	cin >> number_of_wallet;
	
	//创建动态的钱包对象数组
	Wallet* W = new Wallet[number_of_wallet];
	
	//创建对应的动态ServerNode对象数组
	ServerNode* s = new ServerNode[number_of_wallet];

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
