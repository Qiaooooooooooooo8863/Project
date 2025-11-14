#ifndef DATE_MANAGE_H
#define DATE_MANAGE_H

struct Event { 
    char name[50]; 
    char date[20];
    char time[10]; 
    char description[100]; 
};

// 函数声明
int CorrectDate(const char *date);  // 判断日期格式是否正确
int Weekday(int year, int month, int day);  // 计算星期几
void manageDate();  // 日期管理主函数
void DateAdd();//日期加法运算
int MonthDay(int year,int month);
void Datejian();//日期减法运算


#endif