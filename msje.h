#define MAXSIZE     255

//tipos mtype de msgbuf..
#define PREGUNTA 1
#define RESULTADO 2

//tipos de operaciones que puede ejecutar la calculadora
#define SUMA 1
#define RESTA 2
#define MULT 3
#define DIV 4
#define DTB 5
#define BTH 6

//key de la cola IPC
#define KEY 1234

//mensajes que envian y reciben el cliente y el server..
struct msgbuf{

    long    mtype;
    char mtext[MAXSIZE];
};

