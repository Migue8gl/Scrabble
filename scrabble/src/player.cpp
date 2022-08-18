/**
 * @file player.cpp
 * @author DECSAI
 * @note To be implemented by students
 */
#include <cstring>
#include "player.h"
#include "language.h"
///@warning complete missing #includes
/// See http://www.cplusplus.com/reference/cstring/ for details about cstrings
using namespace std;

void removeCString(char *cstr, int pos);

void sortCString(char *cstr);
Player::Player(){
    letters[0] = '\0';
}

int Player::size() const{
    return strlen(letters);
}

std::string Player::to_string() const{
    string aux_std="";
    for(int i=0 ; i<strlen(letters) ; i++)
    {
        aux_std += letters[i];
    }
    return aux_std;
}

void Player::clear(){
    letters[0] = '\0';
}

bool Player::isValid(const std::string s) const{
    string possible_l;

    for(int i=0; i<MAXPLAYER; i++){
        possible_l+=letters[i];
    }

    int n=possible_l.size();
    int count=0;
    bool valid=false,comp=false;

    for(int i=0; i<s.size(); i++){
        for(int j=0; j<n && !comp;j++){

            if(s[i]==possible_l[j]){
                count++;
                comp=true;
                for(int h=j; h<n-1; h++){
                    possible_l[h]=possible_l[h+1];
                    possible_l[h+1]='\0';
                }
            }
        }
        comp=false;
    }

    if(count==s.size())
        valid=true;

    return valid;
}

bool Player::extract(const std::string s){
    char aux;
    int contador=0;
    bool cadencia1 = true;
    if( !isValid(s) )
    {
        return false;
    }
    else
    {
        for(int i=0; i<s.size(); i++)
        {
            cadencia1=true;

            for(int j=0; j<strlen(letters);j++)
            {
                if(s[i] == letters[j] && cadencia1 == true )
                {
                    removeCString(letters, j);
                    cadencia1 = false;
                    contador++;
                }
            }
        }

        if(contador == s.size() )
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

void Player::add(std::string frombag){
    int tam_letters = strlen(letters);
    int total = tam_letters + frombag.size();
    letters[7] = '\0';
    
    if( total <= 7 )
    {
        for(int i=0 ; i<frombag.size();i++)
        {
        letters[tam_letters+i] = frombag[i];
        letters[tam_letters+i+1] = '\0';
        }
    }
    sortCString(letters);
}
/**
 * @brief Removes a position from a cstring
 * @param cstr The cstring
 * @param pos The position
 * @return The cstring is modified
 * @warning To be fully implemented
 */
void removeCString(char *cstr, int pos){
    cstr[pos] = cstr[strlen(cstr)-1];
    cstr[strlen(cstr)-1] = '\0' ;
}
/**
 * @brief Sort a cstring from A to Z
 * @param cstr The cstring
 * @return The cstring is modified
 * @warning To be fully implemented
 */
void sortCString(char *cstr){
    char aux;
    
    for(int i=0; i<strlen(cstr); i++){
        for(int j=strlen(cstr)-1; j>0; j--){
            
            if(cstr[j]<cstr[j-1]){
            
            aux=cstr[j];
            cstr[j]=cstr[j-1];
            cstr[j-1]=aux;
            }  
        }
    }
}


