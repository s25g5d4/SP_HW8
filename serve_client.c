/*
 *	serve_client :
 *	The body of the thread of which one is started for each client.
 *	It receives as its argument a pointer to Servlet of which  precisely
 *	two fields : the start time and socket descriptor are non-null.
 *	The main duty of this thread is to conduct a dialog with the client.
 *	Correct termination is the string "---" in a line by itself.
 *	Incorrect termination and over verbose clients must the suitably handled.
 */

#include "sms.h"

void *serve_client(void *info) { /* body of servlet thread */
	
	return NULL;
}
