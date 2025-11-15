#include <stdio.h>
#include<string.h>
#include<stdlib.h>

// 变量声明
int running = 1;
int option;
char eventNames[100][50]={0};
char eventDates[100][20]={0};
char eventTimes[100][10]={0};
char eventDescriptions[100][100]={0};
int eventReminders[100]={0};
int eventRepeatTypes[100]={0};
int eventCount=0;



char name[50]={0}; 
char date[20]={0};
char time[10]={0}; 
char description[100]={0}; 



// 函数声明//功能点函数
void showMenu();
void manageDate();
void manageEvents(); 
void reminders();
void loadEvents();
void saveEvents();

//补充函数
int CorrectDate(const char *date);
int Weekday(int year, int month, int day);
void manageDate();
void DateAdd();
int MonthDay(int year, int month);
void Datejian();



// 主程序
int main()
 {
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

// 添加事件函数
void addEvent() {
    if (eventCount >= 100) {
        printf("事件数量已达上限!\n");
        return;
    }
    
    printf("请输入事件名称: ");
    scanf("%s", eventNames[eventCount]);
    
    printf("请输入日期 (YYYY-MM-DD): ");
    scanf("%s", eventDates[eventCount]);
    
    printf("请输入时间 (HH:MM): ");
    scanf("%s", eventTimes[eventCount]);
    
    printf("请输入描述: ");
    scanf("%s", eventDescriptions[eventCount]);
    
    printf("请输入提前提醒时间(分钟): ");
    scanf("%d", &eventReminders[eventCount]);
    
    printf("重复类型 (0-不重复,1-每天,2-每周,3-每月): ");
    scanf("%d", &eventRepeatTypes[eventCount]);
    
    eventCount++;
    printf("事件添加成功!\n");
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
        scanf("%d",&choice);

        switch(choice)//根据用户选择执行相应功能
        {
            case 1://功能1：查询日期详细信息
            {
                char date[20];//存储用户输入的日期
                printf("请输入日期(YYYY-MM-DD):");
                scanf("%s",date);

                if(!Correct Date(date))//!表示日期格式错误，函数返回值为0
                {
                    printf("日期格式错误!\n");
                    break;
                }

                int year,month,day;
                sscanf(date,"%d-%d-%d",&year,&month,&day);

                //计算星期几
                int weekday=Weekday(year,month,day);//调用函数
                char *weekdays[]={"日","一","二","三","四","五","六"};
                printf("%d年%d月%d日是星期%s\n",year,month,day,weekdays[weekday]);


                //显示月份天数
                int monthday;
                if(month==2)
                    monthday=(((year%4==0&&year%100!=0)||(year%400==0)))?29:28;//闰年判断
                else if(month==4||month==6||month==9||month==11)
                    monthday=30;
                else
                    monthday=31;
                printf("%d年%d月有%d天\n",year,month,monthday);


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
    } while (choice!=4);//单选择4时，退出循环
}


int CorrectDate(const char *date)//1正确  0错误
{
    int year,month,day;
    if(sscanf(date,"%d-%d-%d",&year,&month,&day)!=3)
        return 0;
    if(month<1||month>12)
        return 0;
    if(day<1||day>31)
        return 0;
    if(month==2)
    {
        if(day>(((year%4==0&&year%100!=0)||(year%400==0)))?29:28)//  1?2:3表示若1成立，就是2，不成立则为3 
        return 0;
    }
    else if(month==4||month==6||month==9||month==11)
    {
        if(day>30)
        return 0;
    }
    return 1;//除特殊条件外，输入的其他日期有效
}


int Weekday(int year,int month,iny day)//计算星期几
{
    int monthday[]={31,28,31,30,31,30,31,31,30,31,30,31};//基础月份
    long totalday=0;

    //1.计算年份天数
    for(int y=2000;y<year;y++)//从2000年开始算总天数,已知2000年1月1日为星期六
    {
        totalday+=365;//每年基础天数365天
    
        if((y%4==0&&y%100!=0)||y%400==0)//闰年多加一天
            totalday++;
    }
    
    //2，计算月份天数
    for(int m=0;m<month-1;m++;)
    {
        totalday+=monthday[m];
    }
        //闰年特殊处理
    if(month>2&&((year%4==0&&year%100!=0)||year%400==0))//输入月份大于2，才包含2月份，从而考虑闰年28，29问题
            totalday++;    


    //3.计算日期天数
    totalday+=day-1;//清除2000年的1月1日

    int weekday=(6+totalday)%7;//6=2000年1月1日星期六
    //totalday为从开始到输入日期的总天数
    return weekday;//返回结果：0周日，1周一...6周六


}


void DateAdd()//日期加法运算
{
    char date[20];
    int add;
    printf("请输入日期(YYYY-MM-DD):");
    scanf("%s",date);
    printf("请输入要加的天数：");
    scanf("%d",&add);

    int year,month,day;
    if(sscanf(date,"%d-%d-%d",&year,&month,&day)!=3)
    {
        printf("日期格式错误!\n");

    }

    //计算
    for(int i=0;i<add;i++)
    {
        day++;
    
    //检查是否跳到下一月
    if(day>MonyhDay(year,month))//每个月的月份不同，又要引用函数
    {
        day=1;
        month++;
    
        //检查是否跳到下一年
        if(month>12)
        {
            month=1;
            year++;
        }
    }
    
    }
    printf("\n%d天后的日期是%04d年%02d月%02d日\n",add,year,month,day);

}


int MonthDay(int year,int month)
{
    int Month[]={31,28,31,30,31,30,31,31,30,31,30,31};
    if(month==2)
    {
        if((year%4==0&&year%100!=0)||year%400==0)
        return 29;
    }
    return Month[month-1];
}


void Datejian()//日期减法运算
{
    char date[20];
    int jian;
    printf("请输入日期(YYYY-MM-DD):");
    scanf("%s",date);
    printf("请输入要减的天数：");
    scanf("%d",&jian);

    int year,month,day;
    if(sscanf(date,"%d-%d-%d",&year,&month,&day)!=3)
    {
        printf("日期格式错误!\n");
    }

    //计算
    for(int i=0;i<jian;i++)
    {
        day--;
    
    //检查是否跳到上一月
    if(day<1)
    {      
        month--;

        //检查是否跳到上一年
        if(month<1)
        {
            month=12;
            year--;
        }
        day=MonthDay(year,month);
    }

    }
    printf("\n%d天前的日期是%04d年%02d月%02d日\n",jian,year,month,day);

}






void manageEvents() {
    printf("日程管理功能开发中...\n");
}
void reminders() {
    printf("提醒功能开发中...\n");
}







void loadEvents() 
{
     FILE*file=fopen("calender_date.txt","r")//read,只读模式

    if(file==NULL)
    {
        printf("提示:没有找到保存的数据文件\n");
        eventCount=0;
        return;
    }

    fscanf(file,"%d",&eventCount);

    //安全检查
    if(eventCount>100)
    {
        printf("警告:数据异常,将重置\n");
        eventCount=0;
        fclose(file);//很重要，有利于数据完整性，打开了就要关上
        return;
    }

    for(int i=0;i<eventCount;i++)
    {
        //fscanf输入到文件
        fscanf(file,"%s\n",eventNames[i]);//事件名称
        fscanf(file,"%s\n",eventDates[i]);//日期
        fscanf(file,"%s\n",eventTimes[i]);//时间
        fscanf(file,"%s\n",eventDescriptions[i]);//描述
        fscanf(file,"%d\n",eventReminders[i]);//提醒时间
        fscanf(file,"%d\n",eventRepeatTypes[i]);//重复类型
    }

    fclose(file);
    printf("数据加载成功!共加载%d个事件\n",eventCount);
}




void saveEvents()
 {
     FILE*file;//声明一个文件类型的指针变量file来存储地址
    file=fopen("calender_date.txt","w")//fopen意为打开文件,("1","2"),1代表文件名，2代表打开模式
    //打开名为calender_date.txt的文件，只写write

    //fopen里只包含操作信息，不包含文件内容，这个函数返回控制块地址


    if(file==NULL)//不检查文件打开是否成功的话，程序会崩溃
    {
        printf("保存失败:无法创建文件!\n");
        return;//立即停止函数执行，不再继续后面的代码
    }


    //写入事件数量（第一行）
    fprintf(file,"%d\n",eventCount);

    //逐个保存每个事件的信息
    for(int i=0;i<eventCount;i++)
    {
        //fprintf输出到文件
        fprintf(file,"%s\n",eventNames[i]);//事件名称
        fprintf(file,"%s\n",eventDates[i]);//日期
        fprintf(file,"%s\n",eventTimes[i]);//时间
        fprintf(file,"%s\n",eventDescriptions[i]);//描述
        fprintf(file,"%d\n",eventReminders[i]);//提醒时间
        fprintf(file,"%d\n",eventRepeatTypes[i]);//重复类型
        //
    }

    fclose(file);
    printf("数据保存成功!共保存%d个事件\n",eventCount);
}