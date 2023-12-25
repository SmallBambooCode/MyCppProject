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
//contacts类的成员函数
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
//List类的成员函数
//======================================================================================================================================

List::List(string username) {
	this->username = username;
	head = nullptr;
	initNode();
}
List::~List() {
	saveNode();
	//循环删除节点
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
	//尾插法，循环寻找末尾
	while (temp->getNext() != nullptr) {
		temp = temp->getNext();
	}
	temp->setNext(newNode);
}
void List::initNode() {
	ifstream inputFile("data.txt");
	if (!inputFile.is_open()) {
		cout << "文件打开失败！" << endl;
		return;
	}
	string line;
	while (getline(inputFile, line)) {
		string username, telephone, mobilephone, qq, name, unit, address, sort, email;
		//GPT教的，不太懂
		istringstream iss(line);
		iss >> username >> telephone >> mobilephone >> qq >> name >> unit >> address >> sort >> email;
		//判断载入的文件是不是当前用户
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
			cout << "无法创建data.txt文件！" << endl;
			Sleep(2000);
			return;
		}
		createFile.close();
		readFile.open("data.txt");
	}
	
	//先把非当前用户数据导入temp缓存中
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
	//然后把当前用户数据覆盖到data数据文件
	ofstream saveFile("data.txt");
	contacts* temp = head;
	while (temp != nullptr) {
		temp->saveToFile(saveFile,username);
		temp = temp->getNext();
	}
	saveFile.close();
	//最后把temp缓存中的数据追加到data数据文件中
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
	//统计节点数
	while (count != nullptr) {
		total++;
		count = count->getNext();
	}
	//计算总页数
	if (total % 10 == 0) {
		allpage = total / 10;
	}
	else {
		allpage = total / 10 + 1;
	}
	if (total == 0) {
		cout << "您的通讯录中无数据，请先添加联系人！" << endl;
		Sleep(3000);
		return;
	}
	while (1) {
		system("cls");
		pos = (page - 1) * 10 + 1;
		temp = head;
		ct = 0;
		cout << "=============================================================================================================" << endl;
		cout << "[                                            通讯录  查看联系人                                   第" << right << setw(2) << page << '/' << left << setw(2) << allpage << "页 ]" << endl;
		cout << "=============================================================================================================" << endl;
		cout << "姓名    手机号         座机号         QQ号           邮箱                     单位      地址      分类  " << endl;
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
		cout << "[                         输入[0]返回至菜单， 输入[1]查看上一页， 输入[2]查看下一页                         ]" << endl;
		cout << "=============================================================================================================" << endl;
		while (1) {
			cin >> input;
			cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
			if (input < '0' || input > '2') {
				cout << "数据输入错误，请重新输入：";
				continue;
			}
			if (input == '0') {
				return;
			}
			else if (input == '1') {
				if (page == 1) {
					cout << "没有上一页了，请重新输入：";
					continue;
				}
				page--;
			}
			else if (input =='2') {
				if (page == allpage) {
					cout << "没有下一页了，请重新输入：";
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
		cout << "[   通讯录  创建联系人   ]" << endl;
		cout << "==========================" << endl;
		cout << "请输入联系人创建数量(1-100)：";
		if (!(cin >> num) || num <= 0 || num > 100) {
			cout << "数据输入错误，请重新输入！" << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			Sleep(2000);
			continue;
		}
		num2 = num;
		cout << "请根据以下格式依次输入联系人信息\n姓名 手机号 座机号 QQ号 邮箱 单位 住址 分类" << endl;
		while (num--) {
			string telephone, mobilephone, qq, name, unit, address, sort, email;
			while (1) {
				int flag = 0;
				cout << "请输入第" << num2 - num << "个联系人信息：" << endl;
				cin >> name >> mobilephone >> telephone >> qq >> email >> unit >> address >> sort;
				contacts* temp = head;
				//姓名查重
				while (temp != nullptr) {
					if (temp->getName() == name) {
						cout << "您输入的姓名与已有姓名重复，请重新输入！" << endl;
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
		cout << "联系人创建完成！程序将在3秒后退出回菜单" << endl;
		Sleep(3000);
		break;
	}
	return;
}
void List::searchContacts() {
	int ct = 0;
	contacts* temp = head;
	if (head == nullptr) {
		cout << "您的通讯录中无数据，请先添加联系人！" << endl;
		Sleep(3000);
		return;
	}
	while (1) {
		int input;
		system("cls");
		cout << "==========================" << endl;
		cout << "[   通讯录  查找联系人   ]" << endl;
		cout << "==========================" << endl;
		cout << "[   [1]姓名    [2]QQ号   ]" << endl;
		cout << "[   [3]单位    [4]分类   ]" << endl;
		cout << "[              [0]退出   ]" << endl;
		cout << "==========================" << endl;
		cout << "请输入序号选择查找方式：";
		if (!(cin >> input) || input < 0 || input > 4) {
			cout << "数据输入错误，请重新输入！" << endl;
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
			cout << "请输入查找内容：";
			cin >> content;
			cout << "查找完成，以下是查找结果：" << endl;
			cout << "姓名    手机号         座机号         QQ号           邮箱                     单位      地址      分类  " << endl;
			while (temp!=nullptr) {
				ct+=temp->searchInfo(input, content);
				temp = temp->getNext();
			}
			if (ct == 0) {
				cout << "没有查找到数据！" << endl;
			}
			cout << "输入0退出本功能：";
			while (getchar() != '0');
			break;
		}
	}
	return;
}
void List::deleteContacts() {
	contacts* temp = head;
	if (head == nullptr) {
		cout << "您的通讯录中无数据，请先添加联系人！" << endl;
		Sleep(3000);
		return;
	}
	string content;
	while (1) {
		cout << "==========================" << endl;
		cout << "[   通讯录  删除联系人   ]" << endl;
		cout << "==========================" << endl;
		cout << "请输入需要删除的联系人姓名：";
		cin >> content;
		if (temp->getName() == content) {
			head = head->getNext();
			delete temp;
			temp = nullptr;
			cout << "删除成功！" << endl;
			Sleep(2000);
			return;
		}
		while (temp->getNext() != nullptr) {
			if (temp->getNext()->getName() == content) {
				contacts* q = temp->getNext();
				temp->setNext(temp->getNext()->getNext());
				delete q;
				q = nullptr;
				cout << "删除成功！" << endl;
				Sleep(2000);
				return;
			}
			temp = temp->getNext();
		}
		cout << "删除失败，无法找到对应的联系人姓名，请重新输入！" << endl;
		Sleep(2000);
		continue;
	}
	return;
}
void List::fixContacts() {
	contacts* temp = head;
	if (head == nullptr) {
		cout << "您的通讯录中无数据，请先添加联系人！" << endl;
		Sleep(3000);
		return;
	}
	string content;
	while (1) {
		cout << "==========================" << endl;
		cout << "[   通讯录  修改联系人   ]" << endl;
		cout << "==========================" << endl;
		string telephone, mobilephone, qq, name, unit, address, sort, email; 
		cout << "请输入需要修改的联系人姓名：";
		cin >> content;
		contacts* temp2 = head;
		while (temp != nullptr) {
			if (temp->getName() == content) {
				cout << "找到数据，";
				while (1) {
					int flag = 0;
					cout << "请按照格式输入修改后的数据：\n姓名 手机号 座机号 QQ号 邮箱 单位 住址 分类" << endl;
					cin >> name >> mobilephone >> telephone >> qq >> email >> unit >> address >> sort;
					temp2 = head;
					while (temp2 != nullptr) {
						if (temp2->getName() == name && temp2->getName() != content) {
							cout << "您输入的姓名与已有姓名重复，请重新输入！" << endl;
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
				cout << "修改成功！" << endl;
				Sleep(2000);
				return;
			}
			temp = temp->getNext();
		}
		cout << "找不到联系人，请重新输入姓名！" << endl;
		continue;
	}
	return;
}
void List::swapNode(contacts* x, contacts* y) {
	contacts* temp = head;
	contacts* prex = nullptr;
	contacts* lastest = y->getNext();
	//寻找x的前一个节点
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
//User类的成员函数
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
//uList类的成员函数
//======================================================================================================================================

uList::uList() {
	currUser = nullptr;
	head = nullptr;
	initNode();
}
uList::~uList() {
	saveNode();
	//循环删除节点
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
	//尾插法，循环寻找末尾
	while (temp->getNext() != nullptr) {
		temp = temp->getNext();
	}
	temp->setNext(newNode);
	return newNode;
}
void uList::initNode() {
	ifstream inputFile("user.txt");
	if (!inputFile.is_open()) {
		cout << "文件打开失败！" << endl;
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
		cout << "文件打开失败！" << endl;
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
	cout << "[   通讯录    统一登录   ]" << endl;
	cout << "==========================" << endl;
	if (head == nullptr) {
		cout << "系统中没有用户，请先注册！" << endl;
		Sleep(2000);
		return 2;
	}
	for (int i = 4; i >= 0; i--) {
		string iusername, ipassword;
		cout << "请输入用户名：";
		int inputCount = 0;
		char usrChar;
		while (1) {
			usrChar = _getch();
			if (usrChar == 13) {
				//循环条件：输入的字符不等于回车
				cout << endl;
				break;
			}
			if (usrChar == 8 && inputCount > 0) {
				//回删功能
				cout << "\b \b";
				iusername.pop_back();//移除末尾字符
				inputCount--;
			}
			else if (usrChar != 8 && usrChar != 32 && inputCount < 20) {
				//正常显示用户名
				cout << usrChar;
				iusername.push_back(usrChar);
				inputCount++;
			}
		}
		//密码输入部分
		cout << "请输入密码：";
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
				//密显功能 
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
					cout << "登陆成功！欢迎您：" << iusername << endl;
				}
				else if (currUser->getType() == "Vip") {
					cout << "登陆成功！尊贵的VIP用户：" << iusername << " 欢迎您！" << endl;
				}
				else if (currUser->getType() == "Root") {
					cout << "管理员登陆成功：" << iusername << endl;
				}
				else {
					cout << "未知的用户类型，欢迎您：" << iusername << endl;
				}
				Sleep(2000);
				return 1;
			}
			temp = temp->getNext();
		}
		cout << "用户名或密码错误，你还有" << i << "次机会！" << endl;
	}
	cout << "登陆失败！" << endl;
	return 0;
}
void uList::signUp() {
	while (1) {
		system("cls");
		cout << "==========================" << endl;
		cout << "[   通讯录    统一注册   ]" << endl;
		cout << "==========================" << endl;
		string iusername, ipassword;
		cout << "请输入用户名：";
		int inputCount = 0;
		char usrChar;
		while (1) {
			usrChar = _getch();
			if (usrChar == 13) {
				//循环条件：输入的字符不等于回车
				cout << endl;
				break;
			}
			if (usrChar == 8 && inputCount > 0) {
				//回删功能
				cout << "\b \b";
				iusername.pop_back();//移除末尾字符
				inputCount--;
			}
			else if (usrChar != 8 && usrChar != 32 && inputCount < 20) {
				//正常显示用户名
				cout << usrChar;
				iusername.push_back(usrChar);
				inputCount++;
			}
		}
		//密码输入部分
		cout << "请输入密码：";
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
				//密显功能 
				cout << "*";
				ipassword.push_back(pwdChar);
				inputCount++;
			}
		}
		User* temp = head;
		int flag = 0;
		while (temp != nullptr) {
			if (temp->getName() == iusername) {
				cout << "注册失败，您输入的用户名已存在~，请重新注册。" << endl;
				Sleep(2000);
				flag = 1;
				break;
			}
			temp = temp->getNext();
		}
		if (flag == 1) {
			continue;
		}
		cout << "注册成功！欢迎您：" << iusername << endl;
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
		cout << "权限不足，禁止访问，即将退出！" << endl;
		Sleep(3000);
		return;
	}
	cout << "========================================" << endl;
	cout << "[         管理员  查看通行证           ]" << endl;
	cout << "========================================" << endl;
	cout << "权限组  用户名" << endl;
	User* temp = head;
	while (temp != nullptr) {
		temp->printInfo();
		temp = temp->getNext();
	}
	cout << "输入0退出本功能：";
	while (getchar() != '0');
}
void uList::fixUserType() {
	system("cls");
	if (currUser->getType() != "Root") {
		cout << "权限不足，禁止访问，即将退出！" << endl;
		Sleep(3000);
		return;
	}
	cout << "========================================" << endl;
	cout << "[         管理员  修改权限组           ]" << endl;
	cout << "========================================" << endl;
	cout << "当前的用户数据如下：" << endl;
	cout << "权限组  用户名" << endl;
	User* temp = head;
	while (temp != nullptr) {
		temp->printInfo();
		temp = temp->getNext();
	}
	string content;
	while (1) {
		temp = head;
		cout << "请输入您要修改的用户：";
		cin >> content;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		while (temp != nullptr) {
			if (temp->getName() == content) {
				int input;
				cout << "找到数据，" << endl;
				cout << "[ [1]普通用户  [2]VIP用户  [3]管理员 ]" << endl;
				while (1) {
					cout << "请选择要修改为的权限组：";
					if (!(cin >> input) || input < 0 || input > 3) {
						cout << "数据输入错误，请重新输入！" << endl;
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
					cout << "修改完成，即将退出！若修改的是当前用户，则重新登录后生效。" << endl;
					Sleep(2000);
					return;
				}
			}
			temp = temp->getNext();
		}
		cout << "找不到用户，请重新输入用户名！" << endl;
		Sleep(2000);
		continue;
	}
}
void uList::fixPassword() {
	system("cls");
	cout << "========================================" << endl;
	cout << "[           通行证  修改密码           ]" << endl;
	cout << "========================================" << endl;
	if (currUser->getType() == "Root") {
		cout << "当前的用户数据如下：" << endl;
		cout << "权限组  用户名" << endl;
		User* temp = head;
		while (temp != nullptr) {
			temp->printInfo();
			temp = temp->getNext();
		}
		string content;
		while (1) {
			temp = head;
			cout << "请输入您要修改的用户：";
			cin >> content;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			while (temp != nullptr) {
				if (temp->getName() == content) {
					cout << "找到数据，请输入用户 " << temp->getName() << " 的新的密码：";
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
							//密显功能 
							cout << "*";
							ipassword.push_back(pwdChar);
							inputCount++;
						}
					}
					encrype(ipassword);
					temp->fixPassword(ipassword);
					cout << "修改成功！即将退出。" << endl;
					Sleep(2000);
					return;
				}
				temp = temp->getNext();
			}
			cout << "找不到用户，请重新输入用户名！" << endl;
			continue;
		}
	}
	else {
		string ipassword;
		cout << "请输入您的新密码：";
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
				//密显功能 
				cout << "*";
				ipassword.push_back(pwdChar);
				inputCount++;
			}
		}
		encrype(ipassword);
		currUser->fixPassword(ipassword);
		cout << "修改成功！即将退出。" << endl;
		Sleep(2000);
		return;
	}
	return;
}
int uList::deleteUser() {
	system("cls");
	cout << "========================================" << endl;
	cout << "[           通行证  删除用户           ]" << endl;
	cout << "========================================" << endl;
	if (currUser->getType() == "Root") {
		string currUsername = currUser->getName();
		cout << "当前的用户数据如下：" << endl;
		cout << "权限组  用户名" << endl;
		User* temp = head;
		while (temp != nullptr) {
			temp->printInfo();
			temp = temp->getNext();
		}
		string content;
		while (1) {
			temp = head;
			cout << "请输入您要删除的用户：";
			cin >> content;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			if (temp->getName() == content) {
				head = head->getNext();
				delete temp;
				temp = nullptr;
				cout << "账户注销成功！" << endl;
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
					cout << "账户注销成功！" << endl;
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
			cout << "找不到用户，请重新输入用户名！" << endl;
			continue;
		}
	}
	else {
		User* temp = head;
		int input;
		cout << "警告！您正在进行当前用户：" << currUser->getName() << " 的注销操作！" << endl;
		while (1) {
			cout << "是否确认注销？[1]确认  [0]取消：" << endl;
			if (!(cin >> input) || input < 0 || input > 1) {
				cout << "数据输入错误，请重新输入！" << endl;
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
					cout << "账户注销成功！" << endl;
					Sleep(2000);
					return 1;
				}
				while (temp->getNext() != nullptr) {
					if (temp->getNext()->getName() == currUser->getName()) {
						User* q = temp->getNext();
						temp->setNext(temp->getNext()->getNext());
						delete q;
						q = nullptr;
						cout << "账户注销成功！" << endl;
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
//GPT教的哈希函数加密password字符串
void uList::encrype(string& password) {
	hash<string> hasher;
	size_t hash = hasher(password);
	password = to_string(hash);
	return;
}