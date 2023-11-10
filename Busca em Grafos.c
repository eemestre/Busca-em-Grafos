#include <stdlib.h>
#include <stdio.h>
#define true 1
#define false 0
typedef int bool;
typedef int TIPOPESO;

typedef struct adjacencia{
    int vertice;
    TIPOPESO peso;
    struct adjacencia *prox;
}ADJACENCIA;

typedef struct vertice{
    ADJACENCIA *cab;
}VERTICE;

typedef struct grafo{
    int vertices;
    int arestas;
    VERTICE *adj;
}GRAFO;

GRAFO *criaGrafo (int v){
    int i;

    GRAFO *g = (GRAFO *)malloc(sizeof(GRAFO));
    g -> vertices = v;
    g -> arestas = 0;
    g -> adj = (VERTICE *)malloc(v*sizeof(VERTICE));

    for (i=0; i<v;i++){
        g -> adj[i].cab=NULL;
    }
    return (g);
}

ADJACENCIA *criadAdj(int v, int peso){
    ADJACENCIA *temp = (ADJACENCIA *)malloc(sizeof(ADJACENCIA));
    temp -> vertice =v;
    temp -> peso = peso;
    temp -> prox = NULL;
    return(temp);
}

bool criaAresta(GRAFO *gr, int vi, int vf, TIPOPESO p){
    if(!gr) return(false);
    if((vf<0)||(vf>=gr->vertices))return(false);
    if((vi<0)||(vi>=gr->vertices))return(false);
    
    ADJACENCIA *novo = criadAdj(vf,p);
    
    novo->prox = gr->adj[vi].cab;
    gr->adj[vi].cab=novo;
    gr->arestas++;
    return (true);
}

void imprime(GRAFO *gr){
    printf("Vertices: %d. Arestas: %d \n",gr->vertices,gr->arestas);
    int i;
    
    for(i = 0; i<gr->vertices; i++){
        printf("v%d:",i+1);
        ADJACENCIA *ad = gr->adj[i].cab;
        while (ad){
            printf("v%d(%d) ",ad->vertice+1,ad->peso);
            ad=ad->prox;
        }
        printf("\n");
    }
}

void matriz(GRAFO *gr){ 
    int i;
    int j;
    for(i = 0; i < gr->vertices; i++) {
        for (j = 0; j < gr->vertices; j++) {
            int adjacencia = 0;
            ADJACENCIA *ad = gr->adj[i].cab;
            while(ad) {
                if(ad->vertice == j) {
                    adjacencia = ad->peso;
                }
                ad = ad->prox;
            }
            printf("%d\t", adjacencia);
        }
        printf("\n");
    }
}

int* buscaEmProfundidadeRecursivo(GRAFO *gr, int inicio, int destino, int *visitados, int *caminho, int contagem) {
    if(inicio == destino) {
        caminho[contagem] = inicio;
        return caminho;
    }

    printf("v%d -> ", inicio);
    visitados[inicio] = 1;
    caminho[contagem] = inicio;
    ADJACENCIA *ad = gr->adj[inicio].cab;

    while(ad) {
        int proximoVertice = ad->vertice;
        if(!visitados[proximoVertice]) {
            caminho = buscaEmProfundidadeRecursivo(gr, proximoVertice, destino, visitados, caminho, contagem+1);
            if(caminho[contagem+1] > -1) {
                return caminho;
            }
        }
        ad = ad->prox;
    }

    visitados[inicio] = 0;
    caminho[contagem] = -1;
    return caminho;
}

int* buscaEmProfundidade(GRAFO *gr, int inicio, int destino) {
    int *visitados = (int*)malloc(gr->vertices * sizeof(int));
    int i;
    for (i = 0; i < gr->vertices; i++) {
        visitados[i] = 0;
    }
    
    int *caminho = (int*)malloc(gr->arestas * sizeof(int));
    for(i = 0; i < gr->arestas; i++) {
        caminho[i] = -1;
    }

    caminho = buscaEmProfundidadeRecursivo(gr, inicio, destino, visitados, caminho, 0);
    printf("v%d\n", destino);
    free(visitados);
    return caminho;
}

int found(int inicio, int destino) {
    if(inicio == destino) {
        return 1;
    } else {
        return 0;
    }
}

int* buscaEmLarguraRecursivo(GRAFO *gr, int inicio, int destino, int *visitados, int *caminho, int contagem, int* cabecas, int contagemCabeca) {
    visitados[inicio] = 1;
    caminho[contagem] = inicio;
    ADJACENCIA *ad = gr->adj[inicio].cab;
    cabecas[0] = inicio;
    
    while(ad != NULL) {
        int proximoVertice = ad->vertice;
        if(!visitados[proximoVertice]) {
            int achou = found(proximoVertice, destino);
            if(achou) {
                caminho[contagem+1] = proximoVertice;
                return caminho;
            }
            cabecas[contagemCabeca] = proximoVertice;
            contagemCabeca++;
            ad = ad->prox;
        }
    }

    int i;
    for(i = 0; i < contagemCabeca-1; i++) {
        cabecas[i] = cabecas[i+1];
    }
    contagemCabeca--;

    caminho = buscaEmLarguraRecursivo(gr, cabecas[0], destino, visitados, caminho, contagem+1, cabecas, contagemCabeca);

    return caminho;
}

int* buscaEmLargura(GRAFO *gr, int inicio, int destino) {
    int *visitados = (int*)malloc(gr->vertices * sizeof(int));
    int i;
    for (i = 0; i < gr->vertices; i++) {
        visitados[i] = 0;
    }
    
    int *caminho = (int*)malloc(gr->arestas * sizeof(int) * 2);
    for(i = 0; i < gr->arestas; i++) {
        caminho[i] = -1;
    }

    int* cabecas = (int*)malloc(sizeof(int)*gr->vertices);
    for(i = 0; i < gr->vertices; i++) {
        cabecas[i] = -1;
    }
    caminho = buscaEmLarguraRecursivo(gr, inicio, destino, visitados, caminho, 0, cabecas, 1);
    free(visitados);
    free(cabecas);
    return caminho;
}

void imprimeCaminho(GRAFO *graf, int *caminho) {
    int i;
    for(i = 0; i < graf->arestas-1; i++) {
        if(caminho[i + 1] == -1) {
            break;
        }
        printf("v%d -> ", caminho[i]);
    }

    printf("v%d", caminho[i]);
}

int main (){
    GRAFO *graf = criaGrafo(15);

    criaAresta(graf, 0, 1, 0); 
    criaAresta(graf, 0, 2, 0);
    criaAresta(graf, 1, 3, 0);
    criaAresta(graf, 1, 4, 0);
    criaAresta(graf, 1, 5, 0);
    criaAresta(graf, 3, 6, 0);
    criaAresta(graf, 3, 7, 0);
    criaAresta(graf, 5, 8, 0);
    criaAresta(graf, 5, 9, 0);
    criaAresta(graf, 7, 10, 0);
    criaAresta(graf, 7, 11, 0);
    criaAresta(graf, 7, 12, 0);
    criaAresta(graf, 9, 13, 0);
    criaAresta(graf, 9, 14, 0);

    printf("\nLista de Adjacencia Grafo 1:\n");
    imprime(graf);

    printf("\n\nMatriz de Adjacencia Grafo 1:\n");
    matriz(graf);
    
    int* caminho = buscaEmLargura(graf, 0, 14);

    printf("\n\nCaminho percorrido de 0 a 14 pela busca em largura:\n");
    imprimeCaminho(graf, caminho);

    printf("\n\n=============================================");

    GRAFO *graf2 = criaGrafo(10);

    criaAresta(graf2, 0, 1, 0); 
    criaAresta(graf2, 1, 2, 0);
    criaAresta(graf2, 1, 4, 0);
    criaAresta(graf2, 2, 3, 0);
    criaAresta(graf2, 2, 4, 0);
    criaAresta(graf2, 2, 9, 0);
    criaAresta(graf2, 3, 4, 0);
    criaAresta(graf2, 4, 5, 0);
    criaAresta(graf2, 4, 6, 0);
    criaAresta(graf2, 4, 7, 0);
    criaAresta(graf2, 5, 6, 0);
    criaAresta(graf2, 7, 8, 0);
    criaAresta(graf2, 7, 9, 0);

    printf("\nLista de Adjacencia Grafo 2:\n");
    imprime(graf2);

    printf("\n\nMatriz de Adjacencia Grafo 2:\n");
    matriz(graf2);
    
    printf("\n\nCaminho percorrido de 0 a 9 pela busca em profundidade:\n");
    int* caminho2 = buscaEmProfundidade(graf2, 0, 9);

    printf("\n\nCaminho direto de 0 a 9 pela busca em profundidade:\n");
    imprimeCaminho(graf, caminho2);


    return 0;
}

/*

*/
