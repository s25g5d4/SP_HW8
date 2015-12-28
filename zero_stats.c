/*
 * zero_stats just frees the stats stack (means "history")
 */

#include "sms.h"

void zero_stats(void) /* re-start statistics */
{
    pthread_mutex_lock(&stat_lock);
    Stats *it = history;
    history = NULL;
    pthread_mutex_unlock(&stat_lock);

    while (it != NULL) {
        Stats *temp = it;
        it = it->next;
        free(temp);
    }

    printf("Statistics is reset...\n");
}
