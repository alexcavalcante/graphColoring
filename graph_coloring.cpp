// A C++ program to implement greedy algorithm for graph coloring
#include <iostream>
#include <algorithm>
#include <fstream>
#include <list>
#include <vector>
#include <array>
#include <time.h>

using namespace std;

// Uma classe que representa um grafo não direcionado
class Graph
{
    int V;    //Numero de vertices
    list<int> *adj;    // Lista de Adjacencia
public:
    // Construtores
    Graph(int V)   { this->V = V; adj = new list<int>[V+1]; }
    ~Graph()       { delete [] adj; }

    // Função para adicionar as arestas
    void addEdge(int v, int w);
    // Imprime usando dSatur
    void dSaturColoring();
    void dSaturColoring2();
};

// Classe de vertice
class Vertex{

    int vertice; // valor do vertice
    int num_conect; // numero de arestas conectado ao vertice
    int color; // numero da cor

public:
    void setValor(int v, int n);
    int getNum_conect();
    int getVertice();
    int getColor();
    void setColor(int c);
};

void Graph::addEdge(int v, int w)
{
    adj[v].push_back(w);
    adj[w].push_back(v);  // Note: the graph is undirected
}

// retorna o vertice com maior numero de arestas conectados
bool compareInterval(Vertex i1, Vertex i2)
{
    return (i1.getNum_conect() > i2.getNum_conect());
}

/* Funções da Classe Vertex*/
void Vertex::setValor(int v, int n){
    vertice = v;
    num_conect = n;
}

void Vertex::setColor(int c){
    color = c;
}

int Vertex::getNum_conect(){
    return num_conect;
}

int Vertex::getVertice(){
    return vertice;
}

int Vertex::getColor(){
    return color;
}


/*************************/

/* STRUCT Cores*/

struct Cores{
    int colors; //Cor usado no vertice
    vector<int> aresta_block; //arestas que não podem ser colorida com a cor
};
/*Função (vetor, tamanho, id) */
bool existeArestas(struct Cores arr[], int n, int id)
{
 for(int i=0;i<=n;i++)
 {  //Testa se o vertice escolhido ta bloqueado para a cor, começando da menor cor possivel, ou seja, 0 -> n..
    if (find(arr[i].aresta_block.begin(), arr[i].aresta_block.end(),id) != arr[i].aresta_block.end())
        return true;
    else
        return false;
 }
 return false;
}

void buscaLocal(vector<Vertex> adjvert, int num_cor, struct Cores arr[]);

void Graph::dSaturColoring(){

    struct Cores colorful[V]; // estrutura para cores,

    vector<int> color; // vetor de cores
    vector<Vertex> adjvert;
    Vertex *adjvert1 = new Vertex;

    int cor = 0;
    int num_cor = 0;

    list<int>::iterator i;
    int cont = 0;
    int aux;
    while(cont != V){
        aux = 0;
        //Cria lista de Adjacencia
        for (i = adj[cont].begin(); i != adj[cont].end(); ++i){
            //cout << "Vertice: " << cont << " Conectado: " << *i << endl;
            aux++;
        }
        adjvert1->setValor(cont,aux);
        adjvert.push_back(*adjvert1);
        cont++;
    }

    sort(adjvert.begin(),adjvert.end(), compareInterval); //Ordem decrescente

    for (int i = 0; i < V; ++i){
//        cout << "Vertice: " << adjvert[i].getVertice() << " Possui: " << adjvert[i].getNum_conect() << endl;
        cout << "AD: " <<adjvert[i].getNum_conect() << " V: " << adjvert[i].getVertice()<<endl;
    }

    adjvert[0].setColor(cor);//seta cor 0 pro vertice de maior grau
    color.push_back(cor);

    // grava a primeira cor e os vertices que não podem ser preenchido com a cor
    for (i = adj[adjvert[0].getVertice()].begin(); i != adj[adjvert[0].getVertice()].end(); ++i){
        colorful[0].colors = 0;
        colorful[0].aresta_block.push_back(*i);
    }
//    cout << "\nEXISTE? 1 - Se Verdade -> " << existeArestas(colorful,V,0)<< endl;
//    cout << "\nEXISTE? 1 - Se Verdade -> " << existeArestas(colorful,V,3)<< endl;
//    cout << "\nEXISTE? 1 - Se Verdade -> " << existeArestas(colorful,V,2)<< endl;
//
//    cout << "Cor -> " << colorful[0].colors << endl;

//    colorf->setInfo(cor,0);//cor e aresta
//    colorf->setInfo(cor,2);
//    colorf->setInfo(cor,3);
//    colorful.push_back(*colorf);

//    cout << "Cor -> " << colorful[0].getColors() << endl;
//    cout << "\nEXISTE? 1 - Se Verdade -> " << colorful[0].existeArestas(0) << endl;
//    cout << "\nEXISTE? 1 - Se Verdade -> " << colorful[0].existeArestas(2) << endl;
//    cout << "\nEXISTE? 1 - Se Verdade -> " << colorful[0].existeArestas(1) << endl;
    int cont_cor = 0;
    for (int u = 1; u < V; u++)
    {   //lista de ajacencia do vertice selecionado
        for (i = adj[adjvert[u].getVertice()].begin(); i != adj[adjvert[u].getVertice()].end(); i++){
            //testa a cor com o total de vertice selecionado
            while(cont_cor != u){
                    // verifica se o vertice selecionado está bloqueado para alguma cor, chama função.
                if(existeArestas(colorful,cor,adjvert[u].getVertice()) == true){
                    cor++; // se tiver bloqueado para a cor, acrescenta cor
                    cont_cor++;
                    break;
                }
                if(existeArestas(colorful,cor,adjvert[u].getVertice()) == false){
                    //cor = cont_cor;
                    break;
                }
                if(adjvert[u].getNum_conect() == 0){
                    cout << "eh igual a zero" << endl;
                    cont_cor = 0;
                    break;
                }
                break;
            }
            cor = cont_cor;
            colorful[cor].colors = cor;
            colorful[cor].aresta_block.push_back(*i);
            color.push_back(cor);
        }
//

        adjvert[u].setColor(cor);

        cont_cor = 0;

    }

     //print the result
    for (int u = 0; u < V; u++)
     // cout << "\nVertice  -> " << adjvert[u].getVertice() << " - Cor do Vertice -> "<< adjvert[u].getColor() << endl;

    //verifica o maior valor de cor utilizado
    for (int i = 0; i < adjvert.size(); ++i)
        if(num_cor < adjvert[i].getColor())
            num_cor = adjvert[i].getColor();


        cout << "\nNumero minimo de cores possiveis: " << num_cor + 1 << endl;


       // buscaLocal(adjvert, num_cor, colorful);
}

void Graph::dSaturColoring2(){

    struct Cores colorful[V]; // estrutura para cores,

    vector<int> color; // vetor de cores
    vector<Vertex> adjvert;
    Vertex *adjvert1 = new Vertex;

    int cor = 0;
    int num_cor = 0;

    list<int>::iterator i;
    int cont = 0;
    int aux;
    while(cont != V){
        aux = 0;
        //Cria lista de Adjacencia
        for (i = adj[cont].begin(); i != adj[cont].end(); ++i){
            //cout << "Vertice: " << cont << " Conectado: " << *i << endl;
            aux++;
        }
        adjvert1->setValor(cont,aux);
        adjvert.push_back(*adjvert1);
        cont++;
    }

    //sort(adjvert.begin(),adjvert.end(), compareInterval); //Ordem decrescente

    for (int i = 0; i < V; ++i){
//        cout << "Vertice: " << adjvert[i].getVertice() << " Possui: " << adjvert[i].getNum_conect() << endl;
        cout << "AD: " <<adjvert[i].getNum_conect() << " V: " << adjvert[i].getVertice()<<endl;
    }

    adjvert[0].setColor(cor);//seta cor 0 pro vertice de maior grau
    color.push_back(cor);

    // grava a primeira cor e os vertices que não podem ser preenchido com a cor
    for (i = adj[adjvert[0].getVertice()].begin(); i != adj[adjvert[0].getVertice()].end(); ++i){
        colorful[cor].colors = 0;
        colorful[cor].aresta_block.push_back(*i);
    }

    int cont_cor = 0;
    for (int u = 1; u < V; u++)
    {   //lista de ajacencia do vertice selecionado
        for (i = adj[adjvert[u].getVertice()].begin(); i != adj[adjvert[u].getVertice()].end(); i++){
            //testa a cor com o total de vertice selecionado
            while(cont_cor != u){
                    // verifica se o vertice selecionado está bloqueado para alguma cor, chama função.
                if(existeArestas(colorful,cor,adjvert[u].getVertice()) == true){
                    cor++; // se tiver bloqueado para a cor, acrescenta cor
                    cont_cor++;
                }
                else if(!existeArestas(colorful,cor,adjvert[u].getVertice())){
                    cor = cont_cor;
                    break;
                }
                break;
            }
            cor = cont_cor;
            colorful[cor].colors = cor;
            colorful[cor].aresta_block.push_back(*i);
            color.push_back(cor);
        }
//

        adjvert[u].setColor(cor);

        cont_cor = 0;

    }

     //print the result
    for (int u = 0; u < V; u++)
      //cout << "\nVertice  -> " << adjvert[u].getVertice() << " - Cor do Vertice -> "<< adjvert[u].getColor() << endl;

    //verifica o maior valor de cor utilizado
    for (int i = 0; i < adjvert.size(); ++i)
        if(num_cor < adjvert[i].getColor())
            num_cor = adjvert[i].getColor();


        cout << "\nNumero minimo de cores possiveis: " << num_cor + 1 << endl;


       // buscaLocal(adjvert, num_cor, colorful);
}
/*BUSCA LOCAL*/

// void buscaLocal(vector<Vertex> adjvert, int num_cor, struct Cores arr[] ){

//     vector<Vertex> aux = adjvert;
//     int cor_menor = 0;
//     int cor_minVertice = INT32_MAX;
//     int contador = 0;
//     int nova_cor = 0;
//     int j;
//     for(j = 0; j < num_cor; j++){
//         for (int i = 0; i < adjvert.size(); ++i)
//             if(adjvert[i].getColor() == j){
//                 contador++;
//             }

//         if(cor_minVertice > contador && contador != 0){
//             cor_minVertice = contador;
//             cor_menor = j;
//         }
//         contador = 0;
//     }

//     while(adjvert[cor_menor].getColor() == cor_menor){
//             adjvert[cor_menor].setColor(-1);
//             cout << "Cor do vertice " << adjvert[cor_menor].getVertice() << " eh: " << adjvert[cor_menor].getColor() << endl;
//     }
//     int i;
//     for(i=0;i<=num_cor;i++)
//     {  //Testa se o vertice escolhido ta bloqueado para a cor, começando da menor cor possivel, ou seja, 0 -> n..
//         if (find(arr[i].aresta_block.begin(), arr[i].aresta_block.end(),adjvert[cor_menor].getVertice()) != arr[i].aresta_block.end()&& i != cor_menor)
//             break;
//         else
//             cout << "\nNao eh possivel colorir para a cor -> " << i << endl;
//     }

//     cout << "Cor possivel para colorir -> " << i << endl;
//     cout << "Numero de vertice com a cor " << cor_menor << " eh: " << cor_minVertice << endl;



// }


// Driver program to test above function
int main(int argc, char* argv[])
{   clock_t tempo;
	tempo = clock();

    string url = argv[1];
    int tam, value, value2;

    ifstream file(url);
    file >> tam;
    Graph g1(tam);
    //Graph g2(tam);
    cout << "tamanho: " << tam << endl;
    while(!file.eof()){
            file >> value;
            file >> value2;
           // cout << value << " " << value2 << endl;
            g1.addEdge(value2,value);
     //       g2.addEdge(value,value2);
    }

    g1.dSaturColoring();

    cout <<"Tempo gasto em (s): " << ((clock() - tempo)/(double)CLOCKS_PER_SEC) << endl;
    //g1.dSaturColoring2();
//    Graph g3(6);
//    g3.addEdge(0, 1);
//    g3.addEdge(0, 2);
//    g3.addEdge(1, 2);
//    g3.addEdge(1, 3);
//    g3.addEdge(2, 3);
//    g3.addEdge(3, 4);
//    g3.addEdge(0, 5);
//    cout << "Coloring of graph 1 \n";
//    g3.dSaturColoring();
//
//
//    Graph g2(5);
//    g2.addEdge(0, 1);
//    g2.addEdge(0, 2);
//    g2.addEdge(1, 2);
//    g2.addEdge(1, 4);
//    g2.addEdge(2, 4);
//    g2.addEdge(4, 3);
//    g2.addEdge(2, 3);
//    g2.addEdge(0, 3);
//    cout << "\nColoring of graph 2 \n";
//    g2.dSaturColoring2();


    return 0;
}
