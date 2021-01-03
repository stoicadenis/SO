#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

int pfd[2];
int received = 0;
char buffer[200];
pid_t pidP1, pidP2;

void handler(int sig)
{
    if(sig == SIGUSR1)
        received = 1;
}

int main()
{
    pipe(pfd);
    signal(SIGUSR1, handler);
    if(pidP1 = fork())
    {
        kill(pidP2, SIGUSR1);
    }
    else
    {
        if(pidP2 = fork())
        {
            close(pfd[0]);
            char msg[10] = "Hello";
            if(received == 1)
                write(pfd[1], msg, strlen(msg) + 1);
            close(pfd[1]);
        }
        else
        {
            close(pfd[1]);
            int n;
            char readMsg[10];
            n = read(pfd[0], readMsg, 10);
            printf("%s", readMsg);
            close(pfd[1]);
        }
        
    }
    
}