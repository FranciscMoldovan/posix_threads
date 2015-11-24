#include <iostream>

extern "C"
{
#include <pthread.h>
#include <unistd.h>
}

using namespace std;

void *function1(void *argument);
void *function2(void *argument);

int main(void)
{
  pthread_t t1, t2; // declare two threads
  // create threads
  pthread_create(&t1, NULL, function1, NULL);
  pthread_create(&t2, NULL, function2, NULL);

  sleep(1);

  return 0;
}
void *function1(void *argument)
{
  cout << "hello ... " << endl ;
  sleep(2); // fall asleep for two seconds
  //pthread_exit(NULL);
}
void *function2(void *argument)
{
  cout << " ... world" << endl;
  pthread_exit(NULL);
}
