#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "../include/utils.h"

void setup_env(const char *env)
{
  system("./scripts/install_setup.sh ");
}

int main(int argc, char *argv[])
{
    const char* filename = "/home/pathan/Desktop/dsa-cli/src/problems.json";
  if(argc<2){
    printf("Usage: %s <command> [args]\n",argv[0]);
    return 1;
  }

  if(strcmp(argv[1],"setup")==0 && argc==3){
    setup_env(argv[2]);
  }else if(strcmp(argv[1],"solve")==0 && argc==3){
    solve_problem(argv[2]);
    display_json_in_window(filename, argv[2]);
  }else{
    printf("Unknown Command:%s \n",argv[1]);
  }
  return EXIT_SUCCESS;
}
