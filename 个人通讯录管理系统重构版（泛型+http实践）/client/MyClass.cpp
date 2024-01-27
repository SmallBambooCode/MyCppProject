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
#include "TemplateFunc.cpp"
using namespace std;
User* currUser;
//ȫ�ֺ���
//��ϣ����
void encrype(string& str) {
	hash<string> hasher;
	size_t hash = hasher(str);
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
		<< std::setw(2) << std::setfill('0') << time.wMonth << "��"
		<< std::setw(2) << std::setfill('0') << time.wDay << "��"
		<< std::setw(2) << std::setfill('0') << time.wHour << ":"
		<< std::setw(2) << std::setfill('0') << time.wMinute << ":"
		<< std::setw(2) << std::setfill('0') << time.wSecond;
	return currTime.str();//ʹ��.str()�������ַ�����ת��Ϊ�ַ���
}
//������־
void logsSave(string content) {
	ofstream saveFile("logs.txt", ios::app);
	if (!saveFile.is_open()) {
		cout << "�ļ���ʧ�ܣ�" << endl;
		Sleep(2000);
		return;
	}
	getTime();	
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
		string time,username,content;
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
	return ;
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
	this->next = nullptr;
}
//Ϊʲô��д����������ɾ������Ϊÿdeleteһ���ڵ㣬���ᴥ��������
User::~User() { }
Root::Root(string username, string password) : User(username, password) { }
Common::Common(string username, string password) : User(username, password) { }
Vip::Vip(string username, string password) : User(username, password) { }

//����ʵ��
User* User::getNext() {
	return next;
}
void User::setNext(User* next) {
	this->next = next;
	return;
}
void User::initNode() {
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
		::addNode(this, t);
	}
	inputFile.close();
	return;
}
//void User::addNode(User* newNode) {
//	//�������addNode���Ǹ����󣨲�һ����ͷ�ڵ㣩���ڽڵ����һ����nullptr��ֱ�Ӱ��½ڵ���ں���
//	if (this->next == nullptr) {
//		this->next = newNode;
//		return;
//	}
//	//���������������β��
//	User* temp = this;
//	while (temp->getNext() != nullptr) {
//		temp = temp->getNext();
//	}
//	//β���½ڵ�
//	temp->setNext(newNode);
//	return;
//}
//void User::deleteNode(User* userHead) {
//	User* temp = userHead;
//	while (temp->getNext() != nullptr) {
//		if (temp->getNext() == this) {
//			temp->setNext(this->getNext());
//			delete this;
//			//��Ȼ˵delete this��һ�����գ�����ֻҪע���Ժ�Ҫ�Ե�ǰ�ڵ�����κβ������ɣ�Ҳ���ǵ��ó�Ա������
//			return;
//		}
//		temp = temp->getNext();
//	}
//}
void User::saveNode(User* userHead) {
	//�ڹ���ʵ������Ҫ����saveNode�����Ա���Ҫ��ͷ�ڵ�ָ�룬�����õ�ǰ�û��Ľڵ�ʹ��ˣ�
	//�½��ļ������
	ofstream saveFile("user.txt");
	if (!saveFile.is_open()) {
		cout << "�ļ���ʧ�ܣ�" << endl;
		Sleep(2000);
		return;
	}
	//��Ϊ����saveNode�Ķ���ֻ��ͷ�ڵ㣬����thisָ�����ͷ�ڵ�ָ�룬����temp���б���
	User* temp = userHead;
	while (temp->getNext() != nullptr) {
		//������������ļ�
		saveFile << temp->getNext()->getType() << ' ' << temp->getNext()->getUsername() << ' ' << temp->getNext()->getPassword() << endl;
		temp = temp->getNext();
	}
	saveFile.close();
	return;
}
//void User::deleteAll(User* userHead) {
//	User* toDelete;
//	while (userHead->getNext() != nullptr) {
//		toDelete = userHead->getNext();
//		userHead->setNext(toDelete->getNext());
//		delete toDelete;
//	}
//}
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
void User::signUp(User* userHead) {
	while (1) {
		system("cls");
		cout << "==========================" << endl;
		cout << "[   ͨѶ¼    ͳһע��   ]" << endl;
		cout << "==========================" << endl;
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
		User* temp = userHead;
		int flag = 0;
		//ע�����
		while (temp->getNext() != nullptr) {
			if (temp->getNext()->getUsername() == iusername) {
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
		cout << "ע��ɹ�����ӭ����" << iusername << endl;
		encrype(ipassword);
		//���½�Common���󣬸�ֵ��currUser���ٰ�currUser����addNodeʵ�ֽڵ����ӣ���addNode��ͬʱ����currUser��
		::addNode(this,(currUser = new Common(iusername, ipassword)));
		/*addNode(currUser = new Common(iusername, ipassword));*/
		saveNode(userHead);
		logsSave("ע��ɹ�");
		Sleep(2000);
		break;
	}
	return;
}
int User::signIn(User* userHead) {
	system("cls");
	cout << "==========================" << endl;
	cout << "[   ͨѶ¼    ͳһ��¼   ]" << endl;
	cout << "==========================" << endl;
	if (userHead->getNext() == nullptr) {
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
		User* temp = userHead;
		while (temp->getNext() != nullptr) {
			if (temp->getNext()->checkInfo(iusername, ipassword)) {
				//���µ�ǰ�û�
				currUser = temp->getNext();
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
void User::menuSelect(User* userHead) {
	currUser->enterMenu(userHead);
	return;
}
string User::getUsername() {
	return username;
}
string User::getPassword() {
	return password;
}
void User::setPassword(string newPassword) {
	this->password = newPassword;
}
void User::setType(string usertype, User* userHead) {
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
	::addNode(this, t);
	//����ɾ����ǰ�û��ڵ㣬��ǰ�û��ڵ㲻���������һ������Ϊ�ո��Ѿ��½��˽ڵ���β��
	User* temp = userHead;
	while (temp->getNext() != nullptr) {
		if (temp->getNext() == this) {
			temp->setNext(this->getNext());
			delete this;
			return;
		}
		temp = temp->getNext();
	}
}

string Root::getType() {
	return "Root";
}
string Common::getType() {
	return "Common";
}
string Vip::getType() {
	return "Vip";
}
void Root::printNode(User* userHead) {
	system("cls");
	cout << "========================================" << endl;
	cout << "[         ����Ա  �鿴ͨ��֤           ]" << endl;
	cout << "========================================" << endl;
	cout << " Ȩ����  �û���" << endl;
	User* temp = userHead;
	while (temp->getNext() != nullptr) {
		cout << setw(7) << temp->getNext()->getType() << setw(8) << temp->getNext()->getUsername() << endl;
		temp = temp->getNext();
	}
	cout << "����0�˳������ܣ�";
	while (getchar() != '0');
	return;
}
//���ڷǹ����鿴�����û���û��Ȩ�޵�
void Common::printNode(User* userHead) {
	cout << "û��Ȩ�ޣ�" << endl;
}
void Vip::printNode(User* userHead) {
	cout << "û��Ȩ�ޣ�" << endl;
}
//Menu
void Root::enterMenu(User* userHead) {
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
		cout << "[        [0]�˳���ϵͳ         ]" << endl;
		cout << "================================" << endl;
		
		cout << "���������ѡ���ܣ�";
		if (!(cin >> input) || input < 0 || input > 6) {
			cout << "��������������������룡" << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			Sleep(2000);
			continue;
		}
		switch (input) {
		case 0:
			flag = 1;
			logsSave("�˳���¼");
			break;
		case 1:
			currUser->printNode(userHead);
			//�鿴��ɺ�ͬʱ��¼��־
			logsSave("�鿴��ͨ��֤");
			break;
		case 2:
			if (currUser->fixType(userHead)) {
				flag = 1;
			}
			break;
		case 3:
			currUser->fixPassword(userHead);
			break;
		case 4:
			if (currUser->deleteUser(userHead, new Contacts)) {
				flag = 1;
			}
			break;
		case 5:
			logsReadRoot();
			logsSave("�鿴��ϵͳ��־");
			break;
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
void Common::enterMenu(User* userHead) {
	Contacts* contactsHead = new Contacts;
	contactsHead->initNode();
	contactsHead->receiveShare(contactsHead);
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
			::deleteAll(contactsHead);
			delete contactsHead;
			contactsHead = nullptr;
			logsSave("�˳���¼");
			break;
		case 1:
			contactsHead->createContacts(contactsHead);
			break;
		case 2:
			contactsHead->printContacts(contactsHead);
			//�鿴��ɺ�ͬʱ��¼��־
			logsSave("�鿴����ϵ��");
			break;
		case 3:
			contactsHead->deleteContacts(contactsHead);
			break;
		case 4:
			contactsHead->fixContacts(contactsHead);
			break;
		case 5:
			contactsHead->shareContacts(contactsHead, userHead);
			break;
		case 6:
			currUser->fixPassword(userHead);
			break;
		case 7:
			if (currUser->deleteUser(userHead, contactsHead)) {
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
void Vip::enterMenu(User* userHead) {
	Contacts* contactsHead = new Contacts;
	contactsHead->initNode();
	contactsHead->receiveShare(contactsHead);
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
		cout << "[        [0]�˳���ϵͳ         ]" << endl;
		cout << "================================" << endl;
		cout << "���������ѡ���ܣ�";
		cin >> input;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		//����VIP����̫���ˣ�������0~9������ֱ�������ַ���0~A
		switch (input) {
		case '0':
			flag = 1;
			::deleteAll(contactsHead);
			delete contactsHead;
			contactsHead = nullptr;
			logsSave("�˳���¼");
			break;
		case '1':
			contactsHead->createContacts(contactsHead);
			break;
		case '2':
			contactsHead->printContacts(contactsHead);
			logsSave("�鿴����ϵ��");
			break;
		case '3':
			contactsHead->searchContacts(contactsHead);
			break;
		case '4':
			contactsHead->deleteContacts(contactsHead);
			break;
		case '5':
			contactsHead->fixContacts(contactsHead);
			break;
		case '6':
			contactsHead->shareContacts(contactsHead, userHead);
			break;
		case '7':
			cout << "�����У����Եȣ���������ʱ���ܺķ�ʱ��ϳ���" << endl;
			contactsHead->sortContacts(contactsHead);
			contactsHead->printContacts(contactsHead);
			logsSave("ִ������ϵ������");
			break;
		case '8':
			currUser->fixPassword(userHead);
			break;
		case '9':
			if (currUser->deleteUser(userHead, contactsHead)) {
				flag = 1;
			}
			break;
		case 'A':
			logsReadClient();
			logsSave("�鿴�˲�����־");
			break;
		case 'B':
			contactsHead->cloudFunction(contactsHead);
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
int Root::deleteUser(User* userHead, Contacts* contactsHead) {
	delete contactsHead;
	system("cls");
	cout << "========================================" << endl;
	cout << "[           ͨ��֤  ɾ���û�           ]" << endl;
	cout << "========================================" << endl;
	User* temp = userHead;
	string currUsername = currUser->getUsername();
	cout << "��ǰ���û��������£�" << endl;
	cout << "Ȩ����  �û���" << endl;
	while (temp->getNext() != nullptr) {
		cout << setw(8) << temp->getNext()->getType() << setw(8) << temp->getNext()->getUsername() << endl;
		temp = temp->getNext();
	}
	string content;
	while (1) {
		temp = userHead;
		int returnValue = 0;
		cout << "��������Ҫɾ�����û�������Աɾ���û�ʱ����������û��ı������ƶ����ݣ���";
		cin >> content;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		if (currUser->getUsername() == content) {
			returnValue = 1;
		}
		else {
			returnValue = 0;
		}
		while (temp->getNext() != nullptr) {
			if (temp->getNext()->getUsername() == content) {
				if (returnValue == 1) {
					cout << "��ɾ�����ǵ�ǰ�����û��������˳���¼��" << endl;
					logsSave("ע�����Լ����˺�");
				}
				else {
					cout << endl;
					stringstream logsContent;
					logsContent << "ɾ�����û�[" << content << ']';
					logsSave(logsContent.str());
				}
				::deleteNode(temp->getNext(), userHead);
				cout << "�û�ɾ���ɹ���";
				saveNode(userHead);
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
int Common::deleteUser(User* userHead, Contacts* contactsHead) {
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
			::deleteAll(contactsHead);
			contactsHead->saveNode(contactsHead);
			logsSave("ע�����Լ����˺�");
			//ֱ�ӵ���deleteNodeɾ����ǰ�ڵ�
			::deleteNode(currUser, userHead);
			cout << "�˻�ע���ɹ���" << endl;
			saveNode(userHead);
			Sleep(2000);
			return 1;
		}
		else {
			return 0;
		}
	}
}
int Vip::deleteUser(User* userHead, Contacts* contactsHead) {
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
			httplib::Client client("example.com", 19060);
			stringstream serverpath;
			serverpath << "/remove/" << filename.str();
			//����GET����ɾ���������е��ļ�
			auto res = client.Get(serverpath.str());
			//ֱ�ӵ���deleteAllɾ���û�����
			::deleteAll(contactsHead);
			contactsHead->saveNode(contactsHead);
			logsSave("ע�����Լ����˺�");
			//ֱ�ӵ���deleteNodeɾ����ǰ�ڵ�
			::deleteNode(currUser, userHead);
			cout << "�˻�ע���ɹ���" << endl;
			saveNode(userHead);
			Sleep(2000);
			return 1;
		}
		else {
			return 0;
		}
	}
}
int Root::fixType(User* userHead) {
	system("cls");
	cout << "========================================" << endl;
	cout << "[         ����Ա  �޸�Ȩ����           ]" << endl;
	cout << "========================================" << endl;
	cout << "��ǰ���û��������£�" << endl;
	cout << "Ȩ����  �û���" << endl;
	User* temp = userHead;
	while (temp->getNext() != nullptr) {
		cout << setw(8) << temp->getNext()->getType() << setw(8) << temp->getNext()->getUsername() << endl;
		temp = temp->getNext();
	}
	string content;
	while (1) {
		temp = userHead;
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
			if (temp->getNext()->getUsername() == content) {
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
						temp->getNext()->setType("Vip", userHead);
					}
					else if (input == 3) {
						temp->getNext()->setType("Root", userHead);
					}
					else {
						temp->getNext()->setType("Common", userHead);
					}
					cout << "�޸���ɣ�";
					saveNode(userHead);
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
int Common::fixType(User* userHead) {
	cout << "û��Ȩ�ޣ�" << endl;
	return 0;
}
int Vip::fixType(User* userHead) {
	cout << "û��Ȩ�ޣ�" << endl;
	return 0;
}
void Root::fixPassword(User* userHead) {
	system("cls");
	cout << "========================================" << endl;
	cout << "[           ͨ��֤  �޸�����           ]" << endl;
	cout << "========================================" << endl;
	cout << "��ǰ���û��������£�" << endl;
	cout << "Ȩ����  �û���" << endl;
	User* temp = userHead;
	while (temp->getNext() != nullptr) {
		cout << setw(8) << temp->getNext()->getType() << setw(8) << temp->getNext()->getUsername() << endl;
		temp = temp->getNext();
	}
	string content;
	while (1) {
		temp = userHead;
		cout << "��������Ҫ�޸ĵ��û���";
		cin >> content;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		while (temp->getNext() != nullptr) {
			if (temp->getNext()->getUsername() == content) {
				cout << "�ҵ����ݣ��������û� " << content << " ���µ����룺";
				//��������
				string ipassword;
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
				encrype(ipassword);
				//��������
				temp->getNext()->setPassword(ipassword);
				cout << "�޸ĳɹ���" << endl;
				stringstream logsContent;
				logsContent << "�޸����û�[" << content << "]������";
				logsSave(logsContent.str());
				saveNode(userHead);
				Sleep(2000);
				return;
			}
			temp = temp->getNext();
		}
		cout << "�Ҳ����û��������������û�����" << endl;
		continue;
	}
}
void Common::fixPassword(User* userHead) {
	system("cls");
	cout << "========================================" << endl;
	cout << "[           ͨ��֤  �޸�����           ]" << endl;
	cout << "========================================" << endl;
	string ipassword;
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
	encrype(ipassword);
	currUser->setPassword(ipassword);
	cout << "�޸ĳɹ��������˳���" << endl;
	logsSave("�޸����Լ�������");
	saveNode(userHead);
	Sleep(2000);
	return;
}
void Vip::fixPassword(User* userHead) {
	system("cls");
	cout << "========================================" << endl;
	cout << "[           ͨ��֤  �޸�����           ]" << endl;
	cout << "========================================" << endl;
	string ipassword;
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
	encrype(ipassword);
	currUser->setPassword(ipassword);
	cout << "�޸ĳɹ��������˳���" << endl;
	logsSave("�޸����Լ�������");
	saveNode(userHead);
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
	this->next = nullptr;
}
//Ϊʲô��д����������ɾ������Ϊÿdeleteһ���ڵ㣬���ᴥ��������
//�������������в����������޷�����������ɾ���ڵ㣬��Ϊ��Ҫ����ͷ�ڵ㡣
Contacts::~Contacts() { }
//����ʵ��
Contacts* Contacts::getNext() {
	return next;
}
void Contacts::setNext(Contacts* next) {
	this->next = next;
	return;
}
void Contacts::initNode() {
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
		::addNode(this, (new Contacts(telephone, mobilephone, qq, name, unit, address, sort, email)));
	}
	inputFile.close();
	return;
}
//void Contacts::addNode(Contacts* newNode) {
//	//�������addNode���Ǹ����󣨲�һ����ͷ�ڵ㣩���ڽڵ����һ����nullptr��ֱ�Ӱ��½ڵ���ں���
//	if (this->next == nullptr) {
//		this->next = newNode;
//		return;
//	}
//	//���������������β��
//	Contacts* temp = this;
//	while (temp->getNext() != nullptr) {
//		temp = temp->getNext();
//	}
//	//β���½ڵ�
//	temp->setNext(newNode);
//	return;
//}
//void Contacts::deleteNode(Contacts* contactsHead) {
//	//˭���ã�ɾ��˭��
//	Contacts* temp = contactsHead;
//	while (temp != nullptr && temp->getNext() != nullptr) {
//		if (temp->getNext() == this) {
//			temp->setNext(this->getNext());
//			delete this;
//			//��Ȼ˵delete this��һ�����գ�����ֻҪע���Ժ�Ҫ�Ե�ǰ�ڵ�����κβ������ɣ�Ҳ���ǵ��ó�Ա������
//			return;
//		}
//		temp = temp->getNext();
//	}
//}
//void Contacts::deleteAll(Contacts* contactsHead) {
//	Contacts* toDelete;
//	while (contactsHead->getNext() != nullptr) {
//		toDelete = contactsHead->getNext();
//		contactsHead->setNext(toDelete->getNext());
//		delete toDelete;
//	}
//}
void Contacts::saveNode(Contacts* contactsHead) {
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
	Contacts* temp = contactsHead;
	while (temp->getNext() != nullptr) {
		saveFile << currUser->getUsername() << ' ' << temp->getNext()->telephone << ' ' << temp->getNext()->mobilephone << ' ' << temp->getNext()->qq << ' ' << temp->getNext()->name << ' ' << temp->getNext()->unit << ' ' << temp->getNext()->address << ' ' << temp->getNext()->sort << ' ' << temp->getNext()->email << endl;
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
string Contacts::getName() {
	return this->name;
}
void Contacts::createContacts(Contacts* contactsHead) {
	//num2���ڱ���num����ʱnum�仯���ֵ
	int num = 0, num2 = 0;
	Contacts* temp = contactsHead;
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
				temp = contactsHead;
				//��������
				while (temp->getNext() != nullptr) {
					if (temp->getNext()->getName() == name) {
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
			::addNode(this,(new Contacts(telephone, mobilephone, qq, name, unit, address, sort, email)));
			stringstream logsContent;
			logsContent << "�½�����Ϊ[" << name << "]����ϵ��";
			logsSave(logsContent.str());
			saveNode(contactsHead);
		}
		cout << "��ϵ�˴�����ɣ�������3����˳��ز˵�" << endl;
		Sleep(3000);
		break;
	}
	return;
}
void Contacts::printContacts(Contacts* contactsHead) {
	//�๦�ܷ�ҳ�鿴���;��忴�ҵĲ��ͣ�https://notes.smallbamboo.cn/CPPDisplayPage.html

	if (contactsHead->getNext() == nullptr) {
		cout << "����ͨѶ¼�������ݣ����������ϵ�ˣ�" << endl;
		Sleep(3000);
		return;
	}
	int pos = 1, page = 1, total = 0, allpage, ct = 0, numPerPage = 10;
	int input;
	Contacts* temp = contactsHead, *count = contactsHead;
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
		temp = contactsHead;
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
			cout << setw(8) << temp->getNext()->name << setw(15) << temp->getNext()->mobilephone << setw(15) << temp->getNext()->telephone << setw(15) << temp->getNext()->qq << setw(25) << temp->getNext()->email << setw(10) << temp->getNext()->unit << setw(10) << temp->getNext()->address << setw(10) << temp->getNext()->sort << endl;
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
void Contacts::deleteContacts(Contacts* contactsHead) {
	if (contactsHead->getNext() == nullptr) {
		cout << "����ͨѶ¼�������ݣ����������ϵ�ˣ�" << endl;
		Sleep(3000);
		return;
	}
	Contacts* temp = contactsHead;
	string content;
	while (1) {
		temp = contactsHead;
		system("cls");
		cout << "==========================" << endl;
		cout << "[   ͨѶ¼  ɾ����ϵ��   ]" << endl;
		cout << "==========================" << endl;
		cout << "��������Ҫɾ������ϵ��������";
		cin >> content;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		while (temp->getNext() != nullptr) {
			if (temp->getNext()->getName() == content) {
				//ֱ�Ӷ�Ҫɾ������ϵ��ʹ��deleteNode����
				::deleteNode(temp->getNext(), contactsHead);
				stringstream logsContent;
				logsContent << "ɾ������ϵ��[" << content << ']';
				logsSave(logsContent.str());
				cout << "ɾ���ɹ���" << endl;
				saveNode(contactsHead);
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
void Contacts::fixContacts(Contacts* contactsHead) {
	if (contactsHead->getNext() == nullptr) {
		cout << "����ͨѶ¼�������ݣ����������ϵ�ˣ�" << endl;
		Sleep(3000);
		return;
	}
	Contacts* temp = contactsHead;
	string content;
	while (1) {
		temp = contactsHead;
		system("cls");
		cout << "==========================" << endl;
		cout << "[   ͨѶ¼  �޸���ϵ��   ]" << endl;
		cout << "==========================" << endl;
		string telephone, mobilephone, qq, name, unit, address, sort, email;
		cout << "��������Ҫ�޸ĵ���ϵ��������";
		cin >> content;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		Contacts* temp2;
		while (temp->getNext() != nullptr) {
			if (temp->getNext()->getName() == content) {
				cout << "�ҵ����ݣ�";
				temp2 = temp->getNext();
				while (1) {
					int flag = 0;
					cout << "�밴�������޸ĺ�����ݣ�" << endl;
					//��������
					cout << "������ĺ���ϵ�˵�������";
					cin >> name;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					temp = contactsHead;
					//��������
					while (temp->getNext() != nullptr) {
						//��������Ҫ�޸ĵ������޸ĺ���֮ǰ����ϵ������
						if (temp->getNext()->getName() == name && name != content) {
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
				temp2->fixInfo(telephone, mobilephone, qq, name, unit, address, sort, email);
				stringstream logsContent;
				logsContent << "�޸�����ϵ��[" << content << "]����Ϣ";
				logsSave(logsContent.str());
				cout << "�޸ĳɹ���" << endl;
				saveNode(contactsHead);
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
void Contacts::searchContacts(Contacts* contactsHead) {
	if (contactsHead->getNext() == nullptr) {
		cout << "����ͨѶ¼�������ݣ����������ϵ�ˣ�" << endl;
		Sleep(3000);
		return;
	}
	Contacts* temp = contactsHead;
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
			while (temp->getNext() != nullptr) {
				if (input == 1 && temp->getNext()->name == content) {
					cout << setw(8) << temp->getNext()->name << setw(15) << temp->getNext()->mobilephone << setw(15) << temp->getNext()->telephone << setw(15) << temp->getNext()->qq << setw(25) << temp->getNext()->email << setw(10) << temp->getNext()->unit << setw(10) << temp->getNext()->address << setw(10) << temp->getNext()->sort << endl;
					count++;
				}
				else if (input == 2 && temp->getNext()->qq == content) {
					cout << setw(8) << temp->getNext()->name << setw(15) << temp->getNext()->mobilephone << setw(15) << temp->getNext()->telephone << setw(15) << temp->getNext()->qq << setw(25) << temp->getNext()->email << setw(10) << temp->getNext()->unit << setw(10) << temp->getNext()->address << setw(10) << temp->getNext()->sort << endl;
					count++;
				}
				else if (input == 3 && temp->getNext()->unit == content) {
					cout << setw(8) << temp->getNext()->name << setw(15) << temp->getNext()->mobilephone << setw(15) << temp->getNext()->telephone << setw(15) << temp->getNext()->qq << setw(25) << temp->getNext()->email << setw(10) << temp->getNext()->unit << setw(10) << temp->getNext()->address << setw(10) << temp->getNext()->sort << endl;
					count++;
				}
				else if (input == 4 && temp->getNext()->sort == content) {
					cout << setw(8) << temp->getNext()->name << setw(15) << temp->getNext()->mobilephone << setw(15) << temp->getNext()->telephone << setw(15) << temp->getNext()->qq << setw(25) << temp->getNext()->email << setw(10) << temp->getNext()->unit << setw(10) << temp->getNext()->address << setw(10) << temp->getNext()->sort << endl;
					count++;
				}
				temp = temp->getNext();
			}
			if (count == 0) {
				cout << "û�в��ҵ��κ�����" << endl;
			}
			logsContent << "�����Ҳ��ҵ���[" << count << "]������";
			logsSave(logsContent.str());
			cout << "����0�˳������ܣ�";
			while (getchar() != '0');
			break;
		}
	}
	return;
}
//�ڵ㽻������
void Contacts::swapNode(Contacts* a, Contacts* b, Contacts* contactsHead) {
	Contacts* temp = contactsHead;
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
}
void Contacts::sortContacts(Contacts* contactsHead) {
	if (contactsHead->getNext() == nullptr) {
		cout << "����ͨѶ¼�������ݣ����������ϵ�ˣ�" << endl;
		Sleep(3000);
		return;
	}
	//ð������
	Contacts* temp = contactsHead;
	int ct = 0;
	while (temp->getNext() != nullptr) {
		ct++;
		temp = temp->getNext();
	}
	while (ct--) {
		temp = contactsHead;
		while (temp->getNext() != nullptr && temp->getNext()->getNext() != nullptr) {
			if (temp->getNext()->getName() > temp->getNext()->getNext()->getName()) {
				swapNode(temp->getNext(), temp->getNext()->getNext(), contactsHead);
			}
			temp = temp->getNext();
		}
	}
}
void Contacts::shareContacts(Contacts* contactsHead, User* userHead) {
	if (contactsHead->getNext() == nullptr) {
		cout << "����ͨѶ¼�������ݣ����������ϵ�ˣ�" << endl;
		Sleep(3000);
		return;
	}
	string tmpName, tmpUsername;
	Contacts* temp = contactsHead;
	int flag = 0;
	while (1) {
		temp = contactsHead;
		system("cls");
		cout << "==========================" << endl;
		cout << "[   ͨѶ¼  ������ϵ��   ]" << endl;
		cout << "==========================" << endl;
		cout << "������Ҫ�������ϵ��������";
		cin >> tmpName;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		while (temp->getNext() != nullptr) {
			if (temp->getNext()->getName() == tmpName) {
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
	flag = 0;
	User* temp2 = userHead;
	while (1) {
		cout << "��ϵ�˻�ȡ�ɹ���������Ҫ�����Ŀ���û���";
		cin >> tmpUsername;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		//ϸ���ж�
		if (tmpUsername == currUser->getUsername()) {
			cout << "�����ܰ���ϵ�˷����Լ�������������Ŀ���û���" << endl;
			Sleep(2000);
			continue;
		}
		temp2 = userHead;
		while (temp2->getNext() != nullptr) {
			if (temp2->getNext()->getUsername() == tmpUsername) {
				//ϸ���ж�
				if (temp2->getNext()->getType() == "Root") {
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
				saveFile << temp2->getNext()->getUsername() << ' ';
				saveFile << currUser->getUsername() << ' ' << temp->getNext()->telephone << ' ' << temp->getNext()->mobilephone << ' ' << temp->getNext()->qq << ' ' << temp->getNext()->name << ' ' << temp->getNext()->unit << ' ' << temp->getNext()->address << ' ' << temp->getNext()->sort << ' ' << temp->getNext()->email << endl;
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
		if (flag == 0) {
			cout << "�Ҳ����û��������������û�����" << endl;
			Sleep(3000);
			continue;
		}
		else if (flag == 2) {
			continue;
		}
		else {
			break;
		}
	}
	return;
}
void Contacts::receiveShare(Contacts* contactsHead) {
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
		Contacts* temp = contactsHead;
		while (temp->getNext() != nullptr) {
			if (name == temp->getNext()->name) {
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
		::addNode(this, (new Contacts(telephone, mobilephone, qq, name, unit, address, sort, email)));
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
void Contacts::cloudFunction(Contacts* contactsHead) {
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
	string username = currUser->getUsername();
	encrype(username);
	stringstream filename;
	filename << username << ".txt";
	if (input == 1) {
		if (contactsHead->getNext() == nullptr) {
			cout << "��ǰ����ϵ�����ݣ������˳����ݡ�";
			Sleep(3000);
			return;
		}
		//���浱ǰ�û�����
		ofstream saveFile(filename.str());
		Contacts* temp = contactsHead;
		while (temp->getNext() != nullptr) {
			saveFile << currUser->getUsername() << ' ' << temp->getNext()->telephone << ' ' << temp->getNext()->mobilephone << ' ' << temp->getNext()->qq << ' ' << temp->getNext()->name << ' ' << temp->getNext()->unit << ' ' << temp->getNext()->address << ' ' << temp->getNext()->sort << ' ' << temp->getNext()->email << endl;
			temp = temp->getNext();
		}
		saveFile.close();

		//��ȡ�ı��ļ�����
		ifstream file(filename.str());
		string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
		file.close();

		//����HTTP�ͻ���
		httplib::Client client("example.com", 19060);
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
		//ɾ���������ļ�����֤��ȫ
		remove(filename.str().c_str());
	}
	else if (input == 2) {
		//����HTTP�ͻ���
		httplib::Client client("example.com", 19060);
		stringstream serverpath;
		serverpath << "/download/" << filename.str();
		//����GET���󣬴ӷ����������ļ�
		auto res2 = client.Get(serverpath.str());
		// �����Ӧ״̬��
		if (res2 && res2->status == 200) {
			// �����ļ����ݵ�����
			ofstream ofs(filename.str(), ios::binary);
			ofs.write(res2->body.c_str(), res2->body.length());
			ofs.close();
		}
		else {
			cout << "�����ƶ˱������ݡ�" << endl;
			Sleep(3000);
			return;
		}
		//����յ�ǰ����
		::deleteAll(contactsHead);
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
			::addNode(this, (new Contacts(telephone, mobilephone, qq, name, unit, address, sort, email)));
		}
		inputFile.close();
		cout << "�ƶ�����ͬ���ɹ������������ѱ�����" << endl;
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