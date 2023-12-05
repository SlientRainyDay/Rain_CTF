#ifndef __HEAD_H__
#define __HEAD_H__
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/ip.h>
#include <pthread.h>
// 使用htonl(INADDR_ANY)的话下边的IP用不到了
// 当然可以修改为自己的IP,那就把htonl改为inet_addr(IP)
#define IP "127.0.0.1"
#define PORT 5212

typedef struct client
{
    struct sockaddr_in client;
    long long int fd;
    struct client *next;
    long long int in_chart_room_or_not;
    long long int money;
    char *characters[100];
    long long int characters_count;
    char username[20];
    char password[20];
    char statement[20];
    char path[100];
}cli_t;
typedef struct cha
{
    char name[100];
    char *skill[2];
    long long int level;
    long long int skill_hurt[2];
    long long int how_much;
    int blood;
}character;
typedef struct msg
{
    pthread_mutex_t lock; // 锁
    char message[512]; // 发送和接收数据
    long long int type; // 类型
    cli_t cli; // 客户端信息结构体
    long long int size; // 链表大小
}msg_t;

#endif
