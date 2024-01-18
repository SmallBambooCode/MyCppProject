#pragma once
#include <iostream>
using namespace std;
class User {
private:
	string usertype;
	string username;
	string password;
	User* next;
public:
	User(string usertype, string username, string password);
	string getType();
	User* getNext();
	void setNext(User* node);
	void setType(string type);
	void printInfo();
	void saveToFile(ofstream& saveFile);
	int check(string username, string password);
	string getName();
	void fixPassword(string password);
};
class uList {
private:
	User* currUser;
	User* head;
	//链表基操
public:
	uList();
	~uList();
	void initNode();
	User* addNode(string usertype, string username, string password);
	void saveNode();
	User* gethead();
	//功能实现
public:
	int signIn();
	void signUp();
	User* getcurrUser();
	void printNode();
	void fixUserType();
	void fixPassword();
	int deleteUser();
	void encrype(string& password);
};
class contacts {
private:
	string telephone;
	string mobilephone;
	string qq;
	string name;
	string unit;
	string address;
	string sort;
	string email;
	contacts* next;
public:
	contacts(string tel, string mob, string qq, string name, string unit, string address, string sort, string email);
	void setNext(contacts* node);
	contacts* getNext();
	string getName();
	void printInfo();
	int searchInfo(int type, string content);
	void saveToFile(ofstream& saveFile, string username);
	void fixInfo(string tel, string mob, string qq, string name, string unit, string address, string sort, string email);
};
class List {
private:
	string username;
	contacts* head;
	//链表基操
public:
	List(string username);
	~List();
	void initNode();
	void addNode(string tel, string mob, string qq, string name, string unit, string address, string sort, string email);
	void saveNode();
	//功能实现
public:
	void printNode();
	void createContacts();
	void searchContacts();
	void deleteContacts();
	void fixContacts();
	void swapNode(contacts* x, contacts* y);
	void sortContacts();
	void shareContacts(User* userhead);
	void receiveShare();
};