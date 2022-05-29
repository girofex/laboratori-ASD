#include "stackASD-ptr.h"

using namespace stack;


struct stack::cell {
    Elem	elem;
    cell	*next;
};


/****************************************************************/
bool stack::isEmpty(const Stack& s)
{
return s == EMPTYSTACK;
}


/**/
void stack::push(const Elem x, Stack& s) //aggiunge x come ultimo elemento di s
{
    cell *aux = new cell; // creo una nuova cella
    aux->elem = x; // aggiungo l'elemento
    aux->next = s; // metto l'elemento in testa
    s = aux; // aux diventa il primo elemento
}

/**/
Elem stack::pop(Stack& s) // toglie l'ultimo elemento e lo restituisce
{
    if (isEmpty(s)) // se è vuota
        return EMPTYELEM; // restituisco errore
    Elem aux = s->elem; // salvo il valore del primo elemento
    s->elem = EMPTYELEM; // rendo il primo elemento vuoto
    return aux; // restituisco l'elemento eliminato
}


/**/
Elem stack::top(Stack& s) //restituisce l'ultimo elemento
{
    if (isEmpty(s)) // se è vuota
        return EMPTYELEM; // restituisco errore
    Elem aux = s->elem; // salvo il valore del primo elemento
    return aux; // restituisco l'elemento eliminato
}


/****************************************************************/
Elem stack::top(Stack& s)
{
    if(isEmpty(s)) //se lo stack è vuoto
        return EMPTYELEM;   //restituisco un errore

    Elem aux = s->elem; //salvo l'elemento da togliere in una variabile ausiliaria

    return aux; //resituisco l'elemento
}



Stack readFromFile(string nome_file)
{
    ifstream ifs(nome_file.c_str()); // apertura di uno stream associato ad un file, in lettura
    return readFromStream(ifs);
}



Stack readFromStdin()
{
    cout << "\nInserire una sequenza di numeri separati da spazi seguiti da " <<  FINEINPUT << " per terminare\n";
    return readFromStream((std::cin));
}



Stack readFromStream(istream& str)
{
    Stack s = EMPTYSTACK;
    Elem e;
    str>>e;
    if (!str) throw runtime_error("Errore inserimento dati\n");
    while (e!= FINEINPUT)  // assumiamo che il segnale di fine input nel file sia il numero  FINEINPUT
    {
    push(e, s);
    str>>e;
    if (!str) throw runtime_error("Errore inserimento dati\n");
    }
return s;
}



void print(const Stack& s)
{
cell* aux = s;
cout << endl;
while (aux != EMPTYSTACK)
   {
   cout << aux->elem << "; "; 
   aux = aux->next;
   }
std::cout << endl;
}
