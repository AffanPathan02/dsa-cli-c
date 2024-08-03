#include "../include/utils.h"
#include <stdio.h>
#include<time.h>

void solve_problem(const char *problem_type) {
    printf("Solving problem type: %s\n", problem_type);


    printf("Enter the solution in solution.c file...\n");


    // system("nano solution.c");


    // if (system("gcc solution.c -o solution && ./solution") != 0) {
    //     printf("Compilation or execution failed.\n");
    // }
}

void countdown_timer(){
    int seconds =60;
    
    while (seconds>0)
    {
        int m=(seconds%3600)/60;
        int s=seconds%60;
        printf("\r%02d:%02d",m,s);
        fflush(stdout);

        clock_t stop=clock() + CLOCKS_PER_SEC;
        while (clock()<stop){ }

        seconds--;
        

    }
    printf("Time Ups");
}
