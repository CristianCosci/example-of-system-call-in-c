#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#if !defined(MAXFILENAME)
#define MAXFILENAME 2048
#endif

//macro per chiamate di sistema 
#define SYSCALL(name, systemcall, returnval, check, str, ...) \
    if ((returnval = systemcall) == check)                   \
    {                                       \
        perror(#name);                      \
        int errno_copy = errno;             \
        print_error(str, __VA_ARGS__);      \
        exit(errno_copy);                   \
    }

//utilizzata da SYSCALL per gestione degli errori
void print_error(const char *str, ...){
    const char err[] = "[ERRORE] ";
    va_list argp;
    char *p = (char *)malloc(strlen(str) + strlen(err) + 512);
    if (!p){
        perror("malloc");
        fprintf(stderr, "FATAL ERROR nella funzione 'print_error'\n");
        return;
    }
    strcpy(p, err);
    strcpy(p + strlen(err), str);
    va_start(argp, str);
    vfprintf(stderr, p, argp);
    va_end(argp);
    free(p);
}

int controlloEstensione(const char estensione[]){ //controllo per vedere se il pattern dell'estensione inserita inizia con il "." seguito da un qualche carattere
    int controllo = 0;
    if(strlen(estensione) > 0 && estensione[0] == '.' && estensione[1] != '\0'){
        controllo = 1;
    }
    return controllo;
}

int isDot(const char dir[]){
    return (strlen(dir) > 0 && dir[strlen(dir) - 1] == '.');
}

void directoryVisit(const char estensione[], const char nomeDirectory[]){
    struct stat statbuf;
    int dummy_return_val;
    SYSCALL(stat, stat(nomeDirectory, &statbuf), dummy_return_val, -1, "Facendo stat del nome %s: errno=%d\n", nomeDirectory, errno);

    //controllo sull'argomento passato
    if (S_ISDIR(statbuf.st_mode)){
        //fprintf(stdout, "%s e' una directory\n", nomeDirectory);
    } else {
        fprintf(stderr, "[ERRORE] %s non e' una directory", nomeDirectory);
        exit(EXIT_FAILURE);
    }

    //apertura directory
    DIR *dir;
    SYSCALL(opendir, opendir(nomeDirectory), dir, NULL, "Facendo opendir su %s: errno=%d\n", nomeDirectory, errno);
    struct dirent *file = NULL;

    //inizio visita directory
    while ((errno = 0, file = readdir(dir)) != NULL){
        char filename[MAXFILENAME];
        if (strlen(nomeDirectory) + strlen(file->d_name) + 2 > MAXFILENAME){
            fprintf(stderr, "ERRORE: dim MAXFILENAME \n");
            exit(EXIT_FAILURE);
        }
        strncpy(filename, nomeDirectory, MAXFILENAME - 1);
        strncat(filename, "/", MAXFILENAME - 1);
        strncat(filename, file->d_name, MAXFILENAME - 1);
        SYSCALL(stat, stat(filename, &statbuf), dummy_return_val, -1, "Facendo stat del nome %s: errno=%d\n", filename, errno);

        //controllo su directory interne
        if (S_ISDIR(statbuf.st_mode)){
            if (!isDot(filename)){
                //printf("\ntrovata dir ");
                directoryVisit(estensione, filename);
            }
        } else {
            char* path = realpath(filename, NULL);
            int a = 0;
            while(filename[a] != '.'){ //ricavo estensione dal file se ne ha una
                a++;
            }
            
            //varie operazioni per gestione dell'estensione del file trovato
            int lista = strlen(filename) - a;
            char extension[lista];
            int sep = strlen(filename) - lista;
            int i = 0;
            while(i <= lista){
                extension[i] = filename[sep + i];
                ++i;
            }
            if(strcmp(estensione,extension)==0){ //controllo se le estensioni tra il file e quella della ricerca combaciano
                fprintf(stdout, "%s", path);
                printf("\tUltima modifica: %s", ctime(&statbuf.st_mtime)); //stampo ultima modifica
                free(path);
            }
        }
    }

    //chiusura directory
    if (errno != 0){
        perror("readdir");
    }
    SYSCALL(closedir, closedir(dir), dummy_return_val, -1, "Facendo closedir su %s: errno=%d\n", dir, errno);
}


int main(int argc, char *argv[]){
    if (argc != 2 && argc != 3){ //controllo sugli argomenti passati
        fprintf(stderr, "utilizzo: %s \"estensione.txt\" e \"directory\" \n", argv[0]);
        return EXIT_FAILURE;
    }
    //controllo su formato estensione inserito
    int controllo = controlloEstensione(argv[1]);
    if(controllo != 1){
        fprintf(stdout, "L'estensione inserita non ha il formato corretto : \".estensione\"\n");
        return EXIT_FAILURE;
    }
    if (argc == 3){
        //fprintf(stdout, "L'estensione da cercare e': %s\n", argv[1]);
        char* path = realpath(argv[2], NULL); //per avere il path assoluto
        //fprintf(stdout, "La cartella da esplorare e': %s\n", path);
        if (errno != 0){
            perror("la directory non esiste");
            exit(EXIT_FAILURE);
        }
        directoryVisit(argv[1], path);
    } else { //nel caso in cui non è stato inserito il secondo argomento, la ricerca parte dalla directory corrente
        //fprintf(stdout, "L'estensione da cercare e': %s\n", argv[1]);
        char* path = realpath(".", NULL); //usato per avere il path assoluto
        //fprintf(stdout, "La cartella da esplorare e': %s\n", path);
        directoryVisit(argv[1], path);
    }
    return 0;
}