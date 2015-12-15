/*
 *	sms_server.c :
 *	1. All global variables are defined in sms.h
 *	2. main, which does the following
 *		a) Initialize the circular list
 *		b) Fire off threads for collect_garb and listen_port
 *		c) Run the menu
 */

#include "sms.h"

Servlet *door;			/* entrance to doubly linked list		*/
Pending *pending_stack;	/* pile of stale servlets				*/
Stats * history;			/* start of stats linked list			*/
sem_t garbage_time;		/* schedule garbage collection			*/
pthread_attr_t attr;		/* set to make ALL threads detached		*/

void main(int argc, char **argv) {
    Menu_item items[4] = {
        { "1) Display number of current connections", list_conn },
        { "2) Display statistics (average connect time etc.)", list_stats },
        { "3) Re-start statistics gathering", zero_stats },
        { "4) Kill stale clients", zap_stale }
    };

    Menu_item *menus[5] = {
        items,
        items + 1,
        items + 2,
        items + 3,
        NULL
};

    pthread_attr_init(&attr);
    sem_init(&garbage_time, 0, 0);

    pthread_t garbage, listen;

    pthread_create(&garbage, &attr, collect_garb, NULL);
    pthread_create(&listen, &attr, listen_port, (void *)PORT);

    menu(menus);

    return 0;
}
