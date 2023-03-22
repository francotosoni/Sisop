#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

typedef int pid_t;

int main (void) {

    int pipe_uno[2];
    int pipe_dos[2];
    pipe(pipe_uno);
    pipe(pipe_dos);

    printf("Hola, soy PID %d: \n  - primer pipe me devuelve: [%d, %d]\n  - segundo pipe me devuelve: [%d, %d]\n\n", getpid(), pipe_uno[0], pipe_uno[1], pipe_dos[0], pipe_dos[1]);

    pid_t i = fork();
    int child_status;

    srandom(time(NULL));
    int num_aleatorio = random();

    if(i < 0){
        exit(-1);
    }

    else if( i == 0){
        int recibido;

        close(pipe_uno[1]);
        read(pipe_uno[0], &recibido, sizeof(recibido));
        close(pipe_uno[0]);
        close(pipe_dos[0]);

        printf("Donde fork me devuelve %d:\n", i);
        printf("  - getpid me devuelve: %d\n", getpid());
        printf("  - getppid me devuelve: %d\n", getppid());
        printf("  - recibo valor %d vía fd=%d\n", recibido , pipe_uno[0]);
        printf("  - reenvío valor en fd=%d y termino\n\n", pipe_dos[1]);

        write(pipe_dos[1], &recibido, sizeof(recibido));
        close(pipe_dos[1]);    
    }
    else{
        int recibido;

        close(pipe_uno[0]);
        close(pipe_dos[1]);

        printf("Donde fork me devuelve %d:\n", i);
        printf("  - getpid me devuelve: %d\n", getpid());
        printf("  - getppid me devuelve: %d\n", getppid());
        printf("  - random me devuelve: %d\n", num_aleatorio);
        printf("  - envío valor %d a través de fd=%d\n\n", num_aleatorio, pipe_uno[1]);

        write(pipe_uno[1], &num_aleatorio, sizeof(num_aleatorio));
        close(pipe_uno[1]);

        waitpid(i, &child_status, 0);
        read(pipe_dos[0], &recibido, sizeof(recibido));
        close(pipe_dos[0]);

        printf("Hola, de nuevo PID %d:\n", getpid());
        printf("  - recibí valor %d vía fd=%d\n", recibido, pipe_dos[0]);

    }


    return 0;
}