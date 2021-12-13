#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#define MAX 50
#define MAX_TRY 1000


int matrix[MAX][MAX];
int sol[MAX];

int n = 0; // numero de colunas da matriz
int m = 0; // numero de linhas da matriz
int maxComprimento; // numero maximo do comprimento das pecas
int compPecas[MAX]; // comp em metros das pecas a cortar
int qtdPecas[MAX]; //
int maxes[MAX];
int totalLoss = 0;
int avaliation = 0;
int triesSol = 0;

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
		if(i != line){
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

int max(int a, int b){
	if (a > b)
		return a;
	else
		return b;
	
}

//usado para fazer a conta da solucao
int contaSol(){
	int sum;

	for(int i = 0 ; i < n ; i++){
		sum = 0;
		for (int j = 0; j < m; j++)
		{
			sum += (matrix[i][j] * sol[i]);
		}	
		if(sum < qtdPecas[i])
			return 1;
	}
	return 0;
}

void solRand(){

	int aux;
	int inc;

	for (int i = 0; i < n; i++){
		maxes[i] = 0;
	}
	
	for (int i = 0; i < n; i++){
		for (int j = 0; j < m; j++){
			if(matrix[i][j] != 0){
				aux = (1.0 * qtdPecas[i]) / matrix[n][m];
				if (aux - (int) aux > 0){
					inc = 1;
				}else{
					inc = 0;
				}
				maxes[j] = max( (qtdPecas[i] / matrix[i][j]) + inc, maxes[j]);
				
			}
		}
		
	}
	do
	{
		for (int i = 0; i < n; i++)
		{
			sol[i] = rand() % (maxes[i] + 1);
		}
		triesSol++;
	} while (contaSol() == 1 && triesSol < MAX_TRY);

	if(triesSol == MAX_TRY){
		for (int i = 0; i < n; i++)
		{
			sol[i] = maxes[i];
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

void getAvaliation(){
	avaliation = totalLoss;
	for (int i = 0; i < n; i++)
	{
		avaliation += sol[i];
	}
}

void optimizeSol(){
	int avaliationTemp;
		
		do{
			solRand();
			avaliationTemp = totalLoss;
			for (int i = 0; i < n; i++){;
				avaliationTemp += sol[i];
			}
		}while(avaliationTemp >= avaliation);
}

int main(int argc, char **argv){
	readFile(argc, argv);
	srand(time(NULL));

	// para determinar o tempo q demorou
	struct timeval tvi, tvf, tvres;
	gettimeofday(&tvi,NULL);
	for(int i=0; i<10000000; i++);	
	
	initializeMatrix();

	printf("Matriz\n");
	printMatrix(matrix);
	printf("Resultado conta\n");
	for (int i = 0; i < n; i++)
	{
		printf("%d ",contaLine(i));
	}

	printf("\nSolucao\n");
	solRand();
	printSol();
	calcLosses();
	printf("\nTotal Loss: %d", totalLoss);

	getAvaliation();
	printf("\nAvaliation: %d", avaliation);

	gettimeofday(&tvf,NULL);
	timersub(&tvf,&tvi,&tvres);
	printf("\nTime = %4ld.%03ld\n", (long)tvres.tv_sec,(long)tvres.tv_usec/1000);
	return 0;
}
