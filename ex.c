#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

int contor = 0;

void director(char *sursa, char *destinatie)
{
    DIR *dir;
    struct dirent *entry;
    struct stat stare, st_lnk;

    char cale[255];
    char cale_dest[255];
    if(!(dir = opendir(sursa)))
    {
        perror("Nu s-a putut deschide directorul sursa!");
        exit(EXIT_FAILURE);
    }
    while((entry = readdir(dir)) != NULL)
    {
        if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            strcpy(cale, sursa);
            strcat(cale, "/");
            strcat(cale, entry->d_name);
            stat(cale, &stare);
            lstat(cale, &st_lnk);

            strcpy(cale_dest, destinatie);
            strcat(cale_dest, "/");
            strcat(cale_dest, entry->d_name);
            if(S_ISDIR(stare.st_mode))
            {
                mkdir(cale_dest, stare.st_mode);
                director(cale, cale_dest);            
            }
            else if(S_ISREG(stare.st_mode) && !S_ISLNK(st_lnk.st_mode))
            {
                char buffer[255];
                int source = open(cale, O_RDONLY), citire;
                long int marime = lseek(source, 0, SEEK_END);
                lseek(source, 0, SEEK_SET);
                if(marime > 500)
                {
                    int output = creat(cale_dest, O_WRONLY);
                    while(citire = read(source, &buffer, sizeof(buffer)))
                    {
                        write(output, buffer, sizeof(buffer));
                    }
                    chmod(cale_dest, stare.st_mode & 0444);
                    close(source);
                    close(output);
                }
                else
                {
                    char lnk[20], cale_dest_lnk[255], cale_sursa_lnk[255];
                    sprintf(lnk, "%s%d", "symlink", contor);
                    contor++;
                    realpath(cale, cale_sursa_lnk);
                    realpath(destinatie, cale_dest_lnk);
                    strcat(cale_dest_lnk, "/");
                    strcat(cale_dest_lnk, lnk);
                    symlink(cale_sursa_lnk, cale_dest_lnk);
                }
            }
        }  
    }
}

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        perror("Eroare in linia de comanda!\n");
        exit(EXIT_FAILURE);
    }
    director(argv[1],argv[2]);
    return 0;
}