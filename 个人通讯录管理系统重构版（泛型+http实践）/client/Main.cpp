#include <iostream>
#include "MyClass.h"
#include "TemplateFunc.cpp"
#define NOMINMAX
#include <windows.h>
using namespace std;
int main() {
	//�½��û�����ͷ
	User* userHead = new Common;
	//��ʼ���û�����
	userHead->initNode();
	//����CMD����
	SetConsoleTitle("����ͨѶ¼����ϵͳ_By�ż�ΰ");
	//�޸��Լ���ɫ
	system("color 0B");
	//ǰ̨��ѭ��
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
		//����Ϸ��Լ��
		if (!(cin >> input) || input < 0 || input > 3) {
			cout << "��������������������룡" << endl;
			//������뻺����
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			Sleep(2000);
			continue;
		}
		//switch-case��֧
		switch (input) {
		//�˳�ϵͳ
		case 0:
			//���浱ǰ�û�
			userHead->saveNode(userHead);
			//���ú���ģ�壬::�ǵ���ȫ�ֺ������˳�ϵͳʱɾ������
			::deleteAll(userHead);
			//ɾ������ͷ�ڵ�
			delete userHead;
			//ͷ�ڵ㸳��
			userHead = nullptr;
			return 0;
		//��¼
		case 1:
			//�жϵ�¼����ֵ
			if (userHead->signIn(userHead) != 1) {
				continue;
			}
			//�ɹ���¼������û��˵�ѡ��
			userHead->menuSelect(userHead);
			break;
		//ע��
		case 2:
			userHead->signUp(userHead);
			userHead->menuSelect(userHead);
			break;
		//����
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
			cout << "[    ���û�֧�֣���ϵ�˷���ͳһ��֤����־ϵͳ    ]\n";
			cout << "[    �ƶ����ݱ��ݻָ����ϸ��������ȵ�          ]\n";
			cout << "====================================================\n";
			cout << "[ ����:23�˹����� �ż�ΰ ][ ��л����ʹ��!�d(�R���Q*)o ]\n";
			cout << "====================================================\n";
			cout << "����0�˳���";
			while (getchar() != '0');
			continue;
		default:
			return 0;
		}
	}
}