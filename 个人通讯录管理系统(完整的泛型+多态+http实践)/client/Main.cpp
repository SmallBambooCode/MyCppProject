﻿#include <iostream>
#define NOMINMAX
#include <windows.h>
#include "MyClass.h"
using namespace std;

int main() {
	//新建用户链表
	LinkList<User*> userList(new Common);
	//初始化用户链表
	userList.getHead()->getData()->initNode(userList);
	//设置CMD标题
	SetConsoleTitle("个人通讯录管理系统_By张嘉伟");
	//修改字体颜色
	system("color 0B");
	//前台死循环
	while (1) {
		int input;
		system("cls");
		cout << "==========================" << endl;
		cout << "[   个人通讯录系统前台   ]" << endl;
		cout << "==========================" << endl;
		cout << "[     [1]通行证登录      ]" << endl;
		cout << "[     [2]注册通行证      ]" << endl;
		cout << "[     [3]关于本系统      ]" << endl;
		cout << "[     [0]退出本系统      ]" << endl;
		cout << "==========================" << endl;
		cout << "请输入序号选择功能：";
		//输入合法性检查
		if (!(cin >> input) || input < 0 || input > 3) {
			cout << "数据输入错误，请重新输入！" << endl;
			//情况输入缓存区
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			Sleep(2000);
			continue;
		}
		//switch-case分支
		switch (input) {
		//退出系统
		case 0:
			//保存当前用户
			userList.getHead()->getData()->saveNode(userList);
			return 0;
		//登录
		case 1:
			//判断登录返回值
			if (userList.getHead()->getData()->signIn(userList) != 1) {
				continue;
			}
			//成功登录后进入用户菜单选项
			userList.getHead()->getData()->menuSelect(userList);
			break;
		//注册
		case 2:
			userList.getHead()->getData()->signUp(userList);
			userList.getHead()->getData()->menuSelect(userList);
			break;
		//关于
		case 3:
			system("cls");
			cout << "====================================================\n";
			cout << "[              关于个人通讯录管理系统              ]\n";
			cout << "====================================================\n";
			cout << "[ 本系统拥有多用户管理，包括普通用户，VIP，管理员  ]\n";
			cout << "[ 包括基本的联系人新建，删除，修改，查询，排序功能 ]\n";
			cout << "[ 基本的用户注册，登录，改密，注销，权限组修改功能 ]\n";
			cout << "====================================================\n";
			cout << "[    程序特色：密码哈希加密保存，多功能分页显示    ]\n";
			cout << "[    多用户支持，联系人分享，统一认证，日志系统    ]\n";
			cout << "[    云端数据备份恢复，严格的输入检测等等          ]\n";
			cout << "====================================================\n";
			cout << "[ 作者:23人工智能 张嘉伟 ][ 感谢您的使用!ヾ(≧▽≦*)o ]\n";
			cout << "====================================================\n";
			cout << "输入0退出：";
			while (getchar() != '0');
			continue;
		default:
			return 0;
		}
	}
}

//泛型测试
int main2() {
	LinkList<double> test;
	test.addNode(3.14);
	test.addNode(4.15);
	cout << test;
	LinkNode<string> test2("哈哈哈哈哈哈");
	cout << test2 << endl;
	Contacts c1("1","2","3","4","5","6","7","8");
	LinkNode<Contacts> test3(c1);
	cout << test3 << endl;
	Common c2("UCommon", "我是密码阿巴阿巴Common");
	Vip c3("UVip", "我是密码阿巴阿巴Vip");
	cout << c2 << endl << c3 << endl;
	LinkList<string> n1;
	n1.addNode("读书的女人最美丽");
	n1.addNode("读书的女人最优秀");
	n1.addNode("读书的女人最通情达理");
	cout << n1;
	return 0;
}