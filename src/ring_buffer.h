#ifndef _RING_BUFFER_H
#define _RING_BUFFER_H 

#include <semaphore.h>
#include <pthread.h>

typedef pthread_mutex_t mutex_t;

struct ring_buffer_t
{
    char* data;      
    size_t size;     
    size_t rpos, wpos;
    int isfull; 

    sem_t sem;  
    mutex_t lock;
};

int rb_init(struct ring_buffer_t* rb, size_t size);

int rb_pop(struct ring_buffer_t* rb, char* buf);

int rb_push(struct ring_buffer_t* rb, char c);

void rb_free(struct ring_buffer_t* rb);


#endif /* _RING_BUFFER_H */ 
