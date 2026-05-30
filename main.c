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

int lerSimNaoUser() {
  char respostaUsuario;
  scanf(" %c", &respostaUsuario);
  if (respostaUsuario == 'S') {
    return 1;
  } else {
    return 0;
  }
}

void jogar(no *noExecucao) {

  if (ehResposta(noExecucao)) {
    printf("A resposta eh: %s", noExecucao->texto);
    printf("Acertei (S/N)? \n");
    if (lerSimNaoUser()) {
      printf("Incrivel! Vamos para mais uma?");
    } else {
      char novoAnimal[50], novaPergunta[50];
      printf("Qual era o animal correto\n?");
      scanf(" %49[^\n]", novoAnimal);
      printf("Qual a pergunta que distingue esse animal\n?");
      scanf(" %49[^\n]", novaPergunta);

      char antigoAnimal[50];
      strcpy(antigoAnimal, noExecucao->texto);
      strcpy(noExecucao->texto, novaPergunta);
      no *respostaSim = criarNo(novoAnimal);
      no *respostaNao = criarNo(antigoAnimal);
      noExecucao->esquerda = respostaSim;
      noExecucao->direita = respostaNao;
    }

  } else {
    char respostaUsuario;
    printf(" %s", noExecucao->texto);
    scanf(" %c", &respostaUsuario);
    if (respostaUsuario == 'S') {
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
        // CRIAR NOVO NO COM A RESPOSTA CORRETA E A RESPOSTA ERRRADA

  // SE NÃO É FOLHA
    // EXIBE PERGUNTA NO ATUAL
        // SE ENTRADA DO USUARIO FOR SIM OU NÃO
          // JOGAR (PROXIMO NO)
}

int main() {
  int fimJogo = 0;
  char respostaReiniciar;
  no *arvorePrincipal = inicializarJogo();
  while (fimJogo != 1) {
    jogar(arvorePrincipal);
    printf("Quer jogar novamente? (S/N)\n");
    scanf(" %c", &respostaReiniciar);
    if (respostaReiniciar == 'N' || respostaReiniciar == 'n') {
      fimJogo = 1;
    }
  }

  return 0;
}