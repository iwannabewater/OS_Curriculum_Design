/*
* 简单实现rmdir命令
* 1509寝W组
* 2021/6/28
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

#define PATH_SIZE 4094

//rmdir
void my_rmdir(const char * path);
 
//main
int main(int argc, char const *argv[])
{
	if (argc != 2)
	{
		fprintf(stdout, "参数错误:参数数量错误\n");
		return 0;
	}
 	//rmdir
	my_rmdir(argv[1]);
	return 0;
}

//rmdir
void my_rmdir(const char * path)
{
	DIR *dirp;
	//打开文件夹
	dirp = opendir(path);
	if (NULL == dirp)
	{
		fprintf(stdout, "参数错误:路径出错\n");
		return;
	}
 	//rm文件
	struct dirent *entry;
	int ret;
	while (1)
	{
		entry = readdir(dirp);
		//文件夹为空
		if (NULL == entry)
		{
			break;
		}
		//skip . & ..
		if (0 == strcmp(".", entry->d_name) || 0 == strcmp("..", entry->d_name))
		{
			continue;
		}
 		//删文件
		char buf[PATH_SIZE];
		snprintf(buf, PATH_SIZE, "%s/%s", path, entry->d_name);
		ret = remove(buf);
		if (-1 == ret)
		{
			//若为目录删子目录
			if (ENOTEMPTY == errno)
			{
				my_rmdir(buf);
				continue;
			}
			perror(buf);
			return;
		}
		fprintf(stdout, "rm file: %s\n", buf);
	}
 	//close
	closedir(dirp);
 	//删除文件夹
	ret = rmdir(path);
	if (-1 == ret)
	{
		perror(path);
		return;
	}
	fprintf(stdout, "rm dir: %s\n", path);
}