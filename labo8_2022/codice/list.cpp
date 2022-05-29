////////////////////////////////////////////////////
//
//  Questa serve per realizzare le liste che vengono fuori come risultato di
//  operazioni quali il percorso tra due citta' (che viene rappresentato come
//  lista di nomi di citta') e l'elenco delle citta' adiacenti a una certa
//  citta' (anch'esso rappresentato come lista di nomi di citta').
//  Non serve invece per le liste dei vertici nel grafo, ne' per le liste di
//  adiacenza di ciascun vertice.
//
////////////////////////////////////////////////////

#include "list.h"

using namespace list;

struct list {
Elem elem;
list *next;
};


/****************************************************************/
int list::size(const List& l)
{
   int cont = 0;
   List q = l;
   while (q != emptyList) {
      q = q->next;
      cont++; // incremento il contatore man mano che scorro la lista
   }
   return cont;
}

/****************************************************************/
void list::clear(List& l){
   List q = l;  // q va alla testa della lista
   List aux;
   while (q != emptyList) {   // avanza finche' non finisce la lista o non trova la posizione pos
      aux = q;
      q = q->next; 
      delete aux; // dealloco ogni elemento della lista
   }
   l->next = l; // risitemo i puntatori next e prev di l in modo che puntino a l stessa
}


/****************************************************************/
void list::set(int pos, Elem e, List& l) // "set" nel senso di "assegna un valore", non nel senso di "insieme"! E' una terminologia standard, non fate confusione con gli insiemi
{
   
}


/****************************************************************/
void list::add(int pos, Elem e, List& l) // aggiunge e nella posizione pos
{
  
}

/****************************************************************/
void list::addBack(Elem e, List& l) // aggiunge e in coda
{
   list* aux = new list;
   aux->elem = e;
   aux->next = emptyList;
   if(l == emptyList)
      l = aux;
   else {
   List q = l;  // q va alla testa della lista 
      while (q->next != emptyList) // scorro la lista
         q = q->next;
   q->next = aux;
   }
}


/****************************************************************/
void list::addFront(Elem e, List& l) // aggiunge e in testa
{
   list* aux = new list;
   aux->elem = e;
   aux->next = l;
   l = aux;
}

/****************************************************************/
void list::removePos(int pos, List& l)
{
   list::List q;
   if (0 > pos || pos >= size(l)) return; // la posizione non corrisponde a un elemento presente nella lista, restituisco l'elemento vuoto
   
   if(pos == 0) { // se l'elemento da eliminare è il primo
      q = l->next;
      delete l;
      l = q;
      return;
   }

   q = l;
   int cont = 0;
   while(q != emptyList && cont < pos) { // scorro la lista fino a pos
      q = q->next;
      cont++;
   }
   delete q;
}

void createEmptyl(List& l) {
   l = emptyList;
}

List list::createEmpty()
{
   List l;
   createEmptyl(l);
   return l;
}

/****************************************************************/
Elem list::get(int pos, const List& l)
{
   if (0 > pos || pos >= size(l)) return emptyElem;

   List q = l;
   int cont = 0;

   while(q != emptyList && cont < pos) { // scorro la lista fino a pos
      q = q->next;
      cont++;
   }
   return q->elem; // ritorno il contenuto informativo dell'element in posizione pos
}


/****************************************************************/
bool list::isEmpty(const List& l)
{
   return l == emptyList;
}

/****************************************************************/
void printList(const List& l)
{
   List q = l;
   
   while (q != emptyList) { // man mano che scorro la lista
      cout << " " << q->elem << ","; // stampo i contenuti informativi degli elementi
      q = q->next; 
   }

   cout << endl;
}
