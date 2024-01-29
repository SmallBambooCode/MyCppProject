#include <iostream>
#include "MyClass.h"
#include "TemplateFunc.cpp"
#define NOMINMAX
#include <windows.h>
using namespace std;
int main() {
	//新建用户链表头
	User* userHead = new Common;
	//初始化用户链表
	userHead->initNode();
	//设置CMD标题
	SetConsoleTitle("个人通讯录管理系统_By张嘉伟");
	//修改自己颜色
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
			userHead->saveNode(userHead);
			//调用函数模板，::是调用全局函数，退出系统时删除链表
			::deleteAll(userHead);
			//删除链表头节点
			delete userHead;
			//头节点赋空
			userHead = nullptr;
			return 0;
		//登录
		case 1:
			//判断登录返回值
			if (userHead->signIn(userHead) != 1) {
				continue;
			}
			//成功登录后进入用户菜单选项
			userHead->menuSelect(userHead);
			break;
		//注册
		case 2:
			userHead->signUp(userHead);
			userHead->menuSelect(userHead);
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