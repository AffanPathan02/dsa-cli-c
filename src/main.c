#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "../include/utils.h"

void* window_thread_func(void* arg) {
    const char* filename = (const char*)arg;
    display_json_in_window_with_timer(filename, "easy");
    return NULL;
}

void setup_env(const char *env) {
    char command[256];
    snprintf(command, sizeof(command), "./scripts/install_setup.sh %s", env);
    if (system(command) == -1) {
        perror("Failed to execute setup script");
    }
}

int main(int argc, char *argv[]) {
    if(argc < 2) {
        printf("Usage: %s <command> [args]\n", argv[0]);
        return 1;
    }

    const char* filename = "problems/problems.json";
    
    if(strcmp(argv[1], "setup") == 0 && argc == 3) {
        setup_env(argv[2]);
    } else if(strcmp(argv[1], "solve") == 0 && argc == 3) {
        solve_problem(argv[2]);
        
        pthread_t window_thread;
        if (pthread_create(&window_thread, NULL, window_thread_func, (void*)filename) != 0) {
            perror("Failed to create window thread");
            return 1;
        }

        if (pthread_join(window_thread, NULL) != 0) {
            perror("Failed to join window thread");
            return 1;
        }
    } else {
        printf("Unknown Command: %s\n", argv[1]);
    }

    return EXIT_SUCCESS;
}
