#ifndef TRIS_LOGIC_H
#define TRIS_LOGIC_H

#include "common.h"
#include "signal.h"

cJSON* get_match_list(match *);
cJSON* create_new_match(create_new_match_buffer*, match *);
cJSON* make_move();
cJSON* send_guest_request();
cJSON* send_guest_response();
cJSON* handle_draw();
cJSON* delete_match();

#endif // TRIS_LOGIC_H