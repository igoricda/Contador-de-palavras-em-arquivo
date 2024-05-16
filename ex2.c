/*
Exercicio 2: Contador de palavras
Igor Correa Domingues de Almeida
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

#define TAM_PALAVRA 30

// define o tipo de dado a ser colocado na lista
typedef struct {
  char palavra[TAM_PALAVRA]; // Palavra
  int apps;                  // Número de aparições da palavra
} Tdado;

typedef struct TreeNode {
  Tdado info;             // dado armazenado no nó
  struct TreeNode *left;  // ponteiro para o ramo esquerdo
  struct TreeNode *right; // ponteiro para o nramo direito
} TreeNode;

//Funcao para liberar a arvore
void freeBinTree(TreeNode *bt) {
  if (!bt) {
    return;
  }
  freeBinTree(bt->left);
  freeBinTree(bt->right);
  free(bt);
  bt = NULL;
}

//--------------------
// função de inserção:
//--------------------
TreeNode *insertBinTree(char pal[], TreeNode *bt) {
  TreeNode *aux = bt;
  int i;
  // verificar se o ponteiro é nulo
  if (!bt) {
    // insere o novo nó
    aux = (TreeNode *)malloc(sizeof(TreeNode));
    if (!aux) {
      printf("Erro: Memória insuficiente!\n");
      return bt;
    } else { //Set aux->info
      strcpy(aux->info.palavra, pal); 
      aux->info.apps = 1;
      aux->left = NULL;
      aux->right = NULL;
      return aux;
    }
  } else {
    if ((i = strcasecmp(aux->info.palavra, pal)) == 0) { //Se as palavras sao iguais
      aux->info.apps++; // Aumentar as aparicoes da palavra
      return bt;
    } else { //Senao
      if (i > 0) {
        aux->left = insertBinTree(pal, aux->left); //Ir para a esquerda
        return bt;
      } else {
        aux->right = insertBinTree(pal, aux->right); // Ir para a direita
        return bt;
      }
    }
  }
}

/***************************************************
 * emordem:: Percorre a árvore em ordem
 ***************************************************/
void emOrdem(TreeNode *bt, FILE *f, int total) {
  int c = 0;
  if (bt == NULL)
    return;
  emOrdem(bt->left, f, total);

  /* Calcula o tamanho real da palavra considerando que letras com acento contam como 2 caracteres
  */
  int tamanho_palavra = strlen(bt->info.palavra);
  for (int i = 0; i < tamanho_palavra; i++) {
    if ((char)bt->info.palavra[i] == (-61)) { // Verifica se o caractere é acentuado
      tamanho_palavra-=2;
      c = 1;
      /*
      -61 é um sinalizador de acento do C
      char a[2] = "É";
      printf("%d", a[0]); retorna -61, com á, é, ã, ó, etc
      */
    }
  }

  // Imprime a palavra e os valores correspondentes alinhados corretamente
  if(c){
  fprintf(f, "%-s", bt->info.palavra); //Print na string

  for(int i = 0; i < 15 - tamanho_palavra; i++)
    fprintf(f, " "); //Print em espaços para alinhar
  fprintf(f, "| %-11d | %6.2lf%%    \n", bt->info.apps, (double)bt->info.apps / total * 100); //Resto das infos
  }
  else{
    fprintf(f, "%-15s | %-11d | %6.2lf%%    \n", bt->info.palavra, bt->info.apps, (double)bt->info.apps / total * 100); //Print em palavras sem acento
  }
  c = 0;

  emOrdem(bt->right, f, total);
}


//Funcao main
int main(void) {
  FILE *fIn, *fOut; // Arquivos de entrada e saida
  char filenameIn[31], palavra[TAM_PALAVRA];     // Nome do arquivo de entrada e string para operacoes
  int total = 0; // total de palavras
  TreeNode *bt = NULL; //Inicializar a arvore

  printf("Escreva o nome do arquivo de entrada:\n");
  scanf("%s", filenameIn); // Pedir o nome do arquivo de entrada

  fIn = fopen(filenameIn, "r"); // Abrir o arquivo de entrada em r
  fOut = fopen("out.txt", "w"); // Abrir o arquivo de saida em w

  // Testes de erros
  if (!fOut) {
    printf("Erro: Arquivo out.txt não pode ser aberto.\n");
    return 1;
  }
  if (!fIn) {
    printf("Erro: Arquivo %s não pode ser aberto.\n", filenameIn);
    return 1;
  }

  //Enquanto existem strings para ler
  while (fscanf(fIn, "%s", palavra) == 1) {
    //Deixar palavras no padrao Zzzzz
    palavra[0] = toupper(palavra[0]);
    for(int i = 1; i < strlen(palavra);i++){
      palavra[i] = tolower(palavra[i]);
    }

  /*-61 = sinalizador de acento do C
  Retirar do fim da palavra sinais como :,; etc
  Trocando-os pelo terminador de string '/0'*/
    if(!((palavra[strlen(palavra) - 1] >= '0'&& palavra[strlen(palavra) - 1] <= '9') || (palavra[strlen(palavra) - 1] >= 'A'&& palavra[strlen(palavra) - 1] <= 'Z') || (palavra[strlen(palavra) - 1] >= 'a'&& palavra[strlen(palavra) - 1] <= 'z') || palavra[strlen(palavra) - 2] == -61)) 
      palavra[strlen(palavra) - 1] = '\0';

    bt = insertBinTree(palavra, bt); //Inserir as palavras na arvore
    total++; //Incrementar o total
  }

  // Cabeçalho a ser impresso no arquivo de saida
  fprintf(fOut, "Programa: Contador de palavras\n");
  fprintf(fOut, "Arquivo: %s\n\n", filenameIn);
  fprintf(fOut, "Contagem de palavras:\n\n");
  fprintf(fOut, "    %s    | %-11s | %-10s\n", "Palavras", "Ocorrências", "Frequência");
  fprintf(fOut, "----------------+-------------+------------\n");

  emOrdem(bt, fOut, total); //Imprimir as colunas
  // Print do final
  fprintf(fOut, "----------------+-------------+------------\nTotal           | %-6d car. | 100.00%%\n", total);

  // Fechar os arquivos e limpar a arvore
  fclose(fIn);
  fclose(fOut);
  freeBinTree(bt);

  // Indicador do arquivo da saída
  printf("A saída está em out.txt\n");
  return 0;
}

