#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *takeNap(void *threadid) {
    // pull the thread name off of the function param
    int tid;
    tid = (int)threadid;

    printf("'Allo It me, thread #%d!\nI like napping, so I'm going to go sleep for 2 seconds \n", tid);
    sleep(2);
    printf("THREAD 1 HERE, I'm BACK, SICK nap, I'M like, gonna STOP EXISTING NOW\n");
    pthread_exit(NULL);
}

int main () {
    pthread_t myThread; // look! A name for our beautiful child to be. So proud.
    int rdata;          // our thread might return an error so we should capture that too.
    int counter;        // we use this for a FOR loop below.


    // Let's make that cake. I mean thread.
    rdata = pthread_create(&myThread, NULL, takeNap, (void *) 1);

    // Let's check in case our new thread returns an error code.
    if (rdata) {
        printf("ERROR: pthread_create() returned an error code of %d\n", rdata);
        exit(-1);
    }

    // Let's make the main function do some work tho right? Let's count some HATS OK?
    for(counter=0; counter<5; counter++){
       sleep(1);
       printf("I see %d cool hats\n", counter);
    }

    // we should exit all our threads I guess.
    pthread_exit(NULL);
}

