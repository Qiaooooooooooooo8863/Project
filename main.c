#include <stdio.h>
#include "date/date_manage.h"  // 引用日期模块

// 变量声明
int running = 1;
int option;

// 函数声明
void showMenu();
void manageEvents(); 
void reminders();
void loadEvents();
void saveEvents();

struct Event { 
    char name[50]; 
    char date[20];
    char time[10]; 
    char description[100]; 
};

// 主程序
int main() {
    loadEvents();    // 加载日程数据
    while (running) {
        showMenu();    // 显示菜单
        scanf("%d", &option);  // 添加输入
        switch(option) {
        case 1: manageDate(); break;  // 调用日期模块
        case 2: manageEvents(); break;
        case 3: reminders(); break;
        case 4: saveEvents(); running = 0; break;
        default: printf("无效选择!\n");
        }
    }
    return 0;
}

// 菜单函数
void showMenu() {
    printf("\n=== 日程管理系统 ===\n");
    printf("1. 日期管理\n");
    printf("2. 日程管理\n");
    printf("3. 提醒功能\n");
    printf("4. 退出系统\n");
    printf("请选择：");
}


// 模块函数
void manageDate() {
    // 实现在 date/date_manage.c 中
}
void manageEvents() {
    printf("日程管理功能开发中...\n");
}
void reminders() {
    printf("提醒功能开发中...\n");
}
void loadEvents() {
    // 加载事件数据
}
void saveEvents() {
    // 保存事件数据
}