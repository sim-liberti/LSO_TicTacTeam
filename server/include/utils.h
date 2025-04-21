#ifndef TRIS_UTILS_H
#define TRIS_UTILS_H

#include "common.h"
#include "buffer.h"
#include "messaging.h"
#include "logic.h"

int check_match_outcome(int (*)[3], int, int);
void convert_json_to_buffer(char *, generic_buffer *);
cJSON* build_first_connection_message(int, match *);
cJSON* build_message(int, message_type_enum, generic_buffer *);
int send_message(int, message_type_enum, generic_buffer *);

#endif