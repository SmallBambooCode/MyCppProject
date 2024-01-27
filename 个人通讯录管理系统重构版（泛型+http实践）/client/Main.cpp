#include <iostream>
#include "MyClass.h"
#include "TemplateFunc.cpp"
#define NOMINMAX
#include <windows.h>
using namespace std;
User* userHead = new Common;
int main() {
	userHead->initNode();
	SetConsoleTitle("个人通讯录管理系统_By张嘉伟");
	system("color 0B");
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
		if (!(cin >> input) || input < 0 || input > 3) {
			cout << "数据输入错误，请重新输入！" << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			Sleep(2000);
			continue;
		}
		switch (input) {
		case 0:
			userHead->saveNode(userHead);
			::deleteAll(userHead);
			delete userHead;
			userHead = nullptr;
			return 0;
		case 1:
			if (userHead->signIn(userHead) != 1) {
				continue;
			}
			userHead->menuSelect(userHead);
			break;
		case 2:
			userHead->signUp(userHead);
			userHead->menuSelect(userHead);
			break;
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
	userHead->saveNode(userHead);
	return 0;
}