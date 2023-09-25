#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <string>


int main () {
    pid_t child_pid = fork();
    if (child_pid == 0) 
    {
        if (execve("date.js", NULL, NULL) == -1)
            std::cout << "pk" << std::endl;
    }
    waitpid(child_pid, NULL, 0);
}