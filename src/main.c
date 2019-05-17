#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

#define NMAX         64
#define N_PROCESSOS  4


unsigned int verifica_primo (unsigned int N);

int main() {  
  unsigned int vetor_numeros[NMAX];
  char c;                           /*auxilia na leitura da entrada*/
  int qntd_numeros = 0;
  int abrir_processos = 0;
  int qntd_primos = 0;

  for(int b = 0; b < NMAX; b++) {   /*forma de 'zerar' meu vetor*/
    vetor_numeros[b] = '\0';
  }
  
  do {
    c = scanf("%d", &vetor_numeros[qntd_numeros]);
    qntd_numeros ++;
  } while( (c != EOF) && (c != '\n') );

  pid_t filho[N_PROCESSOS-1];

  int protection = PROT_READ | PROT_WRITE;
  int visibility = MAP_SHARED | MAP_ANON;
  
  int *primos;

  primos = (int*) mmap(NULL, 64*sizeof(int), protection, visibility, 0, 0);
  
  for(int j; j < NMAX; j++ ) {  /*'zero' meu vetor cujo conteudo de cada indice eh 1 ou 0 (primo ou nao) */
    primos[j] = '\0'; 
  }

  /*Verificacao da qntd de processos filhos a serem criados */

 /* if(qntd_numeros-1 > 3) {
    abrir_processos = N_PROCESSOS-2;
  }
  else if (qntd_numeros-1 == 3) {
    abrir_processos = N_PROCESSOS-3;
  }
  else if (qntd_numeros-1 == 2) {
    abrir_processos = N_PROCESSOS-4;
  }
*/

  if (qntd_numeros > 4){
    abrir_processos = 3;
  }
  else{
    abrir_processos = qntd_numeros-2;
  }

/*Criar processos filhos*/

  for(int i = 0; i < abrir_processos ; i++){
    filho[i] = fork();
    if (filho[i]==0) {
      
      /* Processo filho(i) Opera */ 
      
      for( int k = i ; k  < qntd_numeros ; k += N_PROCESSOS ){
        primos[k+1] += verifica_primo( vetor_numeros[k+1] );
        printf("%d, (%d)\n", vetor_numeros[k+1], primos[k+1] );
      }
      //while(1);
      exit(0);
    }
  }


      /* Processo Pai */

  for(int k = 0 ; k  < qntd_numeros ; k += N_PROCESSOS) {
    primos[k] += verifica_primo( vetor_numeros[k] );
    printf("%d, (%d)\n", vetor_numeros[k], primos[k] );
  }

  for (int k = 0; k < abrir_processos ; k++){
    waitpid (filho[k], NULL, 0);
  }

  for(int j; j < NMAX+1; j++ ){
    qntd_primos += primos[j];
  }

  printf("%d\n", qntd_primos);

  return 0;
}


unsigned int verifica_primo (unsigned int N){
  int divisores = 0;
  
  for (unsigned int i = 1; i <= N; i++) {
    if (divisores > 3){
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
