#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void print_arr (int *arr,int r)
{
    for (int i=0;i<r;i++)
    {
        printf("%d; ",*(arr+i));
    }
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

int main ()
{
    int i,size,range;
    srand((int)time(NULL));
    printf("Start\nPut size of array:");
    scanf("%d",&size);
    printf("Put range of array:");
    scanf("%d",&range);
    int *arr=(int*)malloc(size*sizeof(int));

    for (i=0;i<size;i++)
    {
        *(arr+i)=(rand() % range);
    }

    sequential_sort(arr,0,size-1);

    print_arr(arr,size); 

    free(arr);
    return 0;
    
}
