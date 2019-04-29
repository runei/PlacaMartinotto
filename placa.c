#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define alpha 0.08
#define CICLOS 10000
#define T 2

/***********************************************************************/
void le_dimensoes(char arquivo[50], int *lin, int *col ){
	int l, c;

	FILE *arq = fopen(arquivo,"r");

	if ( arq == NULL ){
		printf("Erro ao abrir o arquivo: %s\n", arquivo);
		exit(0);
	}

	fscanf(arq, "%d %d\n", &l, &c);

	*lin = l;
	*col = c;

	fclose(arq);
}


/***********************************************************************/
float le_matriz( char arquivo[50], float *mat){

	int i, j, l, c;

	FILE *arq = fopen(arquivo,"r");

	if ( arq == NULL ){
		printf("Erro ao abrir o arquivo: %s\n", arquivo);
		exit(0);
	}

	fscanf(arq, "%d %d\n", &l, &c);

	for ( i=0; i<l; i++){
		for ( j=0; j<c; j++){
			fscanf(arq, "%f ", &mat[i*c+j]);
		}
	}

	fclose(arq);

}

/***********************************************************************/
void escreve_matriz (float *mat, int l, int c, int ind){

	int i, j;
	char arquivo[50];

	sprintf(arquivo, "saidas//saida_%05d.txt", ind); 

	FILE *arq = fopen(arquivo,"w");

	if ( arq == NULL ){
		printf("Erro ao abrir o arquivo: %s\n", arquivo);
		exit(0);
	}


	for ( i=0; i<l; i++){
		for ( j=0; j<c; j++){
			fprintf(arq, "%f ",mat[i*c+j]);
		}
		fprintf(arq, "\n");
	}

	fclose(arq);
}
/***********************************************************************/
void calcula (float  *mat_antiga, float *mat_nova, int l, int c){
	int i, j;

	omp_set_num_threads(T);

	#pragma omp parallel for private(j)
		for ( i=1; i<l-1; i++){
			for ( j=1; j<c-1; j++){
				mat_nova[i*c+j] = mat_antiga[i*c+j] + alpha * ( mat_antiga[(i-1)*c+j] + mat_antiga[(i+1)*c+j] + mat_antiga[i*c+(j-1)] + mat_antiga[i*c+(j+1)] - 4 * mat_antiga[i*c+j] );
			}
		}
}

/***********************************************************************/
void copia (float  *mat_antiga, float *mat_nova, int l, int c){
	int i, j;

	omp_set_num_threads(T);

	#pragma omp parallel for private(j)
		for ( i=0; i<l; i++){
			for ( j=0; j<c; j++){
				mat_antiga[i*c+j] = mat_nova[i*c+j];
			}
		}
}


/***********************************************************************/
int main(int argc, char **argv){
	float *mat_nova, *mat_antiga;
	int i, lin, col;

	if ( argc != 2 ){
		printf("./placa  < arquivo entrada >\n");
		exit(0);
	}

	le_dimensoes(argv[1], &lin, &col);

	mat_nova = (float *)malloc(lin*col*sizeof(float));
	mat_antiga = (float *)malloc(lin*col*sizeof(float));

	le_matriz(argv[1],mat_nova);


	for (i=0;i<CICLOS;i++){

		copia(mat_antiga, mat_nova, lin, col);		
		calcula(mat_antiga, mat_nova,lin,col);	
		if ( !(i % 100) ){
			escreve_matriz(mat_nova,lin,col,i);
		}
	}
	
	escreve_matriz(mat_nova,lin,col,i);

}
