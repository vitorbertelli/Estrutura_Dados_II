#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

struct No
{
  int valor;
  struct No *esq;
  struct No *dir;
};

typedef struct No No;
struct No* criarArvore(int valor);
struct No* inserirNo(struct No *raiz, int valor);
struct No* pesquisarNo(struct No *raiz, int valor);
struct No* removerNo(struct No *raiz, int valor);
void imprimirArvore(struct No *raiz);

int main(void)
{
  setlocale(LC_ALL, "Portuguese");
  printf("Olá Mundo!\n");

  struct No *raiz = criarArvore(10);
  inserirNo(raiz, 5);
  inserirNo(raiz, 6);
  inserirNo(raiz, 15);
  inserirNo(raiz, 13);

  imprimirArvore(raiz);

  removerNo(raiz, 10);
  printf("\n");
  
  imprimirArvore(raiz);

  return 0;
}

struct No* criarArvore(int valor)
{
  struct No *raiz = (struct No *)malloc(sizeof(struct No));
  if(raiz == NULL) {
    printf("Erro ao alocar memória.\n");
    exit(EXIT_FAILURE);
  }

  raiz->valor = valor;
  return raiz;
}

struct No* inserirNo(struct No *raiz, int valor)
{
  if(raiz == NULL)
  {
    return criarArvore(valor);
  }

  if(valor < raiz->valor)
  {
    raiz->esq = inserirNo(raiz->esq, valor);
  }
  else if(valor > raiz->valor)
  {
    raiz->dir = inserirNo(raiz->dir, valor);
  }
  else
  {
    printf("Valor já existe na árvore.\n");
    exit(EXIT_FAILURE);
  } 

  return raiz;
}

struct No* pesquisarNo(struct No *raiz, int valor) 
{
  if(raiz == NULL)
  {
    printf("Valor não encontrado na árvore.\n");
    exit(EXIT_FAILURE);
  }

  if(raiz->valor == valor)
  {
    return raiz;
  }

  if(raiz->valor < valor)
  {
    return pesquisarNo(raiz->dir, valor);
  }
  else
  {
    return pesquisarNo(raiz->esq, valor);
  }
}

struct No* removerNo(struct No *raiz, int valor)
{
  // garante que a recursividade funcione
  if(raiz == NULL)
  {
    free(raiz);
    return NULL;
  }
  else
  {
    if(raiz->valor == valor)
    {
      // remove nós que não possuem filho
      if(raiz->esq == NULL && raiz->dir == NULL)
      {
        free(raiz);
        return NULL;
      }
      else
      { 
        struct No *temp;
        // remove nós que possuem apenas um filhos
        if(raiz->esq == NULL || raiz->dir == NULL)
        { 
          if(raiz->esq != NULL)
          {
            temp = raiz->esq;
          }
          else
          {
            temp = raiz->dir;
          }
          free(raiz);
          return temp;
        }
        // remove nós que possuem dois filhos
        else
        {
          temp = raiz->esq;
          while(temp->dir != NULL)
          {
            temp = temp->dir;
          }
          raiz->valor = temp->valor;
          temp->valor = valor;
          raiz->esq = removerNo(raiz->esq, valor);
          return raiz;
        }
      }
    }
    // busca pelo nó a ser deletado
    else
    {
      if(raiz->valor > valor)
      {
        raiz->esq = removerNo(raiz->esq, valor);
      }
      else
      {
        raiz->dir = removerNo(raiz->dir, valor);
      }
      return raiz;
    }
  }
}

void imprimirArvore(struct No *raiz)
{
  if(raiz != NULL)
  {
  imprimirArvore(raiz->esq);
  printf("%d ", raiz->valor);
  imprimirArvore(raiz->dir);
  }
}
