#include <stdio.h>
#include <stdlib.h>

#define ORDEM 4
#define FALSE 0
#define TRUE 1

struct NoArvoreB
{
  int quantidade;
  int valores[ORDEM - 1];
  struct NoArvoreB *filhos[ORDEM];
  int folha;
};

typedef struct NoArvoreB NoArvoreB;
NoArvoreB *criarArvoreB(int folha);
void divideFilho(NoArvoreB *raiz, int i);
void inserirNoArvoreNaoCheia(NoArvoreB *raiz, int valor);
NoArvoreB *inserirNo(NoArvoreB *raiz, int valor);
void imprimirArvoreB(NoArvoreB *raiz);

int main(void)
{
  NoArvoreB *raiz = NULL;
  raiz = inserirNo(raiz, 10);
  raiz = inserirNo(raiz, 5);
  raiz = inserirNo(raiz, 6);
  raiz = inserirNo(raiz, 15);
  raiz = inserirNo(raiz, 13);

  printf("Árvore B:\n");
  imprimirArvoreB(raiz);

  printf("\n%d", raiz->quantidade);
  printf("\n%d", raiz->valores[0]);
  printf("\n%d", raiz->filhos[0]->quantidade);
  printf("\n%d", raiz->filhos[0]->valores[0]);
  printf("\n%d", raiz->filhos[1]->quantidade);
  printf("\n%d", raiz->filhos[1]->valores[0]);
  printf("\n%d", raiz->filhos[1]->valores[1]);
  printf("\n%d", raiz->filhos[1]->valores[2]);

  return 0;
}

NoArvoreB *criarArvoreB(int folha)
{
  NoArvoreB *raiz = (NoArvoreB*)malloc(sizeof(NoArvoreB));
  if(raiz == NULL)
  {
    printf("Erro ao alocar memória.\n");
    exit(EXIT_FAILURE);
  }
  raiz->quantidade = 0;
  raiz->folha = folha;
  for(int i = 0; i < ORDEM; i++)
    raiz->filhos[i] = NULL;
  return raiz;
}

// Função que divide o filho cheio de um nó
void divideFilho(NoArvoreB *raiz, int i)
{
  NoArvoreB *filho = raiz->filhos[i]; // Filho a ser dividido
  NoArvoreB *novoFilho = criarArvoreB(filho->folha); // Novo nó que receberá a metade dos valores do filho
  novoFilho->quantidade = ORDEM / 2 - 1;

  // Realiza a divisão dos valores do filho para o novo filho
  for (int j = 0; j < ORDEM / 2 - 1; j++)
    novoFilho->valores[j] = filho->valores[j + ORDEM / 2];

  if (filho->folha == FALSE)
  {
    for (int j = 0; j < ORDEM / 2; j++)
      novoFilho->filhos[j] = filho->filhos[j + ORDEM / 2];
  }

  filho->quantidade = ORDEM / 2 - 1; // Atualiza a quantidade de valores do filho

  // Realiza o deslocamento dos filhos do pai
  for (int j = raiz->quantidade; j > i + 1; j--)
    raiz->filhos[j + 1] = raiz->filhos[j];
  
  raiz->filhos[i + 1] = novoFilho; // Adiciona o novo filho criado ao pai

  // Move os valores do pai para abrir espaço para o valor do meio
  for (int j = raiz->quantidade - 1; j >= i; j--)
    raiz->valores[j + 1] = raiz->valores[j];

  // Sobe o valor do meio do filho para o pai
  raiz->valores[i] = filho->valores[ORDEM / 2 - 1];
  raiz->quantidade++;
}

// Função que insere um valor em um nó que não está cheio
void inserirNoArvoreNaoCheia(NoArvoreB *raiz, int valor)
{
  int i = raiz->quantidade - 1; // Índice do último valor do nó

  if (raiz->folha == TRUE)
  {
    // Procura a posição correta para inserir o novo valor respeitar a ordem
    while (i >= 0 && raiz->valores[i] > valor)
    {
      raiz->valores[i + 1] = raiz->valores[i];
      i--;
    }
    // Insere o novo valor
    raiz->valores[i + 1] = valor;
    raiz->quantidade++;
  }
  else
  {
    // Encontra o filho que receberá o novo valor
    while (i >= 0 && raiz->valores[i] > valor)
      i--;
    i++;
    // Se o filho está cheio divide o nó
    if (raiz->filhos[i]->quantidade == ORDEM - 1)
    {
      divideFilho(raiz, i);
      if (raiz->valores[i] < valor) // Checa em qual dos filhos o valor será inserido
        i++;
    }
    inserirNoArvoreNaoCheia(raiz->filhos[i], valor); // Insere o valor no filho
  }
}

// Função que insere um valor na árvore B
NoArvoreB *inserirNo(NoArvoreB *raiz, int valor)
{
  if (raiz == NULL)
  {
    raiz = criarArvoreB(TRUE);
    raiz->valores[0] = valor;
    raiz->quantidade = 1;
    return raiz;
  }

  // Verifica se o nó está cheia
  if (raiz->quantidade == ORDEM - 1)
  {
    NoArvoreB *novaRaiz = criarArvoreB(FALSE);
    novaRaiz->filhos[0] = raiz;
    divideFilho(novaRaiz, 0); // Divide o filho da nova raiz
    raiz = novaRaiz;
  }
  inserirNoArvoreNaoCheia(raiz, valor); // Insere o valor na árvore

  return raiz;
}

// Função que imprime a árvore B
void imprimirArvoreB(NoArvoreB *raiz)
{
  if (raiz != NULL)
  {
    int i;
    for (i = 0; i < raiz->quantidade; i++)
    {
      imprimirArvoreB(raiz->filhos[i]);
      printf("%d ", raiz->valores[i]);
    }
    imprimirArvoreB(raiz->filhos[i]);
  }
}