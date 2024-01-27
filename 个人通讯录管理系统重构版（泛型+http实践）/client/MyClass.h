#pragma once
#include <iostream>
using namespace std;
//提前声明Contacts，防止User中不知道有Contacts
class Contacts;
//================================================================================//
//================================================================================//
class User {
private:
	string username;
	string password;
	User* next;
public:
	//构造和析构
	User(string username = "UNKNWON", string password = "UNKNWON");
	~User();
	//链表实现
	void initNode();//初始化链表
	//void addNode(User* newNode);//新建节点
    void saveNode(User* userHead);//存档
    User* getNext();//获取next指针
	void setNext(User* next);//设置next指针
	//void deleteNode(User* userHead);//删除调用这个函数的对象（节点）
	//void deleteAll(User* userHead);//删除链表
	//功能实现
    void signUp(User* userHead);//注册
    int signIn(User* userHead);//登录
    int checkInfo(string username, string password);//检查用户名与密码匹配
    void menuSelect(User* userHead);//菜单选项
	string getUsername();//获取用户名
    string getPassword();//获取密码
    void setPassword(string newPassword);//设置密码
	void setType(string usertype, User* userHead);//设置用户类型
	//多态
	virtual string getType() = 0;//获取用户类型，返回不同类型
	virtual void printNode(User* userHead) = 0;//只有Root能查看
    virtual void enterMenu(User* userHead) = 0;//不同用户进入不同菜单
    virtual int deleteUser(User* userHead, Contacts* contactsHead) = 0;//非Root只能删自己
    virtual int fixType(User* userHead) = 0;//只有Root能修改用户类型
    virtual void fixPassword(User* userHead) = 0;//非Root只能改自己的密码
};
//================================================================================//
class Root : public User {
public:
	Root(string username = "UNKNWON", string password = "UNKNWON");
	string getType();
	void printNode(User* userHead);
    void enterMenu(User* userHead);
    int deleteUser(User* userHead, Contacts* contactsHead);
    int fixType(User* userHead);
    void fixPassword(User* userHead);
};
//================================================================================//
class Common : public User {
public:
	Common(string username = "UNKNWON", string password = "UNKNWON");
	string getType();
	void printNode(User* userHead);
    void enterMenu(User* userHead);
    int deleteUser(User* userHead, Contacts* contactsHead);
    int fixType(User* userHead);
    void fixPassword(User* userHead);
};
//================================================================================//
class Vip : public User {
public:
	Vip(string username = "UNKNWON", string password = "UNKNWON");
	string getType();
	void printNode(User* userHead);
    void enterMenu(User* userHead);
    int deleteUser(User* userHead, Contacts* contactsHead);
    int fixType(User* userHead);
    void fixPassword(User* userHead);
};
//================================================================================//
//================================================================================//
class Contacts {
private:
	string telephone;
	string mobilephone;
	string qq;
	string name;
	string unit;
	string address;
	string sort;
	string email;
	Contacts* next;
public:
    //构造和析构
    Contacts(string tel = "UNKNWON", string mob = "UNKNWON", string qq = "UNKNWON", string name = "UNKNWON", string unit = "UNKNWON", string address = "UNKNWON", string sort = "UNKNWON", string email = "UNKNWON");
    ~Contacts();
	//链表实现
	void initNode();//初始化链表
	//void addNode(Contacts* newNode);//新建节点
	void saveNode(Contacts* contactsHead);//存档
	Contacts* getNext();//获取next指针
	void setNext(Contacts* next);//设置next指针
	//void deleteNode(Contacts* contactsHead);//删除节点
	//void deleteAll(Contacts* contactsHead);//删除contactsHead后面的整个链表
	//功能实现
	string getName();//获取联系人姓名
	void createContacts(Contacts* contactsHead);//新建联系人
	void printContacts(Contacts* contactsHead);//查看联系人
	void deleteContacts(Contacts* contactsHead);//删除联系人
	void searchContacts(Contacts* contactsHead);//搜索联系人
	void fixContacts(Contacts* contactsHead);//修改联系人
	void fixInfo(string tel, string mob, string qq, string name, string unit, string address, string sort, string email);
	void swapNode(Contacts* x, Contacts* y, Contacts* contactsHead);//交换节点
	void sortContacts(Contacts* contactsHead);//联系人排序（依据名字正序）
	void shareContacts(Contacts* contactsHead, User* userHead);//分享联系人
	void receiveShare(Contacts* contactsHead);//接收联系人
	void cloudFunction(Contacts* contactsHead);
};