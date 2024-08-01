#ifndef UTILS_H
#define UTILS_H
#include<cjson/cJSON.h>

void setup_env(const char *env);
void solve_problem(const char *problem_id);
int read_and_print_json(const char* filename,const char* key);
void display_json_in_window(const char* filename, const char* difficulty);



#endif
