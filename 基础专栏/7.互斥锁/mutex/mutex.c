#include<linux/init.h>
#include<linux/module.h>
#include<linux/mutex.h>
#include <linux/kthread.h>

//定义一把锁
struct mutex my_mutex;

//模拟的共享资源
int shared_resource = 0;

// 线程函数1
static int thread_func_1(void *data)
{
    // 3. 加锁
    mutex_lock(&my_mutex);
    printk(KERN_INFO "Thread 1: Lock acquired\n");
    shared_resource++;
    // 4. 解锁
    mutex_unlock(&my_mutex);
    printk(KERN_INFO "Thread 1: Lock released\n");
    return 0;
}

// 线程函数2
static int thread_func_2(void *data)
{
    // 3. 加锁
    mutex_lock(&my_mutex);
    printk(KERN_INFO "Thread 2: Lock acquired\n");
    shared_resource++;
    // 4. 解锁
    mutex_unlock(&my_mutex);
    printk(KERN_INFO "Thread 2: Lock released\n");
    return 0;
}

static int __init my_init(void)
{
    // 2. 初始化互斥锁
    mutex_init(&my_mutex);

    // 使用kthread_create创建内核线程
    struct task_struct *thread1 = kthread_create(thread_func_1, NULL, "thread_1");
    struct task_struct *thread2 = kthread_create(thread_func_2, NULL, "thread_2");

    // 唤醒创建的线程使其开始运行
    if (thread1) {
        wake_up_process(thread1);
    }
    if (thread2) {
        wake_up_process(thread2);
    }

    return 0;
}

static void __exit my_exit(void)
{
    printk(KERN_INFO "Exiting module\n");
}

module_init(my_init);
module_exit(my_exit);
MODULE_LICENSE("GPL");