/* 
 *Feito por 
 *Abram Grossmann 31826131
 *Lucas Quental 31852319 
 *Amilton Costa 31876765
 *Luis Felipe de Oliveira Lacerda e Silva 41922859
 *Guilherme Naboa Ferreira 31876961
 */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <memory.h>


/* 
 * Estrutura de dados para representar grafos
 */
typedef struct aresta{ /* Celula de uma lista de arestas */
	int    nome;
	int    custo;
	struct aresta *prox;
}Aresta;

typedef struct vertice{  /* Cada vertice tem um ponteiro para uma lista de arestas incidentes nele */
	int nome;
	int comp;
	int marc;
	Aresta *prim;
}Vertice;

/*
 * Declaracoes das funcoes para manipulacao de grafos 
 */
void criaGrafo(Vertice **G, int ordem);
void destroiGrafo(Vertice **G, int ordem);
int  acrescentaAresta(Vertice G[], int ordem, int v1, int v2, int custo);
void imprimeGrafo(Vertice G[], int ordem);

int calculaGrauDeVertice(Vertice G[], int ordem, int v);
int calculaGrauMinimo(Vertice G[], int ordem);
int calculaGrauMaximo(Vertice G[], int ordem);
int calculaTamanho(Vertice G[], int ordem);
int eConexo(Vertice G[], int ordem);

/*
 * Criacao de um grafo com ordem predefinida (passada como argumento),
 *   e, inicilamente, sem nenhuma aresta 
 */
void criaGrafo(Vertice **G, int ordem){
	int i;
	*G= (Vertice*) malloc(sizeof(Vertice)*ordem); /* Alcacao dinamica de um vetor de vertices */
	
	for(i=0; i<ordem; i++){
		(*G)[i].nome= i;
		(*G)[i].prim= NULL;    /* Cada vertice sem nenua aresta incidente */
	}
}

/*
 * Deasaloca a memoria dinamica usada para armazenar um grafo.
 */
void destroiGrafo(Vertice **G, int ordem){
	int i;
    Aresta *a, *n;
    
	for(i=0; i<ordem; i++){ /* Remove lista de adjacencia de cada vertice */
	    a= (*G)[i].prim;
        while (a!= NULL){
              n= a->prox;
				free(a);
              a= n;
        }
	}
    free(*G);
}

/*  
 * Acrescenta uma nova aresta em um grafo previamente criado. 
 *   Devem ser passados os extremos v1 e v2 da aresta a ser acrescentada 
 * Como o grafo nao e orientado, para uma aresta com extremos i e j
 *   serao criadas, na estrutura de dados, arestas (i,j) e (j,i)
 */
int acrescentaAresta(Vertice G[], int ordem, int v1, int v2, int c){
    Aresta * A1, *A2;
    
	if (v1<0 || v1 >= ordem) /* Testo se vertices sao validos */
	   return 0;
	if (v2<0 || v2 >= ordem)
	   return 0;
	
	/* Acrescento aresta na lista do vertice v1 */
	A1= (Aresta *) malloc(sizeof(Aresta));
	A1->nome= v2;
	A1->custo= c;
	A1->prox= G[v1].prim;
	G[v1].prim= A1;

	if (v1 == v2) return 1; /* Aresta e? um laco */

	/* Acrescento aresta na lista do vertice v2 se v2 != v1 */	
	A2= (Aresta *) malloc(sizeof(Aresta));
	A2->nome= v1;
	A2->custo= c;
	A2->prox= G[v2].prim;
	G[v2].prim= A2;
	
	return 1;
}


/*  
 * Imprime um grafo com uma notacao similar a uma lista de adjacencia.
 */
void imprimeGrafo(Vertice G[], int ordem){
	int i;
	Aresta *aux;

	printf("\nOrdem:   %d",ordem);
	printf("\nLista de Adjacencia (custos das arestas entre parenteses):\n");

	for (i=0; i<ordem; i++){
		printf("\n    v%d(%d): ", i, G[i].comp);
		aux= G[i].prim;
		for( ; aux != NULL; aux= aux->prox)
			printf("  v%d(%d)", aux->nome, aux->custo);
	}
	printf("\n\n");
}

/*
 * Retorna o grau de um vertice v de um grafo G.
 */
int calculaGrauDeVertice(Vertice G[], int ordem, int v){
	int     i;
	Aresta* aux;
	
	if (v<0 || v>= ordem)
	   return(-1);
	aux= G[v].prim;
	/* Percorre a lista de arestas que incidem em v, contando qtd. */
	for(i=0; aux!=NULL; aux= aux->prox, i++)
	    if(aux->nome == v) i++;  /* Se for laco, conto 2 vezes */
	
	return(i);
}

/*
 * Retorna o grau minimo de um grafo G.
 */
int calculaGrauMinimo(Vertice G[], int ordem){
	int i,j;
	int grauMin= INT_MAX; /* Inicializacao com valor maximo */
	/* Percorro lista de vertices, calculando o grau de cada um */
	for(i=0; i<ordem; i++){
		j= calculaGrauDeVertice(G,ordem,i);
		if (j<grauMin) grauMin=j; /* Encontrando menor, atualizo minimo */
	}
	return(grauMin);
}

/*
 * Retorna o grau maximo de um grafo G.
 */
int calculaGrauMaximo(Vertice G[], int ordem){
	int i,j;
	int grauMax= -1; /* Inicializacao com valor minimo */
	/* Percorro lista de vertices, calculando o grau de cada um */
	for(i=0; i<ordem; i++){
		j= calculaGrauDeVertice(G,ordem,i);
		if (j>grauMax) grauMax=j;/* Encontrando maior, atualizo minimo */
	}
	return(grauMax);
}

/*
 * Retorna o tamanho de um grafo G (no. de vertices + no. de arestas).
 */
int calculaTamanho(Vertice G[], int ordem){
	int i;
	int total= 0;
	/* Qtd. de arestas � igual a metade da soma dos graus dos v�rtices */
	for(i=0; i<ordem; i++){
		total += calculaGrauDeVertice(G,ordem,i);
	}
	return(total/2+ordem);	
}

/*
 * Retorna 1 se um grafo G for conexo; ou 0, caso contrario.
 */
int eConexo(Vertice G[], int ordem){
	int i;
	int achou;
	Aresta *aux;
	
	if (ordem == 0) return(0); /* Grafo vazio */
	/*incializando a variavel comp da estrututra vertice como 0 */
	for (;i<ordem;i++)
	{
		G[i].comp =0;
	}
	G[0].comp= 1; /* Anoto um "vertice inicial" com 1 */
	
	for(i=1; i<ordem; i++){
		G[i].comp= 0;  /* Todos os demais sao marcados com 0 */
	}
	do { /* Repito enquanto encontrar vertice nao anotado adjacente a anotado */
		achou = 0;
		for(i=0; i<ordem; i++){
			if(G[i].comp == 1){ /* Vertice i esta �anotado */
				aux= G[i].prim;
				for( ; aux!=NULL; aux= aux->prox){
					if(G[aux->nome].comp == 0){/* Achei nao anotado adjacente a anotado */
						G[aux->nome].comp= 1;  /* Anoto novo vertice*/
						achou= 1;              /* Indico que anotei um novo vertice */
					}
				}
			}
		}
	} while(achou); /* SE anotei um novo vertice, repito */
	
	for(i=0; i<ordem; i++){ /* SE encontrar vertice nao anotado, grafo e� desconexo */
		if(G[i].comp==0) return(0);
	}
	return(1);
	
	
}


/*
 *Verifica quais sao as arestas de corte
 *retornando 1 se o vertice for, caso contrario 0
 *Isso se da por meio de fazer subgrafos conexos maximos de cada vertice, 
 *e deletando cada aresta e testando para ver se a falta dela deixa 
 *o subgrafo desconexo
 */

int ArestasCorte(Vertice G[], int ordem,int v[])
{
	
	Vertice *Gaux1;		
	int i,j,c,achou;
	Aresta *aux,*a,*n;
	i=j=c=0;
	/*incializando a variavel comp da estrututra vertice como 0 */
	for (;i<ordem;i++)
	{
		G[i].comp =0;
	}
	Gaux1 = G;
	for (i=0;i<ordem;i++)
	{
		/*cria-se um grafo auxiliar, para conter um subgrafo maximo
		* conexo com todos os vertices o possiveis saindo de cada
		*vertice do grafo por completo*/
		aux = G[i].prim;
		for(;aux!=NULL;aux=aux->prox)
		{
			c++;
		}
		
		criaGrafo(&Gaux1, c);
		for(;aux!=NULL;aux=aux->prox)
		{
			acrescentaAresta(Gaux1,c,aux->nome,aux->prox,1);
		}
		for(i=0; i<ordem; i++){
			a = Gaux1[i].prim;
			aux= Gaux1[i].prim;
			for( ; aux!=NULL; aux= aux->prox)
			{
				n= a;
				free(a);
              	if (eConexo(Gaux1,c)==0)
            	{	
            		for (j=0;j<ordem;j++)
            		{
            			if(Gaux1[j].prim!=n->nome)
            			{
            				v[c] = n->nome;
						}
					}
        			
				}
				a= n;
				n=a->prox;
			}
		}
		
	}
return v;

}


/*
 * Programinha simples para testar a representacao de grafo
 */
int main(void) {
    int i,v,c,*Acortes,*a;
	Vertice *G;
	int ordemG= 6; 

		
	criaGrafo(&G, ordemG);
	acrescentaAresta(G,ordemG,2,3,1);
	acrescentaAresta(G,ordemG,0,1,1);
	acrescentaAresta(G,ordemG,1,1,1);
	acrescentaAresta(G,ordemG,0,2,1);
	acrescentaAresta(G,ordemG,0,3,1);
	acrescentaAresta(G,ordemG,4,5,1);
	acrescentaAresta(G,ordemG,4,1,1);

	
	for(v=0; v<ordemG;v++){
	   i= calculaGrauDeVertice(G,ordemG,v);
	   printf("O grau do vertice %d e %d\n", v, i);
    }
    
    i= calculaGrauMaximo(G,ordemG);
    printf("O grau maximo do grafo e %d\n", i);
    
    i= calculaTamanho(G,ordemG);
    printf("O tamanho do grafo e %d\n", i);
    
    if(eConexo(G,ordemG))
       printf("O grafo e conexo\n");
    else
       printf("O grafo nao e conexo\n");
	imprimeGrafo(G, ordemG);
	
	Acortes = ArestaCortes(G,ordemG,a);

	destroiGrafo(&G, ordemG);
    system("PAUSE");
	return 0;
}
