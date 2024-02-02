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
	//�����������е��β�������Ĭ��ֵ�����Ǵ�Ĭ��ֵ�Ĳ�����Ҫ�ź���
	LinkNode(const T& item, LinkNode<T>* ptr = nullptr) {
		next = ptr;
		data = item;
	}
public:
	//���������
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
	//�������
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
	//�޲ι��캯��
	LinkList() {
		this->head = new LinkNode<T>;
	}
	//�вι��캯��
	LinkList(const T& item) {
		this->head = new LinkNode<T>(item);
	}
	//��ֵ/�������캯��
	LinkList(LinkList<T>& List) {
		this->head = List.head;
	}
	//�����������������������delete��
	~LinkList() {
		deleteAll();
		delete this->head;
		head = nullptr;
	}
public:
	//���������
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
	//�������
	LinkNode<T>* getHead() {
		return this->head;
	}
	void addNode(const T& item) {
		//�������addNode���Ǹ����󣨲�һ����ͷ�ڵ㣩���ڽڵ����һ����nullptr��ֱ�Ӱ��½ڵ���ں��棬�����������
		LinkNode<T>* temp = this->head;
		while (temp->getNext() != nullptr) {
			temp = temp->getNext();
		}
		//β���½ڵ�
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
		//��������ɾ�����нڵ�
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
//��ǰ����Contacts����ֹUser�в�֪����Contacts
class Contacts;
//================================================================================//
//================================================================================//
class User {
private:
	string username;
	string password;
public:
	//���������
	User(string username = "UNKNWON", string password = "UNKNWON");
	~User();
	//�ļ�
	void initNode(LinkList<User*>& list);//��ʼ������
	void saveNode(LinkList<User*>& list);//�浵
	//����ʵ��
	void signUp(LinkList<User*>& list);//ע��
	int signIn(LinkList<User*>& list);//��¼
	int checkInfo(string username, string password);//����û���������ƥ��
	void menuSelect(LinkList<User*>& list);//�˵�ѡ��
	string getUsername();//��ȡ�û���
	string getPassword();//��ȡ����
	void setPassword(string newPassword);//��������
	void setType(string usertype, LinkList<User*>& list);//�����û�����
	//��̬
	virtual string getType() = 0;//��ȡ�û����ͣ����ز�ͬ����
	virtual void printUser(LinkList<User*>& list) = 0;//ֻ��Root�ܲ鿴
	virtual void enterMenu(LinkList<User*>& list) = 0;//��ͬ�û����벻ͬ�˵�
	virtual int deleteUser(LinkList<User*>& list, LinkList<Contacts>& clist) = 0;//��Rootֻ��ɾ�Լ�
	virtual int fixType(LinkList<User*>& list) = 0;//ֻ��Root���޸��û�����
	virtual void fixPassword(LinkList<User*>& list) = 0;//��Rootֻ�ܸ��Լ�������
	//���������
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
	string getType();//��ȡ�û����ͣ����ز�ͬ����
	void printUser(LinkList<User*>& list);//ֻ��Root�ܲ鿴
	void enterMenu(LinkList<User*>& list);//��ͬ�û����벻ͬ�˵�
	int deleteUser(LinkList<User*>& list, LinkList<Contacts>& clist);//����Ա����ɾ�������û�
	int fixType(LinkList<User*>& list);//ֻ��Root���޸��û�����
	void fixPassword(LinkList<User*>& list);//����Ա�����޸������˵�
};
//================================================================================//
class Common : public User {
public:
	Common(string username = "UNKNWON", string password = "UNKNWON");
	string getType();//��ȡ�û����ͣ����ز�ͬ����
	void printUser(LinkList<User*>& list);//������Ȩ��
	void enterMenu(LinkList<User*>& list);//��ͬ�û����벻ͬ�˵�
	int deleteUser(LinkList<User*>& list, LinkList<Contacts>& clist);//��ͨ�û��Լ�ע��
	int fixType(LinkList<User*>& list);//������Ȩ��
	void fixPassword(LinkList<User*>& list);//�޸��Լ�������
};
//================================================================================//
class Vip : public User {
public:
	Vip(string username = "UNKNWON", string password = "UNKNWON");
	string getType();//��ȡ�û����ͣ����ز�ͬ����
	void printUser(LinkList<User*>& list);//������Ȩ��
	void enterMenu(LinkList<User*>& list);//��ͬ�û����벻ͬ�˵�
	int deleteUser(LinkList<User*>& list, LinkList<Contacts>& clist);//��ͨ�û��Լ�ע��
	int fixType(LinkList<User*>& list);//������Ȩ��
	void fixPassword(LinkList<User*>& list);//�޸��Լ�������
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
	//���������
	Contacts(string tel = "UNKNWON", string mob = "UNKNWON", string qq = "UNKNWON", string name = "UNKNWON", string unit = "UNKNWON", string address = "UNKNWON", string sort = "UNKNWON", string email = "UNKNWON");
	~Contacts();
	//����ʵ��
	void initNode(LinkList<Contacts>& clist);//��ʼ������
	void saveNode(LinkList<Contacts>& clist);//�浵
	//����ʵ��
	string getName();//��ȡ��ϵ������
	void createContacts(LinkList<Contacts>& clist);//�½���ϵ��
	void printContacts(LinkList<Contacts>& clist);//�鿴��ϵ��
	void deleteContacts(LinkList<Contacts>& clist);//ɾ����ϵ��
	void searchContacts(LinkList<Contacts>& clist);//������ϵ��
	void fixContacts(LinkList<Contacts>& clist);//�޸���ϵ��
	void fixInfo(string tel, string mob, string qq, string name, string unit, string address, string sort, string email);
	void swapNode(LinkNode<Contacts>* a, LinkNode<Contacts>* b, LinkList<Contacts>& clist);//�����ڵ�
	void sortContacts(LinkList<Contacts>& clist);//��ϵ������������������
	void shareContacts(LinkList<Contacts>& clist, LinkList<User*>& list);//������ϵ��
	void receiveShare(LinkList<Contacts>& clist);//������ϵ��
	void cloudFunction(LinkList<Contacts>& clist);//�ƶ����ݷ���
	//���������
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