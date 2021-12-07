#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#define MAX 50


int matrix[MAX][MAX];
int sol[MAX];

int n; // numero de colunas da matriz
int m; // numero de linhas da matriz
int maxComprimento; // numero maximo do comprimento das pecas
int compPecas[MAX]; // comp em metros das pecas a cortar
int qtdPecas[MAX]; //

void load(){
	n = 3;
	m = 3;
	maxComprimento = 9;
	compPecas[0] = 2;
	compPecas[1] = 3;
	compPecas[2] = 4;

	qtdPecas[0] = 20;
	qtdPecas[1] = 10;
	qtdPecas[2] = 20;

}

//fazer print da matriz
void printMatrix(){ 
	for(int i = 0 ; i < n ; i++ ){
		for(int l = 0 ; l < m ; l++ ){						
			printf("%d ",matrix[i][l]);
		}		
		printf("\n");	
	}
}

int contaLine(int line){
	int sum = 0;
	
	for(int i = 0; i < m ; i++){
		sum += (compPecas[line] * matrix[i][line]);
	}

	return sum;
}

void matrixRand(){
	srand(time(NULL));

	//atribuir valores random a matriz
	for(int i = 0 ; i < n ; i++ ){
		for(int l = 0 ; l < m ; l++ ){
			if((i == l)){
				matrix[i][l] = 1 + rand()%(maxComprimento/compPecas[i]); // 1 + rand()%(MaximoComprimento/Comprimentos[i])
			}else{
				matrix[i][l] = 0 + rand()%(maxComprimento/compPecas[i]);
			}
		}		
	}
}

//fazer o check da matriz se e menor q o maxComprimento
void initializeMatrix(){
	for (int i = 0; i < n; i++){
		do{
			matrixRand();
		} while ((contaLine(i) > maxComprimento));
	}
}

//para fazer a solucao to do
void getSol(){
	srand(time(NULL));
	for(int i=0 ; i < n ; i++){
		sol[i] = 1 + rand()%(maxComprimento+1);
	}
}

int checkSol(int col){

	//refazer
	int sum = 0;

	for(int i ; i < n ; i++){
		sum += (matrix[col][i]*sol[i]);
	}

	// sum = (matrix[n][0]*sol[0]) + (matrix[n][1]*sol[1]) + (matrix[n][2]*sol[2]);

	return sum;
}

int main(){
	// para determinar o tempo q demorou
	struct timeval tvi, tvf, tvres;
	gettimeofday(&tvi,NULL);
	for(int i=0; i<10000000; i++);	
	
	load();

	initializeMatrix();
	
	printf("Matriz\n");
	printMatrix(matrix);


	printf("\nSolucao\n");
	
	//for(int i=0;i<n;i++){
	//	printf("%d ",sol[i]);
	//}
	
	printf("\n\nConta Solucao\n");
	//printf("%d %d %d\n\n",checkSol(0),checkSol(1),checkSol(2));

	gettimeofday(&tvf,NULL);
	timersub(&tvf,&tvi,&tvres);
	printf("Time = %4ld.%03ld\n", (long)tvres.tv_sec,(long)tvres.tv_usec/1000);
	return 0;
}
