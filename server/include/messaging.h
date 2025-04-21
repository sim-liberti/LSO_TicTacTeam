#ifndef TRIS_MESSAGING_H
#define TRIS_MESSAGING_H

typedef enum message_type_enum {
    MESSAGE_NOTIFICATION     = 0,
    MESSAGE_ALERT            = 1,
    MESSAGE_RESPONSE         = 2
} message_type_enum;

typedef struct message {
    int socket_fd;
    message_type_enum message_type;
    generic_buffer buffer;
} message;

#endif //TRIS_MESSAGING_H