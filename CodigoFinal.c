#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <termios.h>
#include <stdbool.h>
#include <time.h>

pthread_mutex_t mutex;

enum SINAL {
  VERDE,
  VERMELHO
};
struct Sinal {
  enum SINAL valor;
  char* _char;
};

char VERDE_V[] = "\033[32m==\033[39m";
char VERMELHO_V[] = "\033[31m==\033[39m";

struct Sinal sinal1 = { VERMELHO, VERMELHO_V };

//===================================================//

char VERDE_H[] = "\033[32m║\033[39m";
char VERMELHO_H[] = "\033[31m║\033[39m";

struct Sinal sinal2 = { VERDE, VERDE_H };

//===================================================//

char* vertical[5];
char* horizontal[7];
char* cruzamento;

void fillArray(char** array, int size) {
    for(int i = 0; i < size; i++) {
        array[i] = "  ";
    }
}

int calculado1 = 0;
int calculado2 = 0;

void calcularPosicaoCarro1() {
    if(vertical[0] == "C1") {
        vertical[0] = "  ";
        vertical[1] = "C1";
    } else if (vertical[1] == "C1") {
        if (sinal1.valor == VERDE) {
            vertical[1] = "  ";
            cruzamento = "C1";
            // altera o sinal2
            sinal2.valor = VERMELHO;
            sinal2._char = VERMELHO_H;
        }
    } else if (cruzamento == "C1") {
        cruzamento = "  ";
        vertical[3] = "C1";
        // altera o sinal2
        sinal2.valor = VERDE;
        sinal2._char = VERDE_H;
    } else if (vertical[3] == "C1") {
        vertical[3] = "  ";
        vertical[4] = "C1";
    } else if (vertical[4] == "C1") {
        vertical[4] = "  ";
        vertical[0] = "C1";
    }
}

void calcularPosicaoCarro2() {
    if(horizontal[0] == "C2") {
        horizontal[0] = "  ";
        horizontal[1] = "C2";
    } else if(horizontal[1] == "C2") {
        horizontal[1] = "  ";
        horizontal[2] = "C2";
    } else if (horizontal[2] == "C2") {
        if (sinal2.valor == VERDE) {
            horizontal[2] = "  ";
            cruzamento = "C2";
            // altera o sinal1
            sinal1.valor = VERMELHO;
            sinal1._char = VERMELHO_V;
        }
    } else if (cruzamento == "C2") {
        cruzamento = "  ";
        horizontal[4] = "C2";
        // altera o sinal1
        sinal1.valor = VERDE;
        sinal1._char = VERDE_V;
    } else if (horizontal[4] == "C2") {
        horizontal[4] = "  ";
        horizontal[5] = "C2";
    } else if (horizontal[5] == "C2") {
        horizontal[5] = "  ";
        horizontal[6] = "C2";
    } else if (horizontal[6] == "C2") {
        horizontal[6] = "  ";
        horizontal[0] = "C2";
    }
}

void* carro1() {
    while(1) {
        pthread_mutex_lock(&mutex);
        if (calculado1 == 0) {
            calcularPosicaoCarro1();
            calculado1 = 1;
        }
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(0);
}

void* carro2() {
    while(1) {
        pthread_mutex_lock(&mutex);
        if (calculado2 == 0) {
            calcularPosicaoCarro2();
            calculado2 = 1;
        }
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(0);
}

void* transito() {
    while(1) {
        pthread_mutex_lock(&mutex);
        system("clear");
        printf("\n\n");
        printf("                     rua 1\n");
        printf("                    +    +\n");
        printf("                    │ %s │\n", vertical[0]);
        printf("                    ├────┤\n");
        printf("                    │ %s │\n", vertical[1]);
        printf("      +───┬────┬────┘ %s └────┬────┬───+\n", sinal1._char);
        printf("rua 2  %s │ %s │ %s %s %s ║ %s │ %s │ %s   rua 4\n", horizontal[0], horizontal[1], horizontal[2], sinal2._char, cruzamento, horizontal[4], horizontal[5], horizontal[6]);
        printf("      +───┴────┴────┐ ══ ┌────┴────┴───+\n");
        printf("                    │ %s │\n", vertical[3]);
        printf("                    ├────┤\n");
        printf("                    │ %s │\n", vertical[4]);
        printf("                    +    +\n");
        printf("                     rua 3 \n\n\n");

        calculado1 = 0;
        calculado2 = 0;
        pthread_mutex_unlock(&mutex);
        usleep(500000);
    }
    pthread_exit(0);
}

int main() {
    system("clear");
    fillArray(vertical, 5);
    fillArray(horizontal, 7);
    cruzamento = "  ";

    vertical[0] = "C1";
    horizontal[1] = "C2";

    pthread_mutex_init(&mutex, NULL);
    pthread_t t1, t2, t3;

    pthread_create(&t1, NULL, transito, NULL);
    pthread_create(&t2, NULL, carro1, NULL);
    pthread_create(&t3, NULL, carro2, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    pthread_mutex_destroy(&mutex);
    return 0;
}
