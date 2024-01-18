//一定要让程序正常退出，也就是按0逐步退出，不要在运行时关闭窗口，否则部分数据不会存档，数据保存在析构函数中被调用
#include <iostream>
#include "myclass.h"
//用于解决cin.ignore(numeric_limits<streamsize>::max(), '\n');报错
#define NOMINMAX
#include <windows.h>
#include <iomanip>
using namespace std;
int main() {
	SetConsoleTitle("个人通讯录管理系统_By张嘉伟");
	system("color 0B");
	uList user;
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
				return 0;
			case 1:
				if (user.signIn() != 1) {
					continue;
				}
				break;
			case 2:
				user.signUp();
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
				cout << "[    多用户支持，联系人分享，统一认证登录，严格    ]\n";
				cout << "[    的输入检测等等    ]\n";
				cout << "====================================================\n";
				cout << "[ 作者:23人工智能 张嘉伟 ][ 感谢您的使用!ヾ(≧▽≦*)o ]\n";
				cout << "====================================================\n";
				cout << "输入0退出：";
				while (getchar() != '0');
				continue;
			default:
				return 0;
		}
		List cL(user.getcurrUser()->getName());
		if (user.getcurrUser()->getType() == "User") {
			cL.receiveShare();
			int flag = 0;
			while (1) {
				int input;
				system("cls");
				cout << "================================" << endl;
				cout << "[      个人通讯录管理系统      ]" << endl;
				cout << "[      普通用户：" << left << setw(8) << user.getcurrUser()->getName() << "      ]" << endl;
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
						break;
					case 1:
						cL.createContacts();
						break;
					case 2:
						cL.printNode();
						break;
					case 3:
						cL.deleteContacts();
						break;
					case 4:
						cL.fixContacts();
						break;
					case 5:
						cL.shareContacts(user.gethead());
						break;
					case 6:
						user.fixPassword();
						break;
					case 7:
						if (user.deleteUser()) {
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
		}
		else if (user.getcurrUser()->getType() == "Vip") {
			cL.receiveShare();
			int flag = 0;
			while (1) {
				int input;
				system("cls");
				cout << "================================" << endl;
				cout << "[      个人通讯录管理系统      ]" << endl;
				cout << "[      VIP用户：" << left << setw(8) << user.getcurrUser()->getName() << "       ]" << endl;
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
				cout << "[        [0]退出本系统         ]" << endl;
				cout << "================================" << endl;
				cout << "请输入序号选择功能：";
				if (!(cin >> input) || input < 0 || input > 9) {
					cout << "数据输入错误，请重新输入！" << endl;
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					Sleep(2000);
					continue;
				}
				switch (input) {
					case 0:
						flag = 1;
						break;
					case 1:
						cL.createContacts();
						break;
					case 2:
						cL.printNode();
						break;
					case 3:
						cL.searchContacts();
						break;
					case 4:
						cL.deleteContacts();
						break;
					case 5:
						cL.fixContacts();
						break;
					case 6:
						cL.shareContacts(user.gethead());
						break;
					case 7:
						cout << "排序中，请稍等，数据量大时可能耗费时间较长。" << endl;
						cL.sortContacts();
						system("cls");
						cL.printNode();
						break;
					case 8:
						user.fixPassword();
						break;
					case 9:
						if (user.deleteUser()) {
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
		}
		else if (user.getcurrUser()->getType() == "Root") {
			int flag = 0;
			while (1) {
				int input;
				system("cls");
				cout << "================================" << endl;
				cout << "[      个人通讯录管理系统      ]" << endl;
				cout << "[      管理员用户：" << left << setw(8) << user.getcurrUser()->getName() << "    ]" << endl;
				cout << "================================" << endl;
				cout << "[        [1]查看通行证         ]" << endl;
				cout << "[        [2]修改权限组         ]" << endl; 
				cout << "[        [3]修改通行证         ]" << endl;
				cout << "[        [4]删除通行证         ]" << endl;
				cout << "[        [0]退出本系统         ]" << endl;
				cout << "================================" << endl;
				cout << "请输入序号选择功能：";
				if (!(cin >> input) || input < 0 || input > 4) {
					cout << "数据输入错误，请重新输入！" << endl;
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					Sleep(2000);
					continue;
				}
				switch (input) {
					case 0:
						flag = 1;
						break;
					case 1:
						user.printNode();
						break;
					case 2:
						user.fixUserType();
						break;
					case 3:
						user.fixPassword();
						break;
					case 4:
						if (user.deleteUser()) {
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
		}
	}
	return 0;
}
/*
							   _ooOoo_
							  o8888888o
							  88" . "88
							  (| -_- |)
							  O\  =  /O
						   ____/`---'\____
						 .'  \\|     |//  `.
						/  \\|||  :  |||//  \
					   /  _||||| -:- |||||-  \
					   |   | \\\  -  /// |   |
					   | \_|  ''\---/''  |   |
					   \  .-\__  `-`  ___/-. /
					 ___`. .'  /--.--\  `. . __
				  ."" '<  `.___\_<|>_/___.'  >'"".
				 | | :  `- \`.;`\ _ /`;.`/ - ` : | |
				 \  \ `-.   \_ __\ /__ _/   .-` /  /
			======`-.____`-.___\_____/___.-`____.-'======
							   `=---='
			^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
					   佛祖保佑        永无BUG
			  佛曰:
					 写字楼里写字间，写字间里程序员；
					 程序人员写程序，又拿程序换酒钱。
					 酒醒只在网上坐，酒醉还来网下眠；
					 酒醉酒醒日复日，网上网下年复年。
					 宁愿老死程序间，只要老板多发钱;
					 小车大房不去想，撰个二千好过年。
					 若要见识新世面，公务员比程序员；
					 一个在天一在地，而且还比我们闲。
					 别人看我穿白领，我看别人穿名牌；
					 天生我才写程序，臀大近视肩周炎。
					 年复一年春光度，度得他人做老板；
					 老板扣我薄酒钱，没有酒钱怎过年。
					 春光逝去皱纹起，作起程序也委靡；
					 来到水源把水灌，打死不做程序员。
					 别人笑我忒疯癫，我笑他人命太贱；
					 状元三百六十行，偏偏来做程序员。
					 但愿老死电脑间，不愿鞠躬老板前；
					 奔驰宝马贵者趣，公交自行程序员。
					 别人笑我忒疯癫，我笑自己命太贱；
					 不见满街漂亮妹，哪个归得程序员。
					 不想只挣打工钱，那个老板愿发钱；
					 小车大房咱要想，任我享用多悠闲。
					 比尔能搞个微软，我咋不能捞点钱；
					 一个在天一在地，定有一日乾坤翻。
					 我在天来他在地，纵横天下山水间；
					 傲视武林豪杰墓，一樽还垒风月山。
					 电脑面前眼发直，眼镜下面泪茫茫；
					 做梦发财好几亿，从此不用手指忙。
					 哪知梦醒手空空，老板看到把我训；
					 待到老时眼发花，走路不知哪是家。
					 小农村里小民房，小民房里小民工；
					 小民工人写程序，又拿代码讨赏钱。
					 钱空只在代码中，钱醉仍在代码间；
					 有钱无钱日复日，码上码下年复年。
					 但愿老死代码间，不愿鞠躬奥迪前，
					 奥迪奔驰贵者趣，程序代码贫者缘。
					 若将贫贱比贫者，一在平地一在天；
					 若将贫贱比车马，他得驱驰我得闲。
					 别人笑我忒疯癫，我笑他人看不穿；
					 不见盖茨两手间，财权富贵世人鉴。

*/