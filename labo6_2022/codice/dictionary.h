#include <iostream>
#include <stdexcept>
#include <vector> 
#include <fstream> 
#include <chrono>          // necessario compilare con -std=c++11
#include <stdlib.h>        // srand, rand
#include <string>          // std::string

#include "string-utility.h"


using namespace std::chrono; 
using namespace std; 

namespace dict {

// Codici di errore

enum Error {OK, FAIL};

// Tipi e costanti

const int tableDim = 1000; // da modificare per fare esperimenti diversi

typedef string Key;        // tipo base 
typedef string Value;      // tipo base 

const Key emptyKey = "###RESERVED KEYWORD### EMPTY KEY";
const Value emptyValue = "###RESERVED KEYWORD### EMPTY VALUE";

typedef struct {
    Key	key;
    Value value;
} Elem;


// Implementazione basata su tabella hash

struct bstNode;

typedef bstNode* Dictionary;

// Dictionary emptyNode = nullptr;
// Dictionary emptyDictionary = nullptr;


Error insertElem(const Key, const Value, Dictionary&);
Error deleteElem(const Key, Dictionary&);
Value search(const Key, const Dictionary&);
Dictionary createEmptyDict();


}  // end namespace Dict


dict::Dictionary readFromFile(string);
dict::Dictionary readFromStdin();
dict::Dictionary readFromStream(istream&);
void print(const dict::Dictionary&);
