/*
 * serve_client :
 * The body of the thread of which one is started for each client.
 * It receives as its argument a pointer to Servlet of which  precisely
 * two fields : the start time and socket descriptor are non-null.
 * The main duty of this thread is to conduct a dialog with the client.
 * Correct termination is the string "---" in a line by itself.
 * Incorrect termination and over verbose clients must the suitably handled.
 */

#include "sms.h"

void *serve_client(void *info) /* body of servlet thread */
{
    Servlet *client = (Servlet *)info;
    char *server_msg;
    char client_msg[SHORTMESS] = {0};

    server_msg = "Destination : ";
    write(client->fd, server_msg, strlen(server_msg));

    ssize_t msglen = read(client->fd, client_msg, sizeof (client_msg) - 1);
    if (msglen == -1) {
        disconnect(client);
        return NULL;
    }
    if (msglen > 2
        && (client_msg[msglen - 2] == '\r' || client_msg[msglen - 2] == '\n'))
    {
        client_msg[msglen - 2] = '\0';
        msglen -= 2;
    }
    else if (msglen > 1
             && (client_msg[msglen - 1] == '\r'
                 || client_msg[msglen - 1] == '\n'))
    {
        client_msg[msglen - 1] = '\0';
        msglen -= 1;
    }
    else {
        client_msg[msglen] = '\0';
    }

    memcpy(client->dest, client_msg, (size_t)msglen);
    client->dest[msglen] = '\0';
    
    server_msg = "Now write your message : finish with ---\r\n";
    write(client->fd, server_msg, strlen(server_msg));

    memset(client->message, 0, sizeof (client->message));
    client->chars_read = 0;
    while (1) {
        server_msg = "-->";
        write(client->fd, server_msg, strlen(server_msg));

        msglen = read(client->fd, client_msg, sizeof (client_msg) - 1);

        if (!memcmp(client_msg, "---", 3)
            && (msglen == 3
                || client_msg[3] == '\r' || client_msg[3] == '\n'))
        {
            server_msg = "Bye Bye!\r\n";
            write(client->fd, server_msg, strlen(server_msg));
            break;
        }

        if (client->chars_read + msglen >= sizeof (client->message)) {
            if (client->chars_read <= sizeof (client->message)) {
                memcpy(client->message + client->chars_read, client_msg,
                       sizeof (client->message) - client->chars_read);
                client->chars_read = sizeof (client->message);
            }

            break;
        }

        memcpy(client->message + client->chars_read, client_msg, msglen);
        client->chars_read += msglen;
    }

    disconnect(client);

    return NULL;
}
