/*
 * sms_server.c :
 *     1. All global variables are defined in sms.h
 *     2. main, which does the following
 *         a) Initialize the circular list
 *         b) Fire off threads for collect_garb and listen_port
 *         c) Run the menu
 */

#include "sms.h"

Servlet *door;          /* entrance to doubly linked list   */
Pending *pending_stack; /* pile of stale servlets           */
Stats * history;        /* start of stats linked list       */
sem_t garbage_time;     /* schedule garbage collection      */
pthread_attr_t attr;    /* set to make ALL threads detached */

pthread_mutex_t door_lock;
pthread_mutex_t stat_lock;
pthread_mutex_t pend_lock;

int main(int argc, char **argv)
{
    Menu_item items[5] = {
        { "1) Display number of current connections", list_conn },
        { "2) Display statistics (average connect time etc.)", list_stats },
        { "3) Re-start statistics gathering", zero_stats },
        { "4) Kill stale clients", zap_stale },
        { NULL, NULL }
    };

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    sem_init(&garbage_time, 0, 0);

    pthread_mutex_init(&door_lock, NULL);
    pthread_mutex_init(&stat_lock, NULL);
    pthread_mutex_init(&pend_lock, NULL);

    pthread_t garbage, listen;

    pthread_create(&garbage, &attr, collect_garb, NULL);
    pthread_create(&listen, &attr, listen_port, NULL);

    menu(items);

    return 0;
}
