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

void make_servelet_list();

void *listen_port(void *info) { /* body of port listener */

	return NULL;
}

void make_servelet_list() {
	
}
