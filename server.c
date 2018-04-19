#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "msje.h"

int msqid;
key_t key=1234;
struct msgbuf msje_buf, msgRespuesta;
size_t buflen;


void die(char *s)
{
  perror(s);
  exit(1);
}

void obtBin(int num, char* salida){
  int dig;
  if(num<2){
    sprintf(salida, "%d", num);
  }
  else{
    dig=num%2;
    obtBin(num/2, salida);
    sprintf(salida, "%s%d", salida, dig);
  }
}


void calculadora(){
	while(1){
        //envio la respuesta al mismo cliente

        if (msgrcv(msqid, &msje_buf, MAXSIZE, PREGUNTA, 0)>0){
    //falta hacer que funque con  3 y 4 parametros..
           	int resultado=0;
           	int operacion=-1;
           	int params[4]={0,0,0,0};
           	int i=0;
           	char* aux;
           	

           	operacion =atoi(strtok(msje_buf.mtext,","));
           	printf("operacion: %d\n", operacion);
           	aux=strtok(NULL,",");
			while(aux!=NULL){
				//almaceno el valor (siempre tengo al menos 1 parametro)
				params[i]=atoi(aux);
	        	printf("num%d: %d\n",i,params[i]);
       			aux =strtok(NULL,",");
       			i++;
			}

			switch(operacion){
				case SUMA:{
					resultado=params[0]+params[1]+params[2]+params[3];
					sprintf(msgRespuesta.mtext,"%d",resultado);
				}break;

				case RESTA:{
					resultado=params[0]-params[1]-params[2]-params[3];
					sprintf(msgRespuesta.mtext,"%d",resultado);
				}break;
				case MULT:{
					resultado=params[0]*params[1];
					sprintf(msgRespuesta.mtext,"%d",resultado);
				}break;
				case DIV:{
					resultado=params[0]/params[1];
					sprintf(msgRespuesta.mtext,"%d",resultado);
				}break;
				case DTB:{
					char *salida= malloc(MAXSIZE);
					obtBin(params[0],salida);
					sprintf(msgRespuesta.mtext,"%s",salida);
				}break;
				case BTH:{
					long int binaryval, hexadecimalval = 0, i = 1, remainder;
					char *salida= malloc(MAXSIZE);

					binaryval=params[0];
					while (binaryval != 0){
        				remainder = binaryval % 10;
				        hexadecimalval = hexadecimalval + remainder * i;
				        i = i * 2;
				        binaryval = binaryval / 10;
    				}
    				sprintf(salida, "%lX", hexadecimalval);
    				sprintf(msgRespuesta.mtext,"%s",salida);
				}break;
			}
			printf("Recibi un pedido de operacion %d:\n",operacion);
			//seteo los valores de la struct a enviar
			msgRespuesta.mtype=RESULTADO;
			printf("Respuesta calculada: %s\n",msgRespuesta.mtext);
			buflen=strlen(msgRespuesta.mtext)+1;

			if (msgsnd(msqid, &msgRespuesta, buflen, IPC_NOWAIT) < 0)
				die("msgsnd");
			
			else	
				printf("Message Sent\n");
        }
	}
}

int main(){
  	if ((msqid = msgget(KEY, IPC_CREAT | 0666)) < 0)
		die("msgget()");
	else{
		printf("INICIA EL SERVIDOR..\n");
		calculadora();

	}	
		
}