#ifndef TRIS_UTILS_H
#define TRIS_UTILS_H

#include <stdbool.h>

#include "common.h"
#include "buffer.h"
#include "messaging.h"
#include "logic.h"

void convert_json_to_buffer(char *, generic_buffer *);
cJSON* build_first_connection_message(int, match *);
cJSON* build_message(int, message_type_enum, generic_buffer *);
int send_message(int, message_type_enum, generic_buffer *);

bool check_win(int [3][3], int, int);
bool check_draw(int [3][3]);

#endif