#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#define MAX 50


int matrix[MAX][MAX];
int sol[MAX];

int n = 0; // numero de colunas da matriz
int m = 0; // numero de linhas da matriz
int maxComprimento; // numero maximo do comprimento das pecas
int compPecas[MAX]; // comp em metros das pecas a cortar
int qtdPecas[MAX]; //
int totalLoss = 0;

int processNumber = 0;
int executionTime = 0;
char filename[64] = "";
char line[256];
int lineCount = 0;
FILE *file;

//Função auxiliar para remover os espaços e guardar os dados nos respetivos arrays
void splitLine(int arr[], char line[]){
	int i = 0;
	char delim[2] = " ";
	char *token;

	token = strtok(line, delim);
	while(token != NULL){
		if(i < n){
			arr[i] = atoi(token);
		}

		i++;
		token = strtok(NULL, delim);
	}
}

//Função para tratar os argumentos e ler o ficheiro de PCU_Tests
void readFile(int argc, char **argv){
	if(argc == 4){
		for(int i=0; i<argc; i++){
			strcpy(filename, argv[1]);
			processNumber = atoi(argv[2]);
			executionTime = atoi(argv[3]);
		}
	} else {
		printf("Usage: pcu file.txt number_of_processes execution_time\n");
		printf("Example: pcu prob03.txt 10 60\n");
		exit(1);
	}


	if ((file = fopen(filename, "r")) == NULL) {
		printf("Error! File cannot be opened\n");
		exit(1);
	}

	while (fgets(line, sizeof(line), file)) {
		switch(lineCount){
			case 0:
				n = atoi(line);
				break;
			case 1:
				m = atoi(line);
				break;
			case 2:
				maxComprimento = atoi(line);
				break;
			case 3:{
				splitLine(compPecas, line);
				break;
			}
			case 4:
				splitLine(qtdPecas, line);
				break;
			default:
				printf("Error! Too many lines in this file");
		}
		lineCount++;
	}

	fclose(file);
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

//usado para fazer a conta da linha
int contaLine(int line){
	int sum = 0;
	
	for(int i = 0; i < m ; i++){
		sum += (compPecas[i] * matrix[i][line]);
	}

	return sum;
}

//usado para inicializar a matrix 
void matrixRand(){
	srand(time(NULL));
	//atribuir valores random a matriz
	for(int i = 0 ; i < n ; i++ ){
		for(int l = 0 ; l < m ; l++ ){
			if(i == l){
				matrix[i][l] = 1 + rand()%(maxComprimento/compPecas[i]); // 1 + rand()%(MaximoComprimento/Comprimentos[i])
			}else{
				matrix[i][l] = 0 + rand()%(maxComprimento/compPecas[i]);
			}
		}		
	}
}

//usado para alterar apenas valor numa linha
void matrixRandL(int line){
	srand(time(NULL));

	//atribuir valores random a matriz
	for(int i = 0 ; i < n ; i++ ){
		if(i == line){
			//do nothing
		}else{
			matrix[i][line] = 0 + rand()%(maxComprimento/compPecas[i]);
		}		
	}
}

//fazer o check da matriz se e menor q o maxComprimento
void initializeMatrix(){
	matrixRand();
	for (int i = 0; i < n; i++){
		while(contaLine(i) > maxComprimento){
			matrixRandL(i);
		}
	}
}

//para inicializar solucao
void solRand(){
	srand(time(NULL));

	for(int i=0; i < n ; i++){
		sol[i] = 1 + rand()%(maxComprimento+1);
	}
}

void solRandL(int l){
	srand(time(NULL));

	sol[l] = 1 + rand()%(maxComprimento+1);
}

//usado para fazer a conta da solucao
int contaSol(int col){
	int sum = 0;

	for(int i = 0 ; i < n ; i++){
		sum += (matrix[col][i] * sol[i]);
	}

	return sum;
}

//usado para inicializar a solucao
void initializeSol(){
	solRand();
	for (int i = 0; i < n; i++)
	{
		while(contaSol(i)<qtdPecas[i]){
			solRandL(i);
		}
	}
}

void printSol(){
	for(int i=0;i<n;i++){
		printf("%d ",sol[i]);
	}
}

void calcLosses(){
	int sum = 0;
	for (int i = 0; i < m; i++){
		sum = 0;
		for (int j = 0; j < n; j++){
			sum += sol[j] * matrix[i][j];
		}
		sum -= qtdPecas[i];
		sum *= compPecas[i];
		totalLoss += sum;
	}

	for (int j=0; j < n; j++) {
        sum = 0;
        for (int i=0; i < m; i++) {
            sum += matrix[i][j] * compPecas[i];
        }
        totalLoss += sol[j]* (maxComprimento - sum);
    }

}

int main(int argc, char **argv){
	readFile(argc, argv);

	// para determinar o tempo q demorou
	struct timeval tvi, tvf, tvres;
	gettimeofday(&tvi,NULL);
	for(int i=0; i<10000000; i++);	
	
	//load();
	//loadTeste();

	initializeMatrix();
	
	printf("Matriz\n");
	printMatrix(matrix);
	printf("Resultado conta");
	printf("\n%d %d %d",contaLine(0),contaLine(1),contaLine(2));

	printf("\nSolucao\n");
	initializeSol();
	printSol();
    printf("\nResultado conta");
	printf("\n%d %d %d",contaSol(0),contaSol(1),contaSol(2));

	calcLosses();
	printf("\nTotal Loss");
	printf("\n%d",totalLoss);

	gettimeofday(&tvf,NULL);
	timersub(&tvf,&tvi,&tvres);
	printf("\nTime = %4ld.%03ld\n", (long)tvres.tv_sec,(long)tvres.tv_usec/1000);
	return 0;
}
