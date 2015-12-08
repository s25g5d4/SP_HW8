/*
 *	disconnect is passed a pointer to a Servlet
 *	It should perform the following :
 *		1) Take the servlet out of the circular list
 *		2) Close the socket connection
 *		3) Create a Pending item and push pending stack
 *		4) Alert the garbage collector
 */

#include "sms.h"

void disconnect(Servlet *done) {

}
