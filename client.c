//IPC_msgq_sdr.c

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "msje.h"

void die(char *s)
{
  perror(s);
  exit(1);
}

int chequearMetodo(int argc, char* argv[]);

void llamarHelp(char* nombre){
    printf ("usage: %s SERVER_HOST OPTION\n", nombre);
    printf("\tOPTION:\n");
    printf("\t\tsum NUMBER NUMBER [NUMBER] [NUMBER]\n");
    printf("\t\trest NUMBER NUMBER [NUMBER] [NUMBER]\n");
    printf("\t\tmult NUMBER NUMBER\n");
    printf("\t\tdiv NUMBER NUMBER\n");
    printf("\t\tbth NUMBER\n");
    printf("\t\tdtb NUMBER\n");
}

int main(int argc, char* argv[])
{ 
    int msqid;
    struct msgbuf pregunta, respuesta;
    size_t buflen;


    int tipo= chequearMetodo(argc, argv);
//    printf("el tipo es: %d\n", tipo);

    //me conecto a la cola..
    if ((msqid = msgget(KEY, 0666 )) < 0)   //Get the message queue ID for the given key
      die("msgget");

    //indico al msje que tipo de operacion se ejecutara.

    sprintf(pregunta.mtext, "%d,%s", tipo,argv[2]);
    if(argc>3){
        sprintf(pregunta.mtext, "%s,%s", pregunta.mtext, argv[3]);
    }
    if(argc>4){
        sprintf(pregunta.mtext, "%s,%s", pregunta.mtext, argv[4]);
    }
    if(argc>5){
        sprintf(pregunta.mtext, "%s,%s", pregunta.mtext, argv[5]);   
    }

//    printf("msje: %s\n", pregunta.mtext);
    pregunta.mtype=PREGUNTA;
    
    //determino el tamaño de la struct a enviar..
    buflen = strlen(pregunta.mtext)+1;

    //ENVIO EL MSJE AL SERVER..
    if (msgsnd(msqid, &pregunta, buflen, 0) < 0)
    {
        die("msgsnd");
    }
    else{
        printf("\tMessage Sent\n");

        //Recibe la respuesta del servidor..
        if (msgrcv(msqid, &respuesta, MAXSIZE,RESULTADO, 0) < 0)
            die("msgrcv");

        else{
            printf("Resultado: %s\n", respuesta.mtext);}
    }
    exit(0);
}

int chequearMetodo(int argc, char* argv[]){
    int tipo=-1;
    if (argc < 3) { //funcion host OPTION NUMBER
        llamarHelp(argv[0]);
        exit (1);
    }
    else{
        //obtengo los parametros que voy a utilizar
        int cantParams=argc-2;

        //chequeo que tipo de operacion se solicita
        if(strcmp(argv[1], "sum")==0){
            if(argc>=4 && argc<=6){
                //printf("llame sum(%s,%s,%s,%s)\n", argv[3],argv[4],argv[5],argv[6]);
                tipo=SUMA;
            }
            else {
                printf("error params sum\n");
                exit(1);
            }
        }
        else if(strcmp(argv[1], "rest")==0){
            if(argc>=4 && argc<=6){
                //CODIGO
                //printf("llame rest(%s,%s,%s,%s)\n", argv[3],argv[4],argv[5],argv[6]);
                tipo=RESTA;
            }
            else {
                printf("error params rest\n");
                exit(1);
            }
        }
        else if(strcmp(argv[1], "mult")==0){
            if(argc==4){
                //CODIGO
                //printf("llame mult(%s,%s)\n", argv[3],argv[4]);
                tipo=MULT;
            }
            else {
                printf("error params mult\n");
                exit(1);
            }
        }
        else if(strcmp(argv[1], "div")==0){
            if(argc==4){
                
                tipo=DIV;

            }
            else {
                printf("error params div\n");
                exit(1);
            }
        }
        else if(strcmp(argv[1], "bth")==0){
            if(argc==3){
                //CODIGO
                tipo=BTH;
            }
            else {
                printf("error params bth\n");
                exit(1);
            }
        }
        else if(strcmp(argv[1], "dtb")==0){
            if(argc==3){
                //CODIGO
                tipo=DTB;
            }
            else {
                printf("error params dtb\n");
                exit(1);
            }
        }
        else llamarHelp(argv[0]);
    }
    return tipo;
}