//һ��Ҫ�ó��������˳���Ҳ���ǰ�0���˳�����Ҫ������ʱ�رմ��ڣ����򲿷����ݲ���浵�����ݱ��������������б�����
#include <iostream>
#include "myclass.h"
//���ڽ��cin.ignore(numeric_limits<streamsize>::max(), '\n');����
#define NOMINMAX
#include <windows.h>
#include <iomanip>
using namespace std;
int main() {
	SetConsoleTitle("����ͨѶ¼����ϵͳ_By�ż�ΰ");
	system("color 0B");
	uList user;
	while (1) {
		int input;
		system("cls");
		cout << "==========================" << endl;
		cout << "[   ����ͨѶ¼ϵͳǰ̨   ]" << endl;
		cout << "==========================" << endl;
		cout << "[     [1]ͨ��֤��¼      ]" << endl;
		cout << "[     [2]ע��ͨ��֤      ]" << endl;
		cout << "[     [3]���ڱ�ϵͳ      ]" << endl;
		cout << "[     [0]�˳���ϵͳ      ]" << endl;
		cout << "==========================" << endl;
		cout << "���������ѡ���ܣ�";
		if (!(cin >> input) || input < 0 || input > 3) {
			cout << "��������������������룡" << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			Sleep(2000);
			continue;
		}
		switch (input) {
			case 0:
				return 0;
			case 1:
				if (user.signIn() != 1) {
					continue;
				}
				break;
			case 2:
				user.signUp();
				break;
			case 3:
				system("cls");
				cout << "====================================================\n";
				cout << "[              ���ڸ���ͨѶ¼����ϵͳ              ]\n";
				cout << "====================================================\n";
				cout << "[ ��ϵͳӵ�ж��û�����������ͨ�û���VIP������Ա  ]\n";
				cout << "[ ������������ϵ���½���ɾ�����޸ģ���ѯ�������� ]\n";
				cout << "[ �������û�ע�ᣬ��¼�����ܣ�ע����Ȩ�����޸Ĺ��� ]\n";
				cout << "====================================================\n";
				cout << "[    ������ɫ�������ϣ���ܱ��棬�๦�ܷ�ҳ��ʾ    ]\n";
				cout << "[    ���û�֧�֣���ϵ�˷���ͳһ��֤��¼���ϸ�    ]\n";
				cout << "[    ��������ȵ�    ]\n";
				cout << "====================================================\n";
				cout << "[ ����:23�˹����� �ż�ΰ ][ ��л����ʹ��!�d(�R���Q*)o ]\n";
				cout << "====================================================\n";
				cout << "����0�˳���";
				while (getchar() != '0');
				continue;
			default:
				return 0;
		}
		List cL(user.getcurrUser()->getName());
		if (user.getcurrUser()->getType() == "User") {
			cL.receiveShare();
			int flag = 0;
			while (1) {
				int input;
				system("cls");
				cout << "================================" << endl;
				cout << "[      ����ͨѶ¼����ϵͳ      ]" << endl;
				cout << "[      ��ͨ�û���" << left << setw(8) << user.getcurrUser()->getName() << "      ]" << endl;
				cout << "================================" << endl;
				cout << "[        [1]�½���ϵ��         ]" << endl;
				cout << "[        [2]�鿴��ϵ��         ]" << endl;
				cout << "[        [3]ɾ����ϵ��         ]" << endl;
				cout << "[        [4]�޸���ϵ��         ]" << endl;
				cout << "[        [5]������ϵ��         ]" << endl;
				cout << "[        [6]�޸��ҵ�����       ]" << endl;
				cout << "[        [7]ע��ͨ��֤         ]" << endl;
				cout << "[        [0]�˳���ϵͳ         ]" << endl;
				cout << "================================" << endl;
				cout << "[ ��ϵ����������VIP,����๦�� ]" << endl;
				cout << "================================" << endl;

				cout << "���������ѡ���ܣ�";
				if (!(cin >> input) || input < 0 || input > 7) {
					cout << "��������������������룡" << endl;
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					Sleep(2000);
					continue;
				}
				switch (input) {
					case 0:
						flag = 1;
						break;
					case 1:
						cL.createContacts();
						break;
					case 2:
						cL.printNode();
						break;
					case 3:
						cL.deleteContacts();
						break;
					case 4:
						cL.fixContacts();
						break;
					case 5:
						cL.shareContacts(user.gethead());
						break;
					case 6:
						user.fixPassword();
						break;
					case 7:
						if (user.deleteUser()) {
							flag = 1;
						}
						break;
					default:
						break;
					}
				if (flag == 1) {
					break;
				}
			}
		}
		else if (user.getcurrUser()->getType() == "Vip") {
			cL.receiveShare();
			int flag = 0;
			while (1) {
				int input;
				system("cls");
				cout << "================================" << endl;
				cout << "[      ����ͨѶ¼����ϵͳ      ]" << endl;
				cout << "[      VIP�û���" << left << setw(8) << user.getcurrUser()->getName() << "       ]" << endl;
				cout << "================================" << endl;
				cout << "[        [1]�½���ϵ��         ]" << endl;
				cout << "[        [2]�鿴��ϵ��         ]" << endl;
				cout << "[        [3]������ϵ��         ]" << endl;
				cout << "[        [4]ɾ����ϵ��         ]" << endl;
				cout << "[        [5]�޸���ϵ��         ]" << endl;
				cout << "[        [6]������ϵ��         ]" << endl;
				cout << "[        [7]��ϵ������         ]" << endl;
				cout << "[        [8]�޸��ҵ�����       ]" << endl;
				cout << "[        [9]ע��ͨ��֤         ]" << endl;
				cout << "[        [0]�˳���ϵͳ         ]" << endl;
				cout << "================================" << endl;
				cout << "���������ѡ���ܣ�";
				if (!(cin >> input) || input < 0 || input > 9) {
					cout << "��������������������룡" << endl;
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					Sleep(2000);
					continue;
				}
				switch (input) {
					case 0:
						flag = 1;
						break;
					case 1:
						cL.createContacts();
						break;
					case 2:
						cL.printNode();
						break;
					case 3:
						cL.searchContacts();
						break;
					case 4:
						cL.deleteContacts();
						break;
					case 5:
						cL.fixContacts();
						break;
					case 6:
						cL.shareContacts(user.gethead());
						break;
					case 7:
						cout << "�����У����Եȣ���������ʱ���ܺķ�ʱ��ϳ���" << endl;
						cL.sortContacts();
						system("cls");
						cL.printNode();
						break;
					case 8:
						user.fixPassword();
						break;
					case 9:
						if (user.deleteUser()) {
							flag = 1;
						}
						break;
					default:
						break;
				}
				if (flag == 1) {
					break;
				}
			}
		}
		else if (user.getcurrUser()->getType() == "Root") {
			int flag = 0;
			while (1) {
				int input;
				system("cls");
				cout << "================================" << endl;
				cout << "[      ����ͨѶ¼����ϵͳ      ]" << endl;
				cout << "[      ����Ա�û���" << left << setw(8) << user.getcurrUser()->getName() << "    ]" << endl;
				cout << "================================" << endl;
				cout << "[        [1]�鿴ͨ��֤         ]" << endl;
				cout << "[        [2]�޸�Ȩ����         ]" << endl; 
				cout << "[        [3]�޸�ͨ��֤         ]" << endl;
				cout << "[        [4]ɾ��ͨ��֤         ]" << endl;
				cout << "[        [0]�˳���ϵͳ         ]" << endl;
				cout << "================================" << endl;
				cout << "���������ѡ���ܣ�";
				if (!(cin >> input) || input < 0 || input > 4) {
					cout << "��������������������룡" << endl;
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					Sleep(2000);
					continue;
				}
				switch (input) {
					case 0:
						flag = 1;
						break;
					case 1:
						user.printNode();
						break;
					case 2:
						user.fixUserType();
						break;
					case 3:
						user.fixPassword();
						break;
					case 4:
						if (user.deleteUser()) {
							flag = 1;
						}
						break;
					default:
						break;
				}
				if (flag == 1) {
					break;
				}
			}
		}
	}
	return 0;
}
/*
							   _ooOoo_
							  o8888888o
							  88" . "88
							  (| -_- |)
							  O\  =  /O
						   ____/`---'\____
						 .'  \\|     |//  `.
						/  \\|||  :  |||//  \
					   /  _||||| -:- |||||-  \
					   |   | \\\  -  /// |   |
					   | \_|  ''\---/''  |   |
					   \  .-\__  `-`  ___/-. /
					 ___`. .'  /--.--\  `. . __
				  ."" '<  `.___\_<|>_/___.'  >'"".
				 | | :  `- \`.;`\ _ /`;.`/ - ` : | |
				 \  \ `-.   \_ __\ /__ _/   .-` /  /
			======`-.____`-.___\_____/___.-`____.-'======
							   `=---='
			^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
					   ���汣��        ����BUG
			  ��Ի:
					 д��¥��д�ּ䣬д�ּ������Ա��
					 ������Աд�������ó��򻻾�Ǯ��
					 ����ֻ���������������������ߣ�
					 ��������ո��գ����������긴�ꡣ
					 ��Ը��������䣬ֻҪ�ϰ�෢Ǯ;
					 С���󷿲�ȥ�룬׫����ǧ�ù��ꡣ
					 ��Ҫ��ʶ�����棬����Ա�ȳ���Ա��
					 һ������һ�ڵأ����һ��������С�
					 ���˿��Ҵ����죬�ҿ����˴����ƣ�
					 �����Ҳ�д�����δ���Ӽ����ס�
					 �긴һ�괺��ȣ��ȵ��������ϰ壻
					 �ϰ���ұ���Ǯ��û�о�Ǯ�����ꡣ
					 ������ȥ�������������Ҳί�ң�
					 ����ˮԴ��ˮ�࣬������������Ա��
					 ����Ц��߯��񲣬��Ц������̫����
					 ״Ԫ������ʮ�У�ƫƫ��������Ա��
					 ��Ը�������Լ䣬��Ը�Ϲ��ϰ�ǰ��
					 ���۱������Ȥ���������г���Ա��
					 ����Ц��߯��񲣬��Ц�Լ���̫����
					 ��������Ư���ã��ĸ���ó���Ա��
					 ����ֻ����Ǯ���Ǹ��ϰ�Ը��Ǯ��
					 С������Ҫ�룬�������ö����С�
					 �ȶ��ܸ��΢����զ�����̵�Ǯ��
					 һ������һ�ڵأ�����һ��Ǭ������
					 �����������ڵأ��ݺ�����ɽˮ�䣻
					 �������ֺ���Ĺ��һ�׻��ݷ���ɽ��
					 ������ǰ�۷�ֱ���۾�������ãã��
					 ���η��ƺü��ڣ��Ӵ˲�����ָæ��
					 ��֪�����ֿտգ��ϰ忴������ѵ��
					 ������ʱ�۷�������·��֪���Ǽҡ�
					 Сũ����С�񷿣�С����С�񹤣�
					 С����д�������ô�������Ǯ��
					 Ǯ��ֻ�ڴ����У�Ǯ�����ڴ���䣻
					 ��Ǯ��Ǯ�ո��գ����������긴�ꡣ
					 ��Ը��������䣬��Ը�Ϲ��µ�ǰ��
					 �µϱ��۹���Ȥ���������ƶ��Ե��
					 ����ƶ����ƶ�ߣ�һ��ƽ��һ���죻
					 ����ƶ���ȳ������������ҵ��С�
					 ����Ц��߯��񲣬��Ц���˿�������
					 �����Ǵ����ּ䣬��Ȩ�������˼���

*/