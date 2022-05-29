#include "dictionary.h"

#include <math.h> // sqrt()

using namespace dict;

struct dict::bstNode {
   Elem keyVal;
   bstNode* leftChild;
   bstNode* rightChild;
};

typedef bstNode* Dictionary;

Dictionary emptyNode = nullptr;
Dictionary emptyDictionary = nullptr;


/****************************************************************/
/*              FUNZIONE NON IMPLEMENTATA                       */
/****************************************************************/

Elem deleteMin(Dictionary& s)
{
   if(s->leftChild == emptyNode) { // se il figlio sinistro è vuoto
      Elem canc = s->keyVal; // dichiaro un nuovo elem e lo rendo uguale al contenuto informativo di s
      Dictionary nodo = s->rightChild; // dichiaro un nuovo dizionario e lo rendo uguale al figlio destro di s
      delete s; // cancello s
      s = nodo; // rendo s uguale al suo figlio destro
      return s->keyVal; // ritorno il nuovo contenuto informativo di s
   }

   else // se il figlio sinistro non è vuoto (quindi se non è il più piccolo)
      return deleteMin(s->leftChild); // chiamo ricorsivamente deleteMin sul figlio sinistro
}

Error dict::deleteElem(const Key k, Dictionary& s)
{
   if(s == emptyDictionary) // se il dizionario è vuoto
      return FAIL; // mi fermo
   
   if((s->keyVal).key == k) { // se la chiave di s è uguale a k, che devo cancellare

   // ho 4 casi
      if((s->leftChild == emptyNode) && (s->rightChild == emptyNode)) { // caso 1: il nodo non ha figli
         delete s; // elimino il nodo
         s = emptyNode; // rendo quel nodo vuoto
      }
      
      else if(s->leftChild == emptyNode) { // caso 2: il nodo ha solo il figlio destro
         Dictionary nodo = s->rightChild; // rendo un nuovo nodo uguale al figlio destro di s
         delete s; // elimino s
         s = nodo; // rendo s uguale al nuovo nodo, quindi al suo figlio destro
      }
      
      else if(s->rightChild == emptyNode) { // caso 3: il nodo ha solo il figlio sinistro
         Dictionary nodo = s->leftChild; // rendo un nuovo nodo uguale al figlio sinistro di s
         delete s; // elimino s
         s = nodo; // rendo s uguale al nuovo nodo, quindi al suo figlio sinistro
      }

      else
         s->keyVal = deleteMin(s->rightChild); // caso 4: il nodo ha entrambi i figli, quindi chiamo la deleteMin sul figlio destro
      
      return OK;
   }

   if((s->keyVal).key > k) // se la chiave di s è maggiore di k
      return dict::deleteElem(k, s->leftChild); // chiamo ricorsivamente la deleteElem sul figlio sinistro

   else // se la chiave di s è minore di k
      return dict::deleteElem(k, s->rightChild); // chiamo ricorsivamente la deleteElem sul figlio destro
}


/****************************************************************/
/*              FUNZIONE NON IMPLEMENTATA                       */
/****************************************************************/
Value dict::search(const Key k, const Dictionary& s)
{	
   if(s == emptyDictionary) // se il dizionario è vuoto
      return emptyValue; // ritorno nullptr

   if((s->keyVal).key == k) // se la chiave di s è uguale a k
      return (s->keyVal).value; // ritorno il valore di s

   if((s->keyVal).key > k) // se la chiave di s è maggiore di k
      return dict::search(k, s->leftChild); // chiamo ricorsivamente la search sul figlio sinistro

   else // se la chiave di s è minore di k
      return dict::search(k, s->rightChild); // chiamo ricorsivamente la search sul figlio destro
}


/****************************************************************/
/*              FUNZIONE NON IMPLEMENTATA                       */
/****************************************************************/
Error dict::insertElem(const Key k, const Value v,  Dictionary& s)
{
   if(s == emptyDictionary) { // se il dizionario è vuoto
      bstNode *nodo = new bstNode; // creo un nuovo nodo 
      (nodo->keyVal).value = v; // rendo il valore del nodo uguale a v
      (nodo->keyVal).key = k; // rendo la chiave del nodo uguale a k
      nodo->leftChild = nullptr; // rendo il figlio sinistro vuoto
      nodo->rightChild = nullptr; // rendo il figlio destro vuoto

      s = nodo; // rendo il dizionario s uguale al nuovo nodo

      return OK;
   }
// se il dizionario esiste e non è vuoto
   else if((s->keyVal).key > k) // se la chiave di s è maggiore di k
      return dict::insertElem(k, v, s->leftChild); // chiamo ricorsivamente la search sul figlio sinistro

   else if((s->keyVal).key < k) // se la chiave di s è minore di k
      return dict::insertElem(k, v, s->rightChild); // chiamo ricorsivamente la search sul figlio destro

   return FAIL; // se l'algoritmo non incontra nessuna di queste situazioni, ritorno FAIL
}


/****************************************************************/
Dictionary dict::createEmptyDict()
{
   return emptyDictionary;
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
void print(const Dictionary& s)
  // stampa seguendo una visita ordinata di un albero binario, ovvero una visita in profondita' simmetrica: visita sx, padre, dx.
{
   if(s == emptyDictionary) // controllo che il dizionario non sia vuoto
      return; // se è vuoto mi fermo

   if(s->leftChild != emptyNode) // controllo che il figlio sinistro esista
      print(s->leftChild); // se esiste chiamo ricorsivamente print su di esso per stamparlo prima del padre
   
   cout << (s->keyVal).key << ": " << (s->keyVal).value << "\n"; // stampo l'elementoo corrente in forma "key: value"

   if (s->rightChild != emptyNode) // controllo che il figlio destro esista
      print(s->rightChild); // se esiste chiamo ricorsivamente print su di esso per stamparlo dopo il padre
}

