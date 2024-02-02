#include <iostream>
#include "httplib.h"
#define NOMINMAX
#include <windows.h>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <conio.h>
#include "MyClass.h"
using namespace std;
//ȫ�ֱ���
//��ǰ���û����洢�˸��ࣨUser��ָ�룬���ڶ�̬��
User* currUser;
//ȫ�ֺ���
//��ϣ����
void encrype(string& str) {
	//����һ����ϣ�������Զ��ַ��������ݽ��й�ϣ����
	hash<string> hasher;
	//�����ַ����Ĺ�ϣֵ
	size_t hash = hasher(str);
	//����ϣֵת��Ϊ�ַ�����ʾ��ʽ
	str = to_string(hash);
	return;
}
//��ȡ��ǰʱ��
string getTime() {
	SYSTEMTIME time;
	GetLocalTime(&time);
	//�½��ַ�����
	stringstream currTime;
	//������sprintf�������ݴ���currTime
	currTime << time.wYear << "��"
		<< setw(2) << setfill('0') << time.wMonth << "��"
		<< setw(2) << setfill('0') << time.wDay << "��"
		<< setw(2) << setfill('0') << time.wHour << ":"
		<< setw(2) << setfill('0') << time.wMinute << ":"
		<< setw(2) << setfill('0') << time.wSecond;
	return currTime.str();//ʹ��.str()�������ַ�����ת��Ϊ�ַ���
}
//������־
void logsSave(string content) {
	//��׷�ӷ�ʽ���ļ�
	ofstream saveFile("logs.txt", ios::app);
	if (!saveFile.is_open()) {
		cout << "�ļ���ʧ�ܣ�" << endl;
		Sleep(2000);
		return;
	}
	//getTime()���µ�ǰʱ�䣬����ֵΪ�ַ���
	saveFile << getTime() << ' ' << currUser->getUsername() << ' ' << content << endl;
	saveFile.close();
	return;
}
//��Root��ȡ��־������ȡ��ǰ�û��Ĳ�����־��
void logsReadClient() {
	string line;
	ifstream readFile("logs.txt");
	//���logs.txt�ļ������ڣ������ofstream�����½��ļ�
	if (!readFile.good()) {
		ofstream createFile("logs.txt");
		if (!createFile.is_open()) {
			cout << "�޷�����logs.txt�ļ���" << endl;
			Sleep(2000);
			return;
		}
		createFile.close();
		readFile.open("logs.txt");
	}
	system("cls");
	cout << "==========================" << endl;
	cout << "[  ͨѶ¼  �ҵĲ�����־  ]" << endl;
	cout << "==========================" << endl;
	while (getline(readFile, line)) {
		string time, username, content;
		istringstream iss(line);
		iss >> time >> username >> content;
		//�ж�������ļ��ǲ��ǵ�ǰ�û�
		if (currUser->getUsername() != username) {
			continue;
		}
		cout << left << "����   " << left << time << left << "   " << content << endl;
	}
	readFile.close();
	cout << "����0�˳������ܣ�";
	while (getchar() != '0');
	return;
}
//Root��ȡ��־����ȡ������־��
void logsReadRoot() {
	string line;
	ifstream readFile("logs.txt");
	//���logs.txt�ļ������ڣ������ofstream�����½��ļ�
	if (!readFile.good()) {
		ofstream createFile("logs.txt");
		if (!createFile.is_open()) {
			cout << "�޷�����logs.txt�ļ���" << endl;
			Sleep(2000);
			return;
		}
		createFile.close();
		readFile.open("logs.txt");
	}
	system("cls");
	cout << "==========================" << endl;
	cout << "[      ϵ ͳ  �� ־      ]" << endl;
	cout << "==========================" << endl;
	while (getline(readFile, line)) {
		string time, username, content;
		istringstream iss(line);
		iss >> time >> username >> content;
		cout << "�û�" << setw(8) << left << username << "��" << setw(28) << left << time << content << endl;
	}
	readFile.close();
	cout << "����0�˳������ܣ�";
	while (getchar() != '0');
	return;
}
//�����־
void logsClean() {
	//�ȴ��ٹرգ�����ԭ��־�ļ����ﵽ���Ч��
	ofstream saveFile("logs.txt");
	saveFile.close();
	cout << "������־�����" << endl;
	Sleep(2000);
	return;
}
//=========================================================================
//���죬��������
User::User(string username, string password) {
	this->username = username;
	this->password = password;
}
//Ϊʲô��д����������ɾ������Ϊÿdeleteһ���ڵ㣨�ڵ�������Ķ����ָ�룩�����ᴥ��������
User::~User() { }
Root::Root(string username, string password) : User(username, password) { }
Common::Common(string username, string password) : User(username, password) { }
Vip::Vip(string username, string password) : User(username, password) { }

//����ʵ��
void User::initNode(LinkList<User*>& list) {
	//�½��ļ�������
	ifstream inputFile("user.txt");
	if (!inputFile.is_open()) {
		cout << "�ļ���ʧ�ܣ�" << endl;
		return;
	}
	//�½����ڶ�ȡÿһ�е��ַ���line
	string line;
	//��ÿһ�ж��浽line��
	while (getline(inputFile, line)) {
		string usertype, username, password;
		istringstream iss(line);
		//�з�line�е�����
		iss >> usertype >> username >> password;
		//����usertype�ж��½�ʲô��Ķ���
		User* t = nullptr;
		if (usertype == "Root") {
			t = new Root(username, password);
		}
		else if (usertype == "Vip") {
			t = new Vip(username, password);
		}
		else {
			t = new Common(username, password);
		}
		list.addNode(t);
	}
	inputFile.close();
	return;
}
void User::saveNode(LinkList<User*>& list) {
	//�ڹ���ʵ������Ҫ����saveNode�����Ա���Ҫ��ͷ�ڵ�ָ�룬�����õ�ǰ�û��Ľڵ�ʹ��ˣ�
	//�½��ļ������
	ofstream saveFile("user.txt");
	if (!saveFile.is_open()) {
		cout << "�ļ���ʧ�ܣ�" << endl;
		Sleep(2000);
		return;
	}
	//��Ϊ����saveNode�Ķ���ֻ��ͷ�ڵ㣬����thisָ�����ͷ�ڵ�ָ�룬����temp���б���
	LinkNode<User*>* temp = list.getHead();
	while (temp->getNext() != nullptr) {
		//������������ļ�
		saveFile << temp->getNext()->getData()->getType() << ' ' << temp->getNext()->getData()->getUsername() << ' ' << temp->getNext()->getData()->getPassword() << endl;
		temp = temp->getNext();
	}
	saveFile.close();
	return;
}
//����ʵ��
//�ж��û����������Ƿ�ֱ�ƥ��
int User::checkInfo(string username, string password) {
	if (this->getUsername() == username && this->getPassword() == password) {
		return 1;
	}
	else {
		return 0;
	}
}
//ע��
void User::signUp(LinkList<User*>& list) {
	while (1) {
		system("cls");
		cout << "==========================" << endl;
		cout << "[   ͨѶ¼    ͳһע��   ]" << endl;
		cout << "==========================" << endl;
		string iusername, ipassword;
		LinkNode<User*>* temp = list.getHead();
		int flag = 0;
		while (1) {
			//�û������벿��
			cout << "�������û�����";
			int inputCount = 0;
			char usrChar;
			while (1) {
				usrChar = _getch();
				if (usrChar == 13) {
					//ѭ��������������ַ������ڻس�
					cout << endl;
					break;
				}
				if (usrChar == 8 && inputCount > 0) {
					//��ɾ����
					cout << "\b \b";
					iusername.pop_back();//�Ƴ�ĩβ�ַ�
					inputCount--;
				}
				else if (usrChar != 8 && usrChar != 32 && inputCount < 20) {
					//������ʾ�û���
					cout << usrChar;
					iusername.push_back(usrChar);
					inputCount++;
				}
			}
			if (iusername.length() == 0) {
				cout << "�û������Ϸ������������룡" << endl;
				iusername = "";
				Sleep(2000);
				continue;
			}
			//ע�����
			while (temp->getNext() != nullptr) {
				if (temp->getNext()->getData()->getUsername() == iusername) {
					cout << "ע��ʧ�ܣ���������û����Ѵ���~��������ע�ᡣ" << endl;
					Sleep(2000);
					flag = 1;
					break;
				}
				temp = temp->getNext();
			}
			if (flag == 1) {
				continue;
			}
			break;
		}
		while (1) {
			//�������벿��
			cout << "���������룺";
			int inputCount = 0;
			char pwdChar;
			while (1) {
				pwdChar = _getch();
				if (pwdChar == 13) {
					cout << endl;
					break;
				}
				if (pwdChar == 8 && inputCount > 0) {
					cout << "\b \b";
					ipassword.pop_back();
					inputCount--;
				}
				else if (pwdChar != 8 && inputCount < 20) {
					//���Թ��� 
					cout << "*";
					ipassword.push_back(pwdChar);
					inputCount++;
				}
			}
			if (ipassword.length() == 0) {
				cout << "���벻�Ϸ������������룡" << endl;
				ipassword = "";
				Sleep(2000);
				continue;
			}
			break;
		}
		cout << "ע��ɹ�����ӭ����" << iusername << endl;
		encrype(ipassword);
		//���½�Common���󣬸�ֵ��currUser���ٰ�currUser����addNodeʵ�ֽڵ����ӣ���addNode��ͬʱ����currUser��
		list.addNode(currUser = new Common(iusername, ipassword));
		saveNode(list);
		//������־
		logsSave("ע��ɹ�");
		Sleep(2000);
		break;
	}
	return;
}
////��¼
int User::signIn(LinkList<User*>& list) {
	system("cls");
	cout << "==========================" << endl;
	cout << "[   ͨѶ¼    ͳһ��¼   ]" << endl;
	cout << "==========================" << endl;
	if (list.getHead()->getNext() == nullptr) {
		cout << "ϵͳ��û���û�������ע�ᣡ" << endl;
		Sleep(2000);
		return 2;
	}
	for (int i = 4; i >= 0; i--) {
		string iusername, ipassword;
		cout << "�������û�����";
		int inputCount = 0;
		char usrChar;
		while (1) {
			usrChar = _getch();
			if (usrChar == 13) {
				//ѭ��������������ַ������ڻس�
				cout << endl;
				break;
			}
			if (usrChar == 8 && inputCount > 0) {
				//��ɾ����
				cout << "\b \b";
				iusername.pop_back();//�Ƴ�ĩβ�ַ�
				inputCount--;
			}
			else if (usrChar != 8 && usrChar != 32 && inputCount < 20) {
				//������ʾ�û���
				cout << usrChar;
				iusername.push_back(usrChar);
				inputCount++;
			}
		}
		//�������벿��
		cout << "���������룺";
		inputCount = 0;
		char pwdChar;
		while (1) {
			pwdChar = _getch();
			if (pwdChar == 13) {
				cout << endl;
				break;
			}
			if (pwdChar == 8 && inputCount > 0) {
				cout << "\b \b";
				ipassword.pop_back();
				inputCount--;
			}
			else if (pwdChar != 8 && inputCount < 20) {
				//���Թ��� 
				cout << "*";
				ipassword.push_back(pwdChar);
				inputCount++;
			}
		}
		//�����ϣ����
		encrype(ipassword);
		LinkNode<User*>* temp = list.getHead();
		while (temp->getNext() != nullptr) {
			if (temp->getNext()->getData()->checkInfo(iusername, ipassword)) {
				//���µ�ǰ�û�
				currUser = temp->getNext()->getData();
				//������־
				logsSave("��¼�ɹ�");
				//��ӭ��
				if (currUser->getType() == "Vip") {
					cout << "��½�ɹ�������VIP�û���" << iusername << " ��ӭ����" << endl;
				}
				else if (currUser->getType() == "Root") {
					cout << "����Ա��½�ɹ���" << iusername << endl;
				}
				else {
					cout << "��½�ɹ�����ӭ����" << iusername << endl;
				}
				Sleep(2000);
				return 1;
			}
			temp = temp->getNext();
		}
		cout << "�û�������������㻹��" << i << "�λ��ᣡ" << endl;
	}
	cout << "��½ʧ�ܣ�" << endl;
	return 0;
}
//�˵�ѡ��
void User::menuSelect(LinkList<User*>& list) {
	//��̬ʵ�ֲ�ͬ�û����벻ͬ�˵�
	currUser->enterMenu(list);
	return;
}
//��ȡ�û���
string User::getUsername() {
	return username;
}
//��ȡ����
string User::getPassword() {
	return password;
}
//����������
void User::setPassword(string newPassword) {
	this->password = newPassword;
}
//�����û�Ȩ����
void User::setType(string usertype, LinkList<User*>& list) {
	//ע�����setTypeʱ��һ��Ҫ���û����µ�¼�������ٴ�ʹ�ù���ʱ��ǰ�û��ڵ��Ѿ��ͷ��ˣ�
	//�޷�ֱ���ڲ�ͬ���͵��������ֱ��ת�����������½�һ���µĽڵ�
	User* t = nullptr;
	if (usertype == "Root") {
		t = new Root(this->getUsername(), this->getPassword());
	}
	else if (usertype == "Vip") {
		t = new Vip(this->getUsername(), this->getPassword());
	}
	else {
		t = new Common(this->getUsername(), this->getPassword());
	}
	list.addNode(t);
	//����ɾ����ǰ�û��ڵ㣬��ǰ�û��ڵ㲻���������һ������Ϊ�ո��Ѿ��½��˽ڵ���β��
	LinkNode<User*>* temp = list.getHead();
	while (temp->getNext() != nullptr) {
		if (temp->getNext()->getData() == this) {
			temp->setNext(temp->getNext()->getNext());
			delete this;
			return;
		}
		temp = temp->getNext();
	}
}
//��ȡ�û�����
string Root::getType() {
	return "Root";
}
string Common::getType() {
	return "Common";
}
string Vip::getType() {
	return "Vip";
}
//�鿴ͨ��֤
void Root::printUser(LinkList<User*>& list) {
	system("cls");
	cout << "========================================" << endl;
	cout << "[         ����Ա  �鿴ͨ��֤           ]" << endl;
	cout << "========================================" << endl;
	cout << " Ȩ����  �û���" << endl;
	LinkNode<User*>* temp = list.getHead();
	while (temp->getNext() != nullptr) {
		cout << setw(7) << temp->getNext()->getData()->getType() << setw(8) << temp->getNext()->getData()->getUsername() << endl;
		temp = temp->getNext();
	}
	cout << "����0�˳������ܣ�";
	while (getchar() != '0');
	return;
}
//���ڷǹ����鿴�����û���û��Ȩ�޵�
void Common::printUser(LinkList<User*>& list) {
	cout << "û��Ȩ�ޣ�" << endl;
}
void Vip::printUser(LinkList<User*>& list) {
	cout << "û��Ȩ�ޣ�" << endl;
}
//Menu
void Root::enterMenu(LinkList<User*>& list) {
	int flag = 0;
	while (1) {
		int input;
		system("cls");
		cout << "================================" << endl;
		cout << "[      ����ͨѶ¼����ϵͳ      ]" << endl;
		cout << "[      ����Ա�û���" << left << setw(8) << currUser->getUsername() << "    ]" << endl;
		cout << "================================" << endl;
		cout << "[        [1]�鿴ͨ��֤         ]" << endl;
		cout << "[        [2]�޸�Ȩ����         ]" << endl;
		cout << "[        [3]ͨ��֤����         ]" << endl;
		cout << "[        [4]ɾ��ͨ��֤         ]" << endl;
		cout << "[        [5]�鿴ϵͳ��־       ]" << endl;
		cout << "[        [6]���ϵͳ��־       ]" << endl;
		cout << "[        [0]�˳���¼           ]" << endl;
		cout << "================================" << endl;

		cout << "���������ѡ���ܣ�";
		if (!(cin >> input) || input < 0 || input > 6) {
			cout << "��������������������룡" << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			Sleep(2000);
			continue;
		}
		LinkList<Contacts> clist;
		switch (input) {
		//�˳���¼
		case 0:
			flag = 1;
			saveNode(list);
			logsSave("�˳���¼");
			break;
		//�鿴ͨ��֤
		case 1:
			currUser->printUser(list);
			//�鿴��ɺ�ͬʱ��¼��־
			logsSave("�鿴��ͨ��֤");
			break;
		//�޸�Ȩ����
		case 2:
			if (currUser->fixType(list)) {
				flag = 1;
			}
			break;
		//ͨ��֤����
		case 3:
			currUser->fixPassword(list);
			break;
		//ɾ��ͨ��֤
		case 4:
			if (currUser->deleteUser(list, clist)) {
				flag = 1;
			}
			break;
		//�鿴ϵͳ��־
		case 5:
			logsReadRoot();
			logsSave("�鿴��ϵͳ��־");
			break;
		//���ϵͳ��־
		case 6:
			logsClean();
			logsSave("�����ϵͳ��־");
			break;
		default:
			break;
		}
		if (flag == 1) {
			break;
		}
	}
}
//��ͨ�û��˵�
void Common::enterMenu(LinkList<User*>& list) {
	LinkList<Contacts> clist;
	clist.getHead()->getData().initNode(clist);
	clist.getHead()->getData().receiveShare(clist);
	int flag = 0;
	while (1) {
		int input;
		system("cls");
		cout << "================================" << endl;
		cout << "[      ����ͨѶ¼����ϵͳ      ]" << endl;
		cout << "[      ��ͨ�û���" << left << setw(8) << currUser->getUsername() << "      ]" << endl;
		cout << "================================" << endl;
		cout << "[        [1]�½���ϵ��         ]" << endl;
		cout << "[        [2]�鿴��ϵ��         ]" << endl;
		cout << "[        [3]ɾ����ϵ��         ]" << endl;
		cout << "[        [4]�޸���ϵ��         ]" << endl;
		cout << "[        [5]������ϵ��         ]" << endl;
		cout << "[        [6]�޸��ҵ�����       ]" << endl;
		cout << "[        [7]ע��ͨ��֤         ]" << endl;
		cout << "[        [0]�˳���¼           ]" << endl;
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
			//�˳���¼
		case 0:
			flag = 1;
			clist.getHead()->getData().saveNode(clist);
			logsSave("�˳���¼");
			break;
			//�½���ϵ��
		case 1:clist.getHead()->getData().createContacts(clist);
			break;
			//�鿴��ϵ��
		case 2:
			clist.getHead()->getData().printContacts(clist);
			//�鿴��ɺ�ͬʱ��¼��־
			logsSave("�鿴����ϵ��");
			break;
			//ɾ����ϵ��
		case 3:
			clist.getHead()->getData().deleteContacts(clist);
			break;
			//�޸���ϵ��
		case 4:
			clist.getHead()->getData().fixContacts(clist);
			break;
			//������ϵ��
		case 5:
			clist.getHead()->getData().shareContacts(clist, list);
			break;
			//�޸�����
		case 6:
			currUser->fixPassword(list);
			break;
			//ע��ͨ��֤
		case 7:
			if (currUser->deleteUser(list, clist)) {
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
	return;
}
//Vip�û��˵�
void Vip::enterMenu(LinkList<User*>& list) {
	LinkList<Contacts> clist;
	clist.getHead()->getData().initNode(clist);
	clist.getHead()->getData().receiveShare(clist);
	int flag = 0;
	while (1) {
		char input;
		system("cls");
		cout << "================================" << endl;
		cout << "[      ����ͨѶ¼����ϵͳ      ]" << endl;
		cout << "[      VIP�û���" << left << setw(8) << currUser->getUsername() << "       ]" << endl;
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
		cout << "[        [A]�鿴������־       ]" << endl;
		cout << "[        [B]�ƶ˱��ݻָ�       ]" << endl;
		cout << "[        [0]�˳���¼           ]" << endl;
		cout << "================================" << endl;
		cout << "���������ѡ���ܣ�";
		cin >> input;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		//����VIP����̫���ˣ�������0~9������ֱ�������ַ���0~A
		switch (input) {
			//�˳���¼
		case '0':
			flag = 1;
			clist.getHead()->getData().saveNode(clist);
			logsSave("�˳���¼");
			break;
			//�½���ϵ��
		case '1':
			clist.getHead()->getData().createContacts(clist);
			break;
			//�鿴��ϵ��
		case '2':
			clist.getHead()->getData().printContacts(clist);
			logsSave("�鿴����ϵ��");
			break;
			//�鿴��ϵ��
		case '3':
			clist.getHead()->getData().searchContacts(clist);
			break;
			//ɾ����ϵ��
		case '4':
			clist.getHead()->getData().deleteContacts(clist);
			break;
			//�޸���ϵ��
		case '5':
			clist.getHead()->getData().fixContacts(clist);
			break;
			//������ϵ��
		case '6':
			clist.getHead()->getData().shareContacts(clist, list);
			break;
			//��ϵ������
		case '7':
			cout << "�����У����Եȣ���������ʱ���ܺķ�ʱ��ϳ���" << endl;
			clist.getHead()->getData().sortContacts(clist);
			clist.getHead()->getData().printContacts(clist);
			logsSave("ִ������ϵ������");
			break;
			//�޸�����
		case '8':
			currUser->fixPassword(list);
			break;
			//ע��ͨ��֤
		case '9':
			if (currUser->deleteUser(list, clist)) {
				flag = 1;
			}
			break;
			//�鿴��־
		case 'A':
			logsReadClient();
			logsSave("�鿴�˲�����־");
			break;
			//�ƶ˱��ݻָ�
		case 'B':
			clist.getHead()->getData().cloudFunction(clist);
			break;
		default:
			cout << "��������������������룡" << endl;
			Sleep(2000);
			break;
		}
		if (flag == 1) {
			break;
		}
	}
	return;
}
//����Ա�˵�
int Root::deleteUser(LinkList<User*>& ulist, LinkList<Contacts>& clist) {
	//delete contactsHead;
	system("cls");
	cout << "========================================" << endl;
	cout << "[           ͨ��֤  ɾ���û�           ]" << endl;
	cout << "========================================" << endl;
	LinkNode<User*>* temp = ulist.getHead();
	string currUsername = currUser->getUsername();
	cout << "��ǰ���û��������£�" << endl;
	cout << "Ȩ����  �û���" << endl;
	//��������û�
	while (temp->getNext() != nullptr) {
		cout << setw(8) << temp->getNext()->getData()->getType() << setw(8) << temp->getNext()->getData()->getUsername() << endl;
		temp = temp->getNext();
	}
	string content;
	while (1) {
		temp = ulist.getHead();
		int returnValue = 0;
		cout << "��������Ҫɾ�����û�������Աɾ���û�ʱ����������û��ı������ƶ����ݣ���";
		cin >> content;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		//�ж�Ҫɾ�����û��Ƿ�Ϊ��ǰ��¼�û����Ա�ִ���Զ��˳���¼����
		if (currUser->getUsername() == content) {
			returnValue = 1;
		}
		else {
			returnValue = 0;
		}
		while (temp->getNext() != nullptr) {
			if (temp->getNext()->getData()->getUsername() == content) {
				if (returnValue == 1) {
					cout << "��ɾ�����ǵ�ǰ�����û��������˳���¼��" << endl;
					logsSave("ע�����Լ����˺�");
				}
				else {
					//������־��һ��Ҫ�ȱ�����־����ɾ������Ȼ�ͻ���ÿ�ָ���ˣ�
					stringstream logsContent;
					logsContent << "ɾ�����û�[" << content << ']';
					logsSave(logsContent.str());
				}
				//���ú���ģ�壬ɾ���ڵ�
				ulist.deleteNode(temp->getNext()->getData());
				cout << "�û�ɾ���ɹ���";
				saveNode(ulist);
				Sleep(3000);
				return returnValue;
			}
			temp = temp->getNext();
		}
		cout << "�Ҳ����û��������������û�����" << endl;
		Sleep(2000);
		continue;
	}
	return 0;
}
//�û�ע���Լ����˺�
int Common::deleteUser(LinkList<User*>& list, LinkList<Contacts>& clist) {
	system("cls");
	cout << "========================================" << endl;
	cout << "[           ͨ��֤  �û�ע��           ]" << endl;
	cout << "========================================" << endl;
	int input, flag = 0;
	cout << "���棡�����ڽ��е�ǰ�û���" << currUser->getUsername() << " ��ע��������\n�ò�����ͬʱɾ������������ϵ�ˣ�" << endl;
	while (1) {
		cout << "�Ƿ�ȷ��ע����[1]ȷ��  [0]ȡ����" << endl;
		if (!(cin >> input) || input < 0 || input > 1) {
			cout << "��������������������룡" << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			Sleep(2000);
			continue;
		}
		if (input == 1) {
			//��ɣ������˺þòŷ��ֵģ�
			//һ��Ҫ��ɾ���û������ݣ���ɾ���û���������ɾ���û�����ʱ������currUser�ͱ�ɵ��ÿ�ָ���ˣ�
			clist.deleteAll();
			clist.getHead()->getData().saveNode(clist);
			logsSave("ע�����Լ����˺�");
			//ֱ�ӵ���deleteNodeɾ����ǰ�ڵ�
			list.deleteNode(currUser);
			cout << "�˻�ע���ɹ���" << endl;
			list.getHead()->getData()->saveNode(list);
			Sleep(2000);
			return 1;
		}
		else {
			return 0;
		}
	}
}
//�û�ע���Լ����˺�
int Vip::deleteUser(LinkList<User*>& ulist, LinkList<Contacts>& clist) {
	system("cls");
	cout << "========================================" << endl;
	cout << "[           ͨ��֤  �û�ע��           ]" << endl;
	cout << "========================================" << endl;
	int input, flag = 0;
	//����Vip�û��Ĺ���
	cout << "�װ���Vip�û��������ڽ��е�ǰ�û���" << currUser->getUsername() << " ��ע��������\n�ò�����ͬʱɾ������������ϵ�ˣ������ƶ˱��ݣ���" << endl;
	while (1) {
		cout << "�Ƿ�ȷ��ע����[1]ȷ��  [0]ȡ����" << endl;
		if (!(cin >> input) || input < 0 || input > 1) {
			cout << "��������������������룡" << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			Sleep(2000);
			continue;
		}
		if (input == 1) {
			//ɾ���ƶ˲���
			//Ԥ��������ļ���
			string username = currUser->getUsername();
			encrype(username);
			stringstream filename;
			filename << username << ".txt";
			//����HTTP�ͻ���
			httplib::Client client("mc-sh.xzvps.top", 19060);
			stringstream serverpath;
			serverpath << "/remove/" << filename.str();
			//����GET����ɾ���������е��ļ���Ŀ¼����/remove/114514.txt
			auto res = client.Get(serverpath.str());
			//ֱ�ӵ���deleteAllɾ���û�����
			clist.deleteAll();
			clist.getHead()->getData().saveNode(clist);
			logsSave("ע�����Լ����˺�");
			//ֱ�ӵ���deleteNodeɾ����ǰ�ڵ�
			ulist.deleteNode(currUser);
			cout << "�˻�ע���ɹ���" << endl;
			ulist.getHead()->getData()->saveNode(ulist);
			Sleep(2000);
			return 1;
		}
		else {
			return 0;
		}
	}
}
//����Ա�޸�Ȩ����
int Root::fixType(LinkList<User*>& list) {
	system("cls");
	cout << "========================================" << endl;
	cout << "[         ����Ա  �޸�Ȩ����           ]" << endl;
	cout << "========================================" << endl;
	cout << "��ǰ���û��������£�" << endl;
	cout << "Ȩ����  �û���" << endl;
	LinkNode<User*>* temp = list.getHead();
	while (temp->getNext() != nullptr) {
		cout << setw(8) << temp->getNext()->getData()->getType() << setw(8) << temp->getNext()->getData()->getUsername() << endl;
		temp = temp->getNext();
	}
	string content;
	while (1) {
		temp = list.getHead();
		cout << "��������Ҫ�޸ĵ��û���";
		cin >> content;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		//����returnValue������ʵ���ж��޸���ɺ��Ƿ���Ҫ�Զ��˳��˺ţ�
		int returnValue = 0;
		//����޸ĵ��ǵ�ǰ�û�����ô�����Զ��˳���¼
		if (currUser->getUsername() == content) {
			returnValue = 1;
		}
		while (temp->getNext() != nullptr) {
			if (temp->getNext()->getData()->getUsername() == content) {
				int input;
				cout << "�ҵ����ݣ�" << endl;
				cout << "[ [1]��ͨ�û�  [2]VIP�û�  [3]����Ա ]" << endl;
				while (1) {
					cout << "��ѡ��Ҫ�޸�Ϊ��Ȩ���飺";
					if (!(cin >> input) || input < 0 || input > 3) {
						cout << "��������������������룡" << endl;
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
						Sleep(2000);
						continue;
					}
					string selectedType;
					if (input == 2) {
						selectedType = "Vip";
					}
					else if (input == 3) {
						selectedType = "Root";
					}
					else {
						selectedType = "Common";
					}
					stringstream logsContent;
					logsContent << "���û�[" << content << "]��Ȩ�����Ϊ[" << selectedType << ']';
					logsSave(logsContent.str());
					if (input == 2) {
						temp->getNext()->getData()->setType("Vip", list);
					}
					else if (input == 3) {
						temp->getNext()->getData()->setType("Root", list);
					}
					else {
						temp->getNext()->getData()->setType("Common", list);
					}
					cout << "�޸���ɣ�";
					saveNode(list);
					if (returnValue == 1) {
						cout << "���޸ĵ��ǵ�ǰ�û���Ȩ���飬�����Զ��˳���¼��" << endl;
					}
					else {
						cout << endl;
					}
					Sleep(3000);
					return returnValue;
				}
			}
			temp = temp->getNext();
		}
		cout << "�Ҳ����û��������������û�����" << endl;
		Sleep(2000);
		continue;
	}
}
//�û��޷��޸�Ȩ����
int Common::fixType(LinkList<User*>& list) {
	cout << "û��Ȩ�ޣ�" << endl;
	return 0;
}
//�û��޷��޸�Ȩ����
int Vip::fixType(LinkList<User*>& list) {
	cout << "û��Ȩ�ޣ�" << endl;
	return 0;
}
//����Ա�޸�����������
void Root::fixPassword(LinkList<User*>& list){
	system("cls");
	cout << "========================================" << endl;
	cout << "[           ͨ��֤  �޸�����           ]" << endl;
	cout << "========================================" << endl;
	cout << "��ǰ���û��������£�" << endl;
	cout << "Ȩ����  �û���" << endl;
	LinkNode<User*>* temp = list.getHead();
	while (temp->getNext() != nullptr) {
		cout << setw(8) << temp->getNext()->getData()->getType() << setw(8) << temp->getNext()->getData()->getUsername() << endl;
		temp = temp->getNext();
	}
	string content;
	while (1) {
		temp = list.getHead();
		cout << "��������Ҫ�޸ĵ��û���";
		cin >> content;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		while (temp->getNext() != nullptr) {
			if (temp->getNext()->getData()->getUsername() == content) {
				string ipassword;
				while (1) {
					cout << "�ҵ����ݣ��������û� " << content << " ���µ����룺";
					//��������
					int inputCount = 0;
					char pwdChar;
					while (1) {
						pwdChar = _getch();
						if (pwdChar == 13) {
							cout << endl;
							break;
						}
						if (pwdChar == 8 && inputCount > 0) {
							cout << "\b \b";
							ipassword.pop_back();
							inputCount--;
						}
						else if (pwdChar != 8 && inputCount < 20) {
							//���Թ��� 
							cout << "*";
							ipassword.push_back(pwdChar);
							inputCount++;
						}
					}
					if (ipassword.length() == 0) {
						cout << "���벻�Ϸ������������룡" << endl;
						ipassword = "";
						Sleep(2000);
						continue;
					}
					break;
				}
				encrype(ipassword);
				//��������
				temp->getNext()->getData()->setPassword(ipassword);
				cout << "�޸ĳɹ���" << endl;
				stringstream logsContent;
				logsContent << "�޸����û�[" << content << "]������";
				logsSave(logsContent.str());
				saveNode(list);
				Sleep(2000);
				return;
			}
			temp = temp->getNext();
		}
		cout << "�Ҳ����û��������������û�����" << endl;
		continue;
	}
}
//��ͨ�û��޸��Լ�������
void Common::fixPassword(LinkList<User*>& list) {
	system("cls");
	cout << "========================================" << endl;
	cout << "[           ͨ��֤  �޸�����           ]" << endl;
	cout << "========================================" << endl;
	string ipassword;
	while (1) {
		cout << "���������������룺";
		int inputCount = 0;
		char pwdChar;
		while (1) {
			pwdChar = _getch();
			if (pwdChar == 13) {
				cout << endl;
				break;
			}
			if (pwdChar == 8 && inputCount > 0) {
				cout << "\b \b";
				ipassword.pop_back();
				inputCount--;
			}
			else if (pwdChar != 8 && inputCount < 20) {
				//���Թ��� 
				cout << "*";
				ipassword.push_back(pwdChar);
				inputCount++;
			}
		}
		if (ipassword.length() == 0) {
			cout << "���벻�Ϸ������������룡" << endl;
			ipassword = "";
			Sleep(2000);
			continue;
		}
		break;
	}
	encrype(ipassword);
	currUser->setPassword(ipassword);
	cout << "�޸ĳɹ��������˳���" << endl;
	logsSave("�޸����Լ�������");
	saveNode(list);
	Sleep(2000);
	return;
}
//��ͨ�û��޸��Լ�������
void Vip::fixPassword(LinkList<User*>& list) {
	system("cls");
	cout << "========================================" << endl;
	cout << "[           ͨ��֤  �޸�����           ]" << endl;
	cout << "========================================" << endl;
	string ipassword;
	while (1) {
		cout << "���������������룺";
		int inputCount = 0;
		char pwdChar;
		while (1) {
			pwdChar = _getch();
			if (pwdChar == 13) {
				cout << endl;
				break;
			}
			if (pwdChar == 8 && inputCount > 0) {
				cout << "\b \b";
				ipassword.pop_back();
				inputCount--;
			}
			else if (pwdChar != 8 && inputCount < 20) {
				//���Թ��� 
				cout << "*";
				ipassword.push_back(pwdChar);
				inputCount++;
			}
		}
		if (ipassword.length() == 0) {
			cout << "���벻�Ϸ������������룡" << endl;
			ipassword = "";
			Sleep(2000);
			continue;
		}
		break;
	}
	encrype(ipassword);
	currUser->setPassword(ipassword);
	cout << "�޸ĳɹ��������˳���" << endl;
	logsSave("�޸����Լ�������");
	saveNode(list);
	Sleep(2000);
	return;
}

//=================================================��ϵ����========================================================//
Contacts::Contacts(string tel, string mob, string qq, string name, string unit, string address, string sort, string email) {
	this->telephone = tel;
	this->mobilephone = mob;
	this->qq = qq;
	this->name = name;
	this->unit = unit;
	this->address = address;
	this->sort = sort;
	this->email = email;
}
//Ϊʲô��д����������ɾ������Ϊÿdeleteһ���ڵ㣬���ᴥ��������
//�������������в����������޷�����������ɾ���ڵ㣬��Ϊ��Ҫ����ͷ�ڵ㡣
Contacts::~Contacts() { }
//��ϵ�������ʼ��
void Contacts::initNode(LinkList<Contacts>& clist) {
	//�½��ļ�������
	ifstream inputFile("data.txt");
	if (!inputFile.is_open()) {
		cout << "�ļ���ʧ�ܣ�" << endl;
		return;
	}
	//�½����ڶ�ȡÿһ�е��ַ���line
	string line;
	//��ÿһ�ж��浽line��
	while (getline(inputFile, line)) {
		string username, telephone, mobilephone, qq, name, unit, address, sort, email;
		istringstream iss(line);
		//�з�line�е�����
		iss >> username >> telephone >> mobilephone >> qq >> name >> unit >> address >> sort >> email;
		//�ж�������ļ��ǲ��ǵ�ǰ�û�
		if (currUser->getUsername() != username) {
			continue;
		}
		Contacts* t;
		clist.addNode(*(t = new Contacts(telephone, mobilephone, qq, name, unit, address, sort, email)));
		delete t;
	}
	inputFile.close();
	return;
}
//�����������ļ�
void Contacts::saveNode(LinkList<Contacts>& clist) {
	ifstream readFile("data.txt");
	ofstream tempFile("temp.txt");
	//��ֹû��data�ļ�����Ϊһ��ʼ��data�����һ��ʼдdata�Ͳ��ÿ���data�ļ�û�е������
	if (!readFile.good()) {
		ofstream createFile("data.txt");
		if (!createFile.is_open()) {
			cout << "�޷�����data.txt�ļ���" << endl;
			Sleep(2000);
			return;
		}
		createFile.close();
		readFile.open("data.txt");
	}

	//�Ȱѷǵ�ǰ�û����ݵ���temp������
	string line;
	while (getline(readFile, line)) {
		string username, telephone, mobilephone, qq, name, unit, address, sort, email;
		istringstream iss(line);
		iss >> username >> telephone >> mobilephone >> qq >> name >> unit >> address >> sort >> email;
		if (username != currUser->getUsername()) {
			tempFile << username << ' ' << telephone << ' ' << mobilephone << ' ' << qq << ' ' << name << ' ' << unit << ' ' << address << ' ' << sort << ' ' << email << endl;
		}
	}
	readFile.close();
	tempFile.close();
	//Ȼ��ѵ�ǰ�û����ݸ��ǵ�data�����ļ�
	ofstream saveFile("data.txt");
	LinkNode<Contacts>* temp = clist.getHead();
	while (temp->getNext() != nullptr) {
		saveFile << currUser->getUsername() << ' ' << temp->getNext()->getData().telephone << ' ' << temp->getNext()->getData().mobilephone << ' ' << temp->getNext()->getData().qq << ' ' << temp->getNext()->getData().name << ' ' << temp->getNext()->getData().unit << ' ' << temp->getNext()->getData().address << ' ' << temp->getNext()->getData().sort << ' ' << temp->getNext()->getData().email << endl;
		temp = temp->getNext();
	}
	saveFile.close();
	//����temp�����е�����׷�ӵ�data�����ļ���
	ifstream readtempFile("temp.txt");
	ofstream savedataFile("data.txt", ios::app);
	savedataFile << readtempFile.rdbuf();
	readtempFile.close();
	savedataFile.close();
}
//��ȡ��ϵ������
string Contacts::getName() {
	return this->name;
}
//�½���ϵ��
void Contacts::createContacts(LinkList<Contacts>& clist) {
	//num2���ڱ���num����ʱnum�仯���ֵ
	int num = 0, num2 = 0;
	LinkNode<Contacts>* temp = clist.getHead();
	while (1) {
		system("cls");
		cout << "==========================" << endl;
		cout << "[   ͨѶ¼  ������ϵ��   ]" << endl;
		cout << "==========================" << endl;
		cout << "��������ϵ�˴�������(1-100)��";
		if (!(cin >> num) || num <= 0 || num > 100) {
			cout << "��������������������룡" << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			Sleep(2000);
			continue;
		}
		num2 = num;
		cout << "��������ϵ�˵������Ϣ��" << endl;
		while (num--) {
			string telephone, mobilephone, qq, name, unit, address, sort, email;
			while (1) {
				int flag = 0;
				//��������
				cout << "�������" << num2 - num << "����ϵ�˵�������";
				cin >> name;
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				temp = clist.getHead();
				//��������
				while (temp->getNext() != nullptr) {
					if (temp->getNext()->getData().getName() == name) {
						cout << "����������������������ظ������������룡" << endl;
						Sleep(2000);
						flag = 1;
						break;
					}
					temp = temp->getNext();
				}
				if (flag == 1) {
					continue;
				}
				//�����ֻ���
				cout << "�������" << num2 - num << "����ϵ�˵��ֻ��ţ�";
				cin >> mobilephone;
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				//����������
				cout << "�������" << num2 - num << "����ϵ�˵������ţ�";
				cin >> telephone;
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				//����QQ��
				cout << "�������" << num2 - num << "����ϵ�˵�QQ�ţ�";
				cin >> qq;
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				//����QQ��
				cout << "�������" << num2 - num << "����ϵ�˵����䣺";
				cin >> email;
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				//���뵥λ
				cout << "�������" << num2 - num << "����ϵ�˵ĵ�λ��";
				cin >> unit;
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				//����סַ
				cout << "�������" << num2 - num << "����ϵ�˵�סַ��";
				cin >> address;
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				//�������
				cout << "�������" << num2 - num << "����ϵ�˵ķ��ࣺ";
				cin >> sort;
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				break;
			}
			/*addNode(new Contacts(telephone, mobilephone, qq, name, unit, address, sort, email));*/
			Contacts* t;
			clist.addNode(*(t = new Contacts(telephone, mobilephone, qq, name, unit, address, sort, email)));
			delete t;
			stringstream logsContent;
			logsContent << "�½�����Ϊ[" << name << "]����ϵ��";
			logsSave(logsContent.str());
			saveNode(clist);
		}
		cout << "��ϵ�˴�����ɣ�������3����˳��ز˵�" << endl;
		Sleep(3000);
		break;
	}
	return;
}
//�����ϵ��
void Contacts::printContacts(LinkList<Contacts>& clist) {
	//�๦�ܷ�ҳ�鿴���;��忴�ҵĲ��ͣ�https://notes.smallbamboo.cn/CPPDisplayPage.html

	if (clist.getHead()->getNext() == nullptr) {
		cout << "����ͨѶ¼�������ݣ����������ϵ�ˣ�" << endl;
		Sleep(3000);
		return;
	}
	int pos = 1, page = 1, total = 0, allpage, ct = 0, numPerPage = 10;
	int input;
	LinkNode<Contacts>* temp = clist.getHead(), * count = clist.getHead();
	//ͳ�ƽڵ���
	while (count->getNext() != nullptr) {
		total++;
		count = count->getNext();
	}
	while (1) {
		//������ҳ��
		if (total % numPerPage == 0) {
			allpage = total / numPerPage;
		}
		else {
			allpage = total / numPerPage + 1;
		}
		//��Ļ���
		system("cls");
		pos = (page - 1) * numPerPage + 1;
		temp = clist.getHead();
		ct = 0;
		cout << "=============================================================================================================" << endl;
		cout << "[ " << right << setw(3) << numPerPage << "������/ҳ                               ͨѶ¼  �鿴��ϵ��                                 ��" << right << setw(3) << page << '/' << left << setw(3) << allpage << "ҳ ]" << endl;
		cout << "=============================================================================================================" << endl;
		cout << "����    �ֻ���         ������         QQ��           ����                     ��λ      ��ַ      ����  " << endl;
		while (temp->getNext() != nullptr) {
			if (++ct < pos) {
				temp = temp->getNext();
				continue;
			}
			cout << setw(8) << temp->getNext()->getData().name << setw(15) << temp->getNext()->getData().mobilephone << setw(15) << temp->getNext()->getData().telephone << setw(15) << temp->getNext()->getData().qq << setw(25) << temp->getNext()->getData().email << setw(10) << temp->getNext()->getData().unit << setw(10) << temp->getNext()->getData().address << setw(10) << temp->getNext()->getData().sort << endl;
			temp = temp->getNext();
			pos++;
			if (pos % numPerPage == 1) {
				break;
			}
		}
		cout << "=============================================================================================================" << endl;
		cout << "[     ����[0]�������˵�  ����[1]�鿴��һҳ  ����[2]�鿴��һҳ  ����[3]����ҳ����ת  ����[4]ѡ���ҳ��С     ]" << endl;
		cout << "=============================================================================================================" << endl;
		while (1) {
			if (!(cin >> input) || input < 0 || input > 4) {
				cout << "��������������������룺";
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				continue;
			}
			if (input == 0) {
				return;
			}
			else if (input == 1) {
				if (page == 1) {
					cout << "û����һҳ�ˣ����������룺";
					continue;
				}
				page--;
			}
			else if (input == 2) {
				if (page == allpage) {
					cout << "û����һҳ�ˣ����������룺";
					continue;
				}
				page++;
			}
			else if (input == 3) {
				int target;
				while (1) {
					cout << "������Ҫ��ת����ҳ�룺";
					if (!(cin >> target) || target < 1 || target > allpage) {
						cout << "ҳ�뷶Χ�������������룡" << endl;
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
						Sleep(2000);
						continue;
					}
					break;
				}
				page = target;
			}
			else if (input == 4) {
				int input;
				while (1) {
					cout << "[1]10��/ҳ  [2]20��/ҳ  [3]30��/ҳ  [4]50��/ҳ  [5]100��/ҳ\n��ѡ���µķ�ҳ��С��";
					if (!(cin >> input) || input < 1 || input > 5) {
						cout << "ҳ�뷶Χ�������������룡" << endl;
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
						Sleep(2000);
						continue;
					}
					break;
				}
				//�л���ҳ��С
				switch (input) {
				case 1:
					numPerPage = 10;
					break;
				case 2:
					numPerPage = 20;
					break;
				case 3:
					numPerPage = 30;
					break;
				case 4:
					numPerPage = 50;
					break;
				case 5:
					numPerPage = 100;
					break;
				default:
					numPerPage = 10;
					break;
				}
				page = 1;
			}
			break;
		}
	}
	return;
}
//ɾ����ϵ��
void Contacts::deleteContacts(LinkList<Contacts>& clist) {
	if (clist.getHead()->getNext() == nullptr) {
		cout << "����ͨѶ¼�������ݣ����������ϵ�ˣ�" << endl;
		Sleep(3000);
		return;
	}
	LinkNode<Contacts>* temp = clist.getHead();
	string content;
	while (1) {
		temp = clist.getHead();
		system("cls");
		cout << "==========================" << endl;
		cout << "[   ͨѶ¼  ɾ����ϵ��   ]" << endl;
		cout << "==========================" << endl;
		cout << "��������Ҫɾ������ϵ��������";
		cin >> content;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		while (temp->getNext() != nullptr) {
			if (temp->getNext()->getData().getName() == content) {
				//ֱ�Ӷ�Ҫɾ������ϵ��ʹ��deleteNode����
				clist.deleteNode(temp->getNext()->getData());
				stringstream logsContent;
				logsContent << "ɾ������ϵ��[" << content << ']';
				logsSave(logsContent.str());
				cout << "ɾ���ɹ���" << endl;
				saveNode(clist);
				Sleep(2000);
				return;
			}
			temp = temp->getNext();
		}
		cout << "�Ҳ�����Ӧ����ϵ�����������������룡" << endl;
		Sleep(2000);
		continue;
	}
	return;
}
//�޸���ϵ��
void Contacts::fixContacts(LinkList<Contacts>& clist) {
	if (clist.getHead()->getNext() == nullptr) {
		cout << "����ͨѶ¼�������ݣ����������ϵ�ˣ�" << endl;
		Sleep(3000);
		return;
	}
	LinkNode<Contacts>* temp = clist.getHead();
	string content;
	while (1) {
		temp = clist.getHead();
		system("cls");
		cout << "==========================" << endl;
		cout << "[   ͨѶ¼  �޸���ϵ��   ]" << endl;
		cout << "==========================" << endl;
		string telephone, mobilephone, qq, name, unit, address, sort, email;
		cout << "��������Ҫ�޸ĵ���ϵ��������";
		cin >> content;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		LinkNode<Contacts>* temp2;
		while (temp->getNext() != nullptr) {
			if (temp->getNext()->getData().getName() == content) {
				cout << "�ҵ����ݣ�";
				temp2 = temp->getNext();
				while (1) {
					int flag = 0;
					cout << "�밴�������޸ĺ�����ݣ�" << endl;
					//��������
					cout << "������ĺ���ϵ�˵�������";
					cin >> name;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					temp = clist.getHead();
					//��������
					while (temp->getNext() != nullptr) {
						//��������Ҫ�޸ĵ������޸ĺ���֮ǰ����ϵ������
						if (temp->getNext()->getData().getName() == name && name != content) {
							cout << "����������������������ظ������������룡" << endl;
							Sleep(2000);
							flag = 1;
							break;
						}
						temp = temp->getNext();
					}
					if (flag == 1) {
						continue;
					}
					//�����ֻ���
					cout << "������ĺ���ϵ�˵��ֻ��ţ�";
					cin >> mobilephone;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					//����������
					cout << "������ĺ���ϵ�˵������ţ�";
					cin >> telephone;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					//����QQ��
					cout << "������ĺ���ϵ�˵�QQ�ţ�";
					cin >> qq;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					//����QQ��
					cout << "������ĺ���ϵ�˵����䣺";
					cin >> email;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					//���뵥λ
					cout << "������ĺ���ϵ�˵ĵ�λ��";
					cin >> unit;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					//����סַ
					cout << "������ĺ���ϵ�˵�סַ��";
					cin >> address;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					//�������
					cout << "������ĺ���ϵ�˵ķ��ࣺ";
					cin >> sort;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					break;
				}
				//���ڴ˲���ֻ�ڴ˺����е�����һ�Σ����Բ����ǽ�����װ�ɺ���
				temp2->getData().fixInfo(telephone, mobilephone, qq, name, unit, address, sort, email);
				stringstream logsContent;
				logsContent << "�޸�����ϵ��[" << content << "]����Ϣ";
				logsSave(logsContent.str());
				cout << "�޸ĳɹ���" << endl;
				saveNode(clist);
				Sleep(2000);
				return;
			}
			temp = temp->getNext();
		}
		cout << "�Ҳ�����ϵ�ˣ�����������������" << endl;
		continue;
	}
	return;
}
//������ϵ��
void Contacts::searchContacts(LinkList<Contacts>& clist) {
	if (clist.getHead()->getNext() == nullptr) {
		cout << "����ͨѶ¼�������ݣ����������ϵ�ˣ�" << endl;
		Sleep(3000);
		return;
	}
	LinkNode<Contacts>* temp = clist.getHead();
	while (1) {
		int input;
		system("cls");
		cout << "==========================" << endl;
		cout << "[   ͨѶ¼  ������ϵ��   ]" << endl;
		cout << "==========================" << endl;
		cout << "[   [1]����    [2]QQ��   ]" << endl;
		cout << "[   [3]��λ    [4]����   ]" << endl;
		cout << "[              [0]�˳�   ]" << endl;
		cout << "==========================" << endl;
		cout << "���������ѡ����ҷ�ʽ��";
		if (!(cin >> input) || input < 0 || input > 4) {
			cout << "��������������������룡" << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			Sleep(2000);
			continue;
		}
		if (input == 0) {
			return;
		}
		else {
			string content;
			int count = 0;
			cout << "������������ݣ�";
			cin >> content;
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "������ɣ������ǲ��ҽ����" << endl;
			cout << "����    �ֻ���         ������         QQ��           ����                     ��λ      ��ַ      ����  " << endl;
			//��¼��־
			stringstream logsContent;
			if (input == 1) {
				logsContent << "��������ϵ������[" << content << ']';
			}
			else if (input == 2) {
				logsContent << "��������ϵ��QQ��[" << content << ']';
			}
			else if (input == 3) {
				logsContent << "��������ϵ�˵�λ[" << content << ']';
			}
			else if (input == 4) {
				logsContent << "��������ϵ�˷���[" << content << ']';
			}
			//���Ҷ�Ӧ����Ŀ���������
			while (temp->getNext() != nullptr) {
				if (input == 1 && temp->getNext()->getData().name == content) {
					cout << setw(8) << temp->getNext()->getData().name << setw(15) << temp->getNext()->getData().mobilephone << setw(15) << temp->getNext()->getData().telephone << setw(15) << temp->getNext()->getData().qq << setw(25) << temp->getNext()->getData().email << setw(10) << temp->getNext()->getData().unit << setw(10) << temp->getNext()->getData().address << setw(10) << temp->getNext()->getData().sort << endl;
					count++;
				}
				else if (input == 2 && temp->getNext()->getData().qq == content) {
					cout << setw(8) << temp->getNext()->getData().name << setw(15) << temp->getNext()->getData().mobilephone << setw(15) << temp->getNext()->getData().telephone << setw(15) << temp->getNext()->getData().qq << setw(25) << temp->getNext()->getData().email << setw(10) << temp->getNext()->getData().unit << setw(10) << temp->getNext()->getData().address << setw(10) << temp->getNext()->getData().sort << endl;
					count++;
				}
				else if (input == 3 && temp->getNext()->getData().unit == content) {
					cout << setw(8) << temp->getNext()->getData().name << setw(15) << temp->getNext()->getData().mobilephone << setw(15) << temp->getNext()->getData().telephone << setw(15) << temp->getNext()->getData().qq << setw(25) << temp->getNext()->getData().email << setw(10) << temp->getNext()->getData().unit << setw(10) << temp->getNext()->getData().address << setw(10) << temp->getNext()->getData().sort << endl;
					count++;
				}
				else if (input == 4 && temp->getNext()->getData().sort == content) {
					cout << setw(8) << temp->getNext()->getData().name << setw(15) << temp->getNext()->getData().mobilephone << setw(15) << temp->getNext()->getData().telephone << setw(15) << temp->getNext()->getData().qq << setw(25) << temp->getNext()->getData().email << setw(10) << temp->getNext()->getData().unit << setw(10) << temp->getNext()->getData().address << setw(10) << temp->getNext()->getData().sort << endl;
					count++;
				}
				temp = temp->getNext();
			}
			//������Ϊ0����δ��ѯ����Ϣ
			if (count == 0) {
				cout << "û�в��ҵ��κ�����" << endl;
			}
			//��־�����ѯ��������
			logsContent << "�����Ҳ��ҵ���[" << count << "]������";
			logsSave(logsContent.str());
			cout << "����0�˳������ܣ�";
			while (getchar() != '0');
			break;
		}
	}
	return;
}
void Contacts::swapNode(LinkNode<Contacts>* a, LinkNode<Contacts>* b, LinkList<Contacts>& clist) {
	LinkNode<Contacts>* temp = clist.getHead();
	//��ѭ�������ҵ�a�ڵ����һ���ڵ�
	while (temp->getNext() != a) {
		temp = temp->getNext();
	}
	//��a����һ���ڵ㣨temp��ָ��b
	temp->setNext(b);
	//��a����һ��ָ��b����һ��
	a->setNext(b->getNext());
	//��b����һ��ָ��a
	b->setNext(a);
	return;
}
//��ϵ������
void Contacts::sortContacts(LinkList<Contacts>& clist) {
	if (clist.getHead()->getNext() == nullptr) {
		cout << "����ͨѶ¼�������ݣ����������ϵ�ˣ�" << endl;
		Sleep(3000);
		return;
	}
	//ð������
	LinkNode<Contacts>* temp = clist.getHead();
	int ct = 0;
	while (temp->getNext() != nullptr) {
		ct++;
		temp = temp->getNext();
	}
	while (ct--) {
		temp = clist.getHead();
		while (temp->getNext() != nullptr && temp->getNext()->getNext() != nullptr) {
			if (temp->getNext()->getData().getName() > temp->getNext()->getNext()->getData().getName()) {
				swapNode(temp->getNext(), temp->getNext()->getNext(), clist);
			}
			temp = temp->getNext();
		}
	}
}
//��ϵ�˷���
void Contacts::shareContacts(LinkList<Contacts>& clist, LinkList<User*>& ulist) {
	if (clist.getHead()->getNext() == nullptr) {
		cout << "����ͨѶ¼�������ݣ����������ϵ�ˣ�" << endl;
		Sleep(3000);
		return;
	}
	string tmpName, tmpUsername;
	LinkNode<Contacts>* temp = clist.getHead();
	int flag = 0;
	while (1) {
		temp = clist.getHead();
		system("cls");
		cout << "==========================" << endl;
		cout << "[   ͨѶ¼  ������ϵ��   ]" << endl;
		cout << "==========================" << endl;
		cout << "������Ҫ�������ϵ��������";
		cin >> tmpName;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		while (temp->getNext() != nullptr) {
			if (temp->getNext()->getData().getName() == tmpName) {
				flag = 1;
				break;
			}
			temp = temp->getNext();
		}
		if (flag == 1) {
			break;
		}
		cout << "�Ҳ������������ϵ�ˣ����������룡" << endl;
		Sleep(2000);
		continue;
	}
	LinkNode<User*>* temp2 = ulist.getHead();
	while (1) {
		flag = 0;
		cout << "��ϵ�˻�ȡ�ɹ���������Ҫ�����Ŀ���û���";
		cin >> tmpUsername;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		//ϸ���ж�
		if (tmpUsername == currUser->getUsername()) {
			cout << "�����ܰ���ϵ�˷����Լ�������������Ŀ���û���" << endl;
			Sleep(2000);
			continue;
		}
		temp2 = ulist.getHead();
		while (temp2->getNext() != nullptr) {
			if (temp2->getNext()->getData()->getUsername() == tmpUsername) {
				//ϸ���ж�
				if (temp2->getNext()->getData()->getType() == "Root") {
					flag = 2;
					cout << "�����ܷ�����ϵ��������Ա������������Ŀ���û���" << endl;
					Sleep(2000);
					break;
				}
				ofstream saveFile("share.txt", ios::app);
				if (!saveFile.is_open()) {
					cout << "�ļ���ʧ�ܣ�" << endl;
					Sleep(2000);
					return;
				}
				//��ʽ����дĿ���û������������ǰ�û�����
				saveFile << temp2->getNext()->getData()->getUsername() << ' ';
				saveFile << currUser->getUsername() << ' ' << temp->getNext()->getData().telephone << ' ' << temp->getNext()->getData().mobilephone << ' ' << temp->getNext()->getData().qq << ' ' << temp->getNext()->getData().name << ' ' << temp->getNext()->getData().unit << ' ' << temp->getNext()->getData().address << ' ' << temp->getNext()->getData().sort << ' ' << temp->getNext()->getData().email << endl;
				cout << "����ɹ����Է���½�󼴿��յ����ѡ�" << endl;
				stringstream logsContent;
				logsContent << "����ϵ��[" << tmpName << "]�����û�[" << tmpUsername << ']';
				logsSave(logsContent.str());
				saveFile.close();
				flag = 1;
				Sleep(3000);
				break;
			}
			temp2 = temp2->getNext();
		}
		//�Ҳ�������������
		if (flag == 0) {
			cout << "�Ҳ����û��������������û�����" << endl;
			Sleep(3000);
			continue;
		}
		//���ܷ�������Ա������������
		else if (flag == 2) {
			continue;
		}
		//�˳�
		else {
			break;
		}
	}
	return;
}
//������ϵ�˷���
void Contacts::receiveShare(LinkList<Contacts>& clist) {
	ifstream readFile("share.txt");
	ofstream tempFile("temp.txt");
	//���û��share.txt���Զ�����
	if (!readFile.good()) {
		ofstream createFile("share.txt");
		if (!createFile.is_open()) {
			cout << "�޷�����share.txt�ļ���" << endl;
			Sleep(2000);
			return;
		}
		createFile.close();
		readFile.open("share.txt");
	}
	string line;
	int input;
	while (getline(readFile, line)) {
		string touser, fromuser, telephone, mobilephone, qq, name, unit, address, sort, email;
		istringstream iss(line);
		iss >> touser >> fromuser >> telephone >> mobilephone >> qq >> name >> unit >> address >> sort >> email;
		//�Ѳ��ǵ�ǰ�û��ķ����¼�浽temp.txt�ļ�
		if (currUser->getUsername() != touser) {
			tempFile << touser << ' ' << fromuser << ' ' << telephone << ' ' << mobilephone << ' ' << qq << ' ' << name << ' ' << unit << ' ' << address << ' ' << sort << ' ' << email << endl;
			continue;
		}
		//�û�ѡ���Ƿ���շ���
		stringstream logsContent;
		system("cls");
		cout << "==========================" << endl;
		cout << "[   ͨѶ¼  ������ϵ��   ]" << endl;
		cout << "==========================" << endl;
		cout << "���յ����� " << fromuser << " ����ϵ�˷���" << endl;
		cout << "����    �ֻ���         ������         QQ��           ����                     ��λ      ��ַ      ����  " << endl;
		cout << left << setw(8) << name << setw(15) << mobilephone << setw(15) << telephone << setw(15) << qq << setw(25) << email << setw(10) << unit << setw(10) << address << setw(10) << sort << endl;
		while (1) {
			cout << "��ѡ�� [1]���� �� [0]������";
			if (!(cin >> input) || input < 0 || input > 1) {
				cout << "��������������������룡" << endl;
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				Sleep(2000);
				continue;
			}
			break;
		}
		if (input == 0) {
			cout << "�����ɹ���" << endl;
			logsContent << "�����������û�[" << fromuser << "]����ϵ��[" << name << "]����";
			logsSave(logsContent.str());
			Sleep(1000);
			continue;
		}
		//����û��ظ�
		int flag = 0;
		LinkNode<Contacts>* temp = clist.getHead();
		while (temp->getNext() != nullptr) {
			if (name == temp->getNext()->getData().name) {
				cout << "��⵽��ϵ��������������ϵ���ظ����Զ�������" << endl;
				logsContent << "��������������[" << fromuser << "]����ϵ��[" << name << "]����������ϵ���ظ����Զ�����";
				logsSave(logsContent.str());
				flag = 1;
				Sleep(2000);
				break;
			}
			temp = temp->getNext();
		}
		if (flag == 1) {
			continue;
		}
		//�ѷ���������ӵ���ǰ�û�����ϵ��������
		Contacts* t;
		clist.addNode(*(t = new Contacts(telephone, mobilephone, qq, name, unit, address, sort, email)));
		delete t;
		cout << "����ɹ���" << endl;
		logsContent << "����������[" << fromuser << "]����ϵ��[" << name << "]����";
		logsSave(logsContent.str());
		Sleep(1000);
	}
	//������ɺ󣬰�temp.txt�������û������¼д��share.txt
	//����ɾ����ǰ�û����ѵ������
	readFile.close();
	tempFile.close();
	ifstream readtempFile("temp.txt");
	ofstream saveshareFile("share.txt");
	saveshareFile << readtempFile.rdbuf();
	readtempFile.close();
	saveshareFile.close();
}
void Contacts::fixInfo(string tel, string mob, string qq, string name, string unit, string address, string sort, string email) {
	this->telephone = tel;
	this->mobilephone = mob;
	this->qq = qq;
	this->name = name;
	this->unit = unit;
	this->address = address;
	this->sort = sort;
	this->email = email;
	return;
}
void Contacts::cloudFunction(LinkList<Contacts>& clist) {
	//��Vip���
	if (currUser->getType() != "Vip") {
		cout << "�Ƿ��ķ���" << endl;
	}
	int input;
	while (1) {
		system("cls");
		cout << "==========================" << endl;
		cout << "[   ͨѶ¼  ������ָ�   ]" << endl;
		cout << "==========================" << endl;
		cout << "[   [1] ���ݱ��ݵ��ƶ�   ]" << endl;
		cout << "[   [2] �ƶ�ͬ��������   ]" << endl;
		cout << "[   [0] �˳��˹���       ]" << endl;
		cout << "==========================" << endl;
		cout << "���������ѡ���ܣ�";
		if (!(cin >> input) || input < 0 || input > 2) {
			cout << "��������������������룡" << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			Sleep(2000);
			continue;
		}
		break;
	}
	//������ϣ�������û�����Ϊ�ļ�������ֹ�û������ݱ�����й¶
	string username = currUser->getUsername();
	encrype(username);
	stringstream filename;
	filename << username << ".txt";
	if (input == 1) {
		if (clist.getHead()->getNext() == nullptr) {
			cout << "��ǰ����ϵ�����ݣ������˳����ݡ�";
			Sleep(3000);
			return;
		}
		//���浱ǰ�û��������������ļ�
		ofstream saveFile(filename.str());
		LinkNode<Contacts>* temp = clist.getHead();
		while (temp->getNext() != nullptr) {
			saveFile << currUser->getUsername() << ' ' << temp->getNext()->getData().telephone << ' ' << temp->getNext()->getData().mobilephone << ' ' << temp->getNext()->getData().qq << ' ' << temp->getNext()->getData().name << ' ' << temp->getNext()->getData().unit << ' ' << temp->getNext()->getData().address << ' ' << temp->getNext()->getData().sort << ' ' << temp->getNext()->getData().email << endl;
			temp = temp->getNext();
		}
		saveFile.close();

		//��ȡ�ı��ļ�����
		ifstream file(filename.str());
		string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
		file.close();

		//����HTTP�ͻ���
		httplib::Client client("mc-sh.xzvps.top", 19060);
		//����ʽ���ݼ��б��ʼ��
		httplib::MultipartFormDataItems items = {
		{"ContatcsDataText", content, filename.str(), "text/plain"}
		};
		//����POST���󣬽��ı��ļ�������Ϊ�����巢�͸������
		auto res = client.Post("/upload", items);

		//�����Ӧ״̬��
		if (res) {
			cout << "���ݱ��ݳɹ���" << endl;
			logsSave("�����ݱ��ݵ����ƶ�");
		}
		else {
			cout << "���ݱ���ʧ�ܣ�������룺" << res.error() << endl;
		}
		//ɾ�������ı����ļ�����֤��ȫ
		remove(filename.str().c_str());
	}
	else if (input == 2) {
		//����HTTP�ͻ���
		httplib::Client client("mc-sh.xzvps.top", 19060);
		//�������ط���Ŀ¼�ַ���
		stringstream serverpath;
		serverpath << "/download/" << filename.str();
		//����GET���󣬴ӷ����������ļ�
		auto res2 = client.Get(serverpath.str());
		//�����Ӧ״̬��
		if (res2 && res2->status == 200) {
			//�����ļ����ݵ�����
			ofstream ofs(filename.str(), ios::binary);
			ofs.write(res2->body.c_str(), res2->body.length());
			ofs.close();
		}
		else {
			cout << "�����ƶ˱������ݻ��޷����ӵ���������" << endl;
			Sleep(3000);
			return;
		}
		//����յ�ǰ����
		clist.deleteAll();
		//�����ƶ˱���
		ifstream inputFile(filename.str());
		if (!inputFile.is_open()) {
			cout << "�ļ���ʧ�ܣ�" << endl;
			Sleep(3000);
			return;
		}
		//�½����ڶ�ȡÿһ�е��ַ���line
		string line;
		//��ÿһ�ж��浽line��
		while (getline(inputFile, line)) {
			string username, telephone, mobilephone, qq, name, unit, address, sort, email;
			istringstream iss(line);
			//�з�line�е�����
			iss >> username >> telephone >> mobilephone >> qq >> name >> unit >> address >> sort >> email;
			Contacts* t;
			clist.addNode(*(t = new Contacts(telephone, mobilephone, qq, name, unit, address, sort, email)));
			delete t;
		}
		inputFile.close();
		cout << "�ƶ�����ͬ���ɹ������������ѱ�����" << endl;
		saveNode(clist);
		logsSave("���ƶ�����ͬ�����˱���");
		//ɾ�����ص��ļ�����֤��ȫ
		remove(filename.str().c_str());
	}
	else {
		return;
	}
	Sleep(3000);
	return;
}

//���������
ostream& operator<< (ostream& out, Contacts& t) {
	out << setw(8) << t.name << setw(15) << t.mobilephone << setw(15) << t.telephone << setw(15) << t.qq << setw(25) << t.email << setw(10) << t.unit << setw(10) << t.address << setw(10) << t.sort;
	return out;
}
ostream& operator<< (ostream& out, User& t) {
	out << setw(7) << t.getType() << setw(8) << t.getUsername();
	return out;
}
istream& operator>> (istream& in, Contacts& t) {
	cin >> t.telephone >> t.mobilephone >> t.qq >> t.name >> t.unit >> t.address >> t.sort >> t.email;
	return in;
}
istream& operator>> (istream& in, User& t) {
	cin >> t.username >> t.password;
	return in;
}