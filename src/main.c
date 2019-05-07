#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

#define NMAX         64
#define N_PROCESSOS  3


unsigned int verifica_primo (unsigned int N);

int main() {  
  unsigned int vetor_numeros[NMAX];
  char c;                           /*auxilia na leitura da entrada*/
  int qntd_numeros = 0;

  for(int b = 0; b < NMAX; b++) {   /*forma de 'zerar' meu vetor*/
    vetor_numeros[b] = '\0';
  }
  
  do {
    c = scanf("%d", &vetor_numeros[qntd_numeros]);
    qntd_numeros ++;
  } while( (c != EOF) && (c != '\n') );

  pid_t filho[N_PROCESSOS];

  int protection = PROT_READ | PROT_WRITE;
  int visibility = MAP_SHARED | MAP_ANON;
  
  int *qntd_primos;

  qntd_primos = (int*) mmap(NULL, sizeof(int), protection, visibility, 0, 0);

  (*qntd_primos) = 0;

  for(int i= 0; i < N_PROCESSOS ; i++){
    filho[i] = fork();
    if (filho[i]==0) {
      
      /* Processo filho(i) Opera */ 
      
      for( int k = i ; k  < qntd_numeros ; k += N_PROCESSOS ){
        (*qntd_primos) += verifica_primo( vetor_numeros[k] );
      }
      exit(0);
    }
  }

  /* Processo Pai */
  for (int k = 0; k < N_PROCESSOS; k++){
    waitpid (filho[k], NULL, 0);
  }

  printf("%d\n", *qntd_primos);

  return 0;
}


unsigned int verifica_primo (unsigned int N){
  int divisores = 0;
  
  for (unsigned int i = 1; i <= N; i++) {
    if (divisores > N_PROCESSOS){
      break;
    }

    if (N % i == 0) { 
     divisores++;
    }
  }

  if (divisores == 2){
    divisores = 1;
  }

  else {
    divisores = 0;
  }

  return divisores;
}