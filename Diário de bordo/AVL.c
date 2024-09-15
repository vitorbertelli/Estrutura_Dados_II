#include <stdio.h>
#include <stdlib.h>

// Estrutura usada para criar a árvore AVL
struct No
{
  int valor;
  struct No *esq;
  struct No *dir;
  int altura;
};

// Protótipos das funções usadas no exercício
typedef struct No No;
No* criarArvore(int valor);
int altura(No *no);
int fatorBalanceamento(No *no);
void atualizarAltura(No *no);
void imprimirNo(int valor, int b);
void imprimirArvore(No *raiz, int b);
No* rotacaoSimplesDireita(No *no);
No* rotacaoSimplesEsquerda(No *no);
No* rotacaoDuplaDireita(No* no);
No* rotacaoDuplaEsquerda(No* no);
No* balancear(No *raiz);
No* inserirNo(No *raiz, int valor);
No* pesquisarNo(No *raiz, int valor);
No* removerNo(No *raiz, int valor);

int main(void)
{
  No *raiz = criarArvore(10);
  raiz = inserirNo(raiz, 5);
  raiz = inserirNo(raiz, 6);
  raiz = inserirNo(raiz, 15);
  raiz = inserirNo(raiz, 13);

  imprimirArvore(raiz, 3);

  raiz = removerNo(raiz, 5);
  printf("\n");
  
  imprimirArvore(raiz, 3);

  return 0;
}

// Função para inicializar uma árvore AVL
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
  raiz->altura = 0; // Define a altura inicial como zero

  return raiz;
}

// Função usada para facilitar a verificação do valor da altura do nó durante o processo de balanceamento da árvore
int altura(No *no) // Recebe como parâmetro da função o nó que será retornado sua altura
{
  if(no == NULL) // Verifica se o nó é nulo, caso confirmado, a altura -1 será retornado
  {
    return -1;
  }
  return no->altura; // Se o nó existir, o valor de sua altura será retornado
}

// Função recursiva que calcula o fator de balancemento do nó enviado
int fatorBalanceamento(No *no)
{
  if(no == NULL) // Verifica se o nó existe, caso contrário, retorna o fb como 0
  {
    return 0;
  }
  return altura(no->esq) - altura(no->dir); // retornar o fb do nó usando uma chamada recursiva para calcular o fb total da subárvore
}

// Função auxiliar para atualizar a altura dos nós da árvore nas funções inserirNo, removerNo, e nas rotações
void atualizarAltura(No *no)
{
  if(altura(no->esq) > altura(no->dir))
    no->altura = altura(no->esq) + 1;
  else
    no->altura = altura(no->dir) + 1;
}

// Função copiada do exemplo do professor
// Função auxiliar para imprimir um caractere precedido por uma quantidade específica de espaços
void imprimirNo(int valor, int b)
{
  for(int i = 0; i < b; i++)
  {
    printf("   ");
  }
  printf("%i\n", valor);
}

// Função copiada do exemplo do professor
// Função para exibir a árvore no formato esquerda-raiz-direita segundo Sedgewick
void imprimirArvore(No *raiz, int b)
{
  if (raiz != NULL)
  {
    imprimirArvore(raiz->dir, b + 1);
    imprimirNo(raiz->valor, b);
    imprimirArvore(raiz->esq, b + 1);
  }
}

// Função usada para realizar o rotação simples à direita na árvore AVL
No* rotacaoSimplesDireita(No *no)
{
  No* y = no->esq; // Define a nova raiz a partir do nó à esquerda da raiz dessa árvore
  No* f = y->dir; // Define um ponteiro temporário para fazer a rotação da maneira correta 

  y->dir = no; // Define a antiga raiz como filho esquerdo da nova raiz dessa árvore
  no->esq = f; // Fecha a rotação colocando o filho esquerdo da nova raiz como filho direito da antiga raiz

  // Verifica a altura dos filhos do nó e atualiza de acordo com o maior valor
  atualizarAltura(no);

  // Verifica a altura dos filhos da nova raiz e atualiza de acordo com o maior valor
  atualizarAltura(y);

  return y; // Retorna a nova raiz
}

// Função usada para realizar o rotação simples à esqueda na árvore AVL
No* rotacaoSimplesEsquerda(No *no)
{
  No* y = no->dir; // Define a nova raiz a partir do nó a direita da raiz da subárvore
  No* f = y->esq; // Define um ponteiro temporário para fazer a rotação da maneira correta

  y->esq = no; // Define a raiz como filho esquerdo da nova raiz dessa árvore
  no->dir = f; // Fecha a rotação colocando o filho esquerdo da nova raiz como filho direito da antiga raiz

  // Verifica a altura dos filhos do nó e atualiza de acordo com o maior valor
  atualizarAltura(no);

  // Verifica a altura dos filhos da nova raiz e atualiza de acordo com o maior valor
  atualizarAltura(y);

  return y; // Retorna a nova raiz
}

// Função para realizar a rotação dupla à direita
No* rotacaoDuplaDireita(No* no)
{
  // Assim como a rotação dupla à esquerda, a rotação dupla à direita é composta por duas etapas:
  no->esq = rotacaoSimplesEsquerda(no->esq); // Rotação simples à esquerda no nó filho da direita
  return rotacaoSimplesDireita(no); // Rotação simples à direita no nó raiz
}

// Função para realizar a rotação dupla à esquerda
No* rotacaoDuplaEsquerda(No* no)
{
  // A rotação dupla à esquerda é composta por essas duas etapas:
  no->dir = rotacaoSimplesDireita(no->dir); // Rotação simples à direita no nó filho da esquerda
  return rotacaoSimplesEsquerda(no); // Rotação simples à esquerda no nó raiz
}

// Função auxilar o balanceamento na inserção e remoção de nós na árvore AVL
No* balancear(No *raiz)
{
  int fb = fatorBalanceamento(raiz); // Define o fator de balanceamento do nó raiz da subárvore usada como argumento da função

  // Verifica e realiza a rotação simples à direita
  if (fb > 1 && fatorBalanceamento(raiz->esq) >= 0)
    raiz = rotacaoSimplesDireita(raiz);

  // Verifica e realiza a rotação simples à esquerda
  else if(fb < -1 && fatorBalanceamento(raiz->dir) <= 0)
    raiz = rotacaoSimplesEsquerda(raiz);

  // Verifica e realiza a rotação dupla à direita
  else if(fb > 1 && fatorBalanceamento(raiz->esq) < 0)
    raiz = rotacaoDuplaDireita(raiz);

  // Verifica e realiza a rotação dupla à esquerda
  else if(fb < -1 && fatorBalanceamento(raiz->dir) > 0)
    raiz = rotacaoDuplaEsquerda(raiz);

  return raiz; // Retorna a raiz balanceada
}

// Função para inserção de nós em uma árvore AVL
No* inserirNo(No *raiz, int valor)
{
  if(raiz == NULL)
  {
    return criarArvore(valor);
  }

  // Verifica e insere o valor na árvore
  if(valor < raiz->valor)
    raiz->esq = inserirNo(raiz->esq, valor);
  else if(valor > raiz->valor)
    raiz->dir = inserirNo(raiz->dir, valor);
  else
    printf("Valor já existe na árvore.\n");

  // atualiza a altura do nó raiz da árvore
  atualizarAltura(raiz);
  
  // Realiza o balanceamento da árvore a partir dos nós chamados na função recursiva
  raiz = balancear(raiz);

  return raiz;
}

No* pesquisarNo(No *raiz, int valor) 
{
  if(raiz == NULL)
  {
    return NULL;
  }

  if(raiz->valor == valor)
  {
    return raiz;
  }

  if(raiz->valor < valor)
    return pesquisarNo(raiz->dir, valor);
  else
    return pesquisarNo(raiz->esq, valor);
}

// Função para realizar a remoção de nós em uma árvore AVL
No* removerNo(No *raiz, int valor)
{
  // Condição de parada que garante que a recursividade funcione
  if(raiz == NULL)
  {
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
        No *temp; // Ponteiro temporário para auxiliar as trocas
        // remove nós que possuem apenas um filhos
        if(raiz->esq == NULL || raiz->dir == NULL)
        { 
          if(raiz->esq != NULL)
            temp = raiz->esq;
          else
            temp = raiz->dir;
          
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
          raiz->esq = removerNo(raiz->esq, temp->valor);
          return raiz;
        }
      }
    }
    // busca pelo nó a ser deletado
    else
    {
      if(raiz->valor > valor)
        raiz->esq = removerNo(raiz->esq, valor);
      else
        raiz->dir = removerNo(raiz->dir, valor);

      // atualiza a altura do nó raiz da árvore após a remoção
      atualizarAltura(raiz);
      
      // Realiza o balanceamento da árvore a partir dos nós chamados na função recursiva
      raiz = balancear(raiz);

      return raiz;
    }
  }
}
