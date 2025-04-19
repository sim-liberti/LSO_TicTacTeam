#ifndef TRIS_LOGIC_H
#define TRIS_LOGIC_H

#include "common.h"
#include "signal.h"

char* get_match_list();
char* create_new_match();
char* make_move();
char* send_guest_request();
char* send_guest_response();
char* handle_draw();
char* delete_match();

#endif // TRIS_LOGIC_H