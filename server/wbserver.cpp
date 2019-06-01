#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

//mysql parameters
MYSQL conn;
 
void connection(const char* host, const char* user, const char* password, const char* database) {
	mysql_init(&conn); // 注意取地址符&
 
	if (mysql_real_connect(&conn, host, user, password, database, 0, NULL, 0)) {
		printf("Connection success!\n");
	} else {
		fprintf(stderr, "Connection failed!\n");
		if (mysql_errno(&conn)) {
			fprintf(stderr, "Connection error %d: %s\n", mysql_errno(&conn), mysql_error(&conn));
		}
		exit(EXIT_FAILURE);
	}
}

const char *g_host_name = "localhost";
const char *g_user_name = "wb";
const char *g_password = "wb123";
const char *g_db_name = "wordbattle";
// const unsigned int g_db_port = 3306;


//socket buffer line
#define MAXLINE 1024

//gaming server center
int clientInterpretation(char *msg);

int main(int argc,char **argv)
{
    //mysql
	connection(g_host_name, g_user_name, g_password, g_db_name);
    //socket parameters
    int listenfd, connfd;
    struct sockaddr_in sockaddr;
    char buff[MAXLINE];
    int n;

    memset(&sockaddr, 0, sizeof(sockaddr));

    sockaddr.sin_family = AF_INET;
    // sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    sockaddr.sin_addr.s_addr = inet_addr("107.172.86.173");
    sockaddr.sin_port = htons(10004);

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    bind(listenfd, (struct sockaddr *)&sockaddr, sizeof(sockaddr));
    listen(listenfd, 30);

    printf("Please wait for the client information\n");

    for (;;)
    {
        if ((connfd = accept(listenfd, (struct sockaddr *)NULL, NULL)) == -1)
        {
            printf("accpet socket error: %s errno :%d\n", strerror(errno), errno);
            continue;
        }
        n = recv(connfd, buff, MAXLINE, 0);
        buff[n] = '\0';
        printf("recv msg from client:%s \n", buff);
        //execute gaming processing
        // char sendline[50];
        clientInterpretation(buff);//,sendline
        // send(listenfd,sendline,strlen(sendline),0);

        close(connfd);
    }
    close(listenfd);
    mysql_close(&conn);

}
//gamine
int clientInterpretation(char msg[])//,char sendline[]
{
    char* user;
    char* cmd=(char*)malloc(sizeof(char)*50);
    char  memorystring[500];
    char* sqlquery= memorystring;
    char temp[50];
    user=strtok(msg,"|");
    strcpy(cmd,strtok(NULL,"|"));
    // cmd=strtok(NULL,"|");

    // char online[10]="online";
    // char offline[10]="offline";
    // printf("%s\n%s\n",online,offline);

    // printf("debug1: processing message %s\n",cmd);
    strcpy(temp,user);
    printf(": processing message %c\n",cmd[1]);
    // if(strcmp(online,cmd))
    if(cmd[1]=='n'){
        strcpy(sqlquery,"insert into online(username,c_level,c_stage,c_time) select username,c_level,c_stage,c_time from users where username = '");
        sqlquery=strcat(sqlquery,temp);
        sqlquery=strcat(sqlquery,"';");
        printf("running: %s\n",sqlquery);
        int res =mysql_query(&conn, sqlquery);
        // if (!res) {
        //     printf("affect %lu rows\n", (unsigned long)mysql_affected_rows(&conn));
        // } else {
        //     fprintf(stderr, "execution error %d: %s\n", mysql_errno(&conn), mysql_error(&conn));
        // }
    }
    //delete user from online
    else if(cmd[1]=='f'){
        strcpy(sqlquery,"delete from online where username = '");
        sqlquery=strcat(sqlquery,temp);
        sqlquery=strcat(sqlquery,"';");
        printf("running: %s\n",sqlquery);
        int res =mysql_query(&conn, sqlquery);
        // if (!res) {
        //     printf("affect %lu rows\n", (unsigned long)mysql_affected_rows(&conn));
        // } else {
        //     fprintf(stderr, "execution error %d: %s\n", mysql_errno(&conn), mysql_error(&conn));
        // }
    }

    //erro
    else{
        printf("wrong cmd from client!\n");
    }
    printf("finish a command processing\n");
}


//mysql
// int init_mysql()
// {
//     //init the database connection
//     g_conn = mysql_init(NULL);
//     //connection the database
//     if (!mysql_real_connect(g_conn, g_host_name, g_user_name, g_password, g_db_name, g_db_port, NULL, 0))
//         return -1; //链接失败
//     if (executesql("set names utf8"))
//         return -1;
//     return 0; //返回成功
// }

// void print_mysql_error(const char *msg)
// {
//     if (msg)
//         printf("%s: %s\n", msg, mysql_error(g_conn));
//     else
//         puts(mysql_error(g_conn));
// }
// int executesql(const char *sql)
// {
//     printf("executing %s \n",sql);
//     if (mysql_real_query(g_conn, sql, strlen(sql)))
//         return -1;
//     return 0;
// }
// void create_database()
// {
//     sprintf(sql, "use wordbattle;");
//     if (executesql(sql) == -1)
//     {
//         puts("create database");
//         executesql("create database wordbattle;");
//         print_mysql_error(NULL);
//         puts("choice database");
//         executesql("use wordbattle;");
//         print_mysql_error(NULL);
//         puts("!!!Initialize the success!!!");
//     }
//     else
//     {
//         executesql("use wordbattle;");
//         print_mysql_error(NULL);
//     }
// }
// void create_table()
// {
    
//     executesql("CREATE TABLE IF NOT EXISTS online\
//             (username VARCHAR(128) PRIMARY KEY,\
//             c_stage INT NOT NULL,\
//             c_level INT NOT NULL,\
//             c_time  INT);");
//     executesql("CREATE TABLE IF NOT EXISTS onbattle\
//             (userA VARCHAR(128) PRIMARY KEY,\
//             scoreA INT DEFAULT -1,\
//             userB VARCHAR(128) DEFAULT 'none',\
//             scoreB INT DEFAULT -1,\
//             state INT DEFAULT 0,\
//             currentround INT DEFAULT -1,\
//             currentword VARCHAR(128) DEFAULT 'default',\
//             timeA INT DEFAULT 30,\
//             timeB INT DEFAULT 30);");
// }