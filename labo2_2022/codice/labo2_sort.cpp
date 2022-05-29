#include "labo2_sort.h"


void scambia(vector<int>& v, int i, int j)
{ 
  int tmp = v[j];
  v[j] = v[i];
  v[i] = tmp;
}


/**************************************************************************************
				SELECTION SORT
***************************************************************************************/
void selectionSort(vector<int>& v)
{
   int current_min_index;
   unsigned int size = v.size();
   for (unsigned int i=0; i<size; ++i)
   { 
     current_min_index = i;
     for (unsigned int j=i+1;j<size; ++j)
        if (v[current_min_index] > v[j])
            current_min_index = j;
     scambia(v, i, current_min_index);
   }
}



/**************************************************************************************
				INSERTION SORT
***************************************************************************************/
void insertionSort(vector<int>& v)
{
   int current, prev;
   unsigned int size = v.size();
   for (unsigned int i=1; i<size; ++i)
   { current=i; 
     prev=i-1;
     while(prev>=0 && v[current]<v[prev])
        {
         scambia(v, current, prev);
         --current;
         --prev;
        }
   }
}


/**************************************************************************************
				BUBBLESORT
***************************************************************************************/
void bubbleSort(vector<int>& v)
{
   unsigned int size = v.size();
   bool scambiati;
   for (unsigned int i=1; i<size; ++i)
      {
      scambiati = false;
      for (unsigned int j=0; j<size-i; ++j)
          if(v[j]>v[j+1])
            { 
              scambia(v, j, j+1);
              scambiati = true;
            }
      if (!scambiati) return;
      }
}



/**************************************************************************************
				   MERGESORT
***************************************************************************************/
void fondi(vector<int>& v, unsigned int inizio, unsigned int centro, unsigned int fine)
{
   vector<int> vsinistra, vdestra;

   for (unsigned int i=inizio; i<=centro; ++i)
      vsinistra.push_back(v[i]);

   for (unsigned int i=centro+1; i<=fine; ++i)
      vdestra.push_back(v[i]);

   unsigned int indicesinistra = 0;
   unsigned int maxsin = vsinistra.size();
   unsigned int indicedestra = 0;
   unsigned int maxdes = vdestra.size();
   
   for (unsigned int i=inizio; i<=fine; ++i)
   {
      if (indicesinistra < maxsin && indicedestra < maxdes)
         {
         if (vsinistra[indicesinistra]<vdestra[indicedestra])
            {
             v[i] = vsinistra[indicesinistra];
             indicesinistra++; continue;
             }
         else
            { 
            v[i] = vdestra[indicedestra];
            indicedestra++; continue;
            }
         } 
      
      if (indicesinistra == maxsin && indicedestra < maxdes)
         { 
            v[i] = vdestra[indicedestra];
            indicedestra++; continue;
         }

       if (indicedestra == maxdes && indicesinistra < maxsin)
         { 
            v[i] = vsinistra[indicesinistra];
            indicesinistra++; continue;
         }         
   } 
}

void ms(vector<int>& v, unsigned int inizio, unsigned int fine)
{
    if (inizio < fine)
      {
          unsigned int centro = (inizio+fine)/2;
          ms(v, inizio, centro);
          ms(v, centro+1, fine);
          fondi(v, inizio, centro, fine);
      }
}


void mergeSort(vector<int>& v)
{
   if (v.size() != 0)
      ms(v, 0, v.size()-1);
}


/**************************************************************************************
				QUICKSORT CON SCELTA "BANALE" DEL PIVOT
***************************************************************************************/

//FUNZIONE AUSILIARIA per avere la posizione finale del pivot
int partitionTrivial(vector<int>& v, int l, int r){
   //inizializzo i all'elemento dopo il pivot
   int i = l+1;

   //scorro da dopo il pivot fino all'ultimo elemento del vector
   for(int j=l+1; j<=r; j++){
      //se l'elemento nella posizione successiva a quella del pivot è più piccolo del pivot stesso
      if(v[j]<v[l]){
         scambia(v, i, j); //scambio i 2 elementi
         i++;
      }
   }

   scambia(v, l, i-1);  //scambio gli elementi
   return i-1; //ritorna la posizione del pivot
}

//FUNZIONE AUSILIARIA per poter fare quicksort con altri parametri
void trivialAux(vector<int>& v, int start, int end){
   //controllo che il vector non abbia solo un elemento
   if(start<end){
      //inizializzo il pivot prima al primo elemento, poi all'intero che restituisce partitionTrivial
      int pivot = start;
      pivot = partitionTrivial(v, start, end);

      //chiamo ricorsivamente sulla parte a sinistra rispetto al pivot e poi su quella a destra
      if(pivot!=0)
         trivialAux(v, start, pivot-1);
      
      trivialAux(v, pivot+1, end);
   }
}

void quickSortTrivial(vector<int>& v)
{  
   /* Implementare quickSort banale con partizione in place */
   //controllo che il vector abbia elementi
   if(v.size()!=0)
      trivialAux(v, 0, v.size()-1);
}




/**************************************************************************************
				QUICKSORT RANDOMIZZATO
***************************************************************************************/

//FUNZIONE AUSILIARIA per avere la posizione finale del pivot
int partitionRandom(vector<int>& v, int l, int r){
   //scelgo un indice a caso tra inizio e fine come pivot
   int pivot = l+rand()%(r-l+1);

   //metto il pivot in testa al vector
	scambia(v, pivot, l);

   //inizializzo i all'elemento dopo il pivot
   int i = l+1;

   //scorro da dopo il pivot fino all'ultimo elemento del vector
   for(int j=l+1; j<=r; j++){
      //se l'elemento nella posizione successiva a quella del pivot è più piccolo del pivot stesso
      if(v[j]<v[l]){
         scambia(v, i, j); //scambio i 2 elementi
         i++;
      }
   }

   scambia(v, l, i-1);  //scambio gli elementi
   return i-1; //ritorna la posizione del pivot
}

//FUNZIONE AUSILIARIA per poter fare quicksort con altri parametri
void RandomAux(vector<int>& v, int start, int end){
   //controllo che il vector non abbia solo un elemento
   if(start<end){
      //inizializzo il pivot prima al primo elemento, poi all'intero che restituisce partitionTrivial
      int pivot = start;
      pivot = partitionRandom(v, start, end);

      //chiamo ricorsivamente sulla parte a sinistra rispetto al pivot e poi su quella a destra
      if(pivot!=0)
         RandomAux(v, start, pivot-1);
      
      RandomAux(v, pivot+1, end);
   }
}

void quickSortRandom(vector<int>& v)
{
   /* Implementare quickSort randomizzato con partizione in place */
   //controllo che il vector abbia elementi
   if(v.size()!=0)
      RandomAux(v, 0, v.size()-1);
}
