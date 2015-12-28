/*
 * list_stats produces a report from the stats heap
 */

#include "sms.h"

void list_stats(void) /* Summmarise statistics */
{
    pthread_mutex_unlock(&stat_lock);

    Stats *it = history;
    if (it == NULL) {
        pthread_mutex_unlock(&stat_lock);
        printf("No connections on record\n");
        return;
    }

    int count = 0;
    int aborted = 0;
    int size = 0;
    int connect_time = 0;

    while (it != NULL) {
        ++count;
        size += it->size;
        connect_time += it->connect;
        if (it->aborted == 1)
            ++aborted;

        it = it->next;
    }

    pthread_mutex_unlock(&stat_lock);

    double percent_aborted = (double)aborted / (double)count;
    double average_connect = (double)connect_time / (double)count;
    double average_size = (double)size / (double)count;

    printf("%d connections\n", count);
    printf("%.1lf aborted\n", percent_aborted);
    printf("average connect = %.1lf seconds\n", average_connect);
    printf("average size = %.1lf bytes\n", average_size);
}
