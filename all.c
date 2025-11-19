#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// 定义日程事件结构体


struct Event {
    char name[50];
    char date[20];
    char time[10];
    char description[100];
    int remind;        // 提醒标志
    int repeat;        // 重复类型：0-不重复，1-每天，2-每周
    struct Event* next;
};

// 全局变量
struct Event* eventList = NULL;
int running = 1;

// 函数声明
void showMenu();
void manageDate();
void manage_events();
void reminde_Events();
void loadEvents();
void saveEvents();
void add_Event();
void delete_Event();
void change_Event();
void look_Events();
int check_Date(const char* date);
int check_Time(const char* time);
int getDayOfWeek(int year, int month, int day);
int isLeapYear(int year);
void check_remind_Events();
void Calendar();
void DateAdd();
void Datejian();
void look_date_Events(const char* date);
int MonthDay(int year, int month);
void displayWeekCalendar(int year, int month, int week);
int getFirstDayOfWeek(int year, int month);
void displayCalendar(int year, int month);
int getDaysInMonth(int year, int month);

// 主程序
int main() {
    printf("=== 日历管理系统 ===\n");
    loadEvents();       // 加载日程数据

    while (running) {
        showMenu();     // 显示菜单

        int option;
        printf("请选择操作 (1-6):\n ");
        scanf("%d", &option);
        getchar();  // 清除输入缓冲区

        switch (option) {
        case 1: manageDate(); break;    // 日期管理
        case 2: manage_events(); break;  // 事件管理
        case 3: reminde_Events(); break;     // 提醒功能
        case 4: saveEvents(); break;    // 保存数据
        case 5: look_Events(); break;    // 查看所有事件
        case 6: Calendar(); break;    //日历
        case 7:
            running = 0;
            printf("感谢使用日历管理系统！\n");
            break;
        default:
            printf("无效选择，请重新输入！\n");
        }
    }

    return 0;
}

// 显示主菜单
void showMenu() {
    printf("\n----------------主菜单--------------\n");
    printf("1. 日期管理\n");
    printf("2. 事件管理\n");
    printf("3. 提醒功能\n");
    printf("4. 保存数据\n");
    printf("5. 查看所有事件\n");
    printf("6. 日历周历\n");
    printf("7. 退出系统\n");
    printf("\n------------------------------\n");
}

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
            char* weekdays[] = { "日","一","二","三","四","五","六" };
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
        if (day > (((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))) ? 29 : 28)//  1?2:3表示若1成立，就是2，不成立则为3 
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
    }

    //计算
    for (int i = 0; i < jian; i++)
    {
        day--;

        //检查是否跳到上一月
        if (day < 1)
        {
            month--;

            //检查是否跳到下一年
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
/////////////////////////////////////////////////////////////////////////////



//-----------------------事件管理系统----------
void manage_events() {
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
        case 1: add_Event(); break;      // 加入事件
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

//---------------添加事件的函数模块---------
void add_Event() {
    struct Event* newEvent = (struct Event*)malloc(sizeof(struct Event));//为事件分配内存

    printf("\n------- 添加新事件 -------\n");

    //---------输入事件的信息-----
    printf("请输入事件的名称: ");
    fgets(newEvent->name, sizeof(newEvent->name), stdin); //键盘输入
    //日期
    do {
        printf("日期 (YYYY-MM-DD): ");
        fgets(newEvent->date, sizeof(newEvent->date), stdin);
        if (!check_Date(newEvent->date)) {
            printf("格式错误!!!\n");
        }

    } while (!check_Date(newEvent->date));
    //时间
    do {
        printf("时间 (HH:MM): ");
        fgets(newEvent->time, sizeof(newEvent->time), stdin);
        if (!check_Date(newEvent->date)) {
            printf("格式错误!!!\n");
        }
    } while (!check_Time(newEvent->time));
    //描述
    printf("事件的描述: ");
    fgets(newEvent->description, sizeof(newEvent->description), stdin);

    //提醒
    printf("是否设置提醒? (0-否, 1-是): ");
    scanf("%d", &newEvent->remind);//添加提醒
    getchar();
    printf("重复的类型 (0-不重复, 1-每天, 2-每周): ");
    scanf("%d", &newEvent->repeat);
    getchar();

    //------------------添加到链表里-------------
    newEvent->next = eventList;
    eventList = newEvent;

    printf("\n 事件添加成功！\n");
}

//-----------------------------删除事件---------------------
void delete_Event() {
    //检查是否有事件
    if (eventList == NULL) {
        printf("暂无事件可删除！\n");
        return;
    }


    printf("\n--- 删除事件 ---\n");
    //展示要删的事件
    look_Events();

    int index = 0;
    int i = 1;
    printf("请输入要删除的事件编号: ");
    scanf("%d", &index);
    getchar();
    //-----------删除事件---------
    //指向事件链表的开头
    struct Event* now = eventList;
    //移开指针
    struct Event* p = NULL;
    //找到要删除的事件
    while (now != NULL && i < index) {
        p = now;
        now = now->next;
        i++;
    }

    if (now == NULL) {
        printf("事件编号不存在！\n");
        return;
    }
    //从链表中移除事件
    if (p == NULL) {//如果删除的是第一个事件
        eventList = now->next;
    }
    else {          //删除其他事件
        p->next = now->next;
    }

    free(now);//释放内存！
    printf("事件删除成功！\n");
}



//-------------------------修改事件--------------
void change_Event() {
    //检查有没有有事件
    if (eventList == NULL) {
        printf("\n暂无事件可修改！\n");
        return;
    }

    printf("\n--- 修改事件 ---\n");
    //展示所有的事件
    look_Events();

    int n = 1;
    int i = 1;
    printf("请输入要修改的事件编号:\n ");
    scanf("%d", &n);
    getchar();
    //找到要修改的事件的位置

    struct Event* now = eventList;
    while (now != NULL && i < n) {
        now = now->next;
        i++;
    }
//不存在
    if (now == NULL) {
        printf("事件编号不存在！\n");
        return;
    }

    printf("修改事件: %s\n", now->name);//输出名称

	char input[100];//输入新的信息
    printf("新事件名称:\n ", now->name);
	fgets(input, sizeof(input), stdin);//获取新名称


    printf("新日期 (原: %s): ", now->date);
	fgets(input, sizeof(input), stdin);//获取新日期


    printf("新时间 (原: %s): ", now->time);
	fgets(input, sizeof(input), stdin);//获取新时间


    printf("事件修改成功！\n");
}

//----------------------查看所有事件----------------------
void look_Events() {

    printf("\n--- 所有日程事件 ---\n");
    struct Event* now = eventList;
    int count = 1;

//遍历事件链表并显示
    while (now != NULL) {
        printf("%d. %s | %s %s | %s\n",
            count++,
            now->name,
            now->date,
            now->time,
            now->description);
        now = now->next;
    }
}

//-----------------------查看特定日期事件----------------
void look_date_Events(const char* date) {
//验证日期格式    
    if (!check_Date(date)) {
        printf("日期格式错误！\n");
        return;
    }
//显示该日期的事件
    printf("\n--- %s 的事件安排 ---\n", date);
    struct Event* now = eventList;
    int num = 0;
//当天所有事件
    while (now != NULL) {
        if (strcmp(now->date, date) == 0) {
            printf(" %s | %s | %s\n",
                now->name,
                now->time,
                now->description);
            num = 1;
        }
        now = now->next;
    }
//当天没有事件安排
    if (!num) {
        printf("该日期暂无事件安排。\n");
    }
}

//--------------------提醒功能模块 -----------------
void reminde_Events() {
    printf("\n--- 提醒功能 ---\n");
    check_remind_Events();  // 检查提醒 

    int num;
    printf("\n1. 设置新提醒\n");  // 周期性提醒 
    printf("2. 返回主菜单\n");
    printf("请选择: ");
    scanf("%d", &num);
    getchar();

    if (num == 1) {
        add_Event();  // 添加带提醒的事件
    }
}

// ----------------检查提醒 --------------
void check_remind_Events() {
    time_t now_time = time(NULL);//现在所指的时间
	struct tm* local = localtime(&now_time);//转换为有格式的时间
//输出现在的时间
    char nowDate[20], nowTime[10];
    sprintf(nowDate, "%04d-%02d-%02d",local->tm_year + 1900,local->tm_mon + 1,local->tm_mday);
    sprintf(nowTime, "%02d:%02d",local->tm_hour,local->tm_min);
  printf("当前时间: %s %s\n", nowDate, nowTime);
    printf("今日提醒:\n");

    struct Event* now = eventList;//找提醒
    int find = 0;

    while (now != NULL) {
        if (now->remind && strcmp(now->date, nowDate) == 0) {
            printf("！！！ %s | %s | %s\n",
                now->name,
                now->time,
                now->description);
            find = 1;

// 周期性提醒 
            if (now->repeat > 0) {
                char* repeatTypes[] = { "", "每天", "每周" };
                printf("   (%s重复提醒)\n", repeatTypes[now->repeat]);
            }
        }
        now = now->next;
    }

    if (!find) {
        printf("今日暂无提醒。\n");
    }
}

// 数据存储
void saveEvents() {
    FILE* file = fopen("calendar_data.txt", "w");
    if (file == NULL) {
        printf("无法保存数据文件！\n");
        return;
    }

    struct Event* now = eventList;
    while (now != NULL) {
        fprintf(file, "%s|%s|%s|%s|%d|%d\n",
            now->name,
            now->date,
            now->time,
            now->description,
            now->remind,
            now->repeat);
        now = now->next;
    }

    fclose(file);
    printf("数据保存成功！\n");
}

// 数据加载
void loadEvents() {
    FILE* file = fopen("calendar_data.txt", "r");
    if (file == NULL) {
        printf("无历史数据文件，将创建新文件。\n");
        return;
    }

    char line[300];
    while (fgets(line, sizeof(line), file)) {
        struct Event* newEvent = (struct Event*)malloc(sizeof(struct Event));
        if (newEvent == NULL) continue;

        char* token = strtok(line, "|");
        if (token) strcpy(newEvent->name, token);

        token = strtok(NULL, "|");
        if (token) strcpy(newEvent->date, token);

        token = strtok(NULL, "|");
        if (token) strcpy(newEvent->time, token);

        token = strtok(NULL, "|");
        if (token) strcpy(newEvent->description, token);

        token = strtok(NULL, "|");
        if (token) newEvent->remind = atoi(token);

        token = strtok(NULL, "|");
        if (token) newEvent->repeat = atoi(token);

        newEvent->next = eventList;
        eventList = newEvent;
    }

    fclose(file);
    printf("历史数据加载成功！\n");
}
//-----------------------月历显示函数-----------------------
// 计算某年某月1日是星期几 (0=周日, 1=周一, ..., 6=周六)
int getFirstDayOfWeek(int year, int month) {
    struct tm timeinfo = { 0 };
    timeinfo.tm_year = year - 1900;
    timeinfo.tm_mon = month - 1;
    timeinfo.tm_mday = 1;
    mktime(&timeinfo);
    return timeinfo.tm_wday;
}
//显示日期
void displayCurrentCalendar() {
time_t t = time(NULL);
struct tm* current_time = localtime(&t);
int year = current_time->tm_year + 1900;
int month = current_time->tm_mon + 1;
int day = current_time->tm_mday;

printf("\n当前系统时间: %d年%d月%d日\n", year, month, day);
displayCalendar(year, month);

// 计算当前是第几周
int firstDay = getFirstDayOfWeek(year, month);
int week = (day + firstDay - 1) / 7 + 1;
displayWeekCalendar(year, month, week);
}
// 显示月历
void displayCalendar(int year, int month) {
    printf("\n========== %d年%d月日历 ==========\n", year, month);
    printf("日  一  二  三  四  五  六\n");

    int firstDay = getFirstDayOfWeek(year, month);
    int daysInMonth = getDaysInMonth(year, month);

    // 打印前面的空格
    for (int i = 0; i < firstDay; i++) {
        printf("    ");
    }

    // 打印日期
    for (int day = 1; day <= daysInMonth; day++) {
        printf("%2d  ", day);
        if ((firstDay + day) % 7 == 0) {
            printf("\n");
        }
    }
    printf("\n");
}

//-----------------------周历显示函数-----------------------
void displayWeekCalendar(int year, int month, int week) {
    printf("\n========== %d年%d月第%d周周历 ==========\n", year, month, week);
    printf("星期  日期       事项\n");
    printf("----------------------\n");

    // 计算该周第一天的日期
    int firstDayOfMonth = getFirstDayOfWeek(year, month);
    int startDay = (week - 1) * 7 - firstDayOfMonth + 1;

    char* weekDays[] = { "日", "一", "二", "三", "四", "五", "六" };

    for (int i = 0; i < 7; i++) {
        int currentDay = startDay + i;

        // 检查日期是否在当前月份内
        if (currentDay >= 1 && currentDay <= getDaysInMonth(year, month)) {
            printf("周%s   %d月%d日   \n", weekDays[i], month, currentDay);
        }
        else {
            // 处理跨月的情况
            int adjMonth = month;
            int adjYear = year;
            int adjDay = currentDay;

            if (currentDay < 1) {
                // 上个月
                adjMonth--;
                if (adjMonth < 1) {
                    adjMonth = 12;
                    adjYear--;
                }
                adjDay = getDaysInMonth(adjYear, adjMonth) + currentDay;
            }
            else {
                // 下个月
                adjMonth++;
                if (adjMonth > 12) {
                    adjMonth = 1;
                    adjYear++;
                }
                adjDay = currentDay - getDaysInMonth(year, month);
            }
            printf("周%s   %d月%d日   \n", weekDays[i], adjMonth, adjDay);
        }
    }
}
// 获取某年某月的天数
int getDaysInMonth(int year, int month) {
    int days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (month == 2 && isLeapYear(year)) {
        return 29;
    }
    return days[month - 1];
}
//日历菜单
void Calendar(){
int choice, year, month, week;

printf("=== 日历和周历系统 ===\n");

while (1) {
    printf("\n请选择功能:\n");
    printf("1. 显示当前月份日历和周历\n");
    printf("2. 显示指定月份日历\n");
    printf("3. 显示指定周历\n");
    printf("4. 退出程序\n");
    printf("请输入选择 (1-4): ");

    scanf("%d", &choice);

    switch (choice) {
    case 1:
        displayCurrentCalendar();
        break;

    case 2:
        printf("请输入年份: ");
        scanf("%d", &year);
        printf("请输入月份: ");
        scanf("%d", &month);
        if (month < 1 || month > 12) {
            printf("月份输入错误！\n");
            break;
        }
        displayCalendar(year, month);
        break;

    case 3:
        printf("请输入年份: ");
        scanf("%d", &year);
        printf("请输入月份: ");
        scanf("%d", &month);
        printf("请输入周数 (1-6): ");
        scanf("%d", &week);
        if (month < 1 || month > 12 || week < 1 || week > 6) {
            printf("输入错误！\n");
            break;
        }
        displayWeekCalendar(year, month, week);
        break;

    case 4:
        printf("程序已退出，再见！\n");
        return 0;

    default:
        printf("无效的选择，请重新输入！\n");
    }
}

return 0;
}
//-------------------------------工具函数实现-------------------------------


//-------------验证日期格式--------------
int check_Date(const char* date) {
    int year, month, day;
    if (sscanf(date, "%d-%d-%d", &year, &month, &day) != 3) return 0;
    if (month < 1 || month > 12 || day < 1 || day > 31) return 0;
    if (day > getDaysInMonth(year, month)) return 0;
    return 1;
}


//-------------验证时间的格式---------------
int check_Time(const char* time) {
    int hour, minute;
    if (sscanf(time, "%d:%d", &hour, &minute) != 2) return 0;
    return (hour >= 0 && hour <= 23 && minute >= 0 && minute <= 59);
}


//---------------判断是不是为闰年----------------
int isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}
//---------------获取某月某周周几--------------
int getDayOfWeek(int year, int month, int day) {
    struct tm timeinfo = { 0 };
    timeinfo.tm_year = year - 1900;
    timeinfo.tm_mon = month - 1;
    timeinfo.tm_mday = day;
    mktime(&timeinfo);
    return timeinfo.tm_wday;
}