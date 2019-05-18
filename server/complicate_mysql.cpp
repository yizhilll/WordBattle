/********************************************
* 编译命令：gcc aa.c -lmysqlclient -o aa
* 执行命令：./aa
* ******************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "mysql/mysql.h"

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

/****************************************************
* time : 20180622
* addby : swj
* function :print_mysql_error() 打印错误信息
* ******************************************************/
void print_mysql_error(const char *msg)
{
    if (msg)
        printf("%s: %s\n", msg, mysql_error(g_conn));
    else
        puts(mysql_error(g_conn));
}
/****************************************************
* time : 20180622
* addby : swj
* function :executesql() 执行sql语句，成功返回0，失败返回-1
******************************************************/
int executesql(const char *sql)
{
    if (mysql_real_query(g_conn, sql, strlen(sql)))
        return -1;
    return 0;
}
/****************************************************
* time : 20180622
* addby : swj
* function :init_mysql() 初始化链接
******************************************************/
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
/****************************************************
* * time : 20180622
* * addby : swj
* * function :create_database() 选择数据库 没有的时候 创建数据;有的时候 进去数据
* 库
* ******************************************************/
void create_database()
{
    sprintf(sql, "use workdata");
    if (executesql(sql) == -1)
    {
        puts("create database");
        executesql("create database workdata;");
        print_mysql_error(NULL);
        puts("choice database");
        executesql("use workdata;");
        print_mysql_error(NULL);
        puts("!!!Initialize the success!!!");
    }
    else
    {
        executesql("use workdata;");
        print_mysql_error(NULL);
    }
}
/****************************************************
* * time : 20180622
* * addby : swj
* * function :create_table() 创建表
* ******************************************************/
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
/****************************************************
* * time : 20180622
* * addby : swj
* * function :init_administrator() 初始化管理员账户
* * 管理员用户名：root 密码：root
* * ******************************************************/
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
int main(void)
{
    puts("!!!The system is initializing!!!");
    /*初始化链接*/
    //在初始化的时候 数据库 test 是必须事先创建好的 否则会报错Unknown database 'test'
    if (init_mysql())
        print_mysql_error(NULL); //当链接数据库时候 有错误 会报错
    //选择数据库workdata 没有的时候 创建数据库 有的时候 进去数据库
    create_database();
    //创建表
    create_table();
    //初始化管理员账户
    init_administrator();
    //操作数据库 查询数据库 表中的信息
    if (executesql("select * from users"))
        print_mysql_error(NULL);
    g_res = mysql_store_result(g_conn);        // 从服务器传送结果集至本地，mysql_use_result直接使用服务器上的记录集
    int iNum_rows = mysql_num_rows(g_res);     // 得到记录的行数
    int iNum_fields = mysql_num_fields(g_res); // 得到记录的列数
    printf("共%d个记录，每个记录%d字段\n", iNum_rows, iNum_fields);
    printf("id_ | name_ |password_| create_time_ |creator_id_ | auth_type_ |dyn_sn_| dyn_pass_sn_ |remark_ \n");
    while ((g_row = mysql_fetch_row(g_res)))                                                                                                      // 打印结果集
        printf("%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n", g_row[0], g_row[1], g_row[2], g_row[3], g_row[4], g_row[5], g_row[6], g_row[7], g_row[8]); // 第一，第二字段
    mysql_free_result(g_res);                                                                                                                     // 释放结果集
    mysql_close(g_conn);
    return EXIT_SUCCESS;
}