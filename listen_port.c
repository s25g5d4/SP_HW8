/*
 * listen_port :
 * The body of the thread which listens on a particular
 * port number (it's sole argument).
 * Whenever a new connection is requested it calls
 * make_servelet_list and then fires off a new thread running.
 * It must set two fields in the Servlet structure :
 *     1) fd
 *     2) start
 */

#include "sms.h"


void make_servelet_list();

void *listen_port(void *info) /* body of port listener */
{
    struct sockaddr_in server;
    int sd = socket(AF_INET, SOCK_STREAM, 0);

    if (sd == -1) {
        DIE(__FILE__ ": listen_port: socket");
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sd, (struct sockaddr *)&server, sizeof (server)) == -1) {
        DIE(__FILE__ ": listen_port: bind");
    }

    if (listen(sd, 50) == -1) {
        DIE(__FILE__ ": listen_port: listen");
    }

    while (1) {
        socklen_t n;
        struct sockaddr_in client;
        int cd = accept(sd, (struct sockaddr *)&client, &n);
        if (cd == -1) {
            DIE(__FILE__ ": listen_port: accept");
        }

        pthread_mutex_lock(&door_lock);
        make_servelet_list();
        Servlet *current = door;
        pthread_mutex_unlock(&door_lock);
        current->fd = cd;
        current->start = time(NULL);
        current->cust_ip = ntohl(client.sin_addr.s_addr);
        current->aborted = 0;
        pthread_create(&current->thread, &attr, serve_client, (void *)door);
    }
    return NULL;
}

void make_servelet_list()
{
    if (door == NULL) {
        door = (Servlet *)malloc(sizeof (Servlet));
        door->next = door;
        door->prev = door;
    }
    else {
        Servlet *new_servlet = (Servlet *)malloc(sizeof (Servlet));
        new_servlet->prev = door->prev;
        door->prev->next = new_servlet;
        door->prev = new_servlet;
        new_servlet->next = door;
        door = new_servlet;
    }
}
