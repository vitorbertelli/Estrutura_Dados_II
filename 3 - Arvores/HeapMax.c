#include <stdio.h>
#include <math.h>

// Função para trocar dois elementos
void trocar(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Função para "heapificar" um subárvore com raiz no nó 'i'
// 'n' é o tamanho do heap
void heapificar(int vetor[], int n, int i) {
    int maior = i;         // Inicialmente, a raiz é o maior
    int esquerda = 2 * i + 1;  // Filho esquerdo de i
    int direita = 2 * i + 2;   // Filho direito de i

    // Se o filho esquerdo for maior que a raiz
    if (esquerda < n && vetor[esquerda] > vetor[maior]) {
        maior = esquerda;
    }

    // Se o filho direito for maior que o maior até agora
    if (direita < n && vetor[direita] > vetor[maior]) {
        maior = direita;
    }

    // Se o maior não for a raiz
    if (maior != i) {
        trocar(&vetor[i], &vetor[maior]);

        // Recursivamente heapificar o subárvore afetado
        heapificar(vetor, n, maior);
    }
}

// Função auxiliar para imprimir espaços
void imprimirEspacos(int nivel) {
    for (int i = 0; i < nivel; i++) {
        printf("   ");  // Três espaços
    }
}

// Função recursiva para imprimir a árvore no estilo Sedgwick
void imprimirArvore(int vetor[], int n, int i, int nivel) {
    if (i < n) {
        imprimirArvore(vetor, n, 2 * i + 2, nivel + 1);  // Subárvore direita
        imprimirEspacos(nivel);
        printf("%d\n", vetor[i]);                        // Nó atual
        imprimirArvore(vetor, n, 2 * i + 1, nivel + 1);  // Subárvore esquerda
    }
}

// Função para inserir um elemento e manter a propriedade de Max-Heap
void inserirNoHeap(int vetor[], int n) {
    int i = n - 1;  // Índice do elemento recém-inserido

    // Subir o elemento inserido para manter a propriedade do Max-Heap
    while (i != 0 && vetor[(i - 1) / 2] < vetor[i]) {
        trocar(&vetor[i], &vetor[(i - 1) / 2]);  // Troca com o pai
        i = (i - 1) / 2;  // Move para cima na árvore
    }
}

// Função para construir o Max-Heap inserindo um elemento de cada vez
void construirMaxHeapIncremental(int vetor[], int tamanhoOriginal) {
    for (int i = 1; i <= tamanhoOriginal; i++) {
        printf("\nInserindo elemento %d no heap:\n", vetor[i-1]);
        inserirNoHeap(vetor, i);  // Inserir o i-ésimo elemento
        imprimirArvore(vetor, i, 0, 0);  // Imprimir a árvore após a inserção
    }
}

// Função para excluir a raiz (maior elemento) do Max-Heap
int excluirDoHeap(int vetor[], int* n) {
    if (*n <= 0) {
        printf("Heap vazio!\n");
        return -1;
    }

    // O maior elemento é a raiz do heap (índice 0)
    int raiz = vetor[0];

    // Colocar o último elemento da árvore na posição da raiz
    vetor[0] = vetor[*n - 1];

    // Reduzir o tamanho do heap
    (*n)--;

    // Heapificar para restaurar a propriedade do Max-Heap
    heapificar(vetor, *n, 0);

    // Retorna o elemento removido (antiga raiz)
    return raiz;
}

// Função para excluir o elemento raiz repetidamente e mostrar o heap após cada exclusão
void excluirElementosHeap(int vetor[], int* n) {
    while (*n > 0) {
        int removido = excluirDoHeap(vetor, n);
        printf("\nElemento removido: %d\n", removido);
        printf("Heap após remoção:\n");
        imprimirArvore(vetor, *n, 0, 0);  // Imprimir o heap após cada remoção
    }
}

// Função para imprimir o vetor (linear)
void imprimirVetor(int vetor[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", vetor[i]);
    }
    printf("\n");
}

// Função principal
int main() {
    int vetor[] = {12, 11, 13, 5, 6, 7};
    int tamanho = sizeof(vetor) / sizeof(vetor[0]);

    printf("Vetor original:\n");
    imprimirVetor(vetor, tamanho);

    // Construir o Max-Heap de forma incremental
    construirMaxHeapIncremental(vetor, tamanho);

    printf("\nMax-Heap final:\n");
    imprimirVetor(vetor, tamanho);

    printf("\nÁrvore representada estilo Sedgwick do Max-Heap final:\n");
    imprimirArvore(vetor, tamanho, 0, 0);  // Imprime a árvore em estilo hierárquico

    // Excluir elementos do heap um por um e mostrar o heap após cada remoção
    excluirElementosHeap(vetor, &tamanho);

    return 0;
}
