#include "ring_buffer.h"
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

int rb_init(struct ring_buffer_t* rb, size_t size)
{
    rb->data = (char*)malloc(size);      

    if(rb->data == NULL)
        return -1;

    if(sem_init(&rb->sem, 0, 0) == -1) {
        free(rb->data);
        return -1;
    }

    rb->rpos = 0;
    rb->rpos = rb->wpos;
    rb->size = size;
    rb->isfull = 0;

    return 0;
}

int rb_pop(struct ring_buffer_t* rb, char* buf)
{
    if(sem_trywait(&rb->sem) == -1)
      return -1;  
    
    pthread_mutex_lock(&rb->lock);

    *buf = rb->data[rb->rpos];
    rb->rpos = (rb->rpos + 1) % rb->size;
    
    rb->isfull = 0;

    pthread_mutex_unlock(&rb->lock);

    return 0;
}

int rb_push(struct ring_buffer_t* rb, char c)
{
    pthread_mutex_lock(&rb->lock);
    
    if(rb->isfull)
        return -1;

    rb->data[rb->wpos] = c;
    rb->wpos = (rb->wpos + 1) % rb->size;

    if(rb->wpos == rb->rpos)
        rb->isfull = 1;

    sem_post(&rb->sem);

    pthread_mutex_unlock(&rb->lock);

    return 0;
}

void rb_free(struct ring_buffer_t* rb)
{
    free(rb->data);
    sem_destroy(&rb->sem);
}
