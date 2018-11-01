#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/time.h>

//#define MOSTRAR 1

typedef struct vertices
{
	int id;
	int cor;
	int grau;
}vertices;

vertices * criarListaVertices(int num) // lista usada para guardar os vertices e cada vertice com cor, grau e id
{
	int id = 0, idx;
	vertices * v = (vertices *) malloc(sizeof(struct vertices) * num);
	for(idx = 0; idx < num; idx++){
		v[idx].id   = id++;
		v[idx].cor  = -1;
		v[idx].grau = 0;
	}
	return v;
}

int ** criarMatrizDinamicamenteInt(int n_linhas, int n_colunas) // como nao é possivel prever o tamanho da matriz necessaria, aloca em tempo de execucao
{
	int ** matriz = (int **) malloc(sizeof(int *) * n_linhas); int idx;
	for(n_linhas--; n_linhas >= 0; n_linhas--){
		matriz[n_linhas] = (int *) malloc(sizeof(int) * n_colunas);
		for(idx = 0; idx < n_colunas; idx++)
			matriz[n_linhas][idx] = 0;
	}
	return matriz;
}

char** str_split(char* a_str, const char a_delim) // split de string por caractere
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    count += last_comma < (a_str + strlen(a_str) - 1);

    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}

void preencheGrau(vertices * lista_v, int ** matriz_adj, int numv) // conta o numero de arestas de cada vertice em matriz_adj e insere na lista_v
{
	int count = 0, linha = 0, coluna = 0;
	for(linha = 0; linha < numv; linha++)
	{
		count = 0;
		for(coluna = 0; coluna < numv; coluna++)
		{
			if(matriz_adj[linha][coluna] == 1)
				count++;
		}
		lista_v[linha].grau = count;
	}
}

void ordenaGrauDec(vertices ** lista, int numv) // ordenar grau de forma decrescente
{
	/* selection sort */
	int maior = 0;
	int idx = 0, idx2 = 0;
	for(idx = 0; idx < numv; idx++)
	{
		maior = idx;
		for(idx2 = idx; idx2 < numv; idx2++)
		{
			if((*lista)[maior].grau < (*lista)[idx2].grau)
				maior = idx2;
		}
		if(maior != idx)
		{
			int id = (*lista)[maior].id;
			int grau = (*lista)[maior].grau;
			int cor = (*lista)[maior].cor;
			(*lista)[maior].id = (*lista)[idx].id;
			(*lista)[maior].grau = (*lista)[idx].grau;
			(*lista)[maior].cor = (*lista)[idx].cor;
			(*lista)[idx].id = id;
			(*lista)[idx].grau = grau;
			(*lista)[idx].cor = cor;
		}
	}
}

int faltaColorir(vertices * lista, int num) /* verifica se existe vertice que falta colorir dentro da lista, com excecao do vertice 0 que deve ser desconsiderado visto que nas instancias o menor valer parte de 1*/
{
	int i;
	for(i = 0; i < num; i++)
		if(lista[i].cor == -1 && lista[i].id != 0){
			return 1;
		}
	return 0;
}


int procuraCorArestas(vertices ** lista, vertices v, int ** matriz_adj, int num, int cor)
{
	int i;
	for(i = 1; i < num; i++)
	{
		if(matriz_adj[v.id][i] == 1)
		{
			int j;
			for(j = 0; j < num; j++)
			{
				if((*lista)[j].id == i)
				{
					if((*lista)[j].cor == cor)
						return 1; //caso em que existe um vertice adjacente ja colorido com a cor desejada
				}
			}
		}
	}
	return 0;
}

void insereCor(vertices ** lista, int ** matriz_adj, int id, int num, int cor) /*recebe a lista o id e a cor, pesquisa pelo id e insere a cor desejada nele se ele ja nao estiver colorido*/
{
	int idx = 0;
	for(; idx < num; idx++)
	{
		if((*lista)[idx].id == id && (*lista)[idx].cor == -1 && !procuraCorArestas(lista, (*lista)[idx], matriz_adj, num, cor))
			(*lista)[idx].cor = cor;		
	}
}

int colore(vertices ** lista, int ** matriz_adj, int num) /* executa o algoritmo de welsh e powell, pega uma cor e pinta o vertice corrente e todos os vertices da lista que nao estao adjacentes a ele, em seguida troca de cor*/
{
	int idx_matriz_linha = 1, idx_vetor = 0, idx_matriz_coluna = 1;
	int cor = 1;
	for(idx_vetor = 0; idx_vetor < num - 1; idx_vetor++)
	{
		int id = (*lista)[idx_vetor].id;
		for(idx_matriz_coluna = 1; idx_matriz_coluna < num; idx_matriz_coluna++)
		{
			if(matriz_adj[id][idx_matriz_coluna] == 0)
			{
				insereCor(lista, matriz_adj, idx_matriz_coluna, num, cor);
			}
		}
		if(faltaColorir((*lista), num)){
			cor++;
		}
	}
	return cor;
}

/*
formato de instancia de entrada (exemplo) -- primeiro valor é o numero de vertices do grafo em as proximas linhas sao as arestas

250
1 5
3 2
7 10
.
.
.
*/

int main(int argc, char * argv[])
{
	printf("******* Algoritmo Welsh and Powell ********\n");
	struct timeval stop, start;
	int id1, id2; char ** str_separada;
	if(argc < 2)
		return 1;
	FILE * file = fopen(argv[1], "rb");
	if(!file)
		return 1;
	char buffer[50];
	fgets(buffer, 50, file);	
	int numero_vertices = atoi(buffer);
	int ** matriz_adjacente = criarMatrizDinamicamenteInt(numero_vertices+1, numero_vertices+1);
	vertices * lista_vertices = criarListaVertices(numero_vertices + 1);
	gettimeofday(&start, NULL);
	int num_arestas = 0;
	while(!feof(file))
	{
		fgets(buffer, 50, file);
		num_arestas++;
		str_separada = str_split(buffer, ' ');
		id1 = atoi(str_separada[0]);
		id2 = atoi(str_separada[1]);
		matriz_adjacente[id1][id2] = 1;
		matriz_adjacente[id2][id1] = 1;
	}
	gettimeofday(&stop, NULL);
	printf("Tempo de carregamento da instancia (sec): %f\n", (float)(stop.tv_usec - start.tv_usec)/1000000.0);
	printf("Numero de vertices: %d // Numero de arestas: %d\n", numero_vertices, num_arestas);
	gettimeofday(&start, NULL);
	preencheGrau(lista_vertices, matriz_adjacente, numero_vertices);
	ordenaGrauDec(&lista_vertices, numero_vertices);
	int num_cor = colore(&lista_vertices, matriz_adjacente, numero_vertices + 1);
	gettimeofday(&stop, NULL);
	#ifdef MOSTRAR
	int i;
	for(i = 0; i < numero_vertices; i++)
		printf("%d -- %d\n", lista_vertices[i].cor, lista_vertices[i].id);
	#endif
	printf("Numero de cores necessario: %d\n", num_cor);
	printf("Tempo levado para coloracao (sec): %f\n", (float)(stop.tv_usec - start.tv_usec)/1000000.0);
	return 0;
}	
/*formato de saida é um array de vertices coloridos onde cada vertice é o struct vertice definido com cor, grau e id*/
/*cor no final consiste do numero de cores necessario para colorir o grafo*/
