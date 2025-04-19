#ifndef TRIS_UTILS_H
#define TRIS_UTILS_H

#include <cJSON.h>

#include "common.h"
#include "buffer.h"

int check_match_outcome(int (*)[3], int, int);
void json_to_buffer(char *, generic_buffer *);
char* buffer_to_json(generic_buffer *);

#endif