#include<stdio.h>
#include <string.h>
#
#

struct Event {
	char name[51];              // 事件名称，最多50字符
	char date[20];              // 日期，格式YYYY-MM-DD
	char time[10];              // 时间，格式HH:MM
	char description[101];      // 事件描述，最多100字符
	int repeat;				    // 重复类型，0-不重复，1-每天，2-每周，3-每月 

};

void manageDate();// 日期管理
void manageEvents(); // 事件增删改查
void reminders();// 提醒功能
void loadEvents();// 加载事件数据
void saveEvents(); // 保存事件数据


// 主程序
main() {
    loadEvents();       // 加载日程数据
	////////////////////////////////这里的running，showMenu，option需要定义和实现///////////////////////////////
    while (running) {
        showMenu();     // 显示菜单
        switch (option) {
        case 1: manageDate(); break;
        case 2: manageEvents(); break;
        case 3: reminders(); break;
        case 4: saveEvents(); break;
        }
    }
}

// 模块函数
void manageDate() {
    // 日期管理
}





///////////////////////////////////////////////////////////////////
void manageEvents() {// 事件增删改查
    int choice;
	do{
	printf("****************事件管理****************\n");
	printf("1. 添加事件\n");
	printf("2. 删除事件\n");
	printf("3. 修改事件\n");
	printf("4. 查看事件\n");
	printf("5. 返回主菜单\n");
	printf("请选择操作（1-5）：");

	scanf("%d", &choice);

		switch (choice) {
		case 1:add_event(); break;
		case 2:delete_event(); break;
		case 3:modify_event(); break;
		case 4:look_event(); break;
		case 5:return;
		}
	} while (choice != 5);
}
void add_event() {// 添加事件
	printf("\n ****************添加新事件**************** \n");
	printf("请输入日期（格式：YYYY-MM-DD）：");
	



}
void delete_event() {// 删除事件
	printf("\n ****************删除事件**************** \n");
}
void modify_event() {// 修改事件
	printf("\n ****************修改事件**************** \n");
}
void look_event() {// 查看事件
	printf("\n ****************查看事件**************** \n");
}

//////////////////////////////////////////////////////////////////






void reminders() {
    // 提醒功能
}
void loadEvents() {
    // 加载事件数据
}
void saveEvents() {
    // 保存事件数据
}
