/**
 * @file movelist.cpp
 * @author DECSAI
 * @note To be implemented by students
 * @warning Complete the code
 *  */
#include <iostream>
#include "language.h"
#include "movelist.h"
#include "move.h"
#include <fstream>
#include <cassert>

using namespace std;

void Movelist::allocate(int n){
    if( n != nMove )
    {
        Move *m;
        m = new Move[n];

        if(moves != nullptr){
            if(nMove < n){
            for(int i=0; i<nMove; i++)
               m[i]= moves[i];
            }

            else{
                for(int i=0; i<n; i++)
                m[i]= moves[i];
            }

            deallocate();
        }
        moves = m;
        nMove = n;

        m = nullptr;
    }
}

void Movelist::deallocate(){
    delete []moves;
    nMove = 0;
}


void Movelist::copy(const Movelist& ml){
    
    clear();
    
    for(int i=0 ; i < ml.size() ; i++ )
    {   
        add(ml.get(i));
    }

}

Movelist::Movelist(){
    moves = nullptr;
    nMove = 0;
}

Movelist &Movelist::operator+=(const Move &mov){
    add(mov);
    
    return *this;
}

Movelist::Movelist(int nmov){
    moves = new Move [nmov];
    nMove = nmov;
}

Movelist::~Movelist(){
    clear();
}

Movelist& Movelist::operator = (const Movelist &orig) {
    
    if(&orig != this)
        assign(orig);
    
    return *this;
} 



void Movelist::assign (const Movelist& orig){
    copy(orig);
}


Move Movelist::get(int p) const{
    assert(p>=0 && p<nMove);
    return moves[p];
}

int Movelist::getScore() const{
    int puntos = 0;
    
    for(int i=0 ; i < nMove; i++){
        
        if(moves[i].getScore() >= 0)
            puntos += moves[i].getScore();
        
        else
            return -1;
            
    }
        
      return puntos;  
}

void Movelist::set(int p, const Move &m){
    assert(p>=0 && p<nMove);
    moves[p] = m;
}

void Movelist::add(const Move &mov){
    int nuevo = nMove + 1; 
    allocate(nuevo);
    set(nuevo-1,mov);
      
}

void Movelist::clear(){
    
    deallocate();
    moves = nullptr;
    nMove = 0;
    
}

void Movelist::remove(Move mov){
    
    int posicion = find(mov);
    
    if(posicion != -1);
    remove(posicion);
}

int Movelist::find(const Move &mov){
    
    for(int i=0 ; i < nMove ; i++)
    {
        if( mov.getLetters() == moves[i].getLetters() )
        {
            if( mov.getCol() == moves[i].getCol())
            {
                if( mov.getRow() == moves[i].getRow())
                {
                    if( mov.isHorizontal() == moves[i].isHorizontal())
                    {
                        return i;
                    }
                }
            }
        }
    }
        return -1;
}

void Movelist::remove(int p){
     for(int i=p ; i < nMove-1 ; i++)
    {
        moves[i] = moves[i+1];
    }
  
    allocate(nMove-1);
}

void Movelist::zip(const Language &l){
    string palabra;
    for(int i=0; i < nMove; i++){
        palabra=moves[i].getLetters();
        if(palabra.size()<2||!l.query(palabra)){
            remove(moves[i]);
            i--;
        }    
    }
}

/**
 * @brief Overload of the insertion operator
 * @param os Output stream (cout)
 * @param m The class to be inserted in the stream
 * @return The output stream (cout)
 */
std::ostream & operator<<(std::ostream & os, const Movelist & i){
    for(int j=0; j<i.size(); j++)
        os<<i.get(j);
}

/**
 * @brief Overload of the extraction operator
 * @param os Input stream (cin)
 * @param m The class to be extracted from the stream
 * @return The input stream (cin)
 */
std::istream & operator>>(std::istream & is, Movelist &i){
    Move m;
    
    do{
        is>>m;
        if(m.getLetters().size()>1)
            i+=m;
        
    }while(!is.eof() && m.getLetters() != "_");
    
}