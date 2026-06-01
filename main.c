#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct no {
  char texto[50];      // PERGUNTA OU RESPOSTA
  struct no *esquerda; // SIM
  struct no *direita;  // NÃO
} no;

no *criarNo(char *texto) {
  no *novoNo = (no *)malloc(sizeof(no));
  if (!novoNo) {
    printf("Erro ao alocar memoria!\n");
    exit(1);
  }

  strcpy(novoNo->texto, texto);
  novoNo->direita = NULL;
  novoNo->esquerda = NULL;
  return novoNo;
}

no *inicializarJogo() {
  no *raiz = criarNo("Eh mamifero");
  raiz->esquerda = criarNo("Cachorro");
  raiz->direita = criarNo("Pinguim");
  return raiz;
}

int ehResposta(no *verificar) {
  if (verificar->direita == NULL && verificar->esquerda == NULL) {
    return 1;
  }
  return 0;
}

// Substituido scanf por fgets pois o \n residual no buffer causava
// leitura duplicada no menu principal, imprimindo ele duas vezes
int lerSimNaoUser() {
  char buf[8];
  fgets(buf, sizeof(buf), stdin);
  if (buf[0] == 'S' || buf[0] == 's') {
    return 1;
  } else {
    return 0;
  }
}

void exibirArvore(no *raiz, int nivel) {
  if (raiz == NULL) return;
  exibirArvore(raiz->direita, nivel + 1);
  for (int i = 0; i < nivel; i++) printf("    ");
  printf("[%s] %s\n", ehResposta(raiz) ? "R" : "P", raiz->texto);
  exibirArvore(raiz->esquerda, nivel + 1);
}
 
void salvarNo(no *raiz, FILE *f) {
  if (raiz == NULL) {
    fprintf(f, "#\n");
    return;
  }
  fprintf(f, "%s\n", raiz->texto);
  salvarNo(raiz->esquerda, f);
  salvarNo(raiz->direita, f);
}
 
no *carregarNo(FILE *f) {
  char linha[50];
  if (!fgets(linha, 50, f)) return NULL;
  linha[strcspn(linha, "\n")] = '\0';
  if (strcmp(linha, "#") == 0) return NULL;
  no *novo = criarNo(linha);
  novo->esquerda = carregarNo(f);
  novo->direita = carregarNo(f);
  return novo;
}

void jogar(no *noExecucao) {

  if (ehResposta(noExecucao)) {
    printf("A resposta eh: %s", noExecucao->texto);
    printf("\n\nAcertei (S/N)? \n");
    if (lerSimNaoUser()) {
      printf("Incrivel! Vamos para mais uma?");
    } else {
      char novoAnimal[50], novaPergunta[50];

      // Mesmo problema: scanf deixava \n no buffer apos a leitura,
      // fgets consome a linha inteira e strcspn remove o \n da string
      printf("Qual era o animal correto?\n");
      fgets(novoAnimal, 50, stdin);
      novoAnimal[strcspn(novoAnimal, "\n")] = '\0';
      printf("Qual a pergunta que distingue esse animal?\n");
      fgets(novaPergunta, 50, stdin);
      novaPergunta[strcspn(novaPergunta, "\n")] = '\0';

      char antigoAnimal[50];
      strcpy(antigoAnimal, noExecucao->texto);
      strcpy(noExecucao->texto, novaPergunta);
      no *respostaSim = criarNo(novoAnimal);
      no *respostaNao = criarNo(antigoAnimal);
      noExecucao->esquerda = respostaSim;
      noExecucao->direita = respostaNao;
    }

  } else {
    printf("%s (S/N)\n", noExecucao->texto);
    if (lerSimNaoUser()) {
      jogar(noExecucao->esquerda);
    } else {
      jogar(noExecucao->direita);
    }
  }
  // SE É FOLHA
    // EXIBE RESPOSTAS
    // PERGUNTA SE ACERTOU
    // SE SIM
        // VOLTA AO MENU
    // SE NÃO
        // PERGUNTA QUAL ERA A RESPOSTA CORRETA E QUAL RESPECTIVA PERGUNTA
        // TRANSFORMAR O NO ATUAL EM PERGUNTA
        // CRIAR NOVO NO COM A RESPOSTA CORRETA E A RESPOSTA ERRADA

  // SE NÃO É FOLHA
    // EXIBE PERGUNTA NO ATUAL
        // SE ENTRADA DO USUARIO FOR SIM OU NÃO
          // JOGAR (PROXIMO NO)
}

int main() {
  int fimJogo = 0;
  int opcao;
  no *arvorePrincipal;
 
  FILE *f = fopen("arvore.txt", "r");
  if (f) {
    arvorePrincipal = carregarNo(f);
    fclose(f);
    printf("Arvore carregada!\n");
  } else {
    arvorePrincipal = inicializarJogo();
  }
 
  while (fimJogo != 1) {
    printf("\n1 - Jogar\n2 - Exibir arvore\n3 - Encerrar\n");
    char buf[8];
    fgets(buf, sizeof(buf), stdin);
    opcao = atoi(buf);
 
    if (opcao == 1) {
      jogar(arvorePrincipal);
    } else if (opcao == 2) {
      exibirArvore(arvorePrincipal, 0);
    } else if (opcao == 3) {
      FILE *fs = fopen("arvore.txt", "w");
      if (fs) {
        salvarNo(arvorePrincipal, fs);
        fclose(fs);
        printf("Arvore salva!\n");
      }
      fimJogo = 1;
    }
  }
 
  return 0;
}