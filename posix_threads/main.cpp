extern "C"
{
#include<pthread.h>
#include<unistd.h>
}
#include <iostream>
using namespace std;
const int NUMBER_OF_THREADS=5;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int val=10;
void *thread_talk(void *thread_number)
{
  for(int x = 1; x<500; x++)
  {
      // lock the shared value here
      pthread_mutex_lock(&mutex);
      val*=10;
      val++;
      val--;
      val/=10;
      // unlock it for the next thread:
      pthread_mutex_unlock(&mutex);
  }
  pthread_exit(NULL);
}
int main()
{
    pthread_t thread[NUMBER_OF_THREADS];
    cout << "Starting all threads..." << endl;
    int temp_arg[NUMBER_OF_THREADS];
    for(int current_t=0; current_t<NUMBER_OF_THREADS; current_t++)
    {
        temp_arg[current_t]=current_t;
        int result = pthread_create(&thread[current_t], NULL,thread_talk,static_cast<void*>(&temp_arg[current_t]));
        if(result!=0)
        {
            cout << "Error creating thread " << current_t << ". Return code: " << result << endl;
        }
    }
    for(int current_t=0; current_t<NUMBER_OF_THREADS; current_t++)
    {
        pthread_join(thread[current_t], NULL);
    }
    cout << "All threads completed.";
return 0;
}







