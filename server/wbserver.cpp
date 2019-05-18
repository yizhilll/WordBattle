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
MYSQL *g_conn;    //mysql 链接
MYSQL_RES *g_res; //mysql 记录集
MYSQL_ROW g_row;  //字符串数组，mysql 记录行

const char *g_host_name = "localhost";
const char *g_user_name = "root";
const char *g_password = "asdfgh";
const char *g_db_name = "test";
const unsigned int g_db_port = 3306;

#define MAX_BUF_SIZE 1024 //缓冲区最大字节数
char sql[MAX_BUF_SIZE];
char Time[MAX_BUF_SIZE];

int iNum_rows = 0; //mysql语句执行结果返回行数赋初值
int flag = 0;      //管理员权限开关
int i = 1;         //系统运行开关

//functions 
int init_mysql();
void print_mysql_error(const char *msg);
int executesql(const char *sql);
void create_database();
void create_table();
void init_administrator();

//socket buffer line
#define MAXLINE 1024

//gaming server center
int clientInterpretation(char *msg);

int main(int argc,char **argv)
{
    //mysql
    //connect to mysql server
    if (init_mysql())
        print_mysql_error(NULL);  
    //load or create database
    create_database();

    //socket parameters
    int listenfd, connfd;
    struct sockaddr_in sockaddr;
    char buff[MAXLINE];
    int n;

    memset(&sockaddr, 0, sizeof(sockaddr));

    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    sockaddr.sin_port = htons(10004);

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    bind(listenfd, (struct sockaddr *)&sockaddr, sizeof(sockaddr));
    listen(listenfd, 1024);

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
        printf("recv msg from client:%s", buff);

        //execute gaming processing
        

        
        close(connfd);
    }
    close(listenfd);

}
//gamine
int clientInterpretation(char *msg)
{
    char *cmd=strtok(msg,"|");
    
}


//mysql
int init_mysql()
{
    //init the database connection
    g_conn = mysql_init(NULL);
    //connection the database
    if (!mysql_real_connect(g_conn, g_host_name, g_user_name, g_password, g_db_name, g_db_port, NULL, 0))
        return -1; //链接失败
    if (executesql("set names utf8"))
        return -1;
    return 0; //返回成功
}

void print_mysql_error(const char *msg)
{
    if (msg)
        printf("%s: %s\n", msg, mysql_error(g_conn));
    else
        puts(mysql_error(g_conn));
}
int executesql(const char *sql)
{
    if (mysql_real_query(g_conn, sql, strlen(sql)))
        return -1;
    return 0;
}
void create_database()
{
    sprintf(sql, "use wordbattle");
    if (executesql(sql) == -1)
    {
        puts("create database");
        executesql("create database wordbattle;");
        print_mysql_error(NULL);
        puts("choice database");
        executesql("use wordbattle;");
        print_mysql_error(NULL);
        puts("!!!Initialize the success!!!");
    }
    else
    {
        executesql("use wordbattle;");
        print_mysql_error(NULL);
    }
}
void create_table()
{
    sprintf(sql, "show tables;");
    executesql(sql);
    g_res = mysql_store_result(g_conn);
    iNum_rows = mysql_num_rows(g_res);
    if (iNum_rows == 0)
    {
        puts("create users table");
        executesql("create table users(id_ int(11) unsigned primary key auto_increment,name_ char(255) not null unique,password_ char(32) not null,create_time_ datetime,creator_id_ int(11) unsigned,auth_type_ int(11) not null,dyn_sn_ char(10),dyn_pass_sn_ text,remark_ varchar(200),foreign key(creator_id_) references users(id_));");
    }
    mysql_free_result(g_res); //释放结果集
}

void init_administrator()
{
    sprintf(sql, "select * from users where id_='1' and name_='root';");
    executesql(sql);
    g_res = mysql_store_result(g_conn);
    iNum_rows = mysql_num_rows(g_res);
    if (iNum_rows == 0)
    {
        puts("Init Administrtor User");
        sprintf(sql, "insert into users values(1,'root','root','2017-08-18 12:21:11',1,0,'','','0:VIP 1:local pwd 2:local cert');");
        executesql(sql);
    }
    mysql_free_result(g_res); //释放结果集
}