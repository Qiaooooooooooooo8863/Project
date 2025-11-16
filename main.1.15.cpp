// -------------------------- 头文件保护与包含 -------------------------
#ifndef DATE_MANAGE_H
#define DATE_MANAGE_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#endif

// -------------------------- 结构体与全局变量定义 -------------------------
#define MAX_EVENTS 100   // 最大事件数量
#define MAX_NAME 50       // 事件名称最大长度
#define MAX_DATE 20       // 日期字符串最大长度（格式：YYYY-MM-DD）
#define MAX_TIME 10       // 时间字符串最大长度（格式：HH:MM）
#define MAX_DESC 100      // 事件描述最大长度

//@唐 日期管理
struct Event_T {
	char name[MAX_NAME];
	char date[MAX_DATE];                    
	char time[MAX_TIME];
	char description[MAX_DESC];
};
struct Event events_T[MAX_EVENTS];  // 事件数组

//@乔 事件管理
struct Event_Q {
    char name[50];              // 事件名称，最多50字符
    char date[20];              // 日期，格式YYYY-MM-DD
    char time[10];              // 时间，格式HH:MM
    char description[100];      // 事件描述，最多100字符
    int repeat;				    // 重复类型，0-不重复，1-每天，2-每周，3-每月 

};









// -------------------------- 工具函数 ----------------------------


// 校验日期格式是否合法（YYYY-MM-DD）
int check_Date(const char* date) {
	if (strlen(date) != 10) return 0;
	if (date[4] != '-' || date[7] != '-') return 0;
	int year = atoi(date);
	int month = atoi(date + 5);
	int day = atoi(date + 8);
	return (year >= 2025 && month >= 1 && month <= 12 && day >= 1 && day <= 31); // 简化校验
}





//------------------------------ 函数声明 ------------------------------


void manageDate();// 日期管理
void manageEvents(); // 事件增删改查
void reminders();// 提醒功能
void loadEvents();// 加载事件数据
void saveEvents(); // 保存事件数据
void showMenu();// 显示主菜单


//------------------------------ 主程序-------------------------------------
main() {
	loadEvents();       // 加载日程数据
	////////////////////////////////这里的running，showMenu，option需要定义和实现///////////////////////////////
	while (1) {
		showMenu();     // 显示菜单
		switch (option) {
		case 1: manageDate(); break;
		case 2: manageEvents(); break;
		case 3: reminders(); break;
		case 4: saveEvents(); break;
		}
	}
}





//------------------------------模块函数---------------------------------


// 显示主菜单（原框架函数补充实现）

void showMenu() {
    system("clear"); // 清屏（Windows用"cls"，Linux/macOS用"clear"）
    printf("==================== 日程管理系统 ====================\n");
    printf("当前时间: %s", ctime(&time(NULL))); // 显示系统当前时间
    printf("----------------------------------------------------\n");
    printf("1. 日期管理       2. 事件增删改查       3. 提醒功能\n");
    printf("4. 保存事件数据   5. 退出系统\n");
    printf("====================================================\n");
    printf("请输入选择 (1-5): ");
}







//@唐 这里的判断日期 前面有 引入一下就行 改一下(yes or no)
//@唐 这个Correct Date给你改成了CorrectDate 报错了
//@唐 Weekday我这里报错了不知道你那里怎么回事
//@唐 这里的函数声明可以放在前面

int CorrectDate(const char* date);  // 判断日期格式是否正确
int Weekday(int year, int month, int day);  // 计算星期几
void manageDate();  // 日期管理主函数
void DateAdd();//日期加法运算
int MonthDay(int year, int month);
void Datejian();//日期减法运算


void manageDate() {
	// 日期管理
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

//@唐 这里的"日","一","二","三","四","五","六"报错了不知道为啥，我也不会
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






///////////////////////////////////////////////////////////////////
void manageEvents() {// 事件增删改查
	int choice;
	do {
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
    // 查找日期是否已存在
    for (int i = 0; i < num_dates; i++) {
        if (strcmp(allday_events[i].date, date) == 0) {
            // 日期存在，添加事件
            if (allday_events[i].event_count < MAX_EVENTS_PER_DAY) {
                strcpy(allday_events[i].events[allday_events[i].event_count], event_text);
                allday_events[i].event_count++;
                printf("成功为 %s 添加事件: %s\n", date, event_text);
            }
            else {
                printf("错误: %s 的事件已满！\n", date);
            }
            return;
        }
    }

    // 日期不存在，创建新日期
    if (num_dates < MAX_DATES) {
        strcpy(allday_events[num_dates].date, date);
        strcpy(allday_events[num_dates].events[0], event_text);
        allday_events[num_dates].event_count = 1;
        num_dates++;
        printf("成功创建日期 %s 并添加事件: %s\n", date, event_text);
    }
    else {
        printf("错误: 日历已满！\n");
    }
}




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