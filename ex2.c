#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1024

int pfd[2], received = 0, counter = 0;
char word[64];
pid_t pid;

void handler(int sig)
{
    if(sig == SIGUSR1)
        received = 1;
    if(sig == SIGALRM)
        counter++;
    if(counter == 5)
    {
        printf("Au trecut 5 secunde de la inceperea cautarii!\n");
        exit(EXIT_SUCCESS);
    }
}

int main(int argc, char *argv[])
{
    signal(SIGUSR1, handler);
    signal(SIGALRM, handler);
    pipe(pfd);
    if(argc != 3)
    {
        perror("Eroare la argumente!\n");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if(pid == 0)
    {
        close(pfd[1]);
        char readBuffer[MAX] = "";
        int n;
        n = read(pfd[0], readBuffer, sizeof(readBuffer));
        strcpy(word, argv[2]);
        if(strstr(readBuffer, word))
            kill(getppid(), SIGUSR1);
        close(pfd[0]);
    }
    else
    {   
        close(pfd[0]);
        alarm(1);
        char writeBuffer[MAX] = "", linie[64] = "";
        FILE *f = fopen(argv[1], "r");
        while(fscanf(f, "%s", linie) != EOF)
            strcat(writeBuffer, linie);
        fclose(f);

        write(pfd[1], writeBuffer, sizeof(writeBuffer));
        close(pfd[1]);
        
        sleep(1);
        if(received == 1)
            printf("Gasit!\n");
        else
            printf("Inexistent!\n");
    }
    
}