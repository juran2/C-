/*1.分为loginpool.h和login.cpp，内容分别为线程池类（里面包含队列）和User类（User的性质和登陆函数）;
  2.线程池本身可以看成 线程容器和队列，线程池一直执行队列里面的task，而线程的run可以看成是队列的出队；
  3.互斥锁这一部分可以封装一下。
  made by juran in 2019/4/22
*/
#include <iostream>
#include <thread>
#include "loginpool.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

class User
{
private:
    int number;
    char* name;
    char* password;
public:
    User(int num,char* name,char* password):number(num),
        name(name),password(password)
    {

    }
    ~User()
    {

    }

    int get_num()
    {
        return number;
    }

    bool login();
    bool logout();
};
bool User::login()
{
    sleep(rand()%5);
    if(name == "sospeed" && password == "123456")
    {
        printf("id [%d] name[%s] password[%s] 登陆成功! thread_id[%u]\n",number,name,password,std::this_thread::get_id());
        return true;
    }
    else
    {
        printf("id [%d] 账号或密码错误!",number);
    }
    return false;
}

bool User::logout()
{
    printf("id [%d] 登出！",number);
    return true;
}







int main()
{
     cout << "main_thread_id:"<<this_thread::get_id() << endl;
    User *user;
    LoginPool<User> pool(5,20);

    for(int i=0;i<15;i++)        //入队
    {
        user=new User(i,"sospeed","123456");
        pool.Put(user);
    }
    pool.Start();    //出队,出队用多线程，即是线程池
    sleep(10);
    pool.Stop();

     return 0;
    
}