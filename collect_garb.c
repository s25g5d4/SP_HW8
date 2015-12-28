/*
 * collect_garb :
 * This function has the following responsibilities :
 *     1) Call zap_servlet
 *     2) Remove the head of the pending tray
 * collect_garb is activated by the garbage_time semaphore
 */

#include "sms.h"

void *collect_garb(void *info) /* garbage collector */
{
    while (1) {
        sem_wait(&garbage_time);

        pthread_mutex_lock(&pend_lock);
        Pending *to_remove = pending_stack;
        pending_stack = to_remove->tail;
        pthread_mutex_unlock(&pend_lock);

        zap_servlet(to_remove->head);
        free(to_remove);
    }

    return NULL;
}
