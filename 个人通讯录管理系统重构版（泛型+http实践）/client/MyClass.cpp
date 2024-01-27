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
//全局函数
//哈希加密
void encrype(string& str) {
	hash<string> hasher;
	size_t hash = hasher(str);
	str = to_string(hash);
	return;
}
//获取当前时间
string getTime() {
	SYSTEMTIME time;
	GetLocalTime(&time);
	//新建字符串流
	stringstream currTime;
	//类似于sprintf，把数据存入currTime
	currTime << time.wYear << "年"
		<< std::setw(2) << std::setfill('0') << time.wMonth << "月"
		<< std::setw(2) << std::setfill('0') << time.wDay << "日"
		<< std::setw(2) << std::setfill('0') << time.wHour << ":"
		<< std::setw(2) << std::setfill('0') << time.wMinute << ":"
		<< std::setw(2) << std::setfill('0') << time.wSecond;
	return currTime.str();//使用.str()方法把字符串流转换为字符串
}
//保存日志
void logsSave(string content) {
	ofstream saveFile("logs.txt", ios::app);
	if (!saveFile.is_open()) {
		cout << "文件打开失败！" << endl;
		Sleep(2000);
		return;
	}
	getTime();	
	saveFile << getTime() << ' ' << currUser->getUsername() << ' ' << content << endl;
	saveFile.close();
	return;
}
//非Root读取日志（仅读取当前用户的操作日志）
void logsReadClient() {
	string line;
	ifstream readFile("logs.txt");
	//如果logs.txt文件不存在，则调用ofstream进行新建文件
	if (!readFile.good()) {
		ofstream createFile("logs.txt");
		if (!createFile.is_open()) {
			cout << "无法创建logs.txt文件！" << endl;
			Sleep(2000);
			return;
		}
		createFile.close();
		readFile.open("logs.txt");
	}
	system("cls");
	cout << "==========================" << endl;
	cout << "[  通讯录  我的操作日志  ]" << endl;
	cout << "==========================" << endl;
	while (getline(readFile, line)) {
		string time,username,content;
		istringstream iss(line);
		iss >> time >> username >> content;
		//判断载入的文件是不是当前用户
		if (currUser->getUsername() != username) {
			continue;
		}
		cout << left << "您在   " << left << time << left << "   " << content << endl;
	}
	readFile.close();
	cout << "输入0退出本功能：";
	while (getchar() != '0');
	return ;
}
//Root读取日志（读取所有日志）
void logsReadRoot() {
	string line;
	ifstream readFile("logs.txt");
	//如果logs.txt文件不存在，则调用ofstream进行新建文件
	if (!readFile.good()) {
		ofstream createFile("logs.txt");
		if (!createFile.is_open()) {
			cout << "无法创建logs.txt文件！" << endl;
			Sleep(2000);
			return;
		}
		createFile.close();
		readFile.open("logs.txt");
	}
	system("cls");
	cout << "==========================" << endl;
	cout << "[      系 统  日 志      ]" << endl;
	cout << "==========================" << endl;
	while (getline(readFile, line)) {
		string time, username, content;
		istringstream iss(line);
		iss >> time >> username >> content;
		cout << "用户" << setw(8) << left << username << "在" << setw(28) << left << time << content << endl;
	}
	readFile.close();
	cout << "输入0退出本功能：";
	while (getchar() != '0');
	return;
}
//清除日志
void logsClean() {
	//先打开再关闭，覆盖原日志文件，达到清除效果
	ofstream saveFile("logs.txt");
	saveFile.close();
	cout << "所有日志已清空" << endl;
	Sleep(2000);
	return;
}
//=========================================================================
//构造，析构函数
User::User(string username, string password) {
	this->username = username;
	this->password = password;
	this->next = nullptr;
}
//为什么不写析构把链表删除？因为每delete一个节点，都会触发析构。
User::~User() { }
Root::Root(string username, string password) : User(username, password) { }
Common::Common(string username, string password) : User(username, password) { }
Vip::Vip(string username, string password) : User(username, password) { }

//链表实现
User* User::getNext() {
	return next;
}
void User::setNext(User* next) {
	this->next = next;
	return;
}
void User::initNode() {
	//新建文件输入流
	ifstream inputFile("user.txt");
	if (!inputFile.is_open()) {
		cout << "文件打开失败！" << endl;
		return;
	}
	//新建用于读取每一行的字符串line
	string line;
	//把每一行都存到line里
	while (getline(inputFile, line)) {
		string usertype, username, password;
		istringstream iss(line);
		//切分line中的内容
		iss >> usertype >> username >> password;
		//根据usertype判断新建什么类的对象
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
//	//如果调用addNode的那个对象（不一定是头节点）所在节点的下一个是nullptr，直接把新节点接在后面
//	if (this->next == nullptr) {
//		this->next = newNode;
//		return;
//	}
//	//否则遍历已有链表到尾部
//	User* temp = this;
//	while (temp->getNext() != nullptr) {
//		temp = temp->getNext();
//	}
//	//尾插新节点
//	temp->setNext(newNode);
//	return;
//}
//void User::deleteNode(User* userHead) {
//	User* temp = userHead;
//	while (temp->getNext() != nullptr) {
//		if (temp->getNext() == this) {
//			temp->setNext(this->getNext());
//			delete this;
//			//虽然说delete this有一定风险，但是只要注意以后不要对当前节点进行任何操作即可（也就是调用成员函数）
//			return;
//		}
//		temp = temp->getNext();
//	}
//}
void User::saveNode(User* userHead) {
	//在功能实现中需要调用saveNode，所以必须要传头节点指针，否则用当前用户的节点就错了！
	//新建文件输出流
	ofstream saveFile("user.txt");
	if (!saveFile.is_open()) {
		cout << "文件打开失败！" << endl;
		Sleep(2000);
		return;
	}
	//因为调用saveNode的对象只有头节点，所以this指针就是头节点指针，赋给temp进行遍历
	User* temp = userHead;
	while (temp->getNext() != nullptr) {
		//把内容输出到文件
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
//功能实现
//判断用户名和密码是否分别匹配
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
		User* temp = userHead;
		int flag = 0;
		//注册查重
		while (temp->getNext() != nullptr) {
			if (temp->getNext()->getUsername() == iusername) {
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
		//先新建Common对象，赋值给currUser，再把currUser传入addNode实现节点增加（在addNode的同时更新currUser）
		::addNode(this,(currUser = new Common(iusername, ipassword)));
		/*addNode(currUser = new Common(iusername, ipassword));*/
		saveNode(userHead);
		logsSave("注册成功");
		Sleep(2000);
		break;
	}
	return;
}
int User::signIn(User* userHead) {
	system("cls");
	cout << "==========================" << endl;
	cout << "[   通讯录    统一登录   ]" << endl;
	cout << "==========================" << endl;
	if (userHead->getNext() == nullptr) {
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
		//密码哈希加密
		encrype(ipassword);
		User* temp = userHead;
		while (temp->getNext() != nullptr) {
			if (temp->getNext()->checkInfo(iusername, ipassword)) {
				//更新当前用户
				currUser = temp->getNext();
				//保存日志
				logsSave("登录成功");
				//欢迎语
				if (currUser->getType() == "Vip") {
					cout << "登陆成功！尊贵的VIP用户：" << iusername << " 欢迎您！" << endl;
				}
				else if (currUser->getType() == "Root") {
					cout << "管理员登陆成功：" << iusername << endl;
				}
				else {
					cout << "登陆成功！欢迎您：" << iusername << endl;
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
	//注意调用setType时，一定要让用户重新登录，否则再次使用功能时当前用户节点已经释放了！
	//无法直接在不同类型的子类对象直接转换，所以先新建一个新的节点
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
	//现在删除当前用户节点，当前用户节点不可能是最后一个，因为刚刚已经新建了节点在尾部
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
	cout << "[         管理员  查看通行证           ]" << endl;
	cout << "========================================" << endl;
	cout << " 权限组  用户名" << endl;
	User* temp = userHead;
	while (temp->getNext() != nullptr) {
		cout << setw(7) << temp->getNext()->getType() << setw(8) << temp->getNext()->getUsername() << endl;
		temp = temp->getNext();
	}
	cout << "输入0退出本功能：";
	while (getchar() != '0');
	return;
}
//对于非管理，查看所有用户是没有权限的
void Common::printNode(User* userHead) {
	cout << "没有权限！" << endl;
}
void Vip::printNode(User* userHead) {
	cout << "没有权限！" << endl;
}
//Menu
void Root::enterMenu(User* userHead) {
	int flag = 0;
	while (1) {
		int input;
		system("cls");
		cout << "================================" << endl;
		cout << "[      个人通讯录管理系统      ]" << endl;
		cout << "[      管理员用户：" << left << setw(8) << currUser->getUsername() << "    ]" << endl;
		cout << "================================" << endl;
		cout << "[        [1]查看通行证         ]" << endl;
		cout << "[        [2]修改权限组         ]" << endl;
		cout << "[        [3]通行证改密         ]" << endl;
		cout << "[        [4]删除通行证         ]" << endl;
		cout << "[        [5]查看系统日志       ]" << endl;
		cout << "[        [6]清空系统日志       ]" << endl;
		cout << "[        [0]退出本系统         ]" << endl;
		cout << "================================" << endl;
		
		cout << "请输入序号选择功能：";
		if (!(cin >> input) || input < 0 || input > 6) {
			cout << "数据输入错误，请重新输入！" << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			Sleep(2000);
			continue;
		}
		switch (input) {
		case 0:
			flag = 1;
			logsSave("退出登录");
			break;
		case 1:
			currUser->printNode(userHead);
			//查看完成后同时记录日志
			logsSave("查看了通行证");
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
			logsSave("查看了系统日志");
			break;
		case 6:
			logsClean();
			logsSave("清空了系统日志");
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
		cout << "[      个人通讯录管理系统      ]" << endl;
		cout << "[      普通用户：" << left << setw(8) << currUser->getUsername() << "      ]" << endl;
		cout << "================================" << endl;
		cout << "[        [1]新建联系人         ]" << endl;
		cout << "[        [2]查看联系人         ]" << endl;
		cout << "[        [3]删除联系人         ]" << endl;
		cout << "[        [4]修改联系人         ]" << endl;
		cout << "[        [5]分享联系人         ]" << endl;
		cout << "[        [6]修改我的密码       ]" << endl;
		cout << "[        [7]注销通行证         ]" << endl;
		cout << "[        [0]退出本系统         ]" << endl;
		cout << "================================" << endl;
		cout << "[ 联系管理升级至VIP,享更多功能 ]" << endl;
		cout << "================================" << endl;

		cout << "请输入序号选择功能：";
		if (!(cin >> input) || input < 0 || input > 7) {
			cout << "数据输入错误，请重新输入！" << endl;
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
			logsSave("退出登录");
			break;
		case 1:
			contactsHead->createContacts(contactsHead);
			break;
		case 2:
			contactsHead->printContacts(contactsHead);
			//查看完成后同时记录日志
			logsSave("查看了联系人");
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
		cout << "[      个人通讯录管理系统      ]" << endl;
		cout << "[      VIP用户：" << left << setw(8) << currUser->getUsername() << "       ]" << endl;
		cout << "================================" << endl;
		cout << "[        [1]新建联系人         ]" << endl;
		cout << "[        [2]查看联系人         ]" << endl;
		cout << "[        [3]查找联系人         ]" << endl;
		cout << "[        [4]删除联系人         ]" << endl;
		cout << "[        [5]修改联系人         ]" << endl;
		cout << "[        [6]分享联系人         ]" << endl;
		cout << "[        [7]联系人排序         ]" << endl;
		cout << "[        [8]修改我的密码       ]" << endl;
		cout << "[        [9]注销通行证         ]" << endl;
		cout << "[        [A]查看操作日志       ]" << endl;
		cout << "[        [B]云端备份恢复       ]" << endl;
		cout << "[        [0]退出本系统         ]" << endl;
		cout << "================================" << endl;
		cout << "请输入序号选择功能：";
		cin >> input;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		//由于VIP功能太多了，超过了0~9，所以直接输入字符，0~A
		switch (input) {
		case '0':
			flag = 1;
			::deleteAll(contactsHead);
			delete contactsHead;
			contactsHead = nullptr;
			logsSave("退出登录");
			break;
		case '1':
			contactsHead->createContacts(contactsHead);
			break;
		case '2':
			contactsHead->printContacts(contactsHead);
			logsSave("查看了联系人");
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
			cout << "排序中，请稍等，数据量大时可能耗费时间较长。" << endl;
			contactsHead->sortContacts(contactsHead);
			contactsHead->printContacts(contactsHead);
			logsSave("执行了联系人排序");
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
			logsSave("查看了操作日志");
			break;
		case 'B':
			contactsHead->cloudFunction(contactsHead);
			break;
		default:
			cout << "数据输入错误，请重新输入！" << endl;
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
	cout << "[           通行证  删除用户           ]" << endl;
	cout << "========================================" << endl;
	User* temp = userHead;
	string currUsername = currUser->getUsername();
	cout << "当前的用户数据如下：" << endl;
	cout << "权限组  用户名" << endl;
	while (temp->getNext() != nullptr) {
		cout << setw(8) << temp->getNext()->getType() << setw(8) << temp->getNext()->getUsername() << endl;
		temp = temp->getNext();
	}
	string content;
	while (1) {
		temp = userHead;
		int returnValue = 0;
		cout << "请输入您要删除的用户（管理员删除用户时，不会清空用户的本地与云端数据）：";
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
					cout << "您删除的是当前管理用户，即将退出登录。" << endl;
					logsSave("注销了自己的账号");
				}
				else {
					cout << endl;
					stringstream logsContent;
					logsContent << "删除了用户[" << content << ']';
					logsSave(logsContent.str());
				}
				::deleteNode(temp->getNext(), userHead);
				cout << "用户删除成功！";
				saveNode(userHead);
				Sleep(3000);
				return returnValue;
			}
			temp = temp->getNext();
		}
		cout << "找不到用户，请重新输入用户名！" << endl;
		Sleep(2000);
		continue;
	}
	return 0;
}
int Common::deleteUser(User* userHead, Contacts* contactsHead) {
	system("cls");
	cout << "========================================" << endl;
	cout << "[           通行证  用户注销           ]" << endl;
	cout << "========================================" << endl;
	int input, flag = 0;
	cout << "警告！您正在进行当前用户：" << currUser->getUsername() << " 的注销操作！\n该操作会同时删除您的所有联系人！" << endl;
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
			//大忌！！改了好久才发现的！
			//一定要先删除用户的数据，再删除用户！否则在删除用户数据时，调用currUser就变成调用空指针了！
			::deleteAll(contactsHead);
			contactsHead->saveNode(contactsHead);
			logsSave("注销了自己的账号");
			//直接调用deleteNode删除当前节点
			::deleteNode(currUser, userHead);
			cout << "账户注销成功！" << endl;
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
	cout << "[           通行证  用户注销           ]" << endl;
	cout << "========================================" << endl;
	int input, flag = 0;
	//对于Vip用户的关照
	cout << "亲爱的Vip用户：您正在进行当前用户：" << currUser->getUsername() << " 的注销操作！\n该操作会同时删除您的所有联系人（包括云端备份）！" << endl;
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
			//删除云端部分
			//预处理计算文件名
			string username = currUser->getUsername();
			encrype(username);
			stringstream filename;
			filename << username << ".txt";
			//创建HTTP客户端
			httplib::Client client("example.com", 19060);
			stringstream serverpath;
			serverpath << "/remove/" << filename.str();
			//发送GET请求，删除服务器中的文件
			auto res = client.Get(serverpath.str());
			//直接调用deleteAll删除用户数据
			::deleteAll(contactsHead);
			contactsHead->saveNode(contactsHead);
			logsSave("注销了自己的账号");
			//直接调用deleteNode删除当前节点
			::deleteNode(currUser, userHead);
			cout << "账户注销成功！" << endl;
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
	cout << "[         管理员  修改权限组           ]" << endl;
	cout << "========================================" << endl;
	cout << "当前的用户数据如下：" << endl;
	cout << "权限组  用户名" << endl;
	User* temp = userHead;
	while (temp->getNext() != nullptr) {
		cout << setw(8) << temp->getNext()->getType() << setw(8) << temp->getNext()->getUsername() << endl;
		temp = temp->getNext();
	}
	string content;
	while (1) {
		temp = userHead;
		cout << "请输入您要修改的用户：";
		cin >> content;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		//定义returnValue，最终实现判断修改完成后是否需要自动退出账号！
		int returnValue = 0;
		//如果修改的是当前用户，那么最终自动退出登录
		if (currUser->getUsername() == content) {
			returnValue = 1;
		}
		while (temp->getNext() != nullptr) {
			if (temp->getNext()->getUsername() == content) {
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
					logsContent << "将用户[" << content << "]的权限组改为[" << selectedType << ']';
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
					cout << "修改完成！";
					saveNode(userHead);
					if (returnValue == 1) {
						cout << "您修改的是当前用户的权限组，即将自动退出登录。" << endl;
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
		cout << "找不到用户，请重新输入用户名！" << endl;
		Sleep(2000);
		continue;
	}
}
int Common::fixType(User* userHead) {
	cout << "没有权限！" << endl;
	return 0;
}
int Vip::fixType(User* userHead) {
	cout << "没有权限！" << endl;
	return 0;
}
void Root::fixPassword(User* userHead) {
	system("cls");
	cout << "========================================" << endl;
	cout << "[           通行证  修改密码           ]" << endl;
	cout << "========================================" << endl;
	cout << "当前的用户数据如下：" << endl;
	cout << "权限组  用户名" << endl;
	User* temp = userHead;
	while (temp->getNext() != nullptr) {
		cout << setw(8) << temp->getNext()->getType() << setw(8) << temp->getNext()->getUsername() << endl;
		temp = temp->getNext();
	}
	string content;
	while (1) {
		temp = userHead;
		cout << "请输入您要修改的用户：";
		cin >> content;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		while (temp->getNext() != nullptr) {
			if (temp->getNext()->getUsername() == content) {
				cout << "找到数据，请输入用户 " << content << " 的新的密码：";
				//密码输入
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
				//重设密码
				temp->getNext()->setPassword(ipassword);
				cout << "修改成功！" << endl;
				stringstream logsContent;
				logsContent << "修改了用户[" << content << "]的密码";
				logsSave(logsContent.str());
				saveNode(userHead);
				Sleep(2000);
				return;
			}
			temp = temp->getNext();
		}
		cout << "找不到用户，请重新输入用户名！" << endl;
		continue;
	}
}
void Common::fixPassword(User* userHead) {
	system("cls");
	cout << "========================================" << endl;
	cout << "[           通行证  修改密码           ]" << endl;
	cout << "========================================" << endl;
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
	currUser->setPassword(ipassword);
	cout << "修改成功！即将退出。" << endl;
	logsSave("修改了自己的密码");
	saveNode(userHead);
	Sleep(2000);
	return;
}
void Vip::fixPassword(User* userHead) {
	system("cls");
	cout << "========================================" << endl;
	cout << "[           通行证  修改密码           ]" << endl;
	cout << "========================================" << endl;
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
	currUser->setPassword(ipassword);
	cout << "修改成功！即将退出。" << endl;
	logsSave("修改了自己的密码");
	saveNode(userHead);
	Sleep(2000);
	return;
}

//=================================================联系人类========================================================//
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
//为什么不写析构把链表删除？因为每delete一个节点，都会触发析构。
//析构函数不能有参数，所以无法用析构函数删除节点，因为需要传入头节点。
Contacts::~Contacts() { }
//链表实现
Contacts* Contacts::getNext() {
	return next;
}
void Contacts::setNext(Contacts* next) {
	this->next = next;
	return;
}
void Contacts::initNode() {
	//新建文件输入流
	ifstream inputFile("data.txt");
	if (!inputFile.is_open()) {
		cout << "文件打开失败！" << endl;
		return;
	}
	//新建用于读取每一行的字符串line
	string line;
	//把每一行都存到line里
	while (getline(inputFile, line)) {
		string username, telephone, mobilephone, qq, name, unit, address, sort, email;
		istringstream iss(line);
		//切分line中的内容
		iss >> username >> telephone >> mobilephone >> qq >> name >> unit >> address >> sort >> email;
		//判断载入的文件是不是当前用户
		if (currUser->getUsername() != username) {
			continue;
		}
		::addNode(this, (new Contacts(telephone, mobilephone, qq, name, unit, address, sort, email)));
	}
	inputFile.close();
	return;
}
//void Contacts::addNode(Contacts* newNode) {
//	//如果调用addNode的那个对象（不一定是头节点）所在节点的下一个是nullptr，直接把新节点接在后面
//	if (this->next == nullptr) {
//		this->next = newNode;
//		return;
//	}
//	//否则遍历已有链表到尾部
//	Contacts* temp = this;
//	while (temp->getNext() != nullptr) {
//		temp = temp->getNext();
//	}
//	//尾插新节点
//	temp->setNext(newNode);
//	return;
//}
//void Contacts::deleteNode(Contacts* contactsHead) {
//	//谁调用，删除谁！
//	Contacts* temp = contactsHead;
//	while (temp != nullptr && temp->getNext() != nullptr) {
//		if (temp->getNext() == this) {
//			temp->setNext(this->getNext());
//			delete this;
//			//虽然说delete this有一定风险，但是只要注意以后不要对当前节点进行任何操作即可（也就是调用成员函数）
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
	//防止没有data文件，因为一开始读data，如果一开始写data就不用考虑data文件没有的情况了
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
		if (username != currUser->getUsername()) {
			tempFile << username << ' ' << telephone << ' ' << mobilephone << ' ' << qq << ' ' << name << ' ' << unit << ' ' << address << ' ' << sort << ' ' << email << endl;
		}
	}
	readFile.close();
	tempFile.close();
	//然后把当前用户数据覆盖到data数据文件
	ofstream saveFile("data.txt");
	Contacts* temp = contactsHead;
	while (temp->getNext() != nullptr) {
		saveFile << currUser->getUsername() << ' ' << temp->getNext()->telephone << ' ' << temp->getNext()->mobilephone << ' ' << temp->getNext()->qq << ' ' << temp->getNext()->name << ' ' << temp->getNext()->unit << ' ' << temp->getNext()->address << ' ' << temp->getNext()->sort << ' ' << temp->getNext()->email << endl;
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
string Contacts::getName() {
	return this->name;
}
void Contacts::createContacts(Contacts* contactsHead) {
	//num2用于备份num，到时num变化算差值
	int num = 0, num2 = 0;
	Contacts* temp = contactsHead;
	while (1) {
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
		cout << "请输入联系人的相关信息：" << endl;
		while (num--) {
			string telephone, mobilephone, qq, name, unit, address, sort, email;
			while (1) {
				int flag = 0;
				//输入姓名
				cout << "请输入第" << num2 - num << "个联系人的姓名：";
				cin >> name;
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				temp = contactsHead;
				//姓名查重
				while (temp->getNext() != nullptr) {
					if (temp->getNext()->getName() == name) {
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
				//输入手机号
				cout << "请输入第" << num2 - num << "个联系人的手机号：";
				cin >> mobilephone;
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				//输入座机号
				cout << "请输入第" << num2 - num << "个联系人的座机号：";
				cin >> telephone;
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				//输入QQ号
				cout << "请输入第" << num2 - num << "个联系人的QQ号：";
				cin >> qq;
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				//输入QQ号
				cout << "请输入第" << num2 - num << "个联系人的邮箱：";
				cin >> email;
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				//输入单位
				cout << "请输入第" << num2 - num << "个联系人的单位：";
				cin >> unit;
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				//输入住址
				cout << "请输入第" << num2 - num << "个联系人的住址：";
				cin >> address;
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				//输入分类
				cout << "请输入第" << num2 - num << "个联系人的分类：";
				cin >> sort;
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				break;
			}
			/*addNode(new Contacts(telephone, mobilephone, qq, name, unit, address, sort, email));*/
			::addNode(this,(new Contacts(telephone, mobilephone, qq, name, unit, address, sort, email)));
			stringstream logsContent;
			logsContent << "新建了名为[" << name << "]的联系人";
			logsSave(logsContent.str());
			saveNode(contactsHead);
		}
		cout << "联系人创建完成！程序将在3秒后退出回菜单" << endl;
		Sleep(3000);
		break;
	}
	return;
}
void Contacts::printContacts(Contacts* contactsHead) {
	//多功能分页查看解释具体看我的博客：https://notes.smallbamboo.cn/CPPDisplayPage.html

	if (contactsHead->getNext() == nullptr) {
		cout << "您的通讯录中无数据，请先添加联系人！" << endl;
		Sleep(3000);
		return;
	}
	int pos = 1, page = 1, total = 0, allpage, ct = 0, numPerPage = 10;
	int input;
	Contacts* temp = contactsHead, *count = contactsHead;
	//统计节点数
	while (count->getNext() != nullptr) {
		total++;
		count = count->getNext();
	}
	while (1) {
		//计算总页数
		if (total % numPerPage == 0) {
			allpage = total / numPerPage;
		}
		else {
			allpage = total / numPerPage + 1;
		}
		//屏幕输出
		system("cls");
		pos = (page - 1) * numPerPage + 1;
		temp = contactsHead;
		ct = 0;
		cout << "=============================================================================================================" << endl;
		cout << "[ " << right << setw(3) << numPerPage << "条数据/页                               通讯录  查看联系人                                 第" << right << setw(3) << page << '/' << left << setw(3) << allpage << "页 ]" << endl;
		cout << "=============================================================================================================" << endl;
		cout << "姓名    手机号         座机号         QQ号           邮箱                     单位      地址      分类  " << endl;
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
		cout << "[     输入[0]返回至菜单  输入[1]查看上一页  输入[2]查看下一页  输入[3]进入页数跳转  输入[4]选择分页大小     ]" << endl;
		cout << "=============================================================================================================" << endl;
		while (1) {
			if (!(cin >> input) || input < 0 || input > 4) {
				cout << "数据输入错误，请重新输入：";
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				continue;
			}
			if (input == 0) {
				return;
			}
			else if (input == 1) {
				if (page == 1) {
					cout << "没有上一页了，请重新输入：";
					continue;
				}
				page--;
			}
			else if (input == 2) {
				if (page == allpage) {
					cout << "没有下一页了，请重新输入：";
					continue;
				}
				page++;
			}
			else if (input == 3) {
				int target;
				while (1) {
					cout << "请输入要跳转到的页码：";
					if (!(cin >> target) || target < 1 || target > allpage) {
						cout << "页码范围有误，请重新输入！" << endl;
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
					cout << "[1]10条/页  [2]20条/页  [3]30条/页  [4]50条/页  [5]100条/页\n请选择新的分页大小：";
					if (!(cin >> input) || input < 1 || input > 5) {
						cout << "页码范围有误，请重新输入！" << endl;
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
						Sleep(2000);
						continue;
					}
					break;
				}
				//切换分页大小
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
		cout << "您的通讯录中无数据，请先添加联系人！" << endl;
		Sleep(3000);
		return;
	}
	Contacts* temp = contactsHead;
	string content;
	while (1) {
		temp = contactsHead;
		system("cls");
		cout << "==========================" << endl;
		cout << "[   通讯录  删除联系人   ]" << endl;
		cout << "==========================" << endl;
		cout << "请输入需要删除的联系人姓名：";
		cin >> content;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		while (temp->getNext() != nullptr) {
			if (temp->getNext()->getName() == content) {
				//直接对要删除的联系人使用deleteNode方法
				::deleteNode(temp->getNext(), contactsHead);
				stringstream logsContent;
				logsContent << "删除了联系人[" << content << ']';
				logsSave(logsContent.str());
				cout << "删除成功！" << endl;
				saveNode(contactsHead);
				Sleep(2000);
				return;
			}
			temp = temp->getNext();
		}
		cout << "找不到对应的联系人姓名，请重新输入！" << endl;
		Sleep(2000);
		continue;
	}
	return;
}
void Contacts::fixContacts(Contacts* contactsHead) {
	if (contactsHead->getNext() == nullptr) {
		cout << "您的通讯录中无数据，请先添加联系人！" << endl;
		Sleep(3000);
		return;
	}
	Contacts* temp = contactsHead;
	string content;
	while (1) {
		temp = contactsHead;
		system("cls");
		cout << "==========================" << endl;
		cout << "[   通讯录  修改联系人   ]" << endl;
		cout << "==========================" << endl;
		string telephone, mobilephone, qq, name, unit, address, sort, email;
		cout << "请输入需要修改的联系人姓名：";
		cin >> content;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		Contacts* temp2;
		while (temp->getNext() != nullptr) {
			if (temp->getNext()->getName() == content) {
				cout << "找到数据，";
				temp2 = temp->getNext();
				while (1) {
					int flag = 0;
					cout << "请按照输入修改后的数据：" << endl;
					//输入姓名
					cout << "请输入改后联系人的姓名：";
					cin >> name;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					temp = contactsHead;
					//姓名查重
					while (temp->getNext() != nullptr) {
						//可以让需要修改的数据修改后还是之前的联系人名字
						if (temp->getNext()->getName() == name && name != content) {
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
					//输入手机号
					cout << "请输入改后联系人的手机号：";
					cin >> mobilephone;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					//输入座机号
					cout << "请输入改后联系人的座机号：";
					cin >> telephone;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					//输入QQ号
					cout << "请输入改后联系人的QQ号：";
					cin >> qq;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					//输入QQ号
					cout << "请输入改后联系人的邮箱：";
					cin >> email;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					//输入单位
					cout << "请输入改后联系人的单位：";
					cin >> unit;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					//输入住址
					cout << "请输入改后联系人的住址：";
					cin >> address;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					//输入分类
					cout << "请输入改后联系人的分类：";
					cin >> sort;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					break;
				}
				//由于此操作只在此函数中调用了一次，所以不考虑将他封装成函数
				temp2->fixInfo(telephone, mobilephone, qq, name, unit, address, sort, email);
				stringstream logsContent;
				logsContent << "修改了联系人[" << content << "]的信息";
				logsSave(logsContent.str());
				cout << "修改成功！" << endl;
				saveNode(contactsHead);
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
void Contacts::searchContacts(Contacts* contactsHead) {
	if (contactsHead->getNext() == nullptr) {
		cout << "您的通讯录中无数据，请先添加联系人！" << endl;
		Sleep(3000);
		return;
	}
	Contacts* temp = contactsHead;
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
			cout << "请输入查找内容：";
			cin >> content;
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "查找完成，以下是查找结果：" << endl;
			cout << "姓名    手机号         座机号         QQ号           邮箱                     单位      地址      分类  " << endl;
			stringstream logsContent;
			if (input == 1) {
				logsContent << "查找了联系人姓名[" << content << ']';
			}
			else if (input == 2) {
				logsContent << "查找了联系人QQ号[" << content << ']';
			}
			else if (input == 3) {
				logsContent << "查找了联系人单位[" << content << ']';
			}
			else if (input == 4) {
				logsContent << "查找了联系人分类[" << content << ']';
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
				cout << "没有查找到任何数据" << endl;
			}
			logsContent << "，并且查找到了[" << count << "]条数据";
			logsSave(logsContent.str());
			cout << "输入0退出本功能：";
			while (getchar() != '0');
			break;
		}
	}
	return;
}
//节点交换函数
void Contacts::swapNode(Contacts* a, Contacts* b, Contacts* contactsHead) {
	Contacts* temp = contactsHead;
	//此循环用于找到a节点的上一个节点
	while (temp->getNext() != a) {
		temp = temp->getNext();
	}
	//让a的上一个节点（temp）指向b
	temp->setNext(b);
	//让a的下一个指向b的下一个
	a->setNext(b->getNext());
	//让b的下一个指向a
	b->setNext(a);
}
void Contacts::sortContacts(Contacts* contactsHead) {
	if (contactsHead->getNext() == nullptr) {
		cout << "您的通讯录中无数据，请先添加联系人！" << endl;
		Sleep(3000);
		return;
	}
	//冒泡排序
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
		cout << "您的通讯录中无数据，请先添加联系人！" << endl;
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
		cout << "[   通讯录  分享联系人   ]" << endl;
		cout << "==========================" << endl;
		cout << "请输入要分享的联系人姓名：";
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
		cout << "找不到您输入的联系人，请重新输入！" << endl;
		Sleep(2000);
		continue;
	}
	flag = 0;
	User* temp2 = userHead;
	while (1) {
		cout << "联系人获取成功，请输入要分享的目标用户：";
		cin >> tmpUsername;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		//细化判断
		if (tmpUsername == currUser->getUsername()) {
			cout << "您不能把联系人分享到自己！请重新输入目标用户。" << endl;
			Sleep(2000);
			continue;
		}
		temp2 = userHead;
		while (temp2->getNext() != nullptr) {
			if (temp2->getNext()->getUsername() == tmpUsername) {
				//细化判断
				if (temp2->getNext()->getType() == "Root") {
					flag = 2;
					cout << "您不能分享联系人至管理员，请重新输入目标用户！" << endl;
					Sleep(2000);
					break;
				}
				ofstream saveFile("share.txt", ios::app);
				if (!saveFile.is_open()) {
					cout << "文件打开失败！" << endl;
					Sleep(2000);
					return;
				}
				//格式是先写目标用户名，再输出当前用户数据
				saveFile << temp2->getNext()->getUsername() << ' ';
				saveFile << currUser->getUsername() << ' ' << temp->getNext()->telephone << ' ' << temp->getNext()->mobilephone << ' ' << temp->getNext()->qq << ' ' << temp->getNext()->name << ' ' << temp->getNext()->unit << ' ' << temp->getNext()->address << ' ' << temp->getNext()->sort << ' ' << temp->getNext()->email << endl;
				cout << "分享成功！对方登陆后即可收到提醒。" << endl;
				stringstream logsContent;
				logsContent << "将联系人[" << tmpName << "]分享到用户[" << tmpUsername << ']';
				logsSave(logsContent.str());
				saveFile.close();
				flag = 1;
				Sleep(3000);
				break;
			}
			temp2 = temp2->getNext();
		}
		if (flag == 0) {
			cout << "找不到用户，请重新输入用户名！" << endl;
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
	//如果没有share.txt就自动创建
	if (!readFile.good()) {
		ofstream createFile("share.txt");
		if (!createFile.is_open()) {
			cout << "无法创建share.txt文件！" << endl;
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
		//把不是当前用户的分享记录存到temp.txt文件
		if (currUser->getUsername() != touser) {
			tempFile << touser << ' ' << fromuser << ' ' << telephone << ' ' << mobilephone << ' ' << qq << ' ' << name << ' ' << unit << ' ' << address << ' ' << sort << ' ' << email << endl;
			continue;
		}
		//用户选择是否接收分享
		stringstream logsContent;
		system("cls");
		cout << "==========================" << endl;
		cout << "[   通讯录  接收联系人   ]" << endl;
		cout << "==========================" << endl;
		cout << "接收到来自 " << fromuser << " 的联系人分享：" << endl;
		cout << "姓名    手机号         座机号         QQ号           邮箱                     单位      地址      分类  " << endl;
		cout << left << setw(8) << name << setw(15) << mobilephone << setw(15) << telephone << setw(15) << qq << setw(25) << email << setw(10) << unit << setw(10) << address << setw(10) << sort << endl;
		while (1) {
			cout << "请选择 [1]接收 或 [0]丢弃：";
			if (!(cin >> input) || input < 0 || input > 1) {
				cout << "数据输入错误，请重新输入！" << endl;
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				Sleep(2000);
				continue;
			}
			break;
		}
		if (input == 0) {
			cout << "丢弃成功！" << endl;
			logsContent << "丢弃了来自用户[" << fromuser << "]的联系人[" << name << "]分享";
			logsSave(logsContent.str());
			Sleep(1000);
			continue;
		}
		//检测用户重复
		int flag = 0;
		Contacts* temp = contactsHead;
		while (temp->getNext() != nullptr) {
			if (name == temp->getNext()->name) {
				cout << "检测到联系人姓名与已有联系人重复，自动丢弃。" << endl;
				logsContent << "主动接收了来自[" << fromuser << "]的联系人[" << name << "]分享，但因联系人重复被自动丢弃";
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
		//把分享数据添加到当前用户的联系人链表中
		::addNode(this, (new Contacts(telephone, mobilephone, qq, name, unit, address, sort, email)));
		cout << "保存成功！" << endl;
		logsContent << "接收了来自[" << fromuser << "]的联系人[" << name << "]分享";
		logsSave(logsContent.str());
		Sleep(1000);
	}
	//导入完成后，把temp.txt的其他用户分享记录写回share.txt
	//做到删除当前用户的已导入分享
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
		cout << "[   通讯录  备份与恢复   ]" << endl;
		cout << "==========================" << endl;
		cout << "[   [1] 数据备份到云端   ]" << endl;
		cout << "[   [2] 云端同步到本地   ]" << endl;
		cout << "[   [0] 退出此功能       ]" << endl;
		cout << "==========================" << endl;
		cout << "请输入序号选择功能：";
		if (!(cin >> input) || input < 0 || input > 2) {
			cout << "数据输入错误，请重新输入！" << endl;
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
			cout << "当前无联系人数据，即将退出备份。";
			Sleep(3000);
			return;
		}
		//保存当前用户数据
		ofstream saveFile(filename.str());
		Contacts* temp = contactsHead;
		while (temp->getNext() != nullptr) {
			saveFile << currUser->getUsername() << ' ' << temp->getNext()->telephone << ' ' << temp->getNext()->mobilephone << ' ' << temp->getNext()->qq << ' ' << temp->getNext()->name << ' ' << temp->getNext()->unit << ' ' << temp->getNext()->address << ' ' << temp->getNext()->sort << ' ' << temp->getNext()->email << endl;
			temp = temp->getNext();
		}
		saveFile.close();

		//读取文本文件内容
		ifstream file(filename.str());
		string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
		file.close();

		//创建HTTP客户端
		httplib::Client client("example.com", 19060);
		httplib::MultipartFormDataItems items = {
		{"ContatcsDataText", content, filename.str(), "text/plain"}
		};
		//发送POST请求，将文本文件内容作为请求体发送给服务端
		auto res = client.Post("/upload", items);

		//检查响应状态码
		if (res) {
			cout << "数据备份成功！" << endl;
			logsSave("将数据备份到了云端");
		}
		else {
			cout << "数据备份失败，错误代码：" << res.error() << endl;
		}
		//删除创建的文件，保证安全
		remove(filename.str().c_str());
	}
	else if (input == 2) {
		//创建HTTP客户端
		httplib::Client client("example.com", 19060);
		stringstream serverpath;
		serverpath << "/download/" << filename.str();
		//发送GET请求，从服务器下载文件
		auto res2 = client.Get(serverpath.str());
		// 检查响应状态码
		if (res2 && res2->status == 200) {
			// 保存文件内容到本地
			ofstream ofs(filename.str(), ios::binary);
			ofs.write(res2->body.c_str(), res2->body.length());
			ofs.close();
		}
		else {
			cout << "暂无云端备份数据。" << endl;
			Sleep(3000);
			return;
		}
		//先清空当前链表
		::deleteAll(contactsHead);
		//导入云端备份
		ifstream inputFile(filename.str());
		if (!inputFile.is_open()) {
			cout << "文件打开失败！" << endl;
			Sleep(3000);
			return;
		}
		//新建用于读取每一行的字符串line
		string line;
		//把每一行都存到line里
		while (getline(inputFile, line)) {
			string username, telephone, mobilephone, qq, name, unit, address, sort, email;
			istringstream iss(line);
			//切分line中的内容
			iss >> username >> telephone >> mobilephone >> qq >> name >> unit >> address >> sort >> email;
			::addNode(this, (new Contacts(telephone, mobilephone, qq, name, unit, address, sort, email)));
		}
		inputFile.close();
		cout << "云端数据同步成功，本地数据已被覆盖" << endl;
		logsSave("将云端数据同步到了本地");
		//删除下载的文件，保证安全
		remove(filename.str().c_str());
	}
	else {
		return;
	}
	Sleep(3000);
	return;
}