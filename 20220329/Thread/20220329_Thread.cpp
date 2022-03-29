#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

// 1.1 pthread_create
// int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void *), void *arg)
 /*
void *t_function(void *data)
{
	int id;
	int i = 0;
	id = *((int *)data);
	
	while(1)
	{
		printf("%d : %d \n", id, i);
		i++;
		sleep(1);
	}
}
int main(void)
{
	pthread_t p_thread[2];
	int thr_id;
	int status;
	int a = 1;
	int b = 2;
	
	thr_id = pthread_create(&p_thread[0], NULL, t_function, (void*)&a);
	if (thr_id < 0)
    {
        perror("thread create error : ");
        exit(0);
    }

    // 쓰레드 생성 아규먼트로 2 를 넘긴다. 
    thr_id = pthread_create(&p_thread[1], NULL, t_function, (void *)&b);
    if (thr_id < 0)
    {
        perror("thread create error : ");
        exit(0);
    }

    // 쓰레드 종료를 기다린다. 
    printf("thread join : %d\n", status);
    pthread_join(p_thread[0], (void **)&status);
    pthread_join(p_thread[1], (void **)&status);
	printf("thread join : %d\n", status);
    return 0;
}*/
/*
1.2 pthread_join ->thread가 종료될때 까지 기다림 -> 종료 리턴값을 가져옴 
// 쓰레드 함수 
// 1초를 기다린후 아규먼트^2 을 리턴한다. 
int pthread_join(pthread_t th, void **thread_return);
void *t_function(void *data)
{
    int num = *((int *)data);
    printf("num %d\n", num);
    sleep(1);
    return (void **)(num*num);
}

int main(void)
{
    pthread_t p_thread;
    int thr_id;
    int status;
    int a = 100;

    thr_id = pthread_create(&p_thread, NULL, t_function, (void *)&a);
    if (thr_id < 0)
    {
        perror("thread create error : ");
        exit(0);
    }
    // 쓰레드 식별자 p_thread 가 종료되길 기다렸다가 
    // 종료리턴값을 가져온다. 
    pthread_join(p_thread, (void **)&status);
    printf("thread join : %d\n", status);

    return 0;
}
*/
/*
// 1.3 pthread_detach  - detach는 떼어내다라는 뜻을 가진다. -> detach 되었을경우 해당 쓰레드가 종료될 경우 즉시 모든 자원이 해체
pthread_detach(pthread_t th);
void *t_function(void *data)
{
    char a[100000];
    int num = *((int *)data);
 printf("Thread Start\n");
    sleep(5);
 printf("Thread end\n");
}
   
int main()
{
    pthread_t p_thread;
    int thr_id;
    int status;
    int a = 100;
	printf("Before Thread\n"); 
    thr_id = pthread_create(&p_thread, NULL, t_function, (void *)&a);
    if (thr_id < 0)
    {
        perror("thread create error : ");
        exit(0);
    }
    // 식별번호 p_thread 를 가지는 쓰레드를 detach 
    // 시켜준다. 
    pthread_detach(p_thread);
    pause();
    return 0;
}
*/

/*
//pthread_exit ->현재 실행중인 thread를 종료시킬때 사용 while문 안에 i가 3이면 종료

void pthread_exit(void *retval);
void *t_function(void *data)
{
    int num = *((int *)data);
    int i = 0;
    while(1)
    {
        if (i == 3)
            pthread_exit(0);
        printf("loop %d\n", i);
        i++;
        sleep(1);
    }
}
int main()
{
    pthread_t p_thread;
    int thr_id;
    int status;
    int a = 100;


    thr_id = pthread_create(&p_thread, NULL, t_function, (void *)&a);
    if (thr_id < 0)
    {
        perror("thread create error : ");
        exit(0);
    }
    pthread_join(p_thread, (void **)&status);
    return 0;
}
* */
/*
//pthread_cleanup_push -> cleanup handlers를 인스톨하기 위해서 사용됨 cleanup handler는 주로 자원을 되돌려주거나 잠금등의 해제를 위한 용도 열린 파일지정자를 닫기 위해도 사용. 

void pthrad_cleanup_push(void (*routine) (void *), void *arg);

char *mydata;
void cleanup(void *);
void *t_function(void *data)
{
    int num = *((int *)data);
    int i = 0;
    int a = 1;
    // cleanup handler 로 cleanup 함수를 
    // 지정한다. 
    pthread_cleanup_push(cleanup, (void *)&a);
    mydata = (char *)malloc(1000);
    while(1)
    {
        if (i == 3)
        {
            // pthread_exit 가 호출되면서 
            // cleanup 을 호출하게 된다. 
            pthread_exit(0);
            return 1;
        }
        printf("loop %d\n", i);
        i++;
        sleep(1);
    }
    pthread_cleanup_pop(0);
}
int main()
{
    pthread_t p_thread;
    int thr_id;
    int status;
    int a = 100;


    thr_id = pthread_create(&p_thread, NULL, t_function, (void *)&a);
    if (thr_id < 0)
    {
        perror("thread create error : ");
        exit(0);
    }
    pthread_join(p_thread, (void **)&status);
    printf("Join finish\n");
}

// cleanup handler
void cleanup(void *myarg)
{
    printf("thread is clean up\n");
    printf("resource free\n");
    free(mydata);
}
* */

//pthread_self

pthread_t pthread_self(void);

void *func(void *a)
{
    pthread_t id;
    id = pthread_self();
    printf("->%d\n", id);
}

int main(int argc, char **argv)
{
    pthread_t p_thread;
    pthread_create(&p_thread, NULL, func, (void *)NULL);
    printf("%d\n", p_thread);
    pthread_create(&p_thread, NULL, func, (void *)NULL);
    printf("%d\n", p_thread);

 return 1;
}
