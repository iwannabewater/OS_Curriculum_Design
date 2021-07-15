/*
* 简单实现date命令
* 1509寝W组
* 2021/6/27
*/
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int main(int argc, char* argv[])
{
    time_t t;
    struct tm *p;
	time(&t);
	p = gmtime(&t);
	printf("%04d年 %02d月 %02d日 星期%d %02d:%02d:%02d CST\n",p->tm_year+1900, p->tm_mon+1, p->tm_mday, p->tm_wday==0 ? p->tm_wday+7 : p->tm_wday, p->tm_hour+8, p->tm_min, p->tm_sec);
	return 0;
}
