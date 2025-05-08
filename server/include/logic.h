#ifndef TRIS_LOGIC_H
#define TRIS_LOGIC_H

#include <cJSON.h>

#include "common.h"
#include "buffer.h"

cJSON* get_match_list(match *);
cJSON* create_new_match(create_new_match_buffer *, match *);
cJSON* make_move(int, make_move_buffer *, match *);
cJSON* send_guest_request(int, guest_request_buffer *, match *);
cJSON* send_guest_response(guest_response_buffer *, match *);
cJSON* handle_draw();
cJSON* delete_match(delete_match_buffer *, match *);

#endif // TRIS_LOGIC_H