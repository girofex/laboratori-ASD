#include "dictionary.h"

#include <math.h> // sqrt()

using namespace dict;

struct dict::cell {
        Elem	elem;
 	    cell	*next;
};


/****************************************************************/
/*              FUNZIONE NON IMPLEMENTATA                       */
/****************************************************************/
int h1(Key s) // funzione di hash che considera unicamente il valore ascii del primo carattere della chiave (se esiste) e restituisce il resto della divisione di tale valore per tableDim 
{
   int ascii = 0;
   if (s.size()>0) // se la parola è formata da più lettere 
      ascii = int(s[0]); // prendo la prima lettera e ne salvo il codice ascii
   return ascii%tableDim; // ritorno il modulo tra il codice ascii e la dimensione della mia hash table
}


/****************************************************************/
/*              FUNZIONE NON IMPLEMENTATA                       */
/****************************************************************/
int h2(Key s) // funzione di hash che somma il codice ascii di ogni carattere nella chiave e restituisce il resto della divisione di tale somma per tableDim 
{
   int somma = 0; // inizializzo la variabile somma

   for(unsigned int i=0; i < s.size(); i++){ // faccio un ciclo che scorre tutta la parola
      int ascii = int(s[i]); // per ogni lettera della parola ne salvo il codice ascii
      somma += ascii; // e sommo tra di loro tutti i codici ascii
   }

   return somma%tableDim; // ritorno il modulo tra la somma dei codici ascii e la dimensione della mia hash table
}


/****************************************************************/
/*              FUNZIONE NON IMPLEMENTATA                       */
/****************************************************************/
int h3(Key s) // funzione di hash diversa da h1 ed h2, che progettate e implementate voi seguendo qualche criterio che vi sembra ragionevole
{
   int somma = 0; // inizializzo la variabile somma

   for(unsigned int i=0; i < s.size(); i++){ // faccio un ciclo che scorre tutta la parola
      int ascii = int(toupper(s[i])); // per ogni lettera della parola, la rendo maiuscola e ne salvo il codice ascii
      somma += ascii; // e sommo tra di loro tutti i codici ascii
   }

   return somma%tableDim; // ritorno il modulo tra la somma dei codici ascii e la dimensione della mia hash table
}

/****************************************************************/
/*    FUNZIONE DA MODIFICARE PER FARE ESPERIMENTI DIVERSI       */
/****************************************************************/
int h(Key s)
{
   return h1(s); // modificare questa chiamata per sperimentare l'utilizzo delle funzioni di hash h1, h2, h3, definite prima
}


/****************************************************************/
/*              FUNZIONE NON IMPLEMENTATA                       */
/****************************************************************/

Error dict::deleteElem(const Key k, Dictionary& s)
{
   Bucket b = s[h(k)]; // considero il bucket del dizionario in posizione h(k)
   Bucket auxb = nullptr; // considero un bucket ausiliario del dizionario, questa volta vuoto

   while(b != nullptr) { // scorro la lista di collisione
      if((b->elem).key == k) { // se trovo l'elmento che ha come chiave k
         if(auxb != nullptr) // controllo che il bucket ausiliario non sia vuoto
            auxb->next = b->next; // se non lo è aggiusto i puntatori in modo che b si stacchi dalla lista
         else
            s[h(k)] = b->next; // se lo è aggiusto i puntatori e utilizzo la posizione h(k) del dizionario
         
         delete b; // elimino b
         return OK; // l'operazione è anadata a buon fine
      }
      else { // altrimenti vado avanti
         auxb = b;
         b = b->next;
      }
   }
   return FAIL; // se non trovo l'elemento deleteElem non elimina niente e la lista di collisione rimane invariata
}


/****************************************************************/
/*              FUNZIONE NON IMPLEMENTATA                       */
/****************************************************************/
Value dict::search(const Key k, const Dictionary& s)
{	
   Bucket cur = s[h(k)]; // considero il bucket del dizionario in posizione h(k)
   
   while(cur != nullptr){ // scorro la lista di collisione
      if((cur->elem).key == k) // se trovo l'elmento che ha come chiave k
         return (cur->elem).value; // ritorno il valore di quell'elemento
      else
         cur = cur->next; // altrimento vado avanti
   }

   return emptyValue; // se non trovo l'elemento ritorno emptyValue, cioè un valore vuoto equivalente a nessun valore
}


/****************************************************************/
/*              FUNZIONE NON IMPLEMENTATA                       */
/****************************************************************/
Error dict::insertElem(const Key k, const Value v,  Dictionary& s)
{
   if(search(k,s) != emptyValue) // cerco nel dizionario l'elemento con chiave k
      return FAIL; // se c'è mi fermo perché non posso aggiungerlo di nuovo
   
   Bucket b = new cell; // nuovo bucket
   (b->elem).key = k; // il nuovo bucket avrà come chiave k
   (b->elem).value = v; // e come valore v

   Bucket tmp = s[h(k)]; // ora considero il bucket del dizionario in posizione h(k)
   // aggiusto i puntatori
   b->next = tmp;
   s[h(k)] = b;

   //cout << "Inserimento\n";

   return OK; // l'operazione è andata a buon fine
}


/****************************************************************/
Dictionary dict::createEmptyDict()
{
   Bucket* d = new Bucket[tableDim];
   for (int i=0; i < tableDim; ++i)
      d[i]=emptyBucket;
   return d;
}


/****************************************************************/
Dictionary readFromFile(string nome_file)
{
    ifstream ifs(nome_file.c_str()); // apertura di uno stream associato ad un file, in lettura
    if (!ifs) {cout << "\nErrore apertura file, verificare di avere inserito un nome corretto\n"; return createEmptyDict();}  
    return readFromStream(ifs);
}


/****************************************************************/
Dictionary readFromStdin()
{
    cout << "\nInserire una sequenza di linee che rispettano la sintassi key: value.<enter>\nDigitare CTRL^ D per terminare l'inserimento\n";
    Dictionary d = readFromStream((std::cin));
// Questa serve per aggirare un bug di alcune versioni attuali di glibc.
    clearerr(stdin);
    return d;
}


/****************************************************************/
Dictionary readFromStream(istream& str)
{
    Dictionary d = createEmptyDict();     
    string key, kcopy;
    string value;
    getline (str, key, ':'); 
    getline (str, value); 
    while (!str.eof())  
        {     
        kcopy = key;
        removeBlanksAndLower(kcopy);   
        insertElem(kcopy, value, d); // FINCHE' NON IMPLEMENTATE LA INSERTELEM, NON PUO' FUNZIONARE CORRETTAMENTE: la insertElem e' la prima funzione che dovete implementare
        getline (str, key, ':'); 
        getline (str, value); 
        }
    str.clear();
    return d;
}


/****************************************************************/
int printBucket(Bucket b) // stampa il contenuto di un bucket e restituisce il numero di elementi contenuti nel bucket stesso
{  
   int counter = 0;
   while(b!=emptyBucket) {
        counter++;
        cout << (b->elem).key << ": " << (b->elem).value << "\n"; 
	b=b->next;
	}
   return counter;	
}


/****************************************************************/
void print(const Dictionary& s)
// stampa il contenuto del dizionario ed anche informazioni su come sono stati organizzati gli elementi
{
int bucketDim[tableDim];
int totalElem = 0;
for (unsigned int i=0; i<tableDim; ++i)
   {
    cout << "\nBucket " << i << "\n\n";
    bucketDim[i] = printBucket(s[i]);
    totalElem += bucketDim[i]; 
   }
float means = totalElem/(float)tableDim;
float standardDevSum = 0;
for (unsigned int i=0; i<tableDim; ++i)
     standardDevSum += (bucketDim[i]-means)*(bucketDim[i]-means);
   
/* Lo scarto tipo, deviazione standard, o scarto quadratico medio e' un indice di dispersione statistico, vale a dire una stima della variabilita' di una popolazione di dati o di una variabile casuale.
Lo scarto tipo e' uno dei modi per esprimere la dispersione dei dati intorno ad un indice di posizione, quale puo' essere, ad esempio, la media aritmetica. */

cout << "\n===STATISTICHE SULL'ORGANIZZAZIONE DEL DIZIONARIO===\n";

for (unsigned int i=0; i<tableDim; ++i)
   cout << "\nBucket " << i << ": " << bucketDim[i] << " elementi";

cout << "\nIl numero totale di elementi memorizzati e' " << totalElem;
cout << "\nIl numero di bucket nel dizionario e' " << tableDim;
if (tableDim != 0) cout << "\nIl numero atteso di elementi per bucket e' " << means << "\nLa deviazione standard e' " << sqrt(standardDevSum/tableDim);
}



