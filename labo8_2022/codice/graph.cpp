#include "graph.h"

using namespace graph;

struct graph::vertexNode {
  Label label;
  halfEdgeVertex *adjList;
  vertexNode *next;
  bool visited;
};

struct graph::halfEdgeVertex {
  Label label; // etichetta del vertice destinazione
  Weight weight; // etichetta (o “peso”) dell'arco
  halfEdgeVertex* next; // puntatore al mezzo arco successivo
  vertexNode *verPtr; // puntatore al vertice
};

/*******************************************************************************************************/
// Grafo
/*******************************************************************************************************/

vertexNode* getVertex(Label l, const Graph& g) {
  for (graph::Graph newgraph = g; newgraph != emptyGraph; newgraph = newgraph->next) {
    if (newgraph->label == l) return newgraph; // vertice trovato
  }

  return emptyGraph; // vertice non trovato
}

// Ritorna true se il vertice e' presente nel grafo
bool isVertexInGraph(Label l, const Graph& g) {
  return (getVertex(l, g) != emptyGraph);
}

bool isEdgeInGraph(Label from, Label to, const Graph& g) {
  vertexNode* nodo = getVertex(from, g);

  if (nodo == emptyGraph) return false; // se il grafo è vuoto ritorno false

  for (halfEdgeVertex* n = nodo->adjList; n != nullptr; n = n->next) {
    if (n->verPtr->label == to) return true; // scorro il grafo fino a trovare l'arco che parte da from e arriva a to
  }

  return false; // se non l'ho trovato ritorno false
}

// Restituisce il grafo vuoto
Graph graph::createEmptyGraph()
{
  return emptyGraph;
}

// Aggiunge nuovo vertice con etichetta la stringa. Fallisce se gia' presente
bool graph::addVertex(Label l, Graph& g) {
  if (isVertexInGraph(l, g)) return false; // se il vertice con etichetta l è già presente non lo aggiungo

  Graph newgraph = new vertexNode; // creo un nuovo vertice

  newgraph->label = l; // la sua etichetta diventa l
  newgraph->adjList = nullptr; // inizialmente non ha adiacenze
  newgraph->visited = false; // e non è stato visitato

  if (isEmpty(g)) { // nel caso in cui g sia vuoto
    g = newgraph; // rendo g uguale al mio nuovo vertice
    newgraph->next = emptyGraph; // e il vertice successivo sarà vuoto
  }
  else {
    newgraph->next = g; // altrimenti rendo il vertice successivo uguale a g
    g = newgraph; // e g uguale al nuovo vertice
  }

  return true;
}

// ausiliaria
void addHalfEdge(Label from, Label to, Weight w, Graph& g) {
  halfEdgeVertex *arco = new halfEdgeVertex; // nuovo arco
  arco->verPtr = getVertex(to, g); // inzializzo il suo puntatore al vertice
  arco->weight = w; // e il suo peso
  
  vertexNode* nodo = getVertex(from, g); // nodo che corrisponde al vertice from

  if(nodo->adjList == nullptr) { // se la lista di adiacenza di nodo è vuota
    nodo->adjList = arco; // ci inserisco l'arco
    arco->next = nullptr; // e il suo prossimo vertice sarò vuoto
  }
  else {
    arco->next = nodo->adjList; // altrimenti il prossimo vertice è il nodo
    nodo->adjList = arco; // e inserisco arco nella lista di adiacenza di nodo
  }
}

// Aggiunge un arco di peso "w" tra i nodi con etichetta "f" e "t". Fallisce se esiste gia' l'arco
// se i nodi non esistono nel grafo e se si tenta di inserire un arco tra un nodo ed esso stesso
bool graph::addEdge(Label from, Label to, Weight w, Graph& g) {
  
  if (from == to) return false; // se from e to sono lo stesso vertice non aggiungo l'aro
  
  if(!isVertexInGraph(from, g) || (!isVertexInGraph(to, g))) return false; // se uno dei due vertici non esiste non aggiungo l'arco

  if(isEdgeInGraph(from, to, g) || (isEdgeInGraph(to, from, g))) return false; // se l'arco esiste già non lo aggiungo

  addHalfEdge(from, to, w, g); // creo due mezzi archi
  addHalfEdge(to, from, w, g); // da from a to e da to a from

  return true;
}

// Restituisce true se il grafo e' vuoto, false altrimenti
bool graph::isEmpty(const Graph& g)
{ 
  return g==emptyGraph;
}

// Ritorna il numero di vertici del grafo
int graph::numVertices(const Graph& g) {
  if(isEmpty(g)) return 0;

  int verts = 0;

  for(graph::Graph cur = g; cur != emptyGraph; cur = cur->next) 
    verts++; // incremento il contatore di vertici man mano che scorro il grafo

  return verts;
}

int numHalfEdges(halfEdgeVertex *primo) {
  int halfedges = 0;

  for(halfEdgeVertex *a = primo; a != nullptr; a = a->next) 
    halfedges++; // incremento il contatore di mezzi archi man mano che scorro il grafo

  return halfedges;
}

// Ritorna il numero di archi del grafo
int graph::numEdges(const Graph& g) {
  if(isEmpty(g)) return 0;

  int edges = 0;

  for(Graph cur = g; cur != emptyGraph; cur = cur->next) 
    edges += numHalfEdges(cur->adjList); // aggiungo il numero di mezzi archi al contatore man mano che scorro il grafo

  return edges/2; // divido per due perché nel nostro caso abbiamo un grafo NON orientato (e gli archi saranno sempre pari)
}

// Calcola e ritorna (nel secondo parametro) il grado del nodo. Fallisce
// se il nodo non esiste
bool graph::nodeDegree(Label l, int& degree, const Graph& g) {
  if(isEmpty(g) || !isVertexInGraph(l, g)) return false;

  vertexNode* nodo = getVertex(l, g);
  degree = numHalfEdges(nodo->adjList); // il grado è uguale ai mezzi archi che escono da nodo
  return true;
}

// Verifica se i due vertici v1 e v2 sono adiacenti (ovvero se esiste un arco)
bool graph::areAdjacent(Label v1, Label v2, const Graph& g) {
  if(isVertexInGraph(v1, g) && isVertexInGraph(v2, g)) {
    if(isEdgeInGraph(v1, v2, g) || isEdgeInGraph(v2, v1, g)) return true; // v1 e v2 sono adiacenti solo se entrambi esistono e se esiste un arco che li collega
  }

  return false;
}

// Restituisce la lista di adiacenza di un vertice
list::List graph::adjacentList(Label v1, const Graph& g) {
  list::List lst = list::createEmpty();

  vertexNode *nodo = getVertex(v1, g);
  if(nodo == emptyGraph) return lst;

  for(halfEdgeVertex *hev = nodo->adjList; hev != nullptr; hev = hev->next) // man mano che scorro il grafo
    list::addFront((hev->verPtr)->label, lst); // aggiungo in testa alla lista di adiacenza i vertici adiacenti

  return lst;
}

// Ritorna un cammino tra una citta' ed un altra
// Il cammino da "v1" a "v2" alla fine sara' in "path"
// e la lunghezza sara' in "len".
// Si assume che il chiamante fornisca inizialmente un cammino vuoto.
//
// La funzione rappresenta una variante della visita DFS

void findPathAux(vertexNode *v1, vertexNode *v2, list::List &path, int &len, const Graph& g) {
  if(isEmpty(v1) || isEmpty(v2) || v1 == v2) return;

  v1->visited = true; // vertice visitato

  if(v1->label == v2) return; // se arrivo a v2 esco

  for(halfEdgeVertex *adjnewg = v1->adjList; adjnewg != nullptr; adjnewg = adjnewg->next) {
    if((adjnewg->verPtr)->visited == false) {
      findPathAux(adjnewg->verPtr, v2, path, len, g); // scorro e fino a quando ci sono vertici non visitati chiamo ricorsivamente la funzione

    list::addFront((adjnewg->verPtr)->label, path); // aggiungo in testa
    len += adjnewg->weight; // e aggiungo il peso di ogni arco a len

    if(list::get(list::size(path)-1, path) == v2) return; // se l'ultimo elemento aggiunto è v2 esco

    list::removePos(list::size(path)-1, path); // se non lo è lo elimino 
    len -= adjnewg->weight; // e ne tolgo anche il peso
    }
  }
}

void graph::findPath(Label v1, Label v2, list::List &path, int &len, const Graph& g) {
  if(isEmpty(g)) return;
  if(v1 == v2) return;

  Graph newg = g;

  while(newg != emptyGraph) { // scorro il grafo
    newg->visited = false;
    newg = newg->next;
  }

  vertexNode *vertex1 = getVertex(v1, g);
  vertexNode *vertex2 = getVertex(v2, g);
  findPathAux(vertex1, vertex2, path, len, g);
}

/*******************************************************************************************************/
void printAdjList(Label l, const Graph& g) {
  vertexNode* nodo = getVertex(l, g);

  if (nodo==emptyGraph) return;

  for (halfEdgeVertex* n = nodo->adjList; n != nullptr; n = n->next) {
    cout << "(" << (n->verPtr)->label << ", " << n->weight << ")" << "; ";
  }
  cout << endl;
}

// Stampa il grafo
void printGraph(const graph::Graph& g) {
  for (graph::Graph newgraph = g; newgraph != emptyGraph; newgraph = newgraph->next) { // man mano che scorro il grafo
    cout << newgraph->label << ": "; // stampo ogni città
    printAdjList(newgraph->label, g); // e la rispettiva lista di adiacenza
  }
}
