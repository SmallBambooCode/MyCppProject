#include <iostream>
#define NOMINMAX
#include <windows.h>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <conio.h>
#include "myclass.h"
using namespace std;

//======================================================================================================================================
//contacts��ĳ�Ա����
//======================================================================================================================================

contacts::contacts(string tel, string mob, string qq, string name, string unit, string address, string sort, string email) {
	this->telephone = tel;
	this->mobilephone = mob;
	this->qq = qq;
	this->name = name;
	this->unit = unit;
	this->address = address;
	this->sort = sort;
	this->email = email;
	next = nullptr;
}
void contacts::setNext(contacts* node) {
	next = node;
}
contacts* contacts::getNext() {
	return next;
}
string contacts::getName() {
	return name;
}
void contacts::printInfo() {
	cout << setw(8) << name << setw(15) << mobilephone << setw(15) << telephone << setw(15) << qq << setw(25) << email << setw(10) << unit << setw(10) << address << setw(10) << sort << endl;
	return;
}
int contacts::searchInfo(int type, string content) {
	if (type == 1 && name == content) {
		printInfo();
		return 1;
	}
	else if (type == 2 && qq == content) {
		printInfo();
		return 1;
	}
	else if (type == 3 && unit == content) {
		printInfo();
		return 1;
	}
	else if (type == 4 && sort == content) {
		printInfo();
		return 1;
	}
	return 0;
}
void contacts::saveToFile(ofstream& saveFile, string username) {
	saveFile << username << ' ' << telephone << ' ' << mobilephone << ' ' << qq << ' ' << name << ' ' << unit << ' ' << address << ' ' << sort << ' ' << email << endl;
}
void contacts::fixInfo(string tel, string mob, string qq, string name, string unit, string address, string sort, string email) {
	this->telephone = tel;
	this->mobilephone = mob;
	this->qq = qq;
	this->name = name;
	this->unit = unit;
	this->address = address;
	this->sort = sort;
	this->email = email;
}

//======================================================================================================================================
//List��ĳ�Ա����
//======================================================================================================================================

List::List(string username) {
	this->username = username;
	head = nullptr;
	initNode();
}
List::~List() {
	saveNode();
	//ѭ��ɾ���ڵ�
	while (head != nullptr) {
		contacts* temp = head;
		head = head->getNext();
		delete temp;
	}
	head = nullptr;
}
void List::addNode(string tel, string mob, string qq, string name, string unit, string address, string sort, string email) {
	contacts* newNode = new contacts(tel, mob, qq, name, unit, address, sort, email);
	if (head == nullptr) {
		head = newNode;
		return;
	}
	contacts* temp = head;
	//β�巨��ѭ��Ѱ��ĩβ
	while (temp->getNext() != nullptr) {
		temp = temp->getNext();
	}
	temp->setNext(newNode);
}
void List::initNode() {
	ifstream inputFile("data.txt");
	if (!inputFile.is_open()) {
		cout << "�ļ���ʧ�ܣ�" << endl;
		return;
	}
	string line;
	while (getline(inputFile, line)) {
		string username, telephone, mobilephone, qq, name, unit, address, sort, email;
		//GPT�̵ģ���̫��
		istringstream iss(line);
		iss >> username >> telephone >> mobilephone >> qq >> name >> unit >> address >> sort >> email;
		//�ж�������ļ��ǲ��ǵ�ǰ�û�
		if (this->username != username) {
			continue;
		}
		addNode(telephone, mobilephone, qq, name, unit, address, sort, email);
	}
	inputFile.close();
}
void List::saveNode() {
	ifstream readFile("data.txt");
	ofstream tempFile("temp.txt");
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
		if (username != this->username) {
			tempFile << username << ' ' << telephone << ' ' << mobilephone << ' ' << qq << ' ' << name << ' ' << unit << ' ' << address << ' ' << sort << ' ' << email << endl;
		}
	}
	readFile.close();
	tempFile.close();
	//Ȼ��ѵ�ǰ�û����ݸ��ǵ�data�����ļ�
	ofstream saveFile("data.txt");
	contacts* temp = head;
	while (temp != nullptr) {
		temp->saveToFile(saveFile,username);
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
void List::printNode() {
	int pos = 1, page = 1, total=0, allpage, ct=0;
	char input;
	contacts* temp = head, *count = head;
	//ͳ�ƽڵ���
	while (count != nullptr) {
		total++;
		count = count->getNext();
	}
	//������ҳ��
	if (total % 10 == 0) {
		allpage = total / 10;
	}
	else {
		allpage = total / 10 + 1;
	}
	if (total == 0) {
		cout << "����ͨѶ¼�������ݣ����������ϵ�ˣ�" << endl;
		Sleep(3000);
		return;
	}
	while (1) {
		system("cls");
		pos = (page - 1) * 10 + 1;
		temp = head;
		ct = 0;
		cout << "=============================================================================================================" << endl;
		cout << "[                                            ͨѶ¼  �鿴��ϵ��                                   ��" << right << setw(2) << page << '/' << left << setw(2) << allpage << "ҳ ]" << endl;
		cout << "=============================================================================================================" << endl;
		cout << "����    �ֻ���         ������         QQ��           ����                     ��λ      ��ַ      ����  " << endl;
		while (temp != nullptr) {
			if (++ct<pos) {
				temp = temp->getNext();
				continue;
			}
			temp->printInfo();
			temp = temp->getNext();
			pos++;
			if (pos % 10 == 1) {
				break;
			}
		}
		cout << "=============================================================================================================" << endl;
		cout << "[                         ����[0]�������˵��� ����[1]�鿴��һҳ�� ����[2]�鿴��һҳ                         ]" << endl;
		cout << "=============================================================================================================" << endl;
		while (1) {
			cin >> input;
			cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
			if (input < '0' || input > '2') {
				cout << "��������������������룺";
				continue;
			}
			if (input == '0') {
				return;
			}
			else if (input == '1') {
				if (page == 1) {
					cout << "û����һҳ�ˣ����������룺";
					continue;
				}
				page--;
			}
			else if (input =='2') {
				if (page == allpage) {
					cout << "û����һҳ�ˣ����������룺";
					continue;
				}
				page++;
			}
			break;
		}
	}
	return;
}
void List::createContacts() {
	int num=0,num2=0;
	while(1){
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
		cout << "��������¸�ʽ����������ϵ����Ϣ\n���� �ֻ��� ������ QQ�� ���� ��λ סַ ����" << endl;
		while (num--) {
			string telephone, mobilephone, qq, name, unit, address, sort, email;
			while (1) {
				int flag = 0;
				cout << "�������" << num2 - num << "����ϵ����Ϣ��" << endl;
				cin >> name >> mobilephone >> telephone >> qq >> email >> unit >> address >> sort;
				contacts* temp = head;
				//��������
				while (temp != nullptr) {
					if (temp->getName() == name) {
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
				break;
			}
			addNode(telephone,mobilephone,qq,name,unit,address,sort,email);
		}
		cout << "��ϵ�˴�����ɣ�������3����˳��ز˵�" << endl;
		Sleep(3000);
		break;
	}
	return;
}
void List::searchContacts() {
	int ct = 0;
	contacts* temp = head;
	if (head == nullptr) {
		cout << "����ͨѶ¼�������ݣ����������ϵ�ˣ�" << endl;
		Sleep(3000);
		return;
	}
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
			cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
			Sleep(2000);
			continue;
		}
		if (input == 0) {
			return;
		}
		else {
			string content;
			cout << "������������ݣ�";
			cin >> content;
			cout << "������ɣ������ǲ��ҽ����" << endl;
			cout << "����    �ֻ���         ������         QQ��           ����                     ��λ      ��ַ      ����  " << endl;
			while (temp!=nullptr) {
				ct+=temp->searchInfo(input, content);
				temp = temp->getNext();
			}
			if (ct == 0) {
				cout << "û�в��ҵ����ݣ�" << endl;
			}
			cout << "����0�˳������ܣ�";
			while (getchar() != '0');
			break;
		}
	}
	return;
}
void List::deleteContacts() {
	contacts* temp = head;
	if (head == nullptr) {
		cout << "����ͨѶ¼�������ݣ����������ϵ�ˣ�" << endl;
		Sleep(3000);
		return;
	}
	string content;
	while (1) {
		cout << "==========================" << endl;
		cout << "[   ͨѶ¼  ɾ����ϵ��   ]" << endl;
		cout << "==========================" << endl;
		cout << "��������Ҫɾ������ϵ��������";
		cin >> content;
		if (temp->getName() == content) {
			head = head->getNext();
			delete temp;
			temp = nullptr;
			cout << "ɾ���ɹ���" << endl;
			Sleep(2000);
			return;
		}
		while (temp->getNext() != nullptr) {
			if (temp->getNext()->getName() == content) {
				contacts* q = temp->getNext();
				temp->setNext(temp->getNext()->getNext());
				delete q;
				q = nullptr;
				cout << "ɾ���ɹ���" << endl;
				Sleep(2000);
				return;
			}
			temp = temp->getNext();
		}
		cout << "ɾ��ʧ�ܣ��޷��ҵ���Ӧ����ϵ�����������������룡" << endl;
		Sleep(2000);
		continue;
	}
	return;
}
void List::fixContacts() {
	contacts* temp = head;
	if (head == nullptr) {
		cout << "����ͨѶ¼�������ݣ����������ϵ�ˣ�" << endl;
		Sleep(3000);
		return;
	}
	string content;
	while (1) {
		cout << "==========================" << endl;
		cout << "[   ͨѶ¼  �޸���ϵ��   ]" << endl;
		cout << "==========================" << endl;
		string telephone, mobilephone, qq, name, unit, address, sort, email; 
		cout << "��������Ҫ�޸ĵ���ϵ��������";
		cin >> content;
		contacts* temp2 = head;
		while (temp != nullptr) {
			if (temp->getName() == content) {
				cout << "�ҵ����ݣ�";
				while (1) {
					int flag = 0;
					cout << "�밴�ո�ʽ�����޸ĺ�����ݣ�\n���� �ֻ��� ������ QQ�� ���� ��λ סַ ����" << endl;
					cin >> name >> mobilephone >> telephone >> qq >> email >> unit >> address >> sort;
					temp2 = head;
					while (temp2 != nullptr) {
						if (temp2->getName() == name && temp2->getName() != content) {
							cout << "����������������������ظ������������룡" << endl;
							Sleep(2000);
							flag = 1;
							break;
						}
						temp2 = temp2->getNext();
					}
					if (flag == 1) {
						continue;
					}
					break;
				}
				temp->fixInfo(telephone, mobilephone, qq, name, unit, address, sort, email);
				cout << "�޸ĳɹ���" << endl;
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
void List::swapNode(contacts* x, contacts* y) {
	contacts* temp = head;
	contacts* prex = nullptr;
	contacts* lastest = y->getNext();
	//Ѱ��x��ǰһ���ڵ�
	if (x != head) {
		while (temp != nullptr) {
			if (temp->getNext() == x) {
				prex = temp;
				break;
			}
			temp = temp->getNext();
		}
	}
	if (prex == nullptr) {
		y->setNext(x);
		x->setNext(lastest);
		head = y;
	}
	else {
		y->setNext(x);
		x->setNext(lastest);
		prex->setNext(y);
	}
}
void List::sortContacts() {
	contacts* temp = head;
	int ct = 0;
	while (temp != nullptr) {
		ct++;
		temp = temp->getNext();
	}
	while (ct--) {
		temp = head;
		while (temp != nullptr && temp->getNext() != nullptr) {
			if (temp->getName() > temp->getNext()->getName()) {
				swapNode(temp, temp->getNext());
			}
			temp = temp->getNext();
		}
	}
}
//======================================================================================================================================
//User��ĳ�Ա����
//======================================================================================================================================

User::User(string usertype="User", string username = "UNKNOWN", string password = "UNKNOWN") {
	this->usertype = usertype;
	this->username = username;
	this->password = password;
	next = nullptr;
}
string User::getType() {
	return usertype;
}
User* User::getNext() {
	return next;
}
void User::setNext(User* node) {
	next = node;
}
void User::saveToFile(ofstream& saveFile) {
	saveFile << usertype << ' ' << username << ' ' << password << endl;
}
int User::check(string username, string password) {
	if (this->username == username && this->password == password) {
		return 1;
	}
	else {
		return 0;
	}
}
string User::getName() {
	return username;
}
void User::printInfo() {
	cout << setw(8) << usertype << setw(8) << username << endl;
	return;
}
void User::setType(string type) {
	this->usertype = type;
	return;
}
void User::fixPassword(string password) {
	this->password = password;
	return;
}

//======================================================================================================================================
//uList��ĳ�Ա����
//======================================================================================================================================

uList::uList() {
	currUser = nullptr;
	head = nullptr;
	initNode();
}
uList::~uList() {
	saveNode();
	//ѭ��ɾ���ڵ�
	while (head != nullptr) {
		User* temp = head;
		head = head->getNext();
		delete temp;
	}
	head = nullptr;
}
User* uList::addNode(string usertype, string username, string password) {
	User* newNode = new User(usertype, username, password);
	if (head == nullptr) {
		head = newNode;
		return head;
	}
	User* temp = head;
	//β�巨��ѭ��Ѱ��ĩβ
	while (temp->getNext() != nullptr) {
		temp = temp->getNext();
	}
	temp->setNext(newNode);
	return newNode;
}
void uList::initNode() {
	ifstream inputFile("user.txt");
	if (!inputFile.is_open()) {
		cout << "�ļ���ʧ�ܣ�" << endl;
		return;
	}
	string line;
	while (getline(inputFile, line)) {
		string usertype, username, password;
		istringstream iss(line);
		iss >> usertype >> username >> password;
		addNode(usertype, username, password);
	}
	inputFile.close();
}
void uList::saveNode() {
	ofstream saveFile("user.txt");
	if (!saveFile.is_open()) {
		cout << "�ļ���ʧ�ܣ�" << endl;
		Sleep(2000);
		return;
	}
	User* temp = head;
	while (temp != nullptr) {
		temp->saveToFile(saveFile);
		temp = temp->getNext();
	}
}
User* uList::getcurrUser() {
	return currUser;
}
int uList::signIn() {
	system("cls");
	cout << "==========================" << endl;
	cout << "[   ͨѶ¼    ͳһ��¼   ]" << endl;
	cout << "==========================" << endl;
	if (head == nullptr) {
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
		encrype(ipassword);
		User* temp = head;
		while (temp != nullptr) {
			if (temp->check(iusername, ipassword)) {
				currUser = temp;
				if (currUser->getType() == "User") {
					cout << "��½�ɹ�����ӭ����" << iusername << endl;
				}
				else if (currUser->getType() == "Vip") {
					cout << "��½�ɹ�������VIP�û���" << iusername << " ��ӭ����" << endl;
				}
				else if (currUser->getType() == "Root") {
					cout << "����Ա��½�ɹ���" << iusername << endl;
				}
				else {
					cout << "δ֪���û����ͣ���ӭ����" << iusername << endl;
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
void uList::signUp() {
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
		User* temp = head;
		int flag = 0;
		while (temp != nullptr) {
			if (temp->getName() == iusername) {
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
		currUser = addNode("User", iusername, ipassword);
		Sleep(2000);
		break;
	}
	return;
}
void uList::printNode() {
	system("cls");
	if (currUser->getType() != "Root") {
		cout << "Ȩ�޲��㣬��ֹ���ʣ������˳���" << endl;
		Sleep(3000);
		return;
	}
	cout << "========================================" << endl;
	cout << "[         ����Ա  �鿴ͨ��֤           ]" << endl;
	cout << "========================================" << endl;
	cout << "Ȩ����  �û���" << endl;
	User* temp = head;
	while (temp != nullptr) {
		temp->printInfo();
		temp = temp->getNext();
	}
	cout << "����0�˳������ܣ�";
	while (getchar() != '0');
}
void uList::fixUserType() {
	system("cls");
	if (currUser->getType() != "Root") {
		cout << "Ȩ�޲��㣬��ֹ���ʣ������˳���" << endl;
		Sleep(3000);
		return;
	}
	cout << "========================================" << endl;
	cout << "[         ����Ա  �޸�Ȩ����           ]" << endl;
	cout << "========================================" << endl;
	cout << "��ǰ���û��������£�" << endl;
	cout << "Ȩ����  �û���" << endl;
	User* temp = head;
	while (temp != nullptr) {
		temp->printInfo();
		temp = temp->getNext();
	}
	string content;
	while (1) {
		temp = head;
		cout << "��������Ҫ�޸ĵ��û���";
		cin >> content;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		while (temp != nullptr) {
			if (temp->getName() == content) {
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
					if (input == 2) {
						temp->setType("Vip");
					}
					else if (input == 3) {
						temp->setType("Root");
					}
					else {
						temp->setType("User");
					}
					cout << "�޸���ɣ������˳������޸ĵ��ǵ�ǰ�û��������µ�¼����Ч��" << endl;
					Sleep(2000);
					return;
				}
			}
			temp = temp->getNext();
		}
		cout << "�Ҳ����û��������������û�����" << endl;
		Sleep(2000);
		continue;
	}
}
void uList::fixPassword() {
	system("cls");
	cout << "========================================" << endl;
	cout << "[           ͨ��֤  �޸�����           ]" << endl;
	cout << "========================================" << endl;
	if (currUser->getType() == "Root") {
		cout << "��ǰ���û��������£�" << endl;
		cout << "Ȩ����  �û���" << endl;
		User* temp = head;
		while (temp != nullptr) {
			temp->printInfo();
			temp = temp->getNext();
		}
		string content;
		while (1) {
			temp = head;
			cout << "��������Ҫ�޸ĵ��û���";
			cin >> content;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			while (temp != nullptr) {
				if (temp->getName() == content) {
					cout << "�ҵ����ݣ��������û� " << temp->getName() << " ���µ����룺";
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
					temp->fixPassword(ipassword);
					cout << "�޸ĳɹ��������˳���" << endl;
					Sleep(2000);
					return;
				}
				temp = temp->getNext();
			}
			cout << "�Ҳ����û��������������û�����" << endl;
			continue;
		}
	}
	else {
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
		currUser->fixPassword(ipassword);
		cout << "�޸ĳɹ��������˳���" << endl;
		Sleep(2000);
		return;
	}
	return;
}
int uList::deleteUser() {
	system("cls");
	cout << "========================================" << endl;
	cout << "[           ͨ��֤  ɾ���û�           ]" << endl;
	cout << "========================================" << endl;
	if (currUser->getType() == "Root") {
		string currUsername = currUser->getName();
		cout << "��ǰ���û��������£�" << endl;
		cout << "Ȩ����  �û���" << endl;
		User* temp = head;
		while (temp != nullptr) {
			temp->printInfo();
			temp = temp->getNext();
		}
		string content;
		while (1) {
			temp = head;
			cout << "��������Ҫɾ�����û���";
			cin >> content;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			if (temp->getName() == content) {
				head = head->getNext();
				delete temp;
				temp = nullptr;
				cout << "�˻�ע���ɹ���" << endl;
				Sleep(2000);
				if (currUser->getName() == content) {
					return 1;
				}
				else {
					return 0;
				}
			}
			while (temp->getNext() != nullptr) {
				if (temp->getNext()->getName() == content) {
					User* q = temp->getNext();
					temp->setNext(temp->getNext()->getNext());
					delete q;
					q = nullptr;
					cout << "�˻�ע���ɹ���" << endl;
					Sleep(2000);
					if (currUsername == content) {
						return 1;
					}
					else {
						return 0;
					}
				}
				temp = temp->getNext();
			}
			cout << "�Ҳ����û��������������û�����" << endl;
			continue;
		}
	}
	else {
		User* temp = head;
		int input;
		cout << "���棡�����ڽ��е�ǰ�û���" << currUser->getName() << " ��ע��������" << endl;
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
				if (temp->getName() == currUser->getName()) {
					head = head->getNext();
					delete temp;
					temp = nullptr;
					cout << "�˻�ע���ɹ���" << endl;
					Sleep(2000);
					return 1;
				}
				while (temp->getNext() != nullptr) {
					if (temp->getNext()->getName() == currUser->getName()) {
						User* q = temp->getNext();
						temp->setNext(temp->getNext()->getNext());
						delete q;
						q = nullptr;
						cout << "�˻�ע���ɹ���" << endl;
						Sleep(2000);
						return 1;
					}
					temp = temp->getNext();
				}
			}
			else {
				return 0;
			}
		}
	}
}
//GPT�̵Ĺ�ϣ��������password�ַ���
void uList::encrype(string& password) {
	hash<string> hasher;
	size_t hash = hasher(password);
	password = to_string(hash);
	return;
}