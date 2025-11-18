#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// 事件结构体定义
typedef struct Event {
    char name[50];
    char date[20];
    char time[10];
    char description[100];
    int remind; // 提醒标志
    int repeat; // 重复类型: 0-不重复, 1-每天, 2-每周
    struct Event* next;
} Event;

// 全局变量声明
Event* eventList = NULL;
int running = 1;
int option;
char eventDescriptions[100][100] = { 0 };
int eventReminders[100] = { 0 };
int eventRepeatTypes[100] = { 0 };

char name[50] = { 0 };
char date[20] = { 0 };
char eventTime[10] = { 0 }; // 重命名避免冲突
char description[100] = { 0 };

#define MAX_USERS 100  
typedef struct {
    char username[20];
    char password[20];
} User;
User users[MAX_USERS];
#define MAX_EVENTS 200  
char eventCreators[MAX_EVENTS][20];
#define MAX_SHARES 500  
typedef struct {
    int eventId;
    char sharedUser[20];
    int permission;
} SharedEvent;
SharedEvent sharedEvents[MAX_SHARES];
int currentUserIndex = -1;
int userCount = 0;
int eventShared[MAX_EVENTS] = { 0 };
int eventCount = 0;
int sharedCount = 0;
char eventNames[MAX_EVENTS][50];
char eventDates[MAX_EVENTS][11];
char eventTimes[MAX_EVENTS][6];

// 函数声明
void showMenu();
void manageDate();
void manageEvents();
void reminders();
void loadEvents();
void saveEvents();
int shareEventToUser(int eventId, const char* targetUser, int permission);
int checkEventPermission(int eventId, int needEdit);
void showAllAccessibleEvents();

// 补充函数
int CorrectDate(const char* date);
int Weekday(int year, int month, int day);
void DateAdd();
int MonthDay(int year, int month);
void Datejian();
void manageSharedEvents();

// 新增函数声明
void addEvent();
void delete_Event();
void change_Event();
void look_Events();
void look_date_Events(const char* date);
void reminde_Events();
int check_Date(const char* date);
int check_Time(const char* time);
void check_remind_Events();
void clearInputBuffer();

// 主程序
int main()
{
    loadEvents();    // 加载日程数据
    while (running) {
        showMenu();    // 显示菜单
        scanf("%d", &option);
        clearInputBuffer();
        switch (option) {
        case 1: manageDate(); break;
        case 2: manageEvents(); break;
        case 3: reminders(); break;
        case 4: saveEvents(); running = 0; break;
        case 5: manageSharedEvents(); break;
        default: printf("无效选择!\n");
        }
    }
    return 0;
}

// 清除输入缓冲区
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// 菜单函数
void showMenu() {
    printf("\n=== 日程管理系统 ===\n");
    printf("1. 日期管理\n");
    printf("2. 日程管理\n");
    printf("3. 提醒功能\n");
    printf("4. 退出系统\n");
    printf("5. 共享日程\n");
    printf("请选择：");
}

// 检查日期格式
int check_Date(const char* date) {
    return CorrectDate(date);
}

// 检查时间格式
int check_Time(const char* time) {
    int hour, minute;
    if (sscanf(time, "%d:%d", &hour, &minute) != 2)
        return 0;
    if (hour < 0 || hour > 23 || minute < 0 || minute > 59)
        return 0;
    return 1;
}

//-----------------------事件管理系统----------
void manageEvents() {
    int choice;
    //事件管理菜单
    do {
        printf("\n--------------------- 事件管理 --------------------\n");
        printf("1. 添加事件\n");
        printf("2. 删除事件\n");
        printf("3. 修改事件\n");
        printf("4. 查看特定日期事件\n");
        printf("5. 返回主菜单\n");
        printf("请选择:1~5 ");
        scanf("%d", &choice);
        getchar();              //获取\n

        switch (choice) {
        case 1: addEvent(); break;      // 加入事件
        case 2: delete_Event(); break;   // 删掉事件
        case 3: change_Event(); break;   // 修改事件



        case 4: {           //查看特定的日期事件
            char date[20];
            printf("请输入日期: ");
            scanf("%s", date);
            look_date_Events(date);   //‘查看’的函数
            break;
        }


        case 5: return;//返回菜单
        default: printf("请输入1~5，谢谢\n");
        }
    } while (choice != 5);
}  

// 添加事件函数（链表版本）
void addEvent() {
    Event* newEvent = (Event*)malloc(sizeof(Event));
    printf("\n---添加新事件---\n");

    // 输入事件名称
    printf("请输入事件名称: ");
    fgets(newEvent->name, sizeof(newEvent->name), stdin);
    newEvent->name[strcspn(newEvent->name, "\n")] = 0;

    // 日期输入
    do {
        printf("日期 (YYYY-MM-DD): ");
        fgets(newEvent->date, sizeof(newEvent->date), stdin);
        newEvent->date[strcspn(newEvent->date, "\n")] = 0;
        if (!check_Date(newEvent->date)) {
            printf("格式错误!!!\n");
        }
    } while (!check_Date(newEvent->date));

    // 时间输入
    do {
        printf("时间 (HH:MM): ");
        fgets(newEvent->time, sizeof(newEvent->time), stdin);
        newEvent->time[strcspn(newEvent->time, "\n")] = 0;
        if (!check_Time(newEvent->time)) {
            printf("格式错误!!!\n");
        }
    } while (!check_Time(newEvent->time));

    // 描述
    printf("事件的描述：");
    fgets(newEvent->description, sizeof(newEvent->description), stdin);
    newEvent->description[strcspn(newEvent->description, "\n")] = 0;

    // 提醒
    printf("是否设置提醒？(0-否，1-是)：");
    scanf("%d", &newEvent->remind);
    clearInputBuffer();

    printf("重复类型 (0-不重复，1-每天，2-每周)：");
    scanf("%d", &newEvent->repeat);
    clearInputBuffer();

    // 添加到链表
    newEvent->next = eventList;
    eventList = newEvent;

    printf("\n事件添加成功！\n");
}

// 查看所有事件
void look_Events() {
    printf("\n---所有日程事件---\n");
    Event* now = eventList;
    int count = 1;

    // 遍历事件链表并显示
    while (now != NULL) {
        printf("%d. %s | %s %s | %s\n", count++, now->name, now->date, now->time, now->description);
        now = now->next;
    }
}

// 查看特定日期事件
void look_date_Events(const char* date) {
    // 验证日期格式
    if (!check_Date(date)) {
        printf("日期格式错误！\n");
        return;
    }

    // 显示该日期的事件
    printf("\n---%s 的事件安排---\n", date);
    Event* now = eventList;
    int num = 0;

    // 当天所有事件
    while (now != NULL) {
        if (strcmp(now->date, date) == 0) {
            printf("%s | %s | %s\n", now->name, now->time, now->description);
            num = 1;
        }
        now = now->next;
    }

    // 当天没有事件安排
    if (!num) {
        printf("该日期暂无事件安排。\n");
    }
}

// 删除事件
void delete_Event() {
    // 检查是否有事件
    if (eventList == NULL) {
        printf("暂无事件可删除！\n");
        return;
    }

    printf("\n---删除事件---\n");
    // 展示要删的事件
    look_Events();

    int index = 0;
    int i = 1;
    printf("请输入要删除的事件编号：");
    scanf("%d", &index);
    clearInputBuffer();

    // 删除事件
    Event* now = eventList;
    Event* p = NULL;

    // 找到要删除的事件
    while (now != NULL && i < index) {
        p = now;
        now = now->next;
        i++;
    }

    if (now == NULL) {
        printf("事件编号不存在！\n");
        return;
    }

    // 从链表中移除事件
    if (p == NULL) { // 如果删除的是第一个事件
        eventList = now->next;
    }
    else {
        p->next = now->next;
    }
    free(now); // 释放内存！
    printf("事件删除成功！\n");
}

// 修改事件
void change_Event() {
    // 检查有没有事件
    if (eventList == NULL) {
        printf("\n暂无事件可修改！\n");
        return;
    }

    printf("\n---修改事件---\n");
    // 展示所有的事件
    look_Events();

    int n = 1;
    int i = 1;
    printf("请输入要修改的事件编号: ");
    scanf("%d", &n);
    clearInputBuffer();

    // 找到要修改的事件的位置
    Event* now = eventList;
    while (now != NULL && i < n) {
        now = now->next;
        i++;
    }

    // 不存在
    if (now == NULL) {
        printf("事件编号不存在！\n");
        return;
    }

    printf("修改事件：%s\n", now->name); // 输出名称

    char input[100]; // 输入新的信息
    printf("新事件名称 (原: %s): ", now->name);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;
    if (strlen(input) > 0) strcpy(now->name, input);

    printf("新日期 (原: %s): ", now->date);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;
    if (strlen(input) > 0 && check_Date(input)) {
        strcpy(now->date, input);
    }

    printf("新时间 (原: %s): ", now->time);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;
    if (strlen(input) > 0 && check_Time(input)) {
        strcpy(now->time, input);
    }

    printf("新描述 (原: %s): ", now->description);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;
    if (strlen(input) > 0) strcpy(now->description, input);

    printf("事件修改成功！\n");
}

// 检查提醒
void check_remind_Events() {
    printf("\n---提醒检查---\n");
    Event* now = eventList;
    int found = 0;

    while (now != NULL) {
        if (now->remind) {
            printf("提醒: %s | %s %s | %s\n",
                now->name, now->date, now->time, now->description);
            found = 1;
        }
        now = now->next;
    }

    if (!found) {
        printf("暂无设置提醒的事件\n");
    }
}

// 提醒功能
void reminders() {
    int choice;
    do {
        printf("\n---提醒功能---\n");
        printf("1. 检查提醒\n");
        printf("2. 返回主菜单\n");
        printf("请选择: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
        case 1: check_remind_Events(); break;
        case 2: return;
        default: printf("请输入1~2\n");
        }
    } while (choice != 2);
}

// 模块函数
void manageDate()
{
    int choice;
    do
    {
        printf("\n---日期管理---\n");
        printf("1.查询日期信息\n");
        printf("2.日期加法运算\n");
        printf("3.日期减法运算\n");//子菜单
        printf("4.返回主菜单\n");
        printf("请选择：");
        scanf("%d", &choice);

        switch (choice)//根据用户选择执行相应功能
        {
        case 1://功能1：查询日期详细信息
        {
            char date[20];//存储用户输入的日期
            printf("请输入日期(YYYY-MM-DD):");
            scanf("%s", date);

            if (!CorrectDate(date))//!表示日期格式错误，函数返回值为0
            {
                printf("日期格式错误!\n");
                break;
            }

            int year, month, day;
            sscanf(date, "%d-%d-%d", &year, &month, &day);

            //计算星期几
            int weekday = Weekday(year, month, day);//调用函数
            const char* weekdays[] = { "日","一","二","三","四","五","六" };
            printf("%d年%d月%d日是星期%s\n", year, month, day, weekdays[weekday]);


            //显示月份天数
            int monthday;
            if (month == 2)
                monthday = (((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))) ? 29 : 28;//闰年判断
            else if (month == 4 || month == 6 || month == 9 || month == 11)
                monthday = 30;
            else
                monthday = 31;
            printf("%d年%d月有%d天\n", year, month, monthday);


            break;
        }

        case 2://功能2：日期加法-计算日期加上指定天数后的新日期
        {
            DateAdd();
            break;
        }

        case 3://功能3：日期减法-计算日期减上指定天数后的新日期
        {
            Datejian();
            break;
        }

        case 4://功能4：返回主菜单
            return;//直接退出manageDate函数

        default://如果输出不是1234，执行：
            printf("无效选择！\n");
        }
    } while (choice != 4);//单选择4时，退出循环
}


int CorrectDate(const char* date)//1正确  0错误
{
    int year, month, day;
    if (sscanf(date, "%d-%d-%d", &year, &month, &day) != 3)
        return 0;
    if (month < 1 || month>12)
        return 0;
    if (day < 1 || day>31)
        return 0;
    if (month == 2)
    {
        if (day > (((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) ? 29 : 28))//  1?2:3表示若1成立，就是2，不成立则为3 
            return 0;
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11)
    {
        if (day > 30)
            return 0;
    }
    return 1;//除特殊条件外，输入的其他日期有效
}


int Weekday(int year, int month, int day)//计算星期几
{
    int monthday[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };//基础月份
    long totalday = 0;

    //1.计算年份天数
    for (int y = 2000; y < year; y++)//从2000年开始算总天数,已知2000年1月1日为星期六
    {
        totalday += 365;//每年基础天数365天

        if ((y % 4 == 0 && y % 100 != 0) || y % 400 == 0)//闰年多加一天
            totalday++;
    }

    //2，计算月份天数
    for (int m = 0; m < month - 1; m++)
    {
        totalday += monthday[m];
    }
    //闰年特殊处理
    if (month > 2 && ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0))//输入月份大于2，才包含2月份，从而考虑闰年28，29问题
        totalday++;


    //3.计算日期天数
    totalday += day - 1;//清除2000年的1月1日

    int weekday = (6 + totalday) % 7;//6=2000年1月1日星期六
    //totalday为从开始到输入日期的总天数
    return weekday;//返回结果：0周日，1周一...6周六


}


void DateAdd()//日期加法运算
{
    char date[20];
    int add;
    printf("请输入日期(YYYY-MM-DD):");
    scanf("%s", date);
    printf("请输入要加的天数：");
    scanf("%d", &add);

    int year, month, day;
    if (sscanf(date, "%d-%d-%d", &year, &month, &day) != 3)
    {
        printf("日期格式错误!\n");
        return;

    }

    //计算
    for (int i = 0; i < add; i++)
    {
        day++;

        //检查是否跳到下一月
        if (day > MonthDay(year, month))//每个月的月份不同，又要引用函数
        {
            day = 1;
            month++;

            //检查是否跳到下一年
            if (month > 12)
            {
                month = 1;
                year++;
            }
        }

    }
    printf("\n%d天后的日期是%04d年%02d月%02d日\n", add, year, month, day);

}


int MonthDay(int year, int month)
{
    int Month[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
    if (month == 2)
    {
        if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
            return 29;
    }
    return Month[month - 1];
}


void Datejian()//日期减法运算
{
    char date[20];
    int jian;
    printf("请输入日期(YYYY-MM-DD):");
    scanf("%s", date);
    printf("请输入要减的天数：");
    scanf("%d", &jian);

    int year, month, day;
    if (sscanf(date, "%d-%d-%d", &year, &month, &day) != 3)
    {
        printf("日期格式错误!\n");
        return;
    }

    //计算
    for (int i = 0; i < jian; i++)
    {
        day--;

        //检查是否跳到上一月
        if (day < 1)
        {
            month--;

            //检查是否跳到上一年
            if (month < 1)
            {
                month = 12;
                year--;
            }
            day = MonthDay(year, month);
        }

    }
    printf("\n%d天前的日期是%04d年%02d月%02d日\n", jian, year, month, day);

}

void loadEvents()
{
    FILE* file = fopen("calender_data.txt", "r");//read,只读模式

    if (file == NULL)
    {
        printf("提示:没有找到保存的数据文件\n");
        eventCount = 0;
        return;
    }

    fscanf(file, "%d", &eventCount);

    //安全检查
    if (eventCount > 100)
    {
        printf("警告:数据异常,将重置\n");
        eventCount = 0;
        fclose(file);//很重要，有利于数据完整性，打开了就要关上
        return;
    }

    for (int i = 0; i < eventCount; i++)
    {
        //fscanf输入到文件
        fscanf(file, "%s\n", eventNames[i]);//事件名称
        fscanf(file, "%s\n", eventDates[i]);//日期
        fscanf(file, "%s\n", eventTimes[i]);//时间
        fscanf(file, "%s\n", eventDescriptions[i]);//描述
        fscanf(file, "%d\n", eventReminders[i]);//提醒时间
        fscanf(file, "%d\n", eventRepeatTypes[i]);//重复类型
    }

    fclose(file);
    printf("数据加载成功!共加载%d个事件\n", eventCount);
}




void saveEvents()
{
    FILE* file;//声明一个文件类型的指针变量file来存储地址
    file = fopen("calender_data.txt", "w");//fopen意为打开文件,("1","2"),1代表文件名，2代表打开模式
    //打开名为calender_date.txt的文件，只写write

    //fopen里只包含操作信息，不包含文件内容，这个函数返回控制块地址


    if (file == NULL)//不检查文件打开是否成功的话，程序会崩溃
    {
        printf("保存失败:无法创建文件!\n");
        return;//立即停止函数执行，不再继续后面的代码
    }


    //写入事件数量（第一行）
    fprintf(file, "%d\n", eventCount);

    //逐个保存每个事件的信息
    for (int i = 0; i < eventCount; i++)
    {
        //fprintf输出到文件
        fprintf(file, "%s\n", eventNames[i]);//事件名称
        fprintf(file, "%s\n", eventDates[i]);//日期
        fprintf(file, "%s\n", eventTimes[i]);//时间
        fprintf(file, "%s\n", eventDescriptions[i]);//描述
        fprintf(file, "%d\n", eventReminders[i]);//提醒时间
        fprintf(file, "%d\n", eventRepeatTypes[i]);//重复类型
        //
    }

    fclose(file);
    printf("数据保存成功!共保存%d个事件\n", eventCount);
}


void manageSharedEvents() {
    int choice, eventId, permission;
    char targetUser[20];
    printf("\n--- 共享日程管理 ---\n");
    printf("1. 共享事件给用户\n");
    printf("2. 查看我的共享事件\n");
    printf("请选择：");
    scanf("%d", &choice);

    int res = 0;
    switch (choice) {
    case 1:
        printf("输入事件ID：");
        scanf("%d", &eventId);
        printf("输入目标用户名：");
        scanf("%s", targetUser);
        printf("设置权限（0=仅查看，1=可编辑）：");
        scanf("%d", &permission);
        res = shareEventToUser(eventId, targetUser, permission);
        if (res == -1) printf("[失败] 参数错误或事件不存在\n");
        else if (res == -2) printf("[失败] 无权限（非事件创建者）\n");
        else if (res == -3) printf("[失败] 目标用户不存在\n");
        break;
    case 2:
        showAllAccessibleEvents(); // 调用辅助函数显示可访问事件
        break;
    default:
        printf("无效选项！\n");
    }
}

// 函数功能：显示当前用户可访问的所有事件（个人+共享）
void showAllAccessibleEvents() {
    if (currentUserIndex == -1) {
        printf("请先登录！\n");
        return;
    }

    printf("\n=== 可访问的事件列表（用户：%s）===\n", users[currentUserIndex].username);
    int count = 0;

    // 1. 遍历所有事件，筛选可访问的事件
    for (int i = 0; i < eventCount; i++) {
        // 权限判断：创建者或被共享用户
        int isCreator = (strcmp(eventCreators[i], users[currentUserIndex].username) == 0);
        int isShared = 0;
        for (int j = 0; j < sharedCount; j++) {
            if (sharedEvents[j].eventId == i &&
                strcmp(sharedEvents[j].sharedUser, users[currentUserIndex].username) == 0) {
                isShared = 1;
                break;
            }
        }

        if (isCreator || isShared) {
            count++;
            printf("\n事件ID：%d（%s）\n", i, isCreator ? "个人事件" : "共享事件");
            printf("名称：%s\n", eventNames[i]);
            printf("日期时间：%s %s\n", eventDates[i], eventTimes[i]);
            printf("创建者：%s\n", eventCreators[i]);
            // 显示权限（共享事件专属）
            if (isShared) {
                for (int j = 0; j < sharedCount; j++) {
                    if (sharedEvents[j].eventId == i &&
                        strcmp(sharedEvents[j].sharedUser, users[currentUserIndex].username) == 0) {
                        printf("权限：%s\n", sharedEvents[j].permission ? "可编辑" : "仅查看");
                        break;
                    }
                }
            }
        }
    }

    if (count == 0) printf("暂无可访问的事件！\n");
}
// 函数功能：共享事件给指定用户
// 参数：
//   eventId：待共享的事件ID（需存在且为当前用户创建）
//   targetUser：被共享的用户名（需已注册）
//   permission：权限类型（0=仅查看，1=可编辑）
// 返回值：0=成功，-1=参数错误，-2=无权限，-3=用户不存在
int shareEventToUser(int eventId, const char* targetUser, int permission) {
    // 1. 参数合法性检查
    if (currentUserIndex == -1) return -1; // 未登录
    if (eventId < 0 || eventId >= eventCount) return -1; // 事件ID无效
    if (permission < 0 || permission > 1) return -1; // 权限值非法

    // 2. 验证当前用户是否为事件创建者
    if (strcmp(eventCreators[eventId], users[currentUserIndex].username) != 0) {
        return -2; // 非创建者，无共享权限
    }

    // 3. 检查目标用户是否存在
    int targetIndex = -1;
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, targetUser) == 0) {
            targetIndex = i;
            break;
        }
    }
    if (targetIndex == -1) return -3; // 目标用户不存在

    // 4. 记录共享关系（添加到共享权限数组）
    sharedEvents[sharedCount].eventId = eventId;
    strcpy(sharedEvents[sharedCount].sharedUser, targetUser);
    sharedEvents[sharedCount].permission = permission;
    sharedCount++; // 共享记录数+1

    // 5. 标记事件为共享状态
    eventShared[eventId] = 1;

    // 6. 输出成功信息
    printf("[成功] 事件ID %d 已共享给用户 %s，权限：%s\n",
        eventId, targetUser, permission ? "可编辑" : "仅查看");
    return 0;
}
// 函数功能：检查当前用户对事件的权限
// 参数：eventId=事件ID，needEdit=是否需要编辑权限（1=需要，0=仅查看）
// 返回值：1=有权限，0=无权限
int checkEventPermission(int eventId, int needEdit) {
    if (currentUserIndex == -1) return 0; // 未登录
    if (eventId < 0 || eventId >= eventCount) return 0; // 事件不存在

    // 1. 创建者拥有最高权限
    if (strcmp(eventCreators[eventId], users[currentUserIndex].username) == 0) {
        return 1;
    }

    // 2. 检查共享权限
    for (int i = 0; i < sharedCount; i++) {
        if (sharedEvents[i].eventId == eventId &&
            strcmp(sharedEvents[i].sharedUser, users[currentUserIndex].username) == 0) {
            // 若需要编辑权限，检查permission是否为1；否则仅查看权限即可
            return (needEdit == 0) ? 1 : (sharedEvents[i].permission == 1);
        }
    }

    return 0; // 无权限
}