#include <pthread.h>					//It Will  Create POSIX threads.
#include <unistd.h>					//Thread calls sleep for specified number of seconds.
#include <semaphore.h>					//This header file is to create semaphores
#include <time.h>					//This header file is used for Wait for the random time.
#include <stdlib.h>			
#include <stdio.h>					//Input Output header file

pthread_t *Students;					//N threads running as Students.
pthread_t TA;						//Separate Thread for TA.

int ChairsCount = 0;					//This is a temp variable for counting the no of chairs avialable		
int CurrentIndex = 0;					//This is temp variable for giving currentindex of the student 

							//Declaration of Semaphores and Mutex Lock.
sem_t TA_Sleep;
sem_t Student_Sem;
sem_t ChairsSem[3];
pthread_mutex_t ChairAccess;

							//Declared Functions
void *TA_Activity();
void *Student_Activity(void *threadID);
void *TA_Activity()
{
	while(1)
	{
		sem_wait(&TA_Sleep);									//TA is currently sleeping as no other student is avialable 
		printf("---------------------------TA has been awakened by a student.-----------------------------\n");

		while(1)
		{
													// lock the room for help so taht another student will not come
			pthread_mutex_lock(&ChairAccess);
			if(ChairsCount == 0) 
			{
													//if chairs are empty, break the loop that is no student nis there for help .
				pthread_mutex_unlock(&ChairAccess);
				break;
			}
													//TA gets next student on chair.
			sem_post(&ChairsSem[CurrentIndex]);
			ChairsCount--;
			printf("Student left his/her chair. Remaining Chairs %d\n", 3 - ChairsCount);
			CurrentIndex = (CurrentIndex + 1) % 3;
			pthread_mutex_unlock(&ChairAccess);
													// unlock

			printf("\t TA is currently helping the student.\n");
			sleep(5);
			sem_post(&Student_Sem);
			usleep(1000);
		}
	}
}

void *Student_Activity(void *threadID) 
{
	int ProgrammingTime;

	while(1)
	{
		printf("Student %ld is doing programming assignment.\n", (long)threadID);
		ProgrammingTime = rand() % 10 + 1;
		sleep(ProgrammingTime);								//teacher sleeps for random period Sleep for a random time period.

		printf("Student %ld needs help from the TA\n", (long)threadID);
		
		pthread_mutex_lock(&ChairAccess);
		int count = ChairsCount;
		pthread_mutex_unlock(&ChairAccess);

		if(count < 3)									//As no more chairs avialable for Student but he again tried to sit on a chair but he is unable to sit.
		{
			if(count == 0)								//If student sits on first empty chair, wake up the TA.
				sem_post(&TA_Sleep);
			else
				printf("Student %ld sat on a chair waiting for the TA to finish. \n", (long)threadID);

												// lock
			pthread_mutex_lock(&ChairAccess);
			int index = (CurrentIndex + ChairsCount) % 3;
			ChairsCount++;
 			printf("Student sat on chair.Chairs Remaining: %d\n", 3 - ChairsCount);
			pthread_mutex_unlock(&ChairAccess);
												// unlock

			sem_wait(&ChairsSem[index]);						//Student leaves his/her chair.
			printf("\t Student %ld is getting help from the TA. \n", (long)threadID);
			sem_wait(&Student_Sem);							//Student waits to go next.
			printf("Student %ld left TA room.\n",(long)threadID);
		}
		else 
			printf("Student %ld will return at another time. \n", (long)threadID);
												//If student didn't find any chair to sit on.
	}
}
int main(int argc, char* argv[])
{
	int number_of_students;				//a variable taken from the user to create student threads.	
	int id;						//Default is 5 student threads so that if more than 3 students came its ask next student to come after some time .
	srand(time(NULL));				//random time 

							//Initializing Mutex Lock and Semaphores.
	sem_init(&TA_Sleep, 0, 0);
	sem_init(&Student_Sem, 0, 0);

	for(id = 0; id < 3; ++id)			//Chairs array of 3 semaphores.
		sem_init(&ChairsSem[id], 0, 0);		

	pthread_mutex_init(&ChairAccess, NULL);
	
	if(argc<2)
	{
		printf("Number of Students not specified. Using default (5) students.\n");   		// print that suppose there are five student come for the help 
													//by default one by one
		number_of_students = 5;
	}
	else
	{
		printf("Number of Students specified. Creating %d threads.\n", number_of_students);
		number_of_students = atoi(argv[1]);
	}
		
													//Allocate memory for Students
	Students = (pthread_t*) malloc(sizeof(pthread_t)*number_of_students);

													//Creating TA thread and N Student threads.
	pthread_create(&TA, NULL, TA_Activity, NULL);	
	for(id = 0; id < number_of_students; id++)
		pthread_create(&Students[id], NULL, Student_Activity,(void*) (long)id);

													//Waiting for TA thread and N Student threads.
	pthread_join(TA, NULL);
	for(id = 0; id < number_of_students; id++)
		pthread_join(Students[id], NULL);

													//Free allocated memory
	free(Students); 
	return 0;
}
