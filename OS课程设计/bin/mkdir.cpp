/*
* 简单实现mkdir命令
* 1509寝W组
* 2021/6/28
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

//main
int main(int argc,char *argv[])
{
	if(argc != 2)
        {
       	fprintf(stdout, "参数错误:参数数量错误\n");
		return 0;
        }
        //创建权限为0600的文件夹
       if(mkdir(argv[1], O_CREAT | 0600) == -1)
        {
		fprintf(stdout, "创建失败\n");
       	return 0;
        }
	printf("your operation is successful!\n");
	return 0;
}