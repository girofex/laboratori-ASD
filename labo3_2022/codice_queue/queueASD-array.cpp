#include "queueASD-array.h"

using namespace queue;


/****************************************************************/
/* 			FUNZIONI SULLE CODE 			*/
/****************************************************************/

/****************************************************************/
Queue queue::createEmpty()
{
   Queue q; // creo una nuova coda
   q.size = 0; // inizializzo la sua dimensione
   q.maxsize = BLOCKDIM; // inizializzo la sua capacità massima
   Elem *aux = new Elem[BLOCKDIM]; // creo un nuovo Elem che avrà come dimensione la capacità massima
   q.queue = aux; 
   return q; // ritorno la coda
}

/****************************************************************/
void queue::enqueue(Elem e, Queue& l) // aggiunge e in coda
{
   if (l.size == l.maxsize) // l'array e' pieno, per poter inserire un nuovo elemento devo ridimensionare l'array espandendolo; l'unico modo e' creare un array piu' grande e copiare in esso gli elementi di l
      {
      cout << "\nMessaggio di sistema: Espansione dell'array\n";
      Queue auxQueue; // nuova coda ausiliaria
      auxQueue.size = l.size+1; // la dimensione della nuova coda sarà quella vecchia incrementata di 1
      auxQueue.maxsize = l.maxsize + BLOCKDIM; // la capacità massima della nuova coda sarà quella vecchia + BLOCKDIM
      Elem *aux = new Elem[auxQueue.maxsize]; // nuovo Elem ausiliario con dimensione la capacità massima nuova
      auxQueue.queue = aux;
      for(int i=0; i<l.size; i++)
         auxQueue.queue[i] = l.queue[i]; // copio tutti gli altri elementi nella nuova coda
      auxQueue.queue[auxQueue.size-1] = e; // inserisco e alla fine della coda (da una parte)
      delete[] l.queue; // cancello la coda vecchia
      l = auxQueue;
      return;
      }

   // se la funzione non ha incontrato un return, vuole dire che pos e' nel range corretto e l'array non e' pieno; devo solo posizionare l'elemento in pos e shiftare a destra gli altri, aggiornando la size della lista
   l.size++;
   l.queue[l.size-1] = e;
}


/****************************************************************/
Elem queue::dequeue(Queue& l) // rimuove il primo elemento e lo restituisce
{
   if (isEmpty(l)) // controllo che la coda non sia vuota
      return EMPTYELEM;

   if (l.maxsize-l.size+1==BLOCKDIM && l.maxsize>BLOCKDIM)
   {
      cout << "\nMessaggio di sistema: Contrazione dell'array\n";
      Queue auxQueue; // nuova coda ausiliaria
      auxQueue.size = l.size-1; // la dimensione della nuova coda sarà quella vecchia decrementata di 1
      auxQueue.maxsize = l.maxsize-BLOCKDIM; // la capacità massima della nuova coda sarà quella vecchia - BLOCKDIM
      Elem *aux = new Elem[auxQueue.maxsize];  // nuovo Elem ausiliario con dimensione la capacità massima nuova
      auxQueue.queue = aux;
      int canc = l.queue[0];
      for(int i=0; i<l.size-1; i++)
         auxQueue.queue[i] = l.queue[i+1]; // copio tutti elementi, tranne il primo che elimino, nella nuova coda spostati di uno
      delete[] l.queue; // cancello la coda vecchia
      l = auxQueue; 
      return canc; // ritorno il primo elemento , che ho eliminato
   }

// se la funzione non ha incontrato un return, vuole dire che pos e' nel range corretto e la lista non puo' essere ridotta di dimensione; devo solo shiftare a sinistra gli altri, aggiornando la size della lista.
   int canc = l.queue[0];
   for(int i=1; i<l.size; i++)
      l.queue[i-1]=l.queue[i];
   l.size--;
   return canc;
}


/****************************************************************/
Elem queue::first(Queue& l) // restituisce il primo elemento
{
   if (isEmpty(l)) // controllo che la coda non sia vuota
      return EMPTYELEM;
   return l.queue[0]; // restituisco il primo elemento
}


/****************************************************************/
bool queue::isEmpty(const Queue& l)
{
   return (l.size == 0);
}



/****************************************************************/
Queue readFromFile(string nome_file)
{
    ifstream ifs(nome_file.c_str()); // apertura di uno stream associato ad un file, in lettura
    return readFromStream(ifs);
}


/****************************************************************/
Queue readFromStdin()
{
    cout << "\nInserire una sequenza di numeri separati da spazi seguiti da " << FINEINPUT << " per terminare\n";
    return readFromStream((std::cin));
}


/****************************************************************/
Queue readFromStream(istream& str)
{
    Queue l = createEmpty();        
    int dato;
    str>>dato;
    if (!str) throw runtime_error("Errore inserimento dati\n");
    while (dato!= FINEINPUT)  // assumiamo che il segnale di fine input nel file sia  FINEINPUT
        {     
        enqueue(dato, l);   // aggiunge l'elemento alla fine della coda
        str>>dato;
        if (!str) throw runtime_error("Errore inserimento dati\n");
        }
return l;
}

/****************************************************************/
void print(const Queue& l)
{
cout << endl;
for (int i=0; i<l.size; ++i)
   cout << l.queue[i] << "; "; 
cout << endl;
}
