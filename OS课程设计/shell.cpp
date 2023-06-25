/*
* 实现简单shell命令窗口的框架
* 1509寝W组
* 2021/6/27
* 添加SHELL自带命令框架
* 添加exit功能
* 添加history功能框架
* 2021/6/27
* 添加!!回溯功能
* 添加!N回溯功能
* 2021/6/28
*/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<wait.h>
#include<ctype.h>
#include<pwd.h>
#include<string.h>
#include<list>
#include<pthread.h>

//输出前缀并接收命令
int readCMD(char* buf, int& buf_len);

//将读取到的字符串分成多个字符串
int cutCMD(char* buf, char** MyArgv);

//判断是否为shell应用自带指令
int judgeCMD(char** MyArgv);

//利用fork新建进程执行命令
void forkCMD(char** MyArgv);

//退出shell
void quitSHELL();

//history储存结构
std::pair< int, std::list<char*> > _history(0, NULL);
//查看history
void historySHELL();
//添加history
void historyADD(const char* pbuf, const int buf_len);

//执行指定命令
void runCMD(const char* cmd);

//!!
void rollbackSHELL();

//!N
void rollbackN(const char* cmd);

//&
void* runADD(void* cmd);

//main
int main()
{
	//本次输入命令的长度
	int buf_len;
	//储存每次输入的命令
	char buf[1024];
	//储存切割好的命令
	char* MyArgv[10] = {0};
	//主循环
	while(1)
	{
		//输出前缀并接收命令 返回1则说明正确接收到信息
		if(readCMD(buf, buf_len))
		{
			//copy本次命令 方便存储
			char temp_buf[buf_len];
			strcpy(temp_buf, buf);
			//将读取到的字符串分成多个字符串
			int argc = cutCMD(buf, MyArgv);
			//如果是shell自带的命令的话直接执行
			int temp = judgeCMD(MyArgv);
			if(temp > 0)
			{
				switch(temp)
				{
					//退出shell
					case 1: quitSHELL(); break;
					//查看history
					case 2: historySHELL(); break;
					//!!
					case 3: rollbackSHELL(); break;
					//!N
					case 4: rollbackN(MyArgv[0]); break;
				}
			}
			//否则fork切换进程执行命令
			else
			{
				//若要求后台执行
				if(!strcmp("&", MyArgv[argc - 1]))
				{
					pthread_t id;
					pthread_create(&id, NULL, runADD, (void*)temp_buf);
					//线程等待
					pthread_join(id, NULL);
					//printf("后台执行结束    %s\n", temp_buf);
				}
				else
				{
					//利用fork新建进程执行命令
					forkCMD(MyArgv);
				}
			}
			//记录命令
			historyADD(temp_buf, buf_len);
        	}
	}
    	return 0;
}

//输出前缀并接收命令
int readCMD(char* buf, int& buf_len) 
{
	//清空
	memset(buf, 0, sizeof(buf));
	//前缀显示
	printf("<1509W@OSshell>");
	fflush(stdout);
	//读取字符串
       buf_len = read(0,buf,1024);
       if(buf_len > 0)//有读取到字符
       	{
       	if(buf_len == 1 && buf[0] == '\n')
       		{
       		return 0;
		}
    		for(int i = 0; i < buf_len; ++i)
            	{
            		if(buf[i] == '\b' && i >= 1)
               	 	{
               	 	//    printf("debug:%d\n",i);
               	 	for(int j = i+1; j < buf_len; ++j)
               	 		{
               	 		buf[j-2] = buf[j];
               	 		}
               	 	buf[buf_len-2] = 0;
                    	i-=1;
               	 	}
               	else if(buf[i] == '\b' && i == 0)
                	{
                		//    printf("debug:%d\n",i);
                    	for(int j = 1; j < buf_len; ++j)
                    		{
                        		buf[j-1] = buf[j];
                    		}
                    	buf[buf_len-1] = 0;
                		//    i-=1;
                	}
                	else
                	{
                    	continue;
                	}
            	}
            	buf[buf_len] = 0;
	}
	else
	{
		return 0;
	}
	return 1;
}

//将读取到的字符串分成多个字符串
int cutCMD(char* buf, char** MyArgv)
{
	//清空
	memset(MyArgv, 0, sizeof(MyArgv));
	char* start = buf;
	int i =1;
	MyArgv[0] = start;
	while(*start)
	{
		if(isspace(*start))
		{
			*start = 0;
			start++;
			MyArgv[i++] = start;
		}
		else
		{
			++start;
		}
	}
	MyArgv[i-1] = NULL;
	return i - 1;
}

//判断是否为shell应用自带指令
int judgeCMD(char** MyArgv)
{
	char temp[20];
	strcpy(temp,MyArgv[0]);
	//退出shell
	if(!strcmp("exit",temp))
	{
		return 1;
	}
	//查看history
	if(!strcmp("history",temp))
	{
		return 2;
	}
	//!!
	if(!strcmp("!!",temp))
	{
		return 3;
	}
	//!N
	if(temp[0] == '!')
	{
		return 4;
	}
	return 0;
}

//利用fork新建进程执行命令
void forkCMD(char** MyArgv)
{
	//filename
	char filename[256];
	//get filename
	sprintf(filename,"%s%s", "./bin/", MyArgv[0]);
	//fork新的进程
	int id = fork();
	if(id == 0)
        {
		//child,执行替换操作
		execvp(filename,MyArgv);
		perror("error");
		exit(1);
        }
	else
        {
        	//father
            	wait(NULL);
        }
}

//退出shell
void quitSHELL()
{
	printf("I`m Chinese! 再见\n");
	exit(1);
}

//查看history
void historySHELL()
{
	if(_history.second.size() == 0)
	{
		printf("不存在历史记录\n");
		return;
	}
	auto iter = _history.second.begin();
	for(int i = 1 + _history.first; iter != _history.second.end(); ++iter, ++i)
	{
		printf("%4d %s\n", i, *iter);
	}
}

//添加history
void historyADD(const char* pbuf, const int buf_len)
{
	//new一个char*出来方便储存
	char *temp_buf = new char[buf_len + 1];
	strcpy(temp_buf, pbuf);
	temp_buf[buf_len-1] = 0;
	//如果已储存消息条数小于10则直接添加
	if(_history.second.size() < 10)
	{
		//入表
		_history.second.push_back(temp_buf);
	}
	//否则前端+1 弹表头,入表尾
	else
	{
		//起始序号++
		++_history.first;
		//delete掉头节点 
		char* t = _history.second.front();
		_history.second.pop_front();
		delete[] t;
		//入表
		_history.second.push_back(temp_buf);
	}
}

//执行指定命令
void runCMD(const char* cmd)
{
	//命令长度
	int buf_len = strlen(cmd);
	//储存命令
	char buf[1024];
	strcpy(buf, cmd);
	buf[buf_len] = '\n';
	buf[buf_len + 1] = '\0';
	//储存切割好的命令
	char* MyArgv[10] = {0};
	//将读取到的字符串分成多个字符串
	int argc = cutCMD(buf, MyArgv);
	//如果是shell自带的命令的话直接执行
	int temp = judgeCMD(MyArgv);
	if(temp > 0)
	{
		switch(temp)
		{
			//退出shell
			case 1: quitSHELL(); break;
			//查看history
			case 2: historySHELL(); break;
			//!相关无法被执行
			case 3: 
			case 4: printf("!相关命令无法被重复执行\n"); break;
		}
	}
	//否则fork切换进程执行命令
	else
	{
		//若要求后台执行
		if(!strcmp("&", MyArgv[argc - 1]))
		{
			pthread_t id;
			pthread_create(&id, NULL, runADD, (void*)buf);
			//线程等待
			pthread_join(id, NULL);
			//printf("后台执行结束    %s\n", buf);
		}
		else
		{
			//利用fork新建进程执行命令
			forkCMD(MyArgv);
		}
	}
}

//!!
void rollbackSHELL()
{
	char buf[1024];
	//获取最后一条记录
	strcpy(buf, _history.second.back());
	//执行命令
	runCMD((const char*)buf);
}

//!N
void rollbackN(const char* cmd)
{
	int t = strlen(cmd);
	//回溯指令id
	int num = 0;
	//获取id
	for(int i = 1; i < t; ++i)
	{
		if(cmd[i] >= '0' && cmd[i] <= '9') 
		{
			num = num * 10 + cmd[i] - '0';
		}
		else
		{
			printf("回溯指令行数输入有误\n");
			return;
		}
	}
	//所记录指令的id起始
	int start = _history.first + 1;
	int end = start + _history.second.size() - 1;
	//边缘判定
	if(_history.second.size() == 0)
	{
		printf("历史记录为空 无法执行\n");
		return;
	}
	if(num < start || num > end)
	{
		printf("没有id为%d的历史记录 无法执行\n", num);
		return;
	}
	//找到这条指令
	auto iter = _history.second.begin();
	for(int i = 0; iter != _history.second.end() && i != num - start; ++i)
	{
		++iter;
	}
	//执行命令
	runCMD((const char*)*iter);
}

//&
void* runADD(void* cmd)
{
	char* MYcmd = (char*)cmd;
	system(MYcmd);
	return 0;
}
