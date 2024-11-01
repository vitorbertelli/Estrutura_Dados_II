#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Definição de um nó para uma estrutura árvore + heap (vulgo treap)
struct NoTreap
{
  int valor;            // Valor armazenado no nó
  int prioridade;       // Prioridade do nó
  struct NoTreap *esq;  // Ponteiro para o nó à esquerda
  struct NoTreap *dir;  // Ponteiro para o nó à direita
};

typedef struct NoTreap NoTreap;
NoTreap *criarArvore(int valor);
NoTreap *rotacaoEsquerda(NoTreap *raiz);
NoTreap *rotacaoDireita(NoTreap *raiz);
NoTreap *inserirNo(NoTreap *raiz, int valor);
NoTreap *removerNo(NoTreap *raiz, int valor);
void imprimirArvore(NoTreap *raiz, int nivel);

int main(void)
{
  NoTreap *raiz = criarArvore(10);
  raiz = inserirNo(raiz, 5);
  raiz = inserirNo(raiz, 6);
  raiz = inserirNo(raiz, 15);
  raiz = inserirNo(raiz, 13);

  printf("Árvore Treap:\n");
  imprimirArvore(raiz, 0);

  raiz = removerNo(raiz, 5);
  printf("\nÁrvore Treap após remoção:\n");
  imprimirArvore(raiz, 0);

  return 0;
}

// Função para inicializar uma árvore Treap
NoTreap *criarArvore(int valor)
{
  NoTreap *novoNo = (NoTreap *)malloc(sizeof(NoTreap));
  if (novoNo == NULL) {
    printf("Erro ao alocar memória.\n");
    exit(EXIT_FAILURE);
  }

  novoNo->valor = valor;
  novoNo->prioridade = rand() % 100;
  novoNo->esq = NULL;
  novoNo->dir = NULL;
  return novoNo;
}

// Função para realizar uma rotação à esquerda em uma estrutura treap
NoTreap *rotacaoEsquerda(NoTreap *raiz)
{
  NoTreap *novaRaiz = raiz->dir;
  raiz->dir = novaRaiz->esq;
  novaRaiz->esq = raiz;
  return novaRaiz;
}

// Função para realizar uma rotação à direita em uma estrutura treap
NoTreap *rotacaoDireita(NoTreap *raiz)
{
  NoTreap *novaRaiz = raiz->esq;
  raiz->esq = novaRaiz->dir;
  novaRaiz->dir = raiz;
  return novaRaiz;
}

// Função para inserir um valor na árvore
NoTreap *inserirNo(NoTreap *raiz, int valor)
{
  if (raiz == NULL)
    return criarArvore(valor);

  if (valor < raiz->valor)
  {
    raiz->esq = inserirNo(raiz->esq, valor);

    // É necessário para manter a árvore balanceada
    // Realiza rotação à direita se a prioridade do nó à esquerda for maior
    if (raiz->esq->prioridade > raiz->prioridade)
      raiz = rotacaoDireita(raiz);
  }
  else if (valor > raiz->valor)
  {
    raiz->dir = inserirNo(raiz->dir, valor);

    if (raiz->dir->prioridade > raiz->prioridade)
      raiz = rotacaoEsquerda(raiz);
  }

  return raiz;
}

// Função para remover um valor da árvore
NoTreap *removerNo(NoTreap *raiz, int valor)
{
  if (raiz == NULL)
    return raiz;

  // Realiza a busca pelo valor na árvore
  if (valor < raiz->valor)
    raiz->esq = removerNo(raiz->esq, valor);

  else if (valor > raiz->valor)
    raiz->dir = removerNo(raiz->dir, valor);

  // Se o valor foi encontrado, começa o processo de remoção
  else
  {
    // Caso 1: Nó possui nenhum filho
    if (raiz->esq == NULL && raiz->dir == NULL)
    {
      free(raiz); // Libera a memória do nó removido
      return NULL;
    }
    // Caso 2: Nó possui apenas um filho à direita
    else if (raiz->esq == NULL)
    {
      NoTreap *temp = raiz->dir; // Promove o filho da direita
      free(raiz);
      return temp;
    }
    // Caso 3: Nó possui apenas um filho à esquerda
    else if (raiz->dir == NULL)
    {
      NoTreap *temp = raiz->esq; // Promove o filho da esquerda
      free(raiz);
      return temp;
    }
    // Caso 4: Nó possui dois filhos
    else
    {
      // Procura então o sucessor
      // O sucessor será o filho com maior prioridade
      if (raiz->esq->prioridade > raiz->dir->prioridade)
      {
        raiz = rotacaoDireita(raiz); // Rotaciona a árvore para promover o filho com maior prioridade
        raiz->dir = removerNo(raiz->dir, valor); // Agora remove o valor da árvore
      }
      else
      {
        raiz = rotacaoEsquerda(raiz);
        raiz->esq = removerNo(raiz->esq, valor);
      }
    }
  }

  return raiz;
}

// Função criada pelo ChatGPT para imprimir a árvore Treap
void imprimirArvore(NoTreap *raiz, int nivel)
{
  if (raiz == NULL)
    return;

  // Imprime o filho direito
  imprimirArvore(raiz->dir, nivel + 1);

  // Imprime o nó atual com recuo de acordo com o nível
  for (int i = 0; i < nivel; i++)
    printf("\t");

  printf("(%d, %d)\n", raiz->valor, raiz->prioridade);

  // Imprime o filho esquerdo
  imprimirArvore(raiz->esq, nivel + 1);
}