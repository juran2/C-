#ifndef _PTHREAD_POOL_
#define _PTHREAD_POOL_

#include <mutex>
#include <thread>
#include <condition_variable>
#include <chrono>
#include <iostream>
#include <list>
#include <vector>
#include <memory>

using namespace std;

template<class T>
class LoginPool
{

public:
    LoginPool(int thread_num = 20,int max_task_num =30 );
    ~LoginPool();
    void Put( T *task);
    void Take( T *task);
    bool Empty();
    bool Full();
    size_t Size();
    void Start(); 
    void Stop();
    void Run();
private:
    int thread_number;  //线程池的线程数
    int max_task_number;  //任务队列中的最大任务数
    inline bool isFull() const
    {
        return m_queue.size() == max_task_number;
    }
    inline bool isEmpty() const
    {
        return m_queue.empty();
    }
   
    list<T *> m_queue;  //队列
    mutex m_mutex;  //互斥锁
    condition_variable m_notEmpty; //不为空时的条件变量
    condition_variable m_notFull; //不为满时的条件变量
    bool is_stop=false;
    
    
};

template<class T>
LoginPool<T>::LoginPool(int thread_num,int max_task_num):
    thread_number(thread_num),max_task_number(max_task_num)
{

}

template<class T>
LoginPool<T>::~LoginPool()
{

}

template<class T>
void LoginPool<T>::Put(T *task)
{
   
    unique_lock<mutex> lck(m_mutex);
   
    while(isFull())
    {
        cout << "排队人数已满["<< max_task_number <<"]"<<endl;
        m_notFull.wait(lck);
    }
     
    m_queue.push_back(task);
     cout << "put:" << task->get_num()<< "  left["<< m_queue.size() <<"]" << endl;
    m_notEmpty.notify_one();
}

template<class T>
void LoginPool<T>::Take(T *task)
{
   
    unique_lock<mutex> lck(m_mutex); 
    while(isEmpty())
    {
        cout << "现在没有人排队---" << endl;
        m_notEmpty.wait(lck);
    }
    task = m_queue.front();
   
    m_queue.pop_front();
      cout << "take:" << task->get_num()<< "  left["<< m_queue.size() <<"]" << endl;
    m_notFull.notify_one();
}

template<class T>
bool LoginPool<T>::Empty()
{
    return false;
}

template<class T>
bool LoginPool<T>::Full()
{
    return false;
}

template<class T>
void LoginPool<T>::Start()
{
    for(int i = 0; i < thread_number; i++)
    {
        thread mythread(&LoginPool::Run,this);
        mythread.detach();  //分为join 和detach
    }
    return;
}

template<class T>
void LoginPool<T>::Run()
{
    while(!is_stop)
    {
        // printf("++%d\n",is_stop);
        unique_lock<mutex> lck(m_mutex);
        if(m_queue.empty())
        {
            lck.unlock();
            continue;
        }
        T *task =m_queue.front();
        m_queue.pop_front();
        lck.unlock();
        if(!task)
            continue;
        task->login();
        // printf("++\n");
        
    }
}


template<class T>
void LoginPool<T>::Stop()
{
    is_stop=true;
}






















#endif