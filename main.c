#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double definefun (double k,char fun);
void LU (double a[][10],int num3,double *v, int kl);
void matrizA (double *x,char *fun,double *f,int num2, int num3, int kl);
double soma1 (double *v,double l[][10],int i);
double soma2 (double *so1,double u[][10],double *so2,int i,int num3);
void resolucao (int num3,double *v,double l[][10], double u[][10], int kl);

double definefun (double k,char fun){
    double x;
    if(fun=='a'){
        x = sin(k);
    }
    if(fun=='b'){
        x = cos(k);
    }
    if(fun=='c'){
        x = log(k);
    }
    if(fun=='d'){
        x = exp(k);
    }
    if(fun=='e'){
        x = pow(k,5);
    }
    if(fun=='f'){
        x = pow(k,4);
    }
    if(fun=='g'){
        x = pow(k,3);
    }
    if(fun=='h'){
        x = pow(k,2);
    }
    if(fun=='i'){
        x = k;
    }
    if(fun=='j'){
        x = 1;
    }
    return x;
}

double soma1 (double *so1,double l[][10],int i){
    int j;
    double x;
    x = 0;
    for(j=0;j<i;j++){
        x = x + (so1[j]*l[(i)][j]);
    }
    return x;
}

double soma2 (double *so1,double u[][10],double *so2,int i,int num3){
    int j;
    double x;
    x = 0;
    for(j=i+1;j<num3;j++){
        x = x + (so2[j]*u[i][j]);
    }
    return x;
}

void resolucao (int num3, double *v, double l[][10], double u[][10], int kl){
    FILE *saida;
    int i;
    double so1[10], so2[10], k[10];
    i=0;
    saida = fopen("resultados.txt", "a");
    for(i=0;i<num3;i++){
        if(i==0){
            so1[0]=v[0];
        }else{
            so1[i]=v[i]-soma1(so1,l,i);
        }
    }
    for(i=(num3-1);i>-1;i--){
        if(i==(num3-1)){
            so2[(i)] = so1[(i)]/u[(i)][(i)];
        }else{
            k[i] = so1[i] - soma2(so1,u,so2,i,num3);
            so2[i]=k[i]/u[i][i];
        }
    }
    if(saida == NULL){
        printf("Erro ao criar arquivo.");
    }
    else{
        fprintf(saida, "Caso %d:\n      ", (kl+1));
        for(i=0;i<num3;i++){
            fprintf(saida, "%.5lf ",so2[i]);
        }
        fprintf(saida, "\n");
    }
    fclose(saida);
}

void LU (double a[][10], int num3, double *v, int kl){
    int i, j, k;
    double l[10][10], u[10][10];
    double con;
    for(k=0;k<num3;k++){
        u[0][k]=a[0][k];
    }
    for(k=0;k<(num3-1);k++){
        for(i=k+1;i<num3;i++){
            if(k==0){
                con = (a[i][k]/a[k][k]);
            }
            else{
                con = (u[i][k]/u[k][k]);
            }
            for(j=k;j<num3;j++){
                if(k==0){
                    u[i][j]=a[i][j]-((con)*u[k][j]);
                }else{
                    u[i][j]=u[i][j]-((con)*u[k][j]);
                }
            }
            l[i][k] = con;
        }
    }
    for(i=0;i<num3;i++){
        for(j=0;j<num3;j++){
            if(i==j){
                l[i][j] = 1;
            }
            if(i<j){
                l[i][j] = 0;
            }
        }
    }
    resolucao(num3,v,l,u,kl);
}

void matrizA (double *x,char *fun,double *f,int num2, int num3, int kl){
    int i, j, z;
    double a[10][10], v[10];
    for(i=0;i<num3;i++){
        for(j=0;j<num3;j++){
            a[i][j]=0;
        }
    }
    for(i=0;i<num3;i++){
        for(j=0;j<num3;j++){
            for(z=0;z<num2;z++){
                a[i][j]=a[i][j]+(definefun(x[z],fun[i]))*(definefun(x[z],fun[j]));
            }
        }
    }
    for(i=0;i<num3;i++){
        v[i]=0;
    }
    for(i=0;i<num3;i++){
        for(j=0;j<num2;j++){
            v[i]=v[i]+(definefun(x[j],fun[i])*f[j]);
        }
    }
    LU(a,num3,v,kl);
}

int main(){
    FILE *entrada;
    int num1, num2, num3, kl, w;
    char fun[10];
    double* x; double* f;
    kl=0;
    w=0;
    entrada=fopen("tabela.txt","r");
    if(entrada==NULL){
        printf("Erro ao abrir o arquivo!\n");
    }
    fscanf(entrada,"%d\n",&num1);
    while(kl<num1){
        fscanf(entrada,"%d\n",&num2);
        x = (double* ) malloc(sizeof(double) * num2);
        f = (double* ) malloc(sizeof(double) * num2);
        while(w<(num2-1)){
            fscanf(entrada,"%lf, %lf; ",&x[w], &f[w]);
            w++;
        }
        fscanf(entrada,"%lf, %lf\n",&x[w], &f[w]);
        fscanf(entrada,"%d\n",&num3);
        w=0;
        while(w<(num3-1)){
            fscanf(entrada,"%c+",&fun[w]);
            w++;
        }
        fscanf(entrada,"%c\n",&fun[w]);
        matrizA(x,fun,f,num2,num3,kl);
        kl++;
        w=0;
    }
    free(x);
    free(f);
    fclose(entrada);
    return 0;
}
