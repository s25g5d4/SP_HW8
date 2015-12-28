/*
 * zap_servlet does the following :
 *     1) Append the message to an appropriate file
 *     2) Capture statistics and push onto stat stack
 *     3) Dispose of the Servlet
 */

#include "sms.h"

void zap_servlet(Servlet *victim) /* remove and free memeory */
{
    char *allowed_char = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                         "abcdefghijklmnopqrstuvwxyz0123456789._-";
     int bad_filename = 0;
     char *c;

    if (strchr(allowed_char, victim->dest[0]) == NULL || victim->dest[0] == '-')
    {
        bad_filename = 1;
    }
    else {
        for (c = victim->dest + 1; *c != '\0'; ++c) {
            if (strchr(allowed_char, *c) == NULL)
                bad_filename = 1;
        }
    }

    if (!bad_filename) {
        int fd = open(victim->dest, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if (fd == -1) {
            printf("\nCan't open file \"%s\"\n", victim->dest);
        }
        else {
            write(fd, victim->message, victim->chars_read);
            close(fd);
        }
    }
    else {
        printf("\nBad filename \"%s\"\n", victim->dest);
    }

    Stats *new_stat;
    new_stat = (Stats *)malloc(sizeof (Stats));
    new_stat->cust_ip = victim->cust_ip;
    strncpy(new_stat->dest, victim->dest, sizeof (new_stat->dest));
    new_stat->size = victim->chars_read;
    new_stat->connect = (int)difftime(victim->finish, victim->start);
    new_stat->aborted = victim->aborted;

    pthread_cancel(victim->thread);
    free(victim);

    pthread_mutex_lock(&stat_lock);
    new_stat->next = history;
    history = new_stat;
    pthread_mutex_unlock(&stat_lock);
}
