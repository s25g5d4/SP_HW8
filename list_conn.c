/*
 * list_conn looks for live connections
 * It reports the total number and the age of the oldest
 */

#include "sms.h"

void list_conn(void) /* list number of current connections */
{
    pthread_mutex_lock(&door_lock);
    Servlet *it = door;
    if (it == NULL) {
        printf("There is no live connection\n");
        pthread_mutex_unlock(&door_lock);
        return;
    }

    time_t current_time = time(NULL);

    if (it->next == it) {
        printf("There is 1 live connection\n");
        printf("The oldest began %d seconds ago\n",
               (int)difftime(current_time, it->start));

       pthread_mutex_unlock(&door_lock);
       return;
    }

    int count = 1;
    int max_age = 0;

    while (it->next != door) {
        ++count;
        int diff = (int)difftime(current_time, it->start);
        if (diff > max_age)
            max_age = diff;

        it = it->next;
    }

    pthread_mutex_unlock(&door_lock);

    printf("There are %d live connections\n", count);
    printf("The oldest began %d seconds ago\n", max_age);
}
