#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

unsigned int primo (unsigned int N){
  int div = 0;
  for (unsigned int i = 1; i <= N; i++) {
    if (div > 3){
      break;
    }
    if (N % i == 0) { 
     div++;
     //printf("Resto de %d por %d: %d\n", N, i, N%i);
    }
  }
  //printf("Número dividido: %d\n", N);
  if (div == 2){
    div = 1;
    //printf("É ÍMPAR\n");
  }
  else{
    div = 0;
    //printf("É PAR\n");
  }
  //printf("Div: %d\n", div);
  return div;
}

int main() {  

  unsigned int vetor_numeros[64];
  char c;
  int numeros= 0;

  for(int b = 0; b < 64; b++)
  {
    vetor_numeros[b] = '\0';
  }
  
  do{
    c = scanf("%d", &vetor_numeros[numeros]);
    //printf ("%d\n",vetor_numeros[numeros]);
    numeros ++;
  } while( (c != EOF) && (c != '\n') );
  
  //printf("numeros : %d\n", numeros);
  //printf("ultimo: %d, penultimo: %d, antepenultimo: %d\n", vetor_numeros[numeros], vetor_numeros[numeros-1], vetor_numeros[numeros-2]);

  pid_t filho[3];

  int protection = PROT_READ | PROT_WRITE;
  int visibility = MAP_SHARED | MAP_ANON;
  
  
  int *j;
  int *qntd_primos;

  qntd_primos = (int*) mmap(NULL, sizeof(int), protection, visibility, 0, 0);
  j = (int*) mmap(NULL, sizeof(int), protection, visibility, 0, 0);

  (*qntd_primos) = 0;
  (*j) = 0;

  for(int i= 0; i < 3 ; i++){
    filho[i] = fork();
    //printf("filho%d\n",i);
    if (filho[i]==0) {
      for( int k = i ; k  < numeros; k += 3 ){
        //printf("k: %d\n", k); 
        //printf("Processo Filho%d\n", i);
        //printf("Indice numero tratado: %d\n", k);
        (*qntd_primos) += primo( vetor_numeros[k] );
      /*  if ( k+i == numeros-3 ){
          printf("Ultimo processo %d: %d\n", i, vetor_numeros[k]);
          printf("k = %d, i = %d\n", k, i);
        }  */
        //printf("Qntd primos: %d\n", *qntd_primos);
        //printf("K%d é : %d\n", i, k);
      }
      exit(0);
    }
  }

  //printf("Qntd parcial_depois_Pai: %d\n", *qntd_primos);
  for (int k=0; k<4; k++){
    waitpid (filho[k], NULL, 0);
    //printf("Encerrou filho %d\n", k);
  }

  //printf("Todos os filhos terminaram! Saindo...\n");
  printf("%d\n", *qntd_primos);
  return 0;
}
