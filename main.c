#include <stdio.h>
#include <stdlib.h>

int MAXIMO = 100000;

typedef struct node {
    int val;
    struct node * next;
} node_t;

node_t * read_lines();
int fifo(int qtd_frames, node_t *linked_list);
int lru(int qtd_frames, node_t *linked_list);
int optimal(int qtd_frames, node_t *linked_list);

int main(int argc, char *argv[] ) {
    // Recebendo número de quadros
    int qtd_frames = atoi(argv[1]);

    node_t *linked_list;
    linked_list = read_lines();

    int fifo_page_faults = fifo(qtd_frames, linked_list);
    int lru_page_faults = lru(qtd_frames, linked_list);
    printf("FIFO = %d, LRU = %d\n", fifo_page_faults, lru_page_faults);
}

int fifo(int qtd_frames, node_t *linked_list){
    // Criando a fila e o ponteiro para o último
    int *queue;
    queue = malloc(qtd_frames * sizeof(int));
    int last = 0;

    // Inicializando fila com -1 nas posições de memória
    for(int i = 0; i < qtd_frames; i++)
        queue[i] = -1;

    int page_faults = 0;

    node_t *node = linked_list->next;

    while (node != NULL) {

        // Verifica se a página está na memória
        int not_found = 1;
        for (int j = 0; j < qtd_frames; j++) {
            if(queue[j] == node->val){
                not_found = 0;
            }
        }

        // Se página não esta na memória
        if(not_found) {
            queue[last] = node->val;
            last = (last + 1) % qtd_frames;
            page_faults++;
        }
        node = node->next;
    }

    return(page_faults);
}

int lru(int qtd_frames, node_t *linked_list) {
    int *cache;
    cache = malloc(qtd_frames * sizeof(int));

    for(int i = 0; i < qtd_frames; i++)
        cache[i] = -1;

    int page_faults = 0;

    node_t *node = linked_list->next;

    while (node != NULL) {
        int atual = cache[0];
        int proximo;
        int em_cache = 0;
        for(int i = 1; i <= qtd_frames; i++) {
            if(atual == node->val) {
                em_cache = 1;
                break;
            }
            proximo = cache[i];
            cache[i] = atual;
            atual = proximo;
        }
        if (em_cache == 0) page_faults++;
        cache[0] = node->val;
        node = node->next;
    }

    return(page_faults);
}

node_t *read_lines(){
    node_t *head = NULL;
    head = malloc(sizeof(node_t));
    if (head == NULL) {
        exit(1);
    }

    head->val = -1;
    head->next = NULL;

    node_t *current = head;

    int n = 0;
    while (!feof(stdin) && (n <= MAXIMO)) {
        node_t *new = NULL;
        new = malloc(sizeof(node_t));

        int page;
        scanf("%d\n", &page);

        new->val = page;
        new->next = NULL;
        current->next = new;
        current = new;
        n++;
    }

    return(head);
}