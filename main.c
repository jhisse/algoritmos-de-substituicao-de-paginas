#include <stdio.h>
#include <stdlib.h>

int MAXIMO = 10000000;

typedef struct node {
    int val;
    struct node *next;
} node_t;

node_t *read_lines();

int count_refs(node_t *linked_list);

int fifo(int qtd_frames, node_t *linked_list);

int lru(int qtd_frames, node_t *linked_list);

int optimal(int qtd_frames, node_t *linked_list);

void free_list(struct node *head);

int *cache;
int *queue;

int main(int argc, char *argv[]) {
    // Recebendo número de quadros
    int qtd_frames = atoi(argv[1]);

    node_t *linked_list;
    linked_list = read_lines();

    int refs = count_refs(linked_list);

    int fifo_page_faults = fifo(qtd_frames, linked_list);
    int lru_page_faults = lru(qtd_frames, linked_list);
    int optimal_page_faults = optimal(qtd_frames, linked_list);
    free_list(linked_list);
    printf("%5d quadros, %7d refs: FIFO: %5d PFs, LRU: %5d PFs, OPT: %5d PFs\n", qtd_frames, refs,
           fifo_page_faults, lru_page_faults, optimal_page_faults);
}

int count_refs(node_t *linked_list) {
    linked_list = linked_list->next;
    int counter = 0;
    while (linked_list != NULL) {
        counter++;
        linked_list = linked_list->next;
    }
    return (counter);
}

int fifo(int qtd_frames, node_t *linked_list) {
    // Criando a fila e o ponteiro para o último
    queue = malloc(qtd_frames * sizeof(int));
    int last = 0;

    // Inicializando fila com -1 nas posições de memória
    for (int i = 0; i < qtd_frames; i++)
        queue[i] = -1;

    int page_faults = 0;

    node_t *node = linked_list->next;

    while (node != NULL) {

        // Verifica se a página está na memória
        int not_found = 1;
        for (int j = 0; j < qtd_frames; j++) {
            if (queue[j] == node->val) {
                not_found = 0;
            }
        }

        // Se página não esta na memória
        if (not_found) {
            queue[last] = node->val;
            last = (last + 1) % qtd_frames;
            page_faults++;
        }
        node = node->next;
    }

    return (page_faults);
}

int lru(int qtd_frames, node_t *linked_list) {
    cache = malloc(qtd_frames * sizeof(int));

    for (int i = 0; i < qtd_frames; i++)
        cache[i] = -1;

    int page_faults = 0;

    node_t *node = linked_list->next;

    while (node != NULL) {
        int current = cache[0];
        int next;
        int em_cache = 0;
        for (int i = 1; i <= qtd_frames; i++) {
            if (current == node->val) {
                em_cache = 1;
                break;
            }
            next = cache[i];
            cache[i] = current;
            current = next;
        }
        if (em_cache == 0) page_faults++;
        cache[0] = node->val;
        node = node->next;
    }

    return (page_faults);
}

int optimal(int qtd_frames, node_t *linked_list) {
    cache = malloc(qtd_frames * sizeof(int));

    for (int i = 0; i < qtd_frames; i++)
        cache[i] = -1;

    int page_faults = 0;

    node_t *node = linked_list->next;

    while (node != NULL) {
        // Verifica se a página está em cache
        int present = 0;
        for (int i = 0; i < qtd_frames; i++) {
            if (cache[i] == node->val) {
                present = 1;
                break;
            }
        }
        if (present == 1) {
            node = node->next;
            continue;
        }

        // Página não está em cache
        page_faults++;
        int farthest = 0;
        int idx_farthest = -1;
        for (int i = 0; i < qtd_frames; i++) {
            if (cache[i] == -1) {
                idx_farthest = i;
                break;
            }
            node_t *temp_node = node->next;
            int count = 0;
            while (temp_node != NULL) {
                count++;
                if (temp_node->val == cache[i]) {
                    if (count > farthest) {
                        idx_farthest = i;
                        farthest = count;
                        break;
                    } else {
                        break;
                    }
                }
                temp_node = temp_node->next;
            }
            if (temp_node == NULL) {
                idx_farthest = i;
                break;
            }
        }
        if (idx_farthest != -1) cache[idx_farthest] = node->val;
        node = node->next;
    }

    return (page_faults);
}

node_t *read_lines() {
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

    return (head);
}

void free_list(struct node *head) {
    struct node *tmp;

    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp);
    }

}