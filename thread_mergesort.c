#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include<stdlib.h>

struct data
{
    int *array,start,end;
};
typedef struct data data;

void merge ( int start , int mid , int end , int array[] )
{
        //main index
        int m_i=start;
        //size of first half
        int n1=mid-start+1;
        //size of second half
        int n2=end-mid;
        //aux arrays
        int left[n1] , right[n2];

        //copy first half
        for(int i=0 ; i<n1; i++ )
            left[i]=array[start++];

        //copy second half
        for(int i=0 ; i<n2 ; i++)
            right[i]=array[start++];

        //resort main array
        int i=0 , l=0;
        while (i<n1 && l<n2)
        {
            if (left[i]>right[l])
                array[m_i++]=right[l++];

            else
                array[m_i++]=left[i++];
        }

        //add left items in both arrays
        while (i<n1)
            array[m_i++]=left[i++];

         while (l<n2)
            array[m_i++]=right[l++];
}

void *merge_sort(void *arg )
{

    int start=((data*)arg)->start;
    int end=((data*)arg)->end;
    int size=end-start+1;
    int *arr=malloc((size)*4);
    arr=((data*)arg)->array;

    if (end > start)
    {
        pthread_t th[2];
        data d[2];
        int mid= (end+start)/2;

        d[0].array=malloc(4*size);
        d[0].array=  arr;
        d[0].start=start;
        d[0].end=mid;

        d[1].array=malloc(4*size);
        d[1].array= arr;
        d[1].start=mid+1;
        d[1].end=end;

        pthread_create(&th[0],NULL,&merge_sort,&d[0]);
        pthread_create(&th[1],NULL,&merge_sort,&d[1]);

        pthread_join(th[0],NULL);
        pthread_join(th[1],NULL);

        merge(start ,mid, end , arr );
    }
}

int main(){

    pthread_t th;
    data d;
    int size;
    char name[20];
    printf("Insert File Name: ");
    scanf("%s",name);
    FILE *f=fopen(name,"r");

    //scan index
    fscanf(f,"%d", &size);
    int arr[size];

    //fill array
    for(int i=0; i<size; i++)
        fscanf(f, "%d\n",&arr[i] );
    fclose(f);


    d.array=malloc(size *4);
    d.array=arr;
    d.start=0;
    d.end=size-1;

    pthread_create(&th,NULL,&merge_sort,&d);
    pthread_join(th,NULL);

    printf("sorted array :\n");
    for(int i=0; i<size; i++)
        printf("%d\t",d.array[i] );
    printf("\n");

}
