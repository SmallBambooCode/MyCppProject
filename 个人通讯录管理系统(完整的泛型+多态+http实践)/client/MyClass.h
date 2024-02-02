#pragma once
#include <iostream>
using namespace std;
template<class T>
class LinkNode {
private:
	T data;
	LinkNode<T>* next;
public:
	LinkNode(LinkNode<T>* ptr = nullptr) {
		next = ptr;
	}
	//函数参数表中的形参允许有默认值，但是带默认值的参数需要放后面
	LinkNode(const T& item, LinkNode<T>* ptr = nullptr) {
		next = ptr;
		data = item;
	}
public:
	//运算符重载
	bool operator==(LinkNode<T>& obj) {
		if (this->data == obj.getData()) {
			return true;
		}
		else {
			return false;
		}
	}
	bool operator!=(LinkNode<T>& obj) {
		if (this->data == obj.getData()) {
			return false;
		}
		else {
			return true;
		}
	}
	friend ostream& operator<< (ostream& out, LinkNode<T>& t) {
		out << t.getData();
		return out;
	}
	friend istream& operator>> (istream& in, LinkNode<T>& t) {
		T temp;
		cin >> temp;
		t.setData(temp);
		return in;
	}
	LinkNode<T>& operator=(LinkNode<T>& obj) {
		this->data = obj.data;
		this->next = obj.next;
		return *this;
	}
	//链表操作
	LinkNode<T>* getNext() {
		return this->next;
	}
	T& getData() {
		return this->data;
	}
	void setNext(LinkNode<T>* next) {
		this->next = next;
	}
	void setData(T& item) {
		this->data = item;
	}
};
template<class T>
class LinkList {
private:
	LinkNode<T>* head;
public:
	//无参构造函数
	LinkList() {
		this->head = new LinkNode<T>;
	}
	//有参构造函数
	LinkList(const T& item) {
		this->head = new LinkNode<T>(item);
	}
	//赋值/拷贝构造函数
	LinkList(LinkList<T>& List) {
		this->head = List.head;
	}
	//析构（包含对于整条链表的delete）
	~LinkList() {
		deleteAll();
		delete this->head;
		head = nullptr;
	}
public:
	//运算符重载
	bool operator==(LinkList<T>& obj) {
		if (this->head == obj.getHead()) {
			return true;
		}
		else {
			return false;
		}
	}
	bool operator!=(LinkList<T>& obj) {
		if (this->head == obj.getHead()) {
			return false;
		}
		else {
			return true;
		}
	}
	friend ostream& operator<< (ostream& out, LinkList<T>& t) {
		LinkNode<T>* temp = t.getHead();
		while (temp->getNext() != nullptr) {
			cout << temp->getNext()->getData() << endl;
			temp = temp->getNext();
		}
		return out;
	}
	LinkList<T>& operator=(LinkList<T>& obj) {
		if (this->head != nullptr) {
			this->deleteAll();
			delete this->head;
			this->head = nullptr;
		}
		this->head = obj.head;
		return *this;
	}
	//链表操作
	LinkNode<T>* getHead() {
		return this->head;
	}
	void addNode(const T& item) {
		//如果调用addNode的那个对象（不一定是头节点）所在节点的下一个是nullptr，直接把新节点接在后面，否则遍历链表
		LinkNode<T>* temp = this->head;
		while (temp->getNext() != nullptr) {
			temp = temp->getNext();
		}
		//尾插新节点
		temp->setNext(new LinkNode<T>(item));
		return;
	}
	void deleteNode(T& item) {
		LinkNode<T>* temp = this->head;
		while (temp != nullptr && temp->getNext() != nullptr) {
			if (temp->getNext()->getData() == item) {
				LinkNode<T>* toDelete = temp->getNext();
				temp->setNext(toDelete->getNext());
				delete toDelete;
				return;
			}
			temp = temp->getNext();
		}
	}
	void deleteAll() {
		LinkNode<T>* toDelete;
		//遍历链表删除所有节点
		while (this->head != nullptr && this->head->getNext() != nullptr) {
			toDelete = this->head->getNext();
			this->head->setNext(toDelete->getNext());
			delete toDelete;
		}
	}
	void updateNode(T& toUpdate, T& target) {
		LinkNode<T>* temp = this->head;
		while (temp->getNext() != nullptr) {
			if (temp->getNext()->getData() == toUpdate) {
				temp->getNext()->setData(target);
				return;
			}
			temp = temp->getNext();
		}
		return;
	}
};
//提前声明Contacts，防止User中不知道有Contacts
class Contacts;
//================================================================================//
//================================================================================//
class User {
private:
	string username;
	string password;
public:
	//构造和析构
	User(string username = "UNKNWON", string password = "UNKNWON");
	~User();
	//文件
	void initNode(LinkList<User*>& list);//初始化链表
	void saveNode(LinkList<User*>& list);//存档
	//功能实现
	void signUp(LinkList<User*>& list);//注册
	int signIn(LinkList<User*>& list);//登录
	int checkInfo(string username, string password);//检查用户名与密码匹配
	void menuSelect(LinkList<User*>& list);//菜单选项
	string getUsername();//获取用户名
	string getPassword();//获取密码
	void setPassword(string newPassword);//设置密码
	void setType(string usertype, LinkList<User*>& list);//设置用户类型
	//多态
	virtual string getType() = 0;//获取用户类型，返回不同类型
	virtual void printUser(LinkList<User*>& list) = 0;//只有Root能查看
	virtual void enterMenu(LinkList<User*>& list) = 0;//不同用户进入不同菜单
	virtual int deleteUser(LinkList<User*>& list, LinkList<Contacts>& clist) = 0;//非Root只能删自己
	virtual int fixType(LinkList<User*>& list) = 0;//只有Root能修改用户类型
	virtual void fixPassword(LinkList<User*>& list) = 0;//非Root只能改自己的密码
	//运算符重载
	bool operator==(User& obj) {
		if (this->username == obj.username && this->password == obj.password) {
			return true;
		}
		else {
			return false;
		}
	}
	friend ostream& operator<< (ostream& out, User& t);
	friend istream& operator>> (istream& in, User& t);
};
//================================================================================//
class Root : public User {
public:
	Root(string username = "UNKNWON", string password = "UNKNWON");
	string getType();//获取用户类型，返回不同类型
	void printUser(LinkList<User*>& list);//只有Root能查看
	void enterMenu(LinkList<User*>& list);//不同用户进入不同菜单
	int deleteUser(LinkList<User*>& list, LinkList<Contacts>& clist);//管理员可以删除所有用户
	int fixType(LinkList<User*>& list);//只有Root能修改用户类型
	void fixPassword(LinkList<User*>& list);//管理员可以修改所有人的
};
//================================================================================//
class Common : public User {
public:
	Common(string username = "UNKNWON", string password = "UNKNWON");
	string getType();//获取用户类型，返回不同类型
	void printUser(LinkList<User*>& list);//返回无权限
	void enterMenu(LinkList<User*>& list);//不同用户进入不同菜单
	int deleteUser(LinkList<User*>& list, LinkList<Contacts>& clist);//普通用户自己注销
	int fixType(LinkList<User*>& list);//返回无权限
	void fixPassword(LinkList<User*>& list);//修改自己的密码
};
//================================================================================//
class Vip : public User {
public:
	Vip(string username = "UNKNWON", string password = "UNKNWON");
	string getType();//获取用户类型，返回不同类型
	void printUser(LinkList<User*>& list);//返回无权限
	void enterMenu(LinkList<User*>& list);//不同用户进入不同菜单
	int deleteUser(LinkList<User*>& list, LinkList<Contacts>& clist);//普通用户自己注销
	int fixType(LinkList<User*>& list);//返回无权限
	void fixPassword(LinkList<User*>& list);//修改自己的密码
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
public:
	//构造和析构
	Contacts(string tel = "UNKNWON", string mob = "UNKNWON", string qq = "UNKNWON", string name = "UNKNWON", string unit = "UNKNWON", string address = "UNKNWON", string sort = "UNKNWON", string email = "UNKNWON");
	~Contacts();
	//链表实现
	void initNode(LinkList<Contacts>& clist);//初始化链表
	void saveNode(LinkList<Contacts>& clist);//存档
	//功能实现
	string getName();//获取联系人姓名
	void createContacts(LinkList<Contacts>& clist);//新建联系人
	void printContacts(LinkList<Contacts>& clist);//查看联系人
	void deleteContacts(LinkList<Contacts>& clist);//删除联系人
	void searchContacts(LinkList<Contacts>& clist);//搜索联系人
	void fixContacts(LinkList<Contacts>& clist);//修改联系人
	void fixInfo(string tel, string mob, string qq, string name, string unit, string address, string sort, string email);
	void swapNode(LinkNode<Contacts>* a, LinkNode<Contacts>* b, LinkList<Contacts>& clist);//交换节点
	void sortContacts(LinkList<Contacts>& clist);//联系人排序（依据名字正序）
	void shareContacts(LinkList<Contacts>& clist, LinkList<User*>& list);//分享联系人
	void receiveShare(LinkList<Contacts>& clist);//接收联系人
	void cloudFunction(LinkList<Contacts>& clist);//云端数据服务
	//运算符重载
	bool operator==(Contacts& obj) {
		if (this->telephone == obj.telephone &&
			this->mobilephone == obj.mobilephone &&
			this->qq == obj.qq &&
			this->name == obj.name &&
			this->unit == obj.unit &&
			this->address == obj.address &&
			this->sort == obj.sort &&
			this->email == obj.email) {
			return true;
		}
		else {
			return false;
		}
	}
	friend ostream& operator<< (ostream& out, Contacts& t);
	friend istream& operator>> (istream& in, Contacts& t);
};