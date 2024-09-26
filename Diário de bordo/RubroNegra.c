#include <stdio.h>
#include <stdlib.h>

#define PRETO 0
#define VERMELHO 1

// Estrutura usada para criar uma Árvore Rubro-Negra caída para Esquerda (como apresentado nos slides da matéria)
struct No
{
  int valor;
  struct No *esq;
  struct No *dir;
  int cor;
};

// Protótipos das funções usadas no exercício
typedef struct No No;
int cor(No *no);
void trocarCor(No *no);
No* rotacaoEsquerda(No *no);
No* rotacaoDireita(No *no);
No* criarArvore(int valor);
No* inserir(No* raiz, int valor);
No* inserirNo(No *raiz, int valor);
No* remover(No *raiz, int valor);
No* removerNo(No *raiz, int valor);
No* move2EsqRed(No *no);
No* move2DirRed(No *no);
No* balancear(No *no);
No* procurarMenor(No *raiz);
No* removerMenor(No *raiz);
void imprimirArvore(No *raiz, int b);

int main(void)
{
  No *raiz = criarArvore(10);
  raiz = inserir(raiz, 5);
  raiz = inserir(raiz, 6);
  raiz = inserir(raiz, 15);
  raiz = inserir(raiz, 13);

  imprimirArvore(raiz, 3);

  raiz = remover(raiz, 5);
  printf("\n");
  
  imprimirArvore(raiz, 3);
  
  return 0;
}

// Função para inicializar uma Árvore Rubro-Negra
No* criarArvore(int valor)
{
  No *raiz = (No*)malloc(sizeof(No)); // Aloca espaço na memória HEAP para armazenar o ponteiro referente a raiz da árvore
  if(raiz == NULL) // Verifica se ocorreu tudo certo na alocação da memória, caso contrário, exibe uma mensagem de erro e fecha a aplicação
  {
    printf("Erro ao alocar memória.\n");
    exit(EXIT_FAILURE); 
  }

  raiz->valor = valor; // Armazena o valor que do nó na árvore
  raiz->dir = NULL; // Inicializa os ponteiros esquerdo e direito como nulos
  raiz->esq = NULL;
  raiz->cor = VERMELHO; // Define a cor inicial como vermelho

  return raiz;
}

// Função auxiliar que retorna a cor de um nó
int cor(No *no) 
{
  if(no == NULL) // Se o nó for nulo, ele está no final da raiz e todo nó nulo é preto
    return PRETO;
  return no->cor; // Caso contrário, retorna a cor do nó
}

// Função auxiliar que troca a cor de um nó
void trocarCor(No *no)
{
  no->cor = !no->cor; // Troca a cor do nó usando a negação do valor da cor atual, se for 1 (vermelho), vira 0 (preto) e vice-versa
  // Mudança de cor de cada filho do nó de vermelho para preto e vice-versa
  // Necessário para manter a propriedade de que um nó vermelho não pode ter filhos vermelhos
  // Pode causar violações ao decorer da árvore, logo é necessário balancear a árvore com outras operações 
  if(no->esq != NULL)
    no->esq->cor = !no->esq->cor;
  if(no->dir != NULL)
    no->dir->cor = !no->dir->cor;
}

// Função que realiza a rotação à esquerda
// Recebe um nó raiz de conjunto de tres nós, e realiza a rotação do nó vermelho à direita para à esquerda
No* rotacaoEsquerda(No *no)
{
  // Recebe um nó com x como filho direito
  // Inverte a posição dos nós e o nó x vira a raiz da subárvore
  No *x = no->dir;
  no->dir = x->esq;
  x->esq = no;
  x->cor = no->cor; // Nó x recebe a cor do nó enviado, já que agora ele é a raiz da subárvore
  no->cor = VERMELHO; // E o nó enviado recebe a cor vermelha
  return x;
}

// Função que realiza a rotação à direita
// Recebe um nó raiz de conjunto de tres nós, e realiza a rotação do nó vermelho à esquerda para à direita
No* rotacaoDireita(No *no)
{
  // Recebe um nó com x como filho esquerdo
  // Inverte a posição dos nós e o nó x vira a raiz da subárvore
  No *x = no->esq;
  no->esq = x->dir;
  x->dir = no;
  // Nó x recebe a cor do nó pai, que fica com a cor vermelha
  x->cor = no->cor;
  no->cor = VERMELHO;
  return x;
}

// Função para mover um nó vermelho para esquerda
No* move2EsqRed(No *no)
{
  trocarCor(no);
  if(cor(no->dir->esq) == VERMELHO) // Se o filho à esquerda do filho direito é vermelho,
  // é necessário fazer a rotação à direita no filho direito e uma rotação à esquerda no pai
  {
    no->dir = rotacaoDireita(no->dir);
    no = rotacaoEsquerda(no);
    trocarCor(no);
  }
  return no;
}

// Função para mover um nó vermelho para direita
No* move2DirRed(No *no)
{
  trocarCor(no);
  if(cor(no->esq->esq) == VERMELHO) // Se o filho à esquerda do filho esquerdo é vermelho,
  // é necessário fazer a rotação à direita no pai
  {
    no = rotacaoDireita(no);
    trocarCor(no);
  }
  return no;
}

// Função para balancear a árvore
No* balancear(No *no)
{
  // Existem tres casos para balancear a árvore:
  // 1. Se o filho direito é vermelho: rotação à esquerda
  if(cor(no->dir) == VERMELHO)
    no = rotacaoEsquerda(no);
  // 2. Se o filho esquerdo e o neto esquerdo são vermelhos: rotação à direita
  if(no->esq != NULL && cor(no->esq) == VERMELHO && cor(no->esq->esq) == VERMELHO)
    no = rotacaoDireita(no);
  // 3. Se ambos os filhos são vermelhos: troca de cor dos nós
  if(cor(no->esq) == VERMELHO && cor(no->dir) == VERMELHO)
    trocarCor(no);
  return no;
}

No* inserir(No* raiz, int valor)
{
  raiz = inserirNo(raiz, valor);  // Faz a inserção e reestrutura a árvore

  // Garantindo que a raiz final seja sempre preta
  if (raiz != NULL)
    raiz->cor = PRETO;

  return raiz;
}

// Função para inserir um nó na árvore
No* inserirNo(No *raiz, int valor)
{
  if(raiz == NULL) // Se a raiz for nula,
    return criarArvore(valor); // Cria um novo nó com o valor passado

  // Busca o local correto para inserir o nó
  if(raiz->valor == valor) // Valor duplicado
    return raiz; 
  else if(raiz->valor < valor)
    raiz->dir = inserirNo(raiz->dir, valor);
  else
    raiz->esq = inserirNo(raiz->esq, valor);

  // Balanceia a árvore
  if(cor(raiz->dir) == VERMELHO && cor(raiz->esq) == PRETO) 
    raiz = rotacaoEsquerda(raiz);
  if(cor(raiz->esq) == VERMELHO && cor(raiz->esq->esq) == VERMELHO)
    raiz = rotacaoDireita(raiz);
  if(cor(raiz->esq) == VERMELHO && cor(raiz->dir) == VERMELHO)
    trocarCor(raiz);

  return raiz;
}

No* remover(No* raiz, int valor)
{
  raiz = removerNo(raiz, valor);  // Remove o valor e ajusta a árvore

  // Garantindo que a raiz seja preta
  if (raiz != NULL)
    raiz->cor = PRETO;

  return raiz;
}

// Função para remover um nó da árvore
No* removerNo(No *raiz, int valor)
{
  if(valor < raiz->valor)
  {
    if(cor(raiz->esq) == PRETO && cor(raiz->esq->esq) == PRETO)
      raiz = move2EsqRed(raiz);
    raiz->esq = removerNo(raiz->esq, valor);
  }
  else
  {
    if(cor(raiz->esq) == VERMELHO)
      raiz = rotacaoDireita(raiz);

    if(valor == raiz->valor && raiz->dir == NULL) // Se o valor a ser removido for encontrado e o nó à direita for nulo
    {
      free(raiz); // Libera o nó da memória
      return NULL;
    }

    if(cor(raiz->dir) == PRETO && cor(raiz->dir->esq) == PRETO) // Se o nó à direita e o neto à esquerda forem pretos
      raiz = move2DirRed(raiz); // Move o nó vermelho para a direita

    if(valor == raiz->valor) // Se o valor a ser removido for encontrado
    {
      No* x = procurarMenor(raiz->dir); // Procura o menor valor na sub-árvore direita
      raiz->valor = x->valor; // Substitui o valor do nó atual pelo menor valor encontrado
      raiz->dir = removerMenor(raiz->dir); // Remove o menor valor da sub-árvore direita
    }
    else
      raiz->dir = removerNo(raiz->dir, valor);
  }
  return balancear(raiz); // Ao voltar da recursão, balanceia a árvore em cada sub-árvore
}

// Função para procurar o menor valor na árvore
No* procurarMenor(No *raiz)
{
  if(raiz->esq == NULL) // Se o nó à esquerda for nulo, ele é o menor valor
    return raiz; // Retorna o nó atual
  return procurarMenor(raiz->esq); // Caso contrário, chama a função recursivamente para o nó à esquerda
}

// Função para remover o menor valor da árvore sem desbalancear
No* removerMenor(No *raiz)
{
  if(raiz->esq == NULL) // Se o nó à esquerda for nulo, ele é o menor valor
  {
    free(raiz); // Libera o nó da memória 
    return NULL;
  }
  if(cor(raiz->esq) == PRETO && cor(raiz->esq->esq) == PRETO) // Se o nó à esquerda e o neto à esquerda forem pretos
    raiz = move2EsqRed(raiz); // Move o nó vermelho para a esquerda
  raiz->esq = removerMenor(raiz->esq);
  return balancear(raiz);
}

// Função para imprimir a árvore Red-Black em ordem
void emOrdem(No *raiz)
{
  if (raiz != NULL)
  {
    emOrdem(raiz->esq);
    if (raiz->cor == 1)
      printf("%d RED", raiz->valor);
    else
      printf("%d BLK", raiz->valor);
    emOrdem(raiz->dir);
  }
}

// Função para imprimir a árvore de acordo com o formato esquerda-raiz-direita segundo Sedgewick
void imprimirArvore(No *raiz, int b)
{
  if (raiz != NULL)
  {
    // Chama a função recursivamente para percorrer a subárvore direita
    imprimirArvore(raiz->dir, b + 1);

    // Imprime o valor do nó atual com um espaçamento proporcional à sua profundidade
    for (int i = 0; i < b; i++)
      printf("       "); // espaços por nível
    if (raiz->cor == 1)
      printf("\033[31m%d\033[0m\n\n", raiz->valor);
    else
      printf("%d\n\n", raiz->valor);

    // Chama a função recursivamente para percorrer a subárvore esquerda
    imprimirArvore(raiz->esq, b + 1);
  }
}