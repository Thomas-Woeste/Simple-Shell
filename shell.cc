#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "shell.h"

using namespace std;
const int STDOUT_FD = 1;
const int STDIN_FD = 0;

void call_redirected(const command *cmd){
    pid_t pid = fork();
    
    if(pid < 0){
        perror("could not create a child process");
        return;
    } 
    else if(pid == 0) {
       if(cmd->out_redir){
          int fd = open(cmd->out_redir, O_WRONLY | O_CREAT | O_TRUNC, 0666);

          if(fd < 0){
               perror("could not open file for redirection");
               exit(1);
          }
          if(dup2(fd, STDOUT_FD) < 0){
               perror("dup2 failed");
               exit(2);
          }
       }
       
       if(cmd->in_redir){
            int fd = open(cmd->in_redir, O_RDONLY , 0666);

           if(fd < 0){
                perror("could not open file for redirection");
                exit(1);
           }
           if(dup2(fd, STDIN_FD) < 0){
                 perror("dup2 failed");
                exit(2);
           }
        }

          execvp(cmd->args[0], cmd->args);
          perror("could not execute process");
          exit(3);

    } 
    else {
        signal(SIGINT, SIG_IGN);
        int status = 0;
        if(wait(&status) < 0){
            perror("could not wait for child");
            return;
        }
        if(WIFEXITED(status)){
            if(WEXITSTATUS(status) != 0){
                cerr << "Command exited with status " << WEXITSTATUS(status) << endl; 
            }
        }
        if(WIFSIGNALED(status)){
            if(WTERMSIG(status) != 0){
                cerr << "Command exited with status " << WTERMSIG(status) << endl;
            }
        }
        signal(SIGINT, SIG_DFL);
    }

}

int main(int argc, char **argv){
    ifstream scriptf;
    bool from_script = false;

    if(argc > 2){
        cerr << "Usage: " << argv[0] << " [<scriptfile>]\n";
        exit(1);
    }
    else if(argc == 2){
        from_script = true;
        scriptf.open(argv[1]);
        
        if(scriptf.bad()){
            cerr << "Could not open input file " << argv[1] << ": " << strerror(errno) << endl;
            exit(1);
        }
    }

    istream &inf = from_script ? scriptf : cin;

    string line;

    if(!from_script)
        cerr << "shell> " << flush;

    while(getline(inf, line)) {
        command *cmd = parse_command(line.c_str());

        if(cmd->args[0]) {
            if(string(cmd->args[0]) == "cd"){
                if(cmd->args[1] == NULL){
                    char* dir = getenv("HOME");
                    if(dir == NULL){
                        cerr << "Could not get home directory" << strerror(errno) << endl;
                    }
                    int errorChdir;
                    errorChdir = chdir(dir);
                    if(errorChdir < 0){
                        cerr << "Error changing directory: " << strerror(errno) << endl;
                    }
                }
                else{
                    int errorChdir;
                    errorChdir = chdir(cmd->args[1]);
                    if(errorChdir < 0){
                        cerr << "Error changing directory: " << cmd->args[1] << " " << strerror(errno) << endl;
                    }

                }

            }
            else if(string(cmd->args[0]) == "setenv"){
                if(cmd->args[1] == NULL){
                    cerr << "Did not set any variable" <<  endl;
                }
                else if(cmd->args[2] == NULL){
                    int errorSetenv;
                    errorSetenv = unsetenv(cmd->args[1]);

                    if(errorSetenv < 0){
                        cerr << "Error setting environment: " << strerror(errno) << endl;
                    }
                }
                else{
                    int errorSetenv;
                    errorSetenv = setenv(cmd->args[1],cmd->args[2],1);

                    if(errorSetenv < 0){
                        cerr << "Error setting setenv: " << strerror(errno) << endl; 
                    }
                }
            }
            else if(string(cmd->args[0]) == "exit"){
                free_command(cmd);
                exit(0);
            }
            else{
                call_redirected(cmd);
            }
        } //else, empty command: do nothing

        free_command(cmd);
        if(!from_script)
            cerr << "shell> " << flush;
    }
    if(inf.bad()){
        cerr << "Error reading input: " << strerror(errno) << endl;
        exit(1);
    }
    exit(0);
}




