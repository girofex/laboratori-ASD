#include "tree-chsib.h"

using namespace tree;

/* Nota: nei commenti usiamo "albero" e "puntatore a un nodo" come sinonimi, scegliendo di volta in volta
la formulazione più adatta a dare la specifica di una funzione. 
Dal punto di vista tecnico, usiamo sempre il tipo Tree sia quando operiamo su un albero, sia quando operiamo, 
a livello concettuale, su un puntatore a un nodo dell'albero. 
Questo approccio e' corretto in quanto un albero (o sottoalbero) è un puntatore a una struct di tipo treeNode:
un albero è identificato univocamente dal puntatore alla sua radice */ 



/*******************************************************************************************************/
// il nodo di un albero è caratterizzato dall'etichetta, il puntatore al primo figlio e il puntatore al prossimo fratello
// NON manteniamo il puntatore al padre, anche se sarebbe comodo per alcune funzioni (ma del tutto inutile per altre)
struct tree::treeNode {
    Label label;    
    treeNode *firstChild;
    treeNode *nextSibling;
};



/*******************************************************************************************************/
// isEmpty restituisce true se l'albero t e' vuoto, false altrimenti
bool tree::isEmpty(const Tree& t)
{ 
   return t == emptyTree;
}

/*******************************************************************************************************/
// funzione ausiliaria: crea un nodo con etichetta l
Tree createNode(const Label l)
{ 
   Tree t = new treeNode;
   t->label = l; // l'etichetta di t diventa l
   t->firstChild = t->nextSibling = emptyTree; // primo figlio = prossimo fratello = vuoto
   return t; // ritorno il nodo
}


// restituisce il nodo che si trova in t ed ha come etichetta l
Tree getNode(const Label l, const Tree& t)
{
   if(isEmpty(t) || l==emptyLabel) // se t è vuoto o l'etichetta è vuota restituisco emptyTree
      return emptyTree;
   
   if(t->label == l) // se l'etichetta di t è quella cercata, restituisco t
      return t;

   Tree auxt = t->firstChild; 
   while(!isEmpty(auxt)) { // se non ha ancora fatto return controllo che il primo figlio di t esista
      if(getNode(l, auxt) == emptyTree) // se il nodo in questione è vuoto 
         auxt = auxt->nextSibling; // passo al prossimo fratello
      else
         return getNode(l, auxt); // altrimenti chiamo getNode ricorsivamente per ritornare infine il nodo
   }
   return emptyTree;
}

// funzione ausiliaria: ritorna true se t ha un figlio con etichetta l, false altrimenti
bool hasChild(const Label l, const Tree& t) {
   if(isEmpty(t))
      return false;

   Tree child = t->firstChild; // child è il primo figlio di t

   while(!isEmpty(child)) { // finchè child non è vuoto
      if(child->label == l) // se l'etichetta di child è l ritorno true
         return true;
      else
         child = child->nextSibling; // altrimenti passo al prossimo fratello
   }
   return false;
}

/*******************************************************************************************************/
// addElem aggiunge il nodo etichettato con labelOfNodeToAdd come
// figlio del nodo etichettato con labelOfNodeInTree.
// Caso particolare: aggiunta della radice, che si ottiene
// con labelOfNodeInTree uguale a emptyLabel (= nessun padre), e ha successo
// solo se l'albero e' vuoto (in caso contrario restituisce FAIL 
// e non aggiunge nulla).
// Restituisce FAIL se non esiste un nodo nell'albero etichettato 
// con labelOfNodeInTree oppure se nell'albero esiste gia' un nodo 
// etichettato con labelOfNodeToAdd; aggiunge e restituisce OK altrimenti
Error tree::addElem(const Label labelOfNodeInTree, const Label labelOfNodeToAdd, Tree& t)
{
   if((t == emptyTree) && (labelOfNodeInTree == emptyLabel)) { // se l'albero è vuoto
      t = createNode(labelOfNodeToAdd);
      return OK;
   }

   if(member(labelOfNodeToAdd, t)) // se il nodo con quell'etichetta esiste già in t non aggiungo niente
      return FAIL;

   Tree auxt = getNode(labelOfNodeInTree, t); // recupero il puntatore al nodo dell'albero che ha etichetta labelOfNodeInTree
   
   if (auxt == emptyTree)
      return FAIL; // nell'albero non esiste un nodo con etichetta labelOfNodeInTree, restituisco FAIL
   
   else { // creo il figlio con etichetta labelOfNodeToAdd e lo aggiungo a auxt
      Tree child = createNode(labelOfNodeToAdd);
      child->nextSibling = auxt->firstChild;
      auxt->firstChild = child;
   }

   return OK;
}

// funzione ausiliaria
Tree prevSibling(const Label l, const Tree& t)
{ 
   if (isEmpty(t) || isEmpty(t->firstChild) || l==emptyLabel) // se l'albero e' vuoto oppure non ha figli restituisco il nodo vuoto
      return emptyTree;

   if ((t->firstChild)->label == l) // se il nodo ha come primo figlio quello con l'etichetta cercata, non ha fratelli precedenti
      return emptyTree;
   else {
      Tree prev = t->firstChild; // il primo figlio non e' vuoto quindi pongo prev uguale al primo figlio
      Tree curr = prev->nextSibling; // pongo curr uguale al figlio successivo
      Tree auxt;
      while (!isEmpty(curr) && curr->label != l) { // finché curr non è vuoto e la sua etichetta è diversa da l
         prev = curr; // prev diventa curr
         curr = curr->nextSibling; // e curr diventa il suo prossimo fratello
         }   
      if (!isEmpty(curr) && curr->label == l) // se sono uscito dal while perché ho trovato l'etichetta, restituisco il prev
         return prev; 
      else {
         curr = t->firstChild;   //curr diventa primo figlio
         while (!isEmpty(curr)) {  // finchè curr non è vuoto
            auxt = prevSibling(l, curr);  //cerco il fratello precedente
            if (!isEmpty(auxt)) // se il fratello precedente esiste
               return auxt; // lo restituisco
            else 
               curr = curr->nextSibling; // altrimenti passo al prossimo fratello di curr
            }  
         }
      }
   return emptyTree;
}

// funzione ausiliaria
Error deleteElemAux(const Label l, Tree& t)
{
   if(isEmpty(t))
      return FAIL;

   Tree auxt = t->firstChild;
   Tree prev = createEmpty();

   if(hasChild(l, t)) { // se t ha un figlio con etichetta l
      
      while(auxt->label != l) { // finchè l'etichetta del primo figlio di t è diversa da l
         prev = auxt; // scorro
         auxt = auxt->nextSibling;
      }

      Tree last = auxt; // esco dal while e auxt è diventato il nodo da cancellare

      while(!isEmpty(last->nextSibling)) // scorro fino alla fine dei fratelli
         last = last->nextSibling;

      last->nextSibling = auxt->firstChild; // attacco i figli del nodo da cancellare al suo ultimo fratello

      if(isEmpty(prev)) // se non c'è nessun fratello prima vuol dire che quello è il nodo da cancellare
         t->firstChild = (t->firstChild)->nextSibling;
      else
         prev->nextSibling = auxt->nextSibling; // altrimenti lo salto e passo al fratello dopo

      delete auxt; // cancello il nodo da rimuovere

      return OK;
   }

   Tree child = t->firstChild;
   // se t non è il padre del nodo da cancellare
   while(!isEmpty(child)) {
      if(deleteElemAux(l, child) == OK) // chiamo ricorsivamente sui figli di t
         return OK; // finchè cancello
      else
         child = child->nextSibling; // oppure non ci sono più figli
   }
   return FAIL;
}

/*******************************************************************************************************/
// deleteElem (versione iterativa) rimuove dall'albero il nodo etichettato con la Label l
// e collega al padre di tale nodo tutti i suoi figli
// Restituisce FAIL se si tenta di cancellare la radice e questa ha 
// dei figli (non si saprebbe a che padre attaccarli) oppure se non esiste 
// un nodo nell'albero etichettato con la Label; cancella e restituisce OK altrimenti
Error tree::deleteElemI(const Label l, Tree& t)
{
   if(!member(l, t))
      return FAIL;
   
   Tree fatherTree = getNode(father(l,t), t); // recupero il puntatore al padre del nodo da rimuovere
   
   if (isEmpty(fatherTree)) { // se fatherTree e' vuoto, significa che sto tentando di rimuovere la radice
      if(degree(l, t) == 0) {
         delete t;
         t = emptyTree;
      }
      else
         return FAIL;
   }

   else { // altrimenti sto tentando di rimuovere un nodo interno
      Tree nodeToRemove = getNode(l, t);

      Tree prev = fatherTree->firstChild; // il primo figlio non è vuoto quindi pongo prev uguale al primo figlio
      Tree curr = prev->nextSibling; // pongo curr uguale al figlio successivo
      while (!isEmpty(curr)) {
         prev = curr  // avanzo finché curr diventa vuoto
         curr = curr->nextSibling;    
      }

      prev->nextSibling = nodeToRemove->firstChild; // i figli del nodo da rimuovere devono diventare suoi fratelli

      Tree auxt = prevSibling(l, t);

      if(isEmpty(auxt))
         fatherTree->firstChild = (fatherTree->firstChild)->nextSibling;
      else
         fatherTree->nextSibling = nodeToRemove->nextSibling;
      
      delete nodeToRemove; // in ogni caso, alla fine dealloco il nodo da rimuovere
   }
   return OK;  
}



/*******************************************************************************************************/
// deleteElem (versione ricorsiva): stesso comportamento della versione iterativa, ma implementata ricorsivamente
// (può anche non essere ricorsiva la deleteElemR, ma deve fare uso di funzioni ausiliarie ricorsive)
Error tree::deleteElemR(const Label l, Tree& t)
{
   if (!isEmpty(t) && t->label == l) { // il nodo da rimuovere è la radice; si può rimuovere solo se non ha figli
      if (degree(l, t)==0) { // posso rimuovere la radice solo se non ha figli, nel qual caso
         delete t;  // cancello t
         t = emptyTree;   // e t diventa l'albero vuoto
      }
      else 
         return FAIL;   // altrimenti non posso rimuoverla e restituisco FAIL
   }

   return deleteElemAux(l, t);  // se il nodo da rimuovere NON è la radice chiamo la funzione ausiliaria ricorsiva ^
}



/*******************************************************************************************************/
// father restituisce l'etichetta del padre del nodo con etichetta l se il nodo esiste nell'albero 
// (o sottoalbero) t e se il padre esiste. 
// Restituisce la costante emptyLabel altrimenti
Label tree::father(const Label l, const Tree& t)
{
   if(isEmpty(t))
      return emptyLabel;

   if(hasChild(l, t)) // se t è il padre del nodo con etichetta l
      return t->label; // ritorno l'etichetta del padre
   
   Tree auxt = t->firstChild; // se non faccio return passo al primo figlio di t
   Label auxl; // e inizializzo una nuova etichetta

   while(!isEmpty(auxt)) { // finchè il primo figlio di t non è vuoto
      auxl = father(l, auxt); // la nuova etichetta diventa l'etichetta del padre di auxt
      if(auxl != emptyLabel) // se non è vuota 
         return auxl; // la restituisco
      else 
         auxt = auxt->nextSibling; // altrimenti passo al suo prossimo fratello
   }
   return emptyLabel;
}

/*******************************************************************************************************/
// children restituisce una lista di Label, contenente le etichette 
// di tutti i figli nell'albero t del nodo etichettato con l
// La lista può essere implementata usando una qualunque delle strutture dati viste a lezione
// (non un Vector!!!): si riusi una implementazione del TDD list, adattandola 
// a elementi di tipo string (nelle precedenti esercitazioni gli elementi erano int)
list::List tree::children(const Label l, const Tree& t)
{
   Tree auxt = getNode(l, t); // ottengo il nodo con etichetta l
   list::List lst = list::createEmpty(); // creo una lista inizialmente vuota
   
   if(!isEmpty(auxt)) { // se il nuovo nodo non è vuoto
      Tree child = auxt->firstChild; // passo al suo primo figlio
      while(!isEmpty(child)) { // finché il primo figlio non è vuoto
         list::addBack(child->label, lst); // lo aggiungo in coda alla lista creata prima
         child = child->nextSibling; // e passo al suo prossimo fratello
      }
   }
   return lst; // ritorno la lista con tutti i figli di l
}



/*******************************************************************************************************/
// degree restituisce il numero di archi uscenti dal nodo etichettato con l; restituisce -1 se non esiste 
// nessuna etichetta l nell'albero
int tree::degree(const Label l, const Tree& t)
{
   if (!member(l, t)) // se l non esiste in t
      return -1; // restituisco -1
   else 
      return list::size(children(l, t)); // altrimenti restituisco la dimensione della lista contenente i figli di l
}

bool ancestorAux(const Label l, const Tree& t, list::List& lst)
{
   if(isEmpty(t)) 
      return false;

   if(hasChild(l, t)) { // se t è padre di l
      list::addFront(t->label, lst); // aggiungo in testa l'etichetta di t alla lista creata
      return true;
   }

   Tree auxt = t->firstChild; // se non ritorno passo al primo figlio di t

   while(!isEmpty(auxt)) { // fino a quando il primo figlio non è vuoto
      
      if(ancestorAux(l, auxt, lst)) { // chiamando ricorsivamente la funzione
         list::addFront(t->label, lst); // se è vera aggiungo in testa
         return true;
      }

      else
         auxt = auxt->nextSibling; // altrimenti passo al prossimo fratello
   }
   return false;
}

/*******************************************************************************************************/
// ancestors (versione ricorsiva) restituisce una lista di Label, contenente le etichette 
// di tutti gli antenati del nodo l ESCLUSA l'etichetta del nodo stesso
// La lista può essere implementata usando una delle strutture dati viste a lezione
// (non un Vector)
// (può anche non essere ricorsiva la ancestorsR, ma deve fare uso di funzioni ausiliarie ricorsive)
list::List tree::ancestorsR(const Label l, const Tree& t)
{
   list::List lst = list::createEmpty();
   ancestorAux(l, t, lst); // funzione ricorsiva ^
   return lst;
}



/*******************************************************************************************************/
// ancestors (versione iterativa) restituisce una lista di Label, contenente le etichette 
// di tutti gli antenati del nodo l ESCLUSA l'etichetta del nodo stesso
// La lista può essere implementata usando una delle strutture dati viste a lezione
// (non un Vector)
list::List tree::ancestorsI(const Label l, const Tree& t)
{
   list::List lst = list::createEmpty(); // creo una nuova lista inizialmente vuota

   if(tree::father(l, t)= emptyLabel || isEmpty(t)) // se t non è il padre di l oppure t è vuoto
      return lst; // ritorno la lista

   Label rootl = t->label; // se non faccio return considero l'etichetta della radice
   Label auxl = l; // e inizializzo una nuova etichetta, al momento uguale a l

   while(auxl != rootl) { // fino a quando la nuova etichetta è diversa dalla radice (quindi non è arrivata in cima)
      auxl = father(auxl, t); // passo al padre dell'etichetta e pian piano arrivo in cima all'albero
      list::addFront(auxl, lst); // e aggiungo in testa alla lista l'etichetta in questione
   }
   return lst; // ritorno la lista con tutti gli antenati di l
}



/*******************************************************************************************************/
// leastCommonAncestor restituisce l'etichetta del minimo antenato comune 
// ai nodi etichettati con label1 e label2
Label tree::leastCommonAncestor(const Label label1, const Label label2, const Tree& t)
{
   list::List lst1 = ancestorsR(label1, t); // creo due nuove liste che avranno gli antenati delle due etichette 
   list::List lst2 = ancestorsR(label2, t);

   if (list::isEmpty(lst1) || list::isEmpty(lst2)) // se le liste sono vuote ritorno emptyLabel
      return emptyLabel;

   int cont = 0; // inizializzo un contatore

   while(list::get(cont, lst1) == list::get(cont, lst2) && cont < list::size(lst1) && cont < list::size(lst2))
      cont++; // finché hanno gli stessi antenati e il contatore è minore di entrambe le dimensioni delle liste, incremento il contatore
   
   return get(cont-1, lst1); // ritorno l'ultimo antenato uguale che avevano le liste
}



/*******************************************************************************************************/
// member restituisce true se il nodo etichettato con la label l appartiene all'albero t e false altrimenti
bool tree::member(const Label l, const Tree& t)
{ 
   if(isEmpty(t)) // se t è vuoto ritorno false
      return false;
   
   if(t->label == l) // se l'etichetta di t è l ritorno true
      return true;

   Tree auxt = t->firstChild; // se non faccio return passo al primo figlio di t
   while(!isEmpty(auxt)) { // fino a quando non è vuoto
      if(!member(l, auxt)) // chiamo ricorsivamente member e se è falsa
         auxt = auxt->nextSibling; // passo al prossimo fratello
      else
         return true; // altrimenti ritorno true
   }
   return false;
}



/*******************************************************************************************************/
// numNodes restituisce il numero di nodi nell'albero t mediante una visita ricorsiva in depthfirst
int tree::numNodes(const Tree& t)
{
   if(isEmpty(t)) // se t è vuoto restituisco 0
      return 0;

   int nodi = 1; // contatore che parte dalla radice (primo nodo) dell'albero o sottoalbero
   Tree auxt = t->firstChild; // passo al primo figlio di t

   while(auxt != emptyTree) { // finché non è vuoto
      nodi += numNodes(auxt); // sommo il contatore al numero dei nodi
      auxt = auxt->nextSibling; // e passo al prossimo fratello
   }

   return nodi; // ritorno il numero dei nodi
}



/*******************************************************************************************************/
// createEmptyTree restituisce l'albero vuoto
Tree tree::createEmpty()
{
   return emptyTree;
}



/*******************************************************************************************************/
// Visualizzazione possibilmente strutturata come visto a lezione
void printTreeAux(const Tree& t, int profondita) {
   if(t == emptyTree) // controllo che l'albero non sia vuoto
      return; // se è vuoto mi fermo

   for(int i=0; i<profondita; i++)
      cout << "--"; // inserisco tanti "--" in base alla profondità

   cout << t->label << "\n"; // visualizzo l'etichetta di t

   Tree auxt = t->firstChild; // passo al primo figlio
   while(!isEmpty(auxt)) { // finché non è vuoto
      printTreeAux(auxt, profondita+1); // chiamo ricorsivamente incrementando la profondità
      auxt = auxt->nextSibling; // passo al prossimo fratello
   }
}

/*******************************************************************************************************/
void printTree(const Tree& t) {
   printTreeAux(t, 0); // chiamo la funzione ausiliaria sulla profondità 0 (radice)
}


/****************************************************************/
// Questa funzione NON richiede di conoscere come e' fatta la struttura dati che implementa l'albero: usa solo addElem e quindi e' ad 
// un alto livello di astrazione
// Vantaggi: codice compatto, leggibile, riutilizzabile per qualunque implementazione del TDD Tree
// Svantaggi: inefficiente: la addElem cerca ogni volta il nodo con etichetta fatherLabel, mentre basterebbe una chiamata sola per 
// trovarlo e poi si potrebbero attaccare tutti i figli a esso, senza ripetere la dispendiosa ricerca nell'albero; per renderla 
// efficiente dovrei pero' accedere direttamente alla struttura dati chiamando qualche funzione ausiliaria che operi 
// sui nodi dell'albero, rinunciando a scrivere codice modulare, chiaro e ad un ragionevole livello di astrazione... 
// Non esiste un'opzione migliore tra le due: se il codice deve essere efficiente, è opportuno scrivere una funzione ad hoc per 
// una determinata implementazione; se il codice deve essere riutilizzabile, bisogna limitarsi a usare le funzioni offerte dall'
// interfaccia del TDD in modo che la funzione continui ad andare bene anche se cambia l'implementazione
Tree readFromStream(istream& str)
{
    Tree t = createEmpty(); 
    string line;
    Label rootLabel, fatherLabel, childLabel;
    getline(str, line);  
    istringstream instream;            // uso una variabile di tipo istringstream per poter scandire i pezzi di ogni riga usando >>
    instream.clear();
    instream.str(line);
    instream >> rootLabel;             // il primo elemento che si incontra nel file e' l'etichetta della radice, per convenzione su come deve essere fatto il file
    addElem(emptyLabel, rootLabel, t); // la si inserisce nell'albero vuoto, indicando che il padre non c'e' (primo argomento emptyLabel)
    getline(str, line);                // poi si iniziano a scansionare le righe seguenti
    instream.clear();
    instream.str(line);
    while (!str.eof())  
        {        
	instream >> fatherLabel; // in ogni riga del file, il primo elemento e' l'etichetta del nodo padre e gli altri sono le etichette dei figli
        removeBlanksAndLower(fatherLabel); // normalizzo l'etichetta del padre
        while (!instream.eof())            // finche' la riga corrente non e' terminata
            { 
            instream >> childLabel;              // leggo la prossima etichetta
            removeBlanksAndLower(childLabel);    // la normalizzo
            addElem(fatherLabel, childLabel, t); // e la attacco al padre
            }
        getline(str, line);  
        instream.clear();
        instream.str(line);
        }
    str.clear();
    return t;
}



/****************************************************************/
Tree readFromFile(string nome_file)
{
    ifstream ifs(nome_file.c_str()); // apertura di uno stream associato ad un file, in lettura
    if (!ifs) {cout << "\nErrore apertura file, verificare di avere inserito un nome corretto\n"; return createEmpty();}  
    return readFromStream(ifs);
}

