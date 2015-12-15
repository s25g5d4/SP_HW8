/*
 *	listen_port :
 *	The body of the thread which listens on a particular
 *	port number (it's sole argument).
 *	Whenever a new connection is requested it calls
 *	make_servelet_list and then fires off a new thread running.
 *	It must set two fields in the Servlet structure :
 *		1) fd
 *		2) start
 */

#include "sms.h"

static int sd;
static struct sockaddr_in server;
pthread_mutex_t door_lock;

void make_servelet_list(int cd);

void *listen_port(void *info) { /* body of port listener */
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd == -1) {
        DIE(__FILE__ ": listen_port: socket");
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sd, (struct sockaddr *)&server, sizeof (server)) == -1) {
        DIE(__FILE__ ": listen_port: bind");
    }

    if (listen(sd, 50), == -1) {
        DIE(__FILE__ ": listen_port: listen");
    }

    pthread_mutex_init(&door_lock, NULL);

    while (1) {
        socklen_t n;
        int cd = accept((struct sockaddr *)&server, &n);
        if (cd == -1) {
            DIE(__FILE__ ": listen_port: accept");
        }

        pthread_mutex_lock(&door_lock);
        make_servelet_list(cd);
        pthread_create(&door->thread, &attr, serv_client, (void *)door);
        pthread_mutex_unlock(&door_lock);
    }
    return NULL;
}

void make_servelet_list(int cd) {
    if (door == NULL) {
        door = (Servlet *)malloc(sizeof (Servlet));
        door->next = door;
        door->prev = door;
    }
    else {
        Servlet *new_servlet = (Servlet *)malloc(sizeof (Servlet));
        new_servlet->next = door->next;
        door->next->prev = new_servlet;
        door->next = new_servlet;
        new_servlet->prev = door;
        door = new_servlet;
    }
    door->fd = cd;
    door->start = time(NULL);
}
