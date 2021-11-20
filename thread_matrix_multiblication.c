#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include<stdlib.h>
#include<time.h>

 int out[9][9]; //op array

 struct data
{int **a1,**a2,x,y,index;};
typedef struct data data;


//read arrays
void  read_arr( int m , int n , int c[m][n] , int line ,char *name){

	char ch;
	int linecount=0;

	FILE *f=fopen(name,"r");
	if(f == NULL)
   {
      printf("error reading array!");
      exit(1);
   }

	//
	while ( linecount!=line )
	{
			 ch=fgetc(f);
 			 if (ch=='\n')
    			linecount++;
	}

   	//scan array
   	for(int i=0 ; i<m ; i++)
   	{
   		for(int j=0 ; j<n ; j++)
   			{fscanf(f,"%d", &c[i][j]);}
   	}
   	fclose(f);
}

void *mul_by_element(void * d){

	int output=0;
	int index=((data*)d)->index;
	int x=((data*)d)->x;
	int y=((data*)d)->y;

	while(index >= 0)
	{
		output+= ((data *)d)->a1[x][index] * ((data *)d)->a2[index--][y];
	}

	out[x][y]=output;
}

void *mul_by_row(void * d){

	int output=0;
	int index=((data*)d)->index;
	int x=((data*)d)->x;
	int y=((data*)d)->y;
	int c=((data*)d)->y;

	//x=nu of row = threads
	//y=nu of columns index[3]-1
	//index number of multi index[1]-1
	for (int i = 0; i <= y ; i++)
	{
		index=((data*)d)->index;
		output=0;

		while(index >= 0)
	{
		output+= ((data *)d)->a1[x][index] * ((data *)d)->a2[index--][i];
	}
		out[x][i]=output;
	}
}


int main(){


	double cpu_time_used;

	int index[4], h=0;
	int linecount=0;

	char ch;
	char name[20];

	//file name
    printf("Insert File Name: ");
    scanf("%s", name);

	FILE *f=fopen(name,"r");
	if(f == NULL)
   {
      printf("error reading file!");
      exit(1);
   }

	//reading array
	//get dimentions of first array and scan it
	fscanf(f,"%d %d", &index[0],&index[1]);
	int arr1[index[0]][index[1]];
	read_arr(index[0],index[1],arr1,1,name);



	//move file pointer to sec array
	//get dimentions of sec array and scan it
	while (    linecount!=index[0]+1 )
	{
			 ch=fgetc(f);
 			 if (ch=='\n')
    			linecount++;
	}

	fscanf(f,"%d %d", &index[2],&index[3]);
	int arr2[index[2]][index[3]];
	read_arr(index[2],index[3],arr2,index[0]+2,name);

	fclose(f);

	if(index[1]!=index[2])
	{
		printf("Multipication of Matrix is not Possible !!\n");
		exit(1);
	}

	// dim of output index[0] * index[3]
	int nu=index[0]*index[3];
	pthread_t th[nu];
	//array of structs
	data d[nu];
	nu=0;


	//---------------------------------------------------------

	clock_t start = clock();
    //thread for every output element
	for (int i = 0; i < index[0]; i++)
	{
		for (int j = 0; j < index[3]; j++)
		{
			d[h].a1=malloc	(4*index[0]*index[1]);
			d[h].a2=malloc	(4*index[2]*index[3]);

			for (int k = 0; k< index[0] ; k++)
			d[h].a1[k]= arr1 [k] ;
			for (int k = 0; k < index[2] ; k++)
			d[h].a2[k]= arr2 [k] ;

			//nu of multiplcations occurs
			d[h].index=index[2]-1;

			//index of element
			d[h].x=i;
			d[h].y=j;

			pthread_create(&th[nu++],NULL,&mul_by_element,&d[h++]);
		}}

	//to keep exec till all threads are done
	nu--;
	while(nu>=0)
	{pthread_join(th[nu--],NULL);}

    clock_t end = clock();
    cpu_time_used = ( (double)(end - start) )/ CLOCKS_PER_SEC;

	//free allocated space
	nu=0;
	while(nu< index[0]*index[3])
	{
		free(d[nu].a1);
		free(d[nu++].a2);
	}


	//writing array into file
	f=fopen("out.txt","w");
	if(f == NULL)
   {
      printf("error writing array!");
      exit(1);
   }

   fprintf(f,"BY ELEMENT:\n");
	for (int i = 0; i < index[0]; i++)
	{
		for (int j = 0; j < index[3]; j++)
		{
			fprintf(f, "%d\t",out[i][j] );
		}
		fprintf(f, "\n" );
	}

	printf("TIME TAKEN 1: %f\n",cpu_time_used );
	fprintf(f, "END1  [TIME TAKEN FOR PROCEDURE 1 : %f]\n","cpu_time_used" );

	fclose(f);



	//x=nu of row thread
	//y=nu of columns index[3]
	//index number of multi index[2]
	d[ index[0] ];
	th[index[0]];

	start = clock();

	for (int i = 0; i < index[0]; i++)
	{
			//malloc space for struct of each thread
			d[i].a1=malloc	(4*index[0]*index[1]);
			d[i].a2=malloc	(4*index[2]*index[3]);

			//copying arrays into structs
			for (int k = 0; k< index[0] ; k++)
			d[i].a1[k]= arr1 [k] ;
			for (int k = 0; k < index[2] ; k++)
			d[i].a2[k]= arr2 [k] ;

			//nu of multiplcations occurs
			d[i].index=index[2]-1;

			//nu or row
			d[i].x=i;
			//nu of columns
			d[i].y=index[3]-1;

			pthread_create(&th[i],NULL,&mul_by_row,&d[i]);
	}

	nu=0;
	while(nu<index[0])
	{pthread_join(th[nu++],NULL);}

	end = clock();
	cpu_time_used = ( (double)(end - start) )/ CLOCKS_PER_SEC;

	//free allocated space
	nu=0;
	while(nu< index[0])
	{
		free(d[nu].a1);
		free(d[nu++].a2);
	}

	//writing array into file
	f=fopen("out.txt","a");
	if(f == NULL)
   {
      printf("error writing array!");
      exit(1);
   }

   fprintf(f,"BY ROW:\n");
	for (int i = 0; i < index[0]; i++)
	{
		for (int j = 0; j < index[3]; j++)
		{
			fprintf(f, "%d\t",out[i][j] );
		}
		fprintf(f, "\n" );
	}

	printf("TIME TAKEN 2: %f\n",cpu_time_used );
	fprintf(f, "END2  [TIME TAKEN FOR PROCEDURE 2 : %f]\n","cpu_time_used" );

	fclose(f);

}
