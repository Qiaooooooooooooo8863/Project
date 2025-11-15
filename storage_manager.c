#include<stdio.h>
#include"storage_manage.h"

//%s输入输出字符串的格式符
//保存事件数据到文件
void saveEvent()
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
        fprintf(file,"%s\n",eventName[i]);//事件名称
        fprintf(file,"%s\n",eventDate[i]);//日期
        fprintf(file,"%s\n",eventTimes[i]);//时间
        fprintf(file,"%s\n",eventDescriptions[i]);//描述
        fprintf(file,"%d\n",eventReminders[i]);//提醒时间
        fprintf(file,"%d\n",eventRepeatTypes[i]);//重复类型
        //
    }

    fclose(file);
    printf("数据保存成功!共保存%d个事件\n",eventCount);


}


//从文件加载事件数据
void loadEvents()
{
    FILE*file=fopen("calender_date.txt","r")//read,只读模式

    if(file=NULL)
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



