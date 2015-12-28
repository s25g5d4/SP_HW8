/*
 * get_stale merely locates a stale servlet
 * It returns a valid pointer on success
 * It returns NULL if no stale servlets are found
 */

#include "sms.h"

Servlet *get_stale(int secs) /* find ONE stale client */
{
    Servlet *it = door;
    if (it == NULL)
        return NULL;

    time_t current_time = time(NULL);
    
    it = it->prev;
    while (it != door) {
        if ((int)difftime(current_time, it->start) > secs)
            return it;
        else
            it = it->prev;

    }

    if ((int)difftime(current_time, it->start) > secs)
        return it;
    else
        return NULL;
}
