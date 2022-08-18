/**
 * @file bag.cpp
 * @author DECSAI
 * @note To be implemented by students except function shuffle()
 */

// Alumno1: Álvaro Santana Sánchez
// Alumno2: Miguel García Lopez

#include <cassert>
#include <random>
#include <algorithm>
#include <bag.h>
#include <string>
#include "mi_random.h"
#include "wordlist.h"

///@warning Fill missing #include

using namespace std;

/**
 * @brief Randomly reorders a vector of char
 * @param s Vector of char
 * @param n Number of char in the vector
 * @author DECSAI
 */
void shuffleVector(char *s, int n, int id);

/**
 * @brief Removes a position from a vector and compacts the remaining positions, 
 * thus reducing its size
 * @param s Vector of char
 * @param n Number of chars
 * @param p Position to remove
 * @warning TO BE FULLY IMPLEMENTED. 
 * @warning Check parameters const/&/* and their types
 */
void removeVector(char *s, int &n, int p);


// Main methods

Bag::Bag() {
    nletters = 0;
    id = time(NULL);
}

char Bag::get(int p) const{
    assert(p >= 0 && p < nletters);
    return letters[p];
}

void Bag::set(int p, char c){
    assert(p >= 0 && p < nletters);
    letters[p]=c;
}

void Bag::setRandom(int _id){
    assert(_id >= 0);
    id=_id;
}

void Bag::define(const Language &l){
    string Abecedario;
    int i=0,frecuencia;

    Abecedario = toISO(l.getLetterSet());
    
    for( i=0 ; i<sizeof(Abecedario) ; i++)
    {
        frecuencia = l.getFrequency(Abecedario[i]);
        for( int j=0; j!=frecuencia; j++ ) 
         {
            letters[nletters]=Abecedario[i];
            nletters++;
         }
    }
    
    shuffleVector(letters, nletters, id);
}

string Bag::extract(int n){
    string ext_letters="";
    char aux;
    
    if(nletters < n )
    {
        n=nletters;
    }
    
    for( int i=0; i<n; i++ )
        ext_letters+=letters[i];
   
    for( int i=0; i<n; i++ )
    {
        removeVector(letters, nletters , 0 );
    }
    
    return ext_letters;
}

void Bag::set(std::string s){
    int i=0;
    nletters=0;
    s=toISO(s);
    for( i=0 ; i<s.size() ; i++)
    {
        letters[i]=s[i];
        nletters++;
    }
 
}

int Bag::size() const{
    return nletters;
}

string Bag::to_string() const{
    string remain_letters;
    
    for( int i=0; i<nletters; i++ ){
        remain_letters+=letters[i];
    }
    
    return remain_letters;
}
///@warning Implement all the methods

// Auxiliary functions 

void shuffleVector(char *s, int n, int id) {
    vector <char> aux(s, s+n);
    unsigned seed = id;
    std::shuffle(aux.begin(), aux.end(), std::default_random_engine(seed));
    shuffle(aux.begin(), aux.end(), mi_random(id));
    for (int i=0; i<n; i++)
        s[i] = aux[i];     
}

void removeVector(char *s, int &n, int p) {
    vector <char> aux(s, s+n);
    for( int i=p ; i<n ; i++ )
    {
        s[i]=s[i+1];
    }
    n--;
}
