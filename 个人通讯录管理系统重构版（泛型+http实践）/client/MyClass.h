#pragma once
#include <iostream>
using namespace std;
//��ǰ����Contacts����ֹUser�в�֪����Contacts
class Contacts;
//================================================================================//
//================================================================================//
class User {
private:
	string username;
	string password;
	User* next;
public:
	//���������
	User(string username = "UNKNWON", string password = "UNKNWON");
	~User();
	//����ʵ��
	void initNode();//��ʼ������
	//void addNode(User* newNode);//�½��ڵ�
    void saveNode(User* userHead);//�浵
    User* getNext();//��ȡnextָ��
	void setNext(User* next);//����nextָ��
	//void deleteNode(User* userHead);//ɾ��������������Ķ��󣨽ڵ㣩
	//void deleteAll(User* userHead);//ɾ������
	//����ʵ��
    void signUp(User* userHead);//ע��
    int signIn(User* userHead);//��¼
    int checkInfo(string username, string password);//����û���������ƥ��
    void menuSelect(User* userHead);//�˵�ѡ��
	string getUsername();//��ȡ�û���
    string getPassword();//��ȡ����
    void setPassword(string newPassword);//��������
	void setType(string usertype, User* userHead);//�����û�����
	//��̬
	virtual string getType() = 0;//��ȡ�û����ͣ����ز�ͬ����
	virtual void printNode(User* userHead) = 0;//ֻ��Root�ܲ鿴
    virtual void enterMenu(User* userHead) = 0;//��ͬ�û����벻ͬ�˵�
    virtual int deleteUser(User* userHead, Contacts* contactsHead) = 0;//��Rootֻ��ɾ�Լ�
    virtual int fixType(User* userHead) = 0;//ֻ��Root���޸��û�����
    virtual void fixPassword(User* userHead) = 0;//��Rootֻ�ܸ��Լ�������
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
    //���������
    Contacts(string tel = "UNKNWON", string mob = "UNKNWON", string qq = "UNKNWON", string name = "UNKNWON", string unit = "UNKNWON", string address = "UNKNWON", string sort = "UNKNWON", string email = "UNKNWON");
    ~Contacts();
	//����ʵ��
	void initNode();//��ʼ������
	//void addNode(Contacts* newNode);//�½��ڵ�
	void saveNode(Contacts* contactsHead);//�浵
	Contacts* getNext();//��ȡnextָ��
	void setNext(Contacts* next);//����nextָ��
	//void deleteNode(Contacts* contactsHead);//ɾ���ڵ�
	//void deleteAll(Contacts* contactsHead);//ɾ��contactsHead�������������
	//����ʵ��
	string getName();//��ȡ��ϵ������
	void createContacts(Contacts* contactsHead);//�½���ϵ��
	void printContacts(Contacts* contactsHead);//�鿴��ϵ��
	void deleteContacts(Contacts* contactsHead);//ɾ����ϵ��
	void searchContacts(Contacts* contactsHead);//������ϵ��
	void fixContacts(Contacts* contactsHead);//�޸���ϵ��
	void fixInfo(string tel, string mob, string qq, string name, string unit, string address, string sort, string email);
	void swapNode(Contacts* x, Contacts* y, Contacts* contactsHead);//�����ڵ�
	void sortContacts(Contacts* contactsHead);//��ϵ������������������
	void shareContacts(Contacts* contactsHead, User* userHead);//������ϵ��
	void receiveShare(Contacts* contactsHead);//������ϵ��
	void cloudFunction(Contacts* contactsHead);
};