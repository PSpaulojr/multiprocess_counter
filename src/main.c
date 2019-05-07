#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

unsigned int primo (unsigned int );

int main() {  

  unsigned int vetor_numeros[64];
  char c;
  int numeros= 0;

  for(int b = 0; b < 64; b++){
    vetor_numeros[b] = '\0';
  }
  
  do{
    c = scanf("%d", &vetor_numeros[numeros]);
    numeros ++;
  } while( (c != EOF) && (c != '\n') );

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
    if (filho[i]==0) {
      for( int k = i ; k  < numeros; k += 3 ){
        (*qntd_primos) += primo( vetor_numeros[k] );
      }
      exit(0);
    }
  }

  for (int k = 0; k < 3; k++){
    waitpid (filho[k], NULL, 0);
  }
  printf("%d\n", *qntd_primos);

  return 0;
}


unsigned int primo (unsigned int N){
  int div = 0;
  for (unsigned int i = 1; i <= N; i++) {
    if (div > 3){
      break;
    }
    if (N % i == 0) { 
     div++;
    }
  }
  if (div == 2){
    div = 1;
  }
  else{
    div = 0;
  }
  return div;
}