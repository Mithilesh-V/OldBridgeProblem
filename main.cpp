#include <iostream>
using namespace std;
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>

int currentdirection; // 0 or 1
int cars; // no of cars on the bridge
int waiters[2];
pthread_cond_t waitingToGo[2]; //waiting queues for each direction
pthread_mutex_t lock;
int Max_Run = 1;

void ArriveBridge(int direction);                                                                                                                                                                                                                                                                                                                                                                                                                                                               void ExitBridge();
void ExitBridge();

void *vehicle( void * arg)
{
        time_t t;
        int *id = (int *) arg;
        char Dir[2][4]= {"<--","-->"};
        srand((unsigned) time(&t));
        int i;
        int Direction;

        cout << "Vehicle-"<< *id << " has started its journey !!!!!!!!!!" << endl;
        for (i = 1; i <= Max_Run; i++)
        {                                               // for each crossing
                sleep(2);                           // rest for a while
                Direction = rand() % 2;            // a random direction

                cout <<"Vehicle-"<< *id
                << " (" << i << ") arrives at the bridge in direction "
                << Dir[Direction] << endl;
                ArriveBridge(Direction);         // arrive at the bridge

                cout <<"Vehicle-"<< *id
                << " (" << i << ") is on the bridge......" << endl;
                sleep(2);                           // crossing the bridge

                ExitBridge();                           // exit the bridge
                cout <<"Vehicle-"<< *id
                << "(" << i << ") has left the bridge" << endl;
        }

        cout <<"Vehicle-"<< *id << " journey has ended." << endl;
}

int isSafe(int direction)
{
        if(cars==3)                             //Make the thread wait if number of cars in the bridge are 3
        {
                return 1;
        }
        else if((cars > 0) && (currentdirection != direction))      //Make the thread wait if the current direction is diffferent from vehicle direction
        {
                return 1;
        }
        else
        {
                return 0;
        }
}


void ArriveBridge(int direction)
{
        pthread_mutex_lock(&lock);
        // while can't get on the bridge, wait

        while (isSafe(direction))
        {
                waiters[direction]++;
                pthread_cond_wait(&waitingToGo[direction],&lock);
                waiters[direction]--;
        }

        // get on the bridge
        cars++;

        currentdirection = direction;

        pthread_mutex_unlock(&lock);
}


void ExitBridge()
{
        pthread_mutex_lock(&lock);

        // get off the bridge
        cars--;

        // if anybody wants to go the same direction, wake them
        if (waiters[currentdirection] > 0)
        {
                pthread_cond_signal(&waitingToGo[currentdirection]);
        }

        // else if empty, try to wake somebody going the other way
        else if (cars == 0)
        {
                pthread_cond_broadcast(&waitingToGo[1-currentdirection]);
        }

        pthread_mutex_unlock(&lock);
}


int main(int argc,char *argv[])
{
        int max_run,i;
        int threads;
        int j[50];
        pthread_t t_id[50];
        threads = abs(atoi(argv[1]));

        for(i=0;i<threads;i++)          //creating a thread for each vehicle
        {
                j[i]= i + 1;
                sleep(1);
                pthread_create(&t_id[i], NULL, vehicle,(void *) &j[i]);
        }
        for( i = 0;i<threads; i++)
        {
                pthread_join(t_id[i], NULL);
        }
        pthread_mutex_destroy(&lock);
}
