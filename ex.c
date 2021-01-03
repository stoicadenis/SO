#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

#define time 5

unsigned long long contorSignal[5]; // Contor semnal/ sec
unsigned long long contor = 0; // Contor caractere

int Child[2], Parent[2]; // Pipes
int j = 0; // Index pentru secunda respectiva
int contorTimp = 0; // Contor pentru urmarirea secundei

pid_t pid; // Variabila pentru id-ul procesului

// Contorizeaza pentru fiecare secunda cate a-uri au fost transmise
void handlerContor()
{
    contorSignal[j] = contor;
    j++;
}

// Transmite informatia catre pipe-ul Parent pentru a fi afisata in statistica
void handlerInfo()
{
    write(Parent[1], &contor, sizeof(unsigned long long));
    for(int i = 0; i < time; i++)
        write(Parent[1], &contorSignal[i], sizeof(unsigned long long));
    
    close(Parent[1]);
}

// Transmiterea semnalului SIGUSR1 in fiecare secunda, urmata de contorizarea timpului pana ajunge la sec. 5
// Dupa care se apeleaza semnalul SIGUSR2 pentru transmiterea informatiilor si ulterior citirea si afisarea statisticii finale
void handlerStatistics()
{
    if(contorTimp < time)
    {
        kill(pid, SIGUSR1);
        contorTimp++;
        alarm(1);  
    }
    else
    {
        kill(pid, SIGUSR2);
        read(Parent[0], &contor, sizeof(unsigned long long));
        for(int i = 0; i < time; i++)
        {
            if(read(Parent[0], &contorSignal[i], sizeof(unsigned long long)) <= 0) break;
            printf("Pentru secunda %d au fost trimise %lld caractere!\n", i + 1, contorSignal[i]);
        }
        printf("---------------------------------------------------\n");
        printf("In total au fost trimise %lld caractere catre procesul fiu!\n", contor);

        close(Parent[0]);
        exit(1); 
    }   
}

int main()
{
    char buffer;

    if(pipe(Child) == -1 || pipe(Parent) == -1) // Verificam daca pipe-urile pot fi apelate
    {
        perror("Eroare la Pipes!\n");
        exit(0);
    }

    pid = fork();
    if(pid < 0) // Verificam daca fork-ul a clonat cu succes sau nu
    {
        perror("Eroare la fork()\n");
        exit(0);
    }

    if(pid == 0) // proces fiu
    {
        close(Child[1]); // Inchidem pipe-ul de afisare Child[1]

        signal(SIGUSR1, handlerContor); // Legam semnalul de SIGUSR1
        signal(SIGUSR2, handlerInfo); // Legam semnalul de SIGUSR2

        while(read(Child[0], &buffer, 1) == 1) // Citim in while caracterul din Child, daca exista vreun caracter atunci se contorizeaza
            contor++; 

        close(Child[0]); // Inchidem pipe-ul de citire Child[0]
    }
    else // proces parinte
    {    
        close(Child[0]); // Inchidem pipe-ul de citire Child[0]
        signal(SIGALRM, handlerStatistics); // Legam semnalul de SIGALRM
        alarm(1);
        
        // While infinit pentru trimiterea caracterului in pipe-ul Child[1]
        while(1)
            write(Child[1], "a", 1);

        close(Child[1]); // Inchidem pipe-ul de afisare Child[1]
    } 
    return 0;
}
