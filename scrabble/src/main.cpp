/**
 * @file main.cpp
 * @author DECSAI
 * @note To be implemented by students either completely or by giving them
 * key functions prototipes to guide the implementation
 */

#include <string>
#include <cassert>
#include <iostream>
#include <random>
#include <fstream>
#include <cmath>
#include "language.h"
#include "bag.h"
#include "player.h"
#include "move.h"
#include "movelist.h"
#include "tiles.h"
#include "window.h"
#include "AnsiTerminal.h"
using namespace std;


#define ERROR_ARGUMENTS 1
#define ERROR_OPEN 2
#define ERROR_DATA 3
#define GENERAL_ERROR 4
#define PASSWORD "MPALABRADOS-V2"

// Alumno1: Santana Sánchez Álvaro
// Alumno2: García López Miguel

/**
 * @brief Reports an important error and exits the program
 * @param errorcode An integer representing the error detected, which is represented
 * by several symbolic constants: ERROR_ARGUMENTS, ERROR_OPEN y ERROR_DATA
 * @param errorinfo Additional information regarding the error: "" (empty string) for 
 * errors parsing the arguments to main() and, for the case of errors opening or 
 * reading/writing data, the name of the file thas has failed.
 */
void errorBreak(int errorcode, const string & errorinfo);


/**
 * @brief Main function. 
 * @return 
 */
int main(int nargs, char * args[]) {
    Move move;
    Game game;
    int w=-1, h=-1, wait=0, jugada=0, score=0;
    string lang="",ifilematch="", ofilematch="", word, sarg, hand;
    ifstream ifile; ofstream ofile;
    istream *input=&cin; ostream *output=&cout; 
    bool end=false, r_include=false, l_include=false , finish=false, b_include=false,
    open_include=false, save_include=false, coor_include, W=false, H=false, AllGood = true, Primera=true;
    char c;
    
    /// Check arguments
    
    char *arg[7];   
    
    for(int i=1; i<nargs;){
        sarg=args[i++];
        
        if(sarg == "-l"){
            l_include = true;
            arg[0]=args[i++];
        }
        
        else if(sarg == "-r"){
            r_include=true;
            arg[1]=args[i++];
        }
        
        else if(sarg == "-b"){
            b_include=true;
            arg[2]=args[i++];
        }
        else if(sarg == "-open"){
            open_include=true;
            arg[3]=args[i++];
        }
        else if(sarg == "-w"){
            W=true;
            arg[4]=args[i++];
        }
        else if(sarg == "-h"){
            H=true;
            arg[5]=args[i++];
        }
        else if(sarg == "-save"){
            save_include=true;
            arg[6]=args[i++];
        }
        else
            errorBreak(ERROR_ARGUMENTS,"");
    }
   
    game.random = 0;
    game.score = 0;
    
    // Process arguments
    
    if(W&&H)
        coor_include=true;
    else if(!W&&!H)
        coor_include=false;
    else
        errorBreak(ERROR_ARGUMENTS,"");   
    
    if(l_include&&coor_include&&!open_include){
        
        lang = arg[0];
        
        w=atoi(arg[4]);
        h=atoi(arg[5]);
        game.tiles.setSize(w,h);
    }

    else if(open_include){
        Primera = false;
        string contrasenia;
        ifilematch = arg[3];
        input = &ifile;
        ifile.open(ifilematch.c_str());
        
        if(!ifile)
            errorBreak(ERROR_OPEN, ifilematch);
        
        *input>>contrasenia;
        
        if(contrasenia != PASSWORD)
            errorBreak(ERROR_DATA, ifilematch);
        else{
            *input >> score;
            game.score = score;
            *input >> lang;
            *input>>game;
            
            if((*input).eof())
                errorBreak(ERROR_DATA, ifilematch);
        }
        
        game.language.setLanguage(lang);

        ifile.close();
    
    }
    
    else
        errorBreak(ERROR_ARGUMENTS,"");
    
    if (r_include&&!open_include)
        game.random = atoi(arg[1]);
        
    if (b_include&&!open_include)
        game.bag.set(toISO(arg[2])); 

    game.language.setLanguage(lang);

    cout << "ALLOWED LETTERS: " << toUTF(game.language.getLetterSet()) <<endl;/// Show allowed letters
    
    if(!r_include&&!open_include){
        cout << "TYPE SEED (<0 RANDOM): ";
        cin >> game.random; 
    }
    
    if(game.random >= 0)
        game.bag.setRandom(game.random);
    
    if(!b_include&&!open_include)
        game.bag.define(lang);

    // Game's main loop 
    // 1) First set the size of the window according to the size (rows & columns) of
    // the new Tiles

    game.player.add(game.bag.extract(7 - game.player.size()));
    game.setWindowSize();

    
    while (!end) {
        // 2) Given the inner data members, it pretty-prints the screen
        AllGood = true;
         game.doPaint();

        // 3) Reads the movement from cin
        cin >> move;

        word = move.getLetters();

        if (word == "_")
            end = true;

        else {

            if (game.player.isValid(word)) {

                // Checks whether the movement is valid accoring to the letters in player 
                // Finds all the crosswords produced by move
                game.crosswords = game.tiles.findCrosswords(move, game.language);
               

                for (int i = 0; i < game.crosswords.size(); i++) {
                    if (game.crosswords.get(i).getScore() < 0) {
                        AllGood = false;
                    }

                }
                //Checks that the crosswords are valid, that is either has a positive score
                //or produces at least a cross with other existin letters
                // If valid, computes the score and adds it
                if (!AllGood || game.crosswords.size() == 0 || (game.crosswords.size()==1 && game.acceptedmovements.size() >= 1 && game.crosswords.get(0).getLetters().size() == move.getLetters().size())) {
                    if (game.crosswords.size() == 0){
                        game.doBadCrosswords("\t\t..OUT OF THE BOARD.. ");
                    } else {

                        game.doBadCrosswords("Bad crosswords found: ");
                        game.rejectedmovements += move;

                    }
  
                } else {
                    
                    if (game.doConfirmCrosswords("Crosswords found: ")) {
                        game.tiles.add(move);
                        game.player.extract(word);
                        for (int i = 0; i < game.crosswords.size(); i++) {
                            game.score += game.crosswords.get(i).getScore();
                        }
                        game.acceptedmovements += move;
                    }

                }
       
            } else {
                game.crosswords.clear();
                move.setScore(INFEASIBLE_WORD);
                game.crosswords += move;
                game.doBadCrosswords("Infeasible word");
                game.rejectedmovements += move;
            }
            
         
            game.player.add(game.bag.extract(7 - game.player.size()));
        }
    }
 
    // End of game
    // Save file or print screen
    
    cout<<endl;

    if (save_include) {
        ofilematch = arg[6];
        ofile.open(ofilematch.c_str());
        output = &ofile;
        
        *output << PASSWORD;
        *output << game;

        ofile.close();
    }
    else{
        *output << "%%%OUTPUT";
        *output << game;
    }

    return 0;
}

istream & operator>>(istream & is, Game & game) {
    string a;

    is >> game.tiles;

    is >> a>>a;
    game.player.add(toISO(a));
    is >> a >> a;
    game.bag.set(toISO(a));
    
    int x=0,r=0,c=0;
    string l;
    char h;
    Move m;
    is >> x;
 
    for(int i=0; i<x; i++){
        if(is.eof())
            break;
        is >> h >> r >> c >> l;
        m.set(r,c,h,l);
        game.acceptedmovements.add(m);
    }
    
    is >> x;
    
    for(int i=0; i<x; i++){
        if(is.eof())
            break;
        is >> h >> r >> c >> l;
        m.set(r,c,h,l);
        game.rejectedmovements.add(m);
    }
    
}

ostream & operator<<(ostream & os, const Game & game) {
    
        os << endl << game.score << endl << game.language.getLanguage() << endl << game.tiles.getHeight() << " " << game.tiles.getWidth() << endl;
        os << game.tiles;
        os << endl << game.player.size() << " " << toUTF(game.player.to_string());
        os << endl << game.bag.size() << " " << game.bag.to_string() << endl;
        os << game.acceptedmovements.size() << endl;
        
        Move m;
        for(int i=0; i<game.acceptedmovements.size(); i++){
            m = game.acceptedmovements.get(i);
            
            if(m.isHorizontal())
                os << 'H';
            else
                os << 'V';
            
            os << " " << m.getRow() <<  " " << m.getCol() << " " << toUTF(m.getLetters()) << " ";
        }
        
        os << endl << game.rejectedmovements.size() << endl;
        
        for(int i=0; i<game.rejectedmovements.size(); i++){
            m = game.rejectedmovements.get(i);
            
            if(m.isHorizontal())
                os << 'H';
            else
                os << 'V';
            
            os << " " << m.getRow() <<  " " << m.getCol() << " " << toUTF(m.getLetters()) << " ";
        }
        
        os << endl;
}

void errorBreak(int errorcode, const string &errordata) {
    cerr << endl << "%%%OUTPUT" << endl;
    switch(errorcode) {
        case ERROR_ARGUMENTS:
            cerr<<"Error in call. Please use either:"<<endl;
            cerr<< "-l <language> -w <width> -h <height> [-b <bag> -r <randomnumber> -save <matchfile>]"<<endl;
            cerr<< "-open <matchfile> [-save <matchfile>]"<<endl;            
            break;
        case ERROR_OPEN:
            cerr<<"Error opening file "<<errordata << endl;
            break;
        case ERROR_DATA:
            cerr<<"Data error in file "<<errordata << endl;
            break;
        case GENERAL_ERROR:
            cerr<<"Error: "<<errordata << endl;
            break;
    }
    std::exit(1);
}

