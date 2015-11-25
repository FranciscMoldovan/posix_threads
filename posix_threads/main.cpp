#include <iostream>
extern "C"
{
#include <pthread.h>
}

using namespace std;

pthread_mutex_t count_mutex     = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t  condition_var   = PTHREAD_COND_INITIALIZER;

void *function1(void *arg);
void *function2(void *arg);

int  count = 0;

const int COUNT_DONE  = 10;

main()
{
  pthread_t thread1, thread2;

  pthread_create(&thread1, NULL, &function1, NULL);
  pthread_create(&thread2, NULL, &function2, NULL);

  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);

  cout << "Final count: " <<  count << endl;

  return 0;
}

// Write numbers 1-3 and 8-10 as permitted by function2()

void *function1(void *arg)
{
  // function1 (thread1) likes to add 1 to count regardless of it's current value,
  // but will only do it if function2 is done working with count.

  for(;;)
  {
    // Lock mutex and then wait for signal to relase mutex
    pthread_mutex_lock(&count_mutex);


    // frunction1 gives control over count to function2 to see if function2 wants to do anything
    // with count. It will unlock count until function2 signals it's done working with count.
    pthread_cond_wait(&condition_var, &count_mutex);

    // add this point function2 has given back control of count to funciton1 and
    // has either added 1 to count or gave back control to function1 without changing count.
    cout << "function1 looks at count (" << count << ") and just adds 1 to it." << endl;
    count++;

    pthread_mutex_unlock(&count_mutex);

    if(count >= COUNT_DONE)
    {
      pthread_exit(NULL);
    }
  }
}



void *function2(void *arg)
{
  // function 2 only likes to add 1 to odd numbers and will leave even numbers alone.

  for(;;)
  {
    pthread_mutex_lock(&count_mutex);

    if(count & 1)   // check if count is odd
    {
      cout << "function2 detected count (" << count << ") is odd, so it adds 1 to it." << endl;
      count++;
    }
    else
    {

      // function 2 detected the number is even, and will leave it alone and signal function1
      // it has control back over count.
      pthread_cond_signal(&condition_var);

    }

    pthread_mutex_unlock(&count_mutex);

    if(count >= COUNT_DONE)
    {
      pthread_exit(NULL);
    }
  }

}
