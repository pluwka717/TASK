#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <string.h>

typedef struct {
    int *arr;
    int l;
    int r;
} thr_arg;

pthread_mutex_t mutex_count;
pthread_cond_t cond_count;
int current_th =0;
int MAX_current_th=4;
int thread_minsize=1000;

void print_arr (int *arr,int r)
{
    for (int i=0;i<r;i++)
    {
        printf("- %d\n",*(arr+i));
    }
}

void merge (int *arr, int l, int m, int r)
{
    int i,j,k=l;
    int size_first= m+1-l;
    int size_second=r-m;

    int *first_arr=(int *)malloc(size_first*sizeof(int));
    int *second_arr=(int *)malloc(size_second*sizeof(int));

    for (i=0;i<size_first;i++)
    {
        *(first_arr+i)=*(arr+i+l);
    }

    for (j=0;j<size_second;j++)
    {
        *(second_arr+j)=*(arr+j+m+1);
    }

    i=j=0;

    while (i< size_first && j<size_second)
    {
        if (*(first_arr+i) <= *(second_arr+j)) {
            *(arr+k)=*(first_arr+i);
            i++;
        }
        else {
            *(arr+k)=*(second_arr+j);
            j++;
        }
        k++;
    }
        while (i<size_first)
        {
        *(arr+k)=*(first_arr+i);
        i++;
        k++;
        }
        while (j<size_second)
        {
            *(arr+k)=*(second_arr+j);
            j++;
            k++;
        }

    free(first_arr);
    free(second_arr);
}

void sequential_sort(int *arr,int l,int r)
{
    if (l<r) {
        int m=l+(r-l)/2;
        sequential_sort(arr,l,m);
        sequential_sort(arr,m+1,r);
        merge(arr,l,m,r);
    }
}

void get_rand_array(int *arr, int size,int range)
{
        for (int i=0;i<size;i++)
    {
        *(arr+i)=(rand() % range);
    }
}

void *thread_ent (void *arg);

void merge_thread_sort(int *arr,int l,int r)
{
    if(l>=r){return;}
    int m=l+(r-l)/2;

    pthread_t tid_left;
    thr_arg *args_left=0;
    int th_created=0;

    pthread_mutex_lock(&mutex_count);
    if (current_th < MAX_current_th && (r-l+1)>thread_minsize){
        current_th++;
        pthread_mutex_unlock(&mutex_count);

        args_left = (thr_arg *)malloc (sizeof(thr_arg));
        if(!args_left){
            printf("error: thread storage\n");
            pthread_mutex_lock(&mutex_count);
            current_th--;
            pthread_mutex_unlock(&mutex_count);
            exit(EXIT_FAILURE);
        }
        args_left->arr=arr;
        args_left->l=l;
        args_left->r=m;

        if (pthread_create(&tid_left,NULL,thread_ent,args_left)!=0) {
            free(args_left);
            pthread_mutex_lock(&mutex_count);
            current_th--;
            pthread_mutex_unlock(&mutex_count);
            sequential_sort(arr,l,m);
        } else {
            th_created=1;
            }
        }
    else {
            pthread_mutex_unlock(&mutex_count);
            sequential_sort(arr,l,m);
    }
    merge_thread_sort(arr,m+1,r);
        
    if (th_created){
        pthread_join(tid_left,NULL);
        pthread_mutex_lock(&mutex_count);
        current_th--;
        pthread_cond_signal(&cond_count);
        pthread_mutex_unlock(&mutex_count);
        }
    merge(arr,l,m,r);
}
    
void *thread_ent (void *arg)
{
    thr_arg *params = (thr_arg *)arg;

    merge_thread_sort(params->arr,params->l,params->r);
    free(params);

    return NULL;
}

int main ()
{
    int i,size,range;
    srand((int)time(NULL));
    printf("Start\nPut size of array:");
    scanf("%d",&size);
    printf("Put range of array:");
    scanf("%d",&range);
    int *arr=(int*)malloc(size*sizeof(int));
    if (!arr){
        printf ("error: array doesnt created\n");
        exit(EXIT_FAILURE);
    }

    get_rand_array(arr,size,range);

    print_arr(arr,size);
    printf ("\n");

    merge_thread_sort(arr,0,size-1);

    print_arr(arr,size); 

    free(arr);
    return 0;
    
}
