/**
 * @file move.cpp
 * @author DECSAI
 * @note To be implemented by students
 */
#include <cstring>
#include <cassert>
#include "move.h"
#include "wordlist.h"
#include <iostream>

using namespace std;

Move::Move(){
    row = 0 ;
    column = 0;        
    ishorizontal = false;
    score = 0;
    letters = "";
}

Move Move::operator =(const Move &orig) {
    
    char h='h';
    
    if(!orig.isHorizontal())
        h='v';
    
    set(orig.getRow(),orig.getCol(),h,orig.getLetters());
    score=orig.getScore();
    
    return *this;
} 

void Move::set(int r, int c, char h, const std::string  &l){
    setRow(r);
    setCol(c);
    setHorizontal(h);
    setLetters(l); 
}

void Move::setRow(int r){
    row = r;
}

void Move::setCol(int c){
    column = c;
}

void Move::setHorizontal(char h){
    if(h == 'H' || h == 'h')
        ishorizontal = true;
    else
        ishorizontal = false;
}

void Move::setLetters(const string &l){
    letters = normalizeWord(l);
}

void Move::setScore(int s){
    score = s;
}

int Move::getScore() const{
    return score;
}

int Move::getCol() const{
    return column;
}

int Move::getRow() const{
    return row;
}

bool Move::isHorizontal() const{
    return ishorizontal;
}

string Move::getLetters() const{
    return letters;
}

void Move::findScore(const Language &l){
    score=0;
    
    if( l.query(letters) ){
        
        for(int i=0 ; i<letters.size() ; i++)
        {
            score += l.getScore(letters[i]);
        }
    }
    else
        score = UNKNOWN;
}

/**
 * @brief Overload of the insertion operator
 * @param os Output stream (cout)
 * @param m The class to be inserted in the stream
 * @return The output stream (cout)
 */
std::ostream& operator<<(std::ostream& os, const Move &m){
    string h="V";
    
    if(m.ishorizontal)
        h="H";
     
    os<<h<<" "<<m.getRow()<<" "<<m.getCol()<<" "<<toUTF(m.getLetters());
}
/**
 * @brief Overload of the extraction operator
 * @param os Input stream (cin)
 * @param m The class to be extracted from the stream
 * @return The input stream (cin)
 */
std::istream& operator>>(std::istream& is, Move &m){
    char h='h';
    int r,c;
    string l;
    
    is>>h>>r>>c>>l;
        
    m.set(r,c,h,l);
}



