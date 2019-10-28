#include <stdio.h>
#include <stdlib.h>
#include <usp10.h>

int MAXIMO = 100000;

int fifo(int qtd_frames);

int main(int argc, char *argv[] ) {
    // Recebendo número de quadros
    int qtd_frames = atoi(argv[1]);

    int fifo_page_faults = fifo(qtd_frames);
    printf("%d\n", fifo_page_faults);
}

int fifo(int qtd_frames){
    // Criando a fila e o ponteiro para o último
    int *queue;
    queue = malloc(qtd_frames * sizeof(int));
    int last = 0;

    // Inicializando fila com -1 nas posições de memória
    for(int i = 0; i < qtd_frames; i++)
        queue[i] = -1;

    int page_faults = 0;

    int n = 0;
    while (!feof(stdin) && (n <= MAXIMO)) {
        n++;

        int page = 0;
        scanf("%d\n", &page);

        // Verifica se a página está na memória
        int not_found = 1;
        for (int j = 0; j < qtd_frames; j++) {
            if(queue[j] == page){
                not_found = 0;
            }
        }

        // Se página não esta na memória
        if(not_found) {
            queue[last] = page;
            last = (last + 1) % qtd_frames;
            page_faults++;
        }
    }

    return(page_faults);
}