/*
* 简单实现cp命令
* 1509寝W组
* 2021/6/28
*/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
 
#define BUFF_SIZE 4096
#define COPY_MODE 0644
 
int main(int argc, char *argv[])
{
	int  in_fd, out_fd, n_chars;
	char buf[BUFF_SIZE];
	//参数过多
	if(argc != 3)
	{
		fprintf(stdout, "参数错误\n");
		return 0;
	}
	//打开原文件
	if((in_fd = open(argv[1], O_RDONLY)) == -1)
	{
		fprintf(stdout, "打开文件失败\n");
		return 0;
	}
 	//新建复制文件
	if((out_fd = creat(argv[2], COPY_MODE)) == -1 )
	{
		fprintf(stdout, "创建文件失败\n");
		return 0;
	}
	//复制文件内容
	while((n_chars = read(in_fd, buf, BUFF_SIZE)) > 0 )
	{
		if(write(out_fd, buf, n_chars) != n_chars )
		{
			fprintf(stdout, "复制中出现错误\n");
			return 0;
		}
	}
 	//读取文件内容错误
	if(n_chars == -1)
	{
		fprintf(stdout, "读取文件内容错误\n");
		return 0;
	}
	//关闭文件
	if(close(in_fd) == -1 || close(out_fd) == -1)
	{
		fprintf(stdout, "关闭文件出现错误\n");
		return 0;
	}
	fprintf(stdout, "cp success\n");
	return 0;
}
