#ifndef TRIS_UTILS_H
#define TRIS_UTILS_H

#include <cJSON.h>

#include "common.h"
#include "buffer.h"
#include "messaging.h"

int check_match_outcome(int (*)[3], int, int);
void json_to_buffer(char *, generic_buffer *);
cJSON* build_message(message_type_enum, generic_buffer *);
int send_message(int, message_type_enum, generic_buffer *);

#endif