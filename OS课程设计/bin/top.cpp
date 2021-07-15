/*
* 简单实现top命令
* 1509寝W组
* 2021/6/28
*/
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<wait.h>

//main
int main(int argc,char * argv[])
{
	/*/fork新的进程
	int id = fork();
	if(id == 0)
        	{
		//child,执行替换操作
		//系统调用
		execvp(argv[0], argv);
		perror("error");
		exit(1);
       	 }
	else
        	{
        		//father
            		wait(NULL);
        	}*/
        	//系统调用
	execvp(argv[0], argv);
	return 0;
}