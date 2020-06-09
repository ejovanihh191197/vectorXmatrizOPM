#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<math.h>
#include<omp.h>
using namespace std;

void vectorxmatriz(double* vector, double** matriz, double* resultado,int tamanio, int tamanioM);

int getDimension(const char* name,int dim){
    int rows=0;
    int cols=0;
    FILE *archivo;
 	char caracteres[1000];
    char * token;

 	archivo = fopen(name,"r");
 	
 	while (feof(archivo) == 0){
 		fgets(caracteres,100,archivo);
 		token = strtok(caracteres, ",");
        cols=0;
        while( token != NULL ) {
            token = strtok(NULL, ",");
            cols++;
        }
        rows++;
    }

    fclose(archivo);

    if(dim==0){
        return rows;
    }else{
        return cols;
    }
}

double** getMatriz(const char* name){
    int rows=getDimension(name,0);
    int cols=getDimension(name,1);
    int i,j;
    FILE *archivo;
 	double** mat;
 	char caracteres[1000];
    char * token;

 	archivo = fopen(name,"r");
 	
    mat = new double*[rows];
    for(int x=0; x<rows; x++){
        mat[x] = new double[cols];
    }
    i=0;
 	while (feof(archivo) == 0){
 		fgets(caracteres,100,archivo);
 		token = strtok(caracteres, ",");
        j=0;
        while( token != NULL ) {
            mat[i][j] = atof(token);
            //printf("%s ", token );
            token = strtok(NULL, ",");
            j++;
        }    
        i++;        
    }
    fclose(archivo);
    return mat;
}

double* getVector(const char* name){
    int rows=getDimension(name,0);
    int cols=getDimension(name,1);
    int i,j;
    FILE *archivo;
 	double* mat;
 	char caracteres[1000];
    char * token;

 	archivo = fopen(name,"r");
 	
    mat = new double[cols];
    i=0;
 	while (feof(archivo) == 0){
 		fgets(caracteres,100,archivo);
 		token = strtok(caracteres, ",");
        j=0;
        while( token != NULL ) {
            mat[j] = atof(token);
            token = strtok(NULL, ",");
            j++;
        }    
    }
    fclose(archivo);
    return mat;
}

int main(){
    double **M = getMatriz("matriz.txt");
    double *M2 = getVector("vector.txt");
    int rowsM=getDimension("matriz.txt",0);
    int colsM=getDimension("matriz.txt",1);
    int cols=getDimension("vector.txt",1);
    double resul[cols];
    
    omp_set_num_threads(4);
    #pragma omp parallel
    {
        vectorxmatriz(M2,M,resul,cols,rowsM);
    }
    
    cout<<"\n";
    for (int x = 0; x < cols; x++){
        cout<<resul[x]<<"\t";
    }
    cout<<"\n";
    cout<<"\n";
    return 0;
}

void vectorxmatriz(double* vector, double** matriz, double* resultado,int tamanio, int tamanioM){
    int suma =0;
    
    #pragma omp for
    for (int i = 0; i < tamanio; i++){
        for (int j = 0; j < tamanioM; j++){
            suma = suma + matriz[i][j] * vector[j];
        }
        resultado[i] = suma;
        suma=0;
    }  
}