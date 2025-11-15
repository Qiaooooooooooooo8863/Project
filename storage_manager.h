#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H

#define MAX_EVENTS 100

// 声明外部全局变量
extern char eventNames[][50];
extern char eventDates[][20];
extern char eventTimes[][10];
extern char eventDescriptions[][100];
extern int eventReminders[];
extern int eventRepeatTypes[];
extern int eventCount;

void saveEvents();
void loadEvents();

#endif