/*
 * zap_stale does the following
 * for each stale client found
 *     1) Set abort flag
 *     2) Kill the thread
 *     3) Call disconnect
 */

#include "sms.h"

void zap_stale(void) /* call disconnect when needed */
{
    printf("How many seconds as stale ? ");

    int stale;
    scanf("%d", &stale);

    Servlet *found = get_stale(stale);
    if (found != NULL) {
        printf("Found a stale one\n");
        found->aborted = 1;
        char *stale_msg = "Sorry time's up\n";
        write(found->fd, stale_msg, strlen(stale_msg));

        pthread_cancel(found->thread);
        disconnect(found);
        return;
    }

    printf("Not found any stale connection\n");
}
