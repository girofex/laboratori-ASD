// Implementa le funzioni sulle liste realizzate con strutture doppiamente collegate, circolari, con sentinella, senza ordinamento

// FILE DA MODIFICARE

#include "ASD-doubly-linked-list.h"


using namespace list;

struct list::node {  // descrivo qui, fuori dal namespace, come e' fatta la struttura node: i dettagli interni non saranno visibili dal main
    Elem info;
    node *prev;
    node *next;
};

/*************************************************/
/* Implementazione delle operazioni di utilita'� */
/*************************************************/

void readFromStream(istream& str, List& l) {
    createEmpty(l);
    Elem e;
    str>>e;
    if (!str) throw runtime_error("Errore inserimento dati\n");
    // assumiamo che il segnale di fine input nel file sia  FINEINPUT
    while (e != FINEINPUT)  {     
        addRear(e, l);   // aggiunge l'elemento alla fine della lista
        str>>e;
        if (!str) throw runtime_error("Errore inserimento dati\n");
    }
}

/* legge il contenuto di una lista da file */
void readFromFile(string nome_file, List& l) {
    ifstream ifs(nome_file.c_str()); // apertura di uno stream associato ad un file, in lettura
    readFromStream(ifs, l);
}


/* legge il contenuto di una lista da standard input */
void readFromStdin(List& l) {
   cout << "\nInserire una sequenza di numeri separati da spazi seguiti da " << FINEINPUT << " per terminare\n";
   readFromStream((std::cin), l);
}

/* stampa la lista */
void print(const List& l) {
   List q = l->next;  // q "salta la sentinella" e va alla testa della lista
   while (q != l) {   // avanza finche' non finisce la lista
      cout << q->info << "; ";
      q = q->next; 
    }
}

/**************************************************/
/* Implementazione delle operazioni nel namespace */
/**************************************************/

/* crea la lista vuota */
void list::createEmpty(List& l) {
    l = new node;   //inizializzo la nuova lista
    //aggiusto i puntatori
    l->next = l;
    l->prev = l;
}

/* "smantella" la lista svuotandola */
void list::clear(const List& l) {
    //salto la sentinella e creo una variabile ausiliaria
    node *s = l->next;
    node *aux;

    //finchè non arrivo alla fine della lista
    while(s!=l){
        //dealloco
        aux = s->next;  //salvo il valore della cella successiva
        delete s;   //cancello la cella corrente
        s = aux;    //scambio la posizione
    }

    //faccio puntare alla sentinella
    l->next = l;
    l->prev = l;
}

/* restituisce l'elemento in posizione pos se presente; restituisce un elemento 
che per convenzione si decide che rappresenta l'elemento vuoto altrimenti*/
Elem list::get(int pos, const List& l) {
    //controllo che la posizione sia valida
    if(pos<0 || pos>=size(l))
        return EMPTYELEM;
    
    node *aux = l->next;  //salto la sentinella

    //scorro finchè non arrivo alla posizione cercata
    for(int i=0; i<pos; i++)
        aux = aux->next;
    
    return aux->info; //ritorno il valore nella posizione cercata
}

/* modifica l'elemento in posizione pos, se la posizione e' ammissibile */
void list::set(int pos, Elem e, const List& l) {
    //vado avanti solo se la posizione è valida
    if(pos>=0 && pos<size(l)){
        node *aux = l->next;  //salto la sentinella

        //scorro finchè non arrivo alla posizione cercata
        for(int i=0; i<pos; i++)
            aux = aux->next;

        aux->info = e;    //modifico l'elemento
    }
}

/* inserisce l'elemento in posizione pos, shiftando a destra gli altri elementi */
void list::add(int pos, Elem e, const List& l) {
    //vado avanti solo se la posizione è valida
    if(pos>=0 && pos<=size(l)){
        //controllo se posso usare delle funzioni ausiliarie
        if(pos==0)
            addFront(e, l);
        
        else if(pos==size(l))
            addRear(e, l);
        
        else{
            node *aux = l->next;  //salto la sentinella

            //scorro finchè non arrivo alla posizione cercata
            for(int i=0; i<pos; i++)
                aux = aux->next;

            List to_add = new node;  //creo un nuovo nuovo
            to_add->info = e; //ci metto l'elemento da aggiungere

            //aggiusto i puntatori
            to_add->prev = aux;
            to_add->next = aux->next;
            aux->next = to_add;
            to_add->next->prev = to_add;
        }
    }
}

/* inserisce l'elemento alla fine della lista */
void list::addRear(Elem e,  const List& l) {
    List to_add = new node;  //creo un nuovo nuovo
    to_add->info = e; //ci metto l'elemento da aggiungere

    //sistemo i puntatori
    to_add->prev = l->prev;
    to_add->prev->next = to_add;
    to_add->next = l;
    l->prev = to_add;
}

/* inserisce l'elemento all'inizio della lista */
void list::addFront(Elem e, const List& l) {
    List to_add = new node;  //creo un nodo nuovo
    to_add->info = e; //ci metto l'elemento da aggiungere

    //sistemo i puntatori
    to_add->next = l->next;
    to_add->next->prev = to_add;
    to_add->prev = l;
    l->next = to_add;
}

/* cancella l'elemento in posizione pos dalla lista */
void list::removePos(int pos, const List& l) {
    //vado avanti solo se la posizione è valida
    if(pos>=0 && pos<=size(l)){
        node *aux = l->next;  //salto la sentinella

        //scorro finchè non arrivo alla posizione cercata
        for(int i=0; i<pos; i++)
            aux = aux->next;

        //aggiusto i puntatori
        aux->prev->next = aux->next;
        aux->next->prev = aux->prev;

        //cancello l'elemento
        delete aux;
    }
}

 /* cancella tutte le occorrenze dell'elemento elem, se presenti, dalla lista */
void list::removeEl(Elem e, const List& l) {
    node *aux = l->next;  //salto la sentinella

    //scorro finchè non arrivo alla fine della lista
    while(aux!=l){
        //se nella posizione corrente ho l'elemento da cancellare
        if(aux->info == e)
            delete aux;   //cancello l'elemento

        //scorro
        aux = aux->next;
    }
}

/* restituisce true se la lista e' vuota (ed e' vuota se il next di l, e' l stessa */
bool list::isEmpty(const List& l) {
    return l->next == l;
}

 /* restituisce la dimensione della lista */
int list::size(const List& l) {
    int cont = 0;   //inizializzo un contatore
    List aux = l->next;   //salto la sentinella
    
    //finchè non sono alla fine della lista
    while(aux!=l){
        aux = aux->next;    //scorro
        cont++; //incremento il contatore
    }

    return cont;
}


