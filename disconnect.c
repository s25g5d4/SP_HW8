/*
 * disconnect is passed a pointer to a Servlet
 * It should perform the following :
 *     1) Take the servlet out of the circular list
 *     2) Close the socket connection
 *     3) Create a Pending item and push pending stack
 *     4) Alert the garbage collector
 */

#include "sms.h"

void disconnect(Servlet *done)
{
    pthread_mutex_lock(&door_lock);
    done->prev->next = done->next;
    done->next->prev = done->prev;
    if (door == done)
        door = done->next;
    if (door == done)
        door = NULL;
    pthread_mutex_unlock(&door_lock);

    done->finish = time(NULL);
    close(done->fd);

    Pending *pend = (Pending *)malloc(sizeof (Pending));
    pend->head = done;

    pthread_mutex_lock(&pend_lock);
    pend->tail = pending_stack;
    pending_stack = pend;
    pthread_mutex_unlock(&pend_lock);

    sem_post(&garbage_time);
}
