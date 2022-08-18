
#include <iostream>
#include <cassert>
#include "move.h"
#include "movelist.h"
#include "tiles.h"

using namespace std;

void Tiles::allocate(int r, int c) {

    char **aux;
    aux = new char* [r];


    for (int i = 0; i < r; i++)
        aux[i] = new char [c];



    for (int i = rows; i < r; i++)
        for (int j = columns; j < c; j++)
            aux[i][j] = EMPTY;

    if (cell != nullptr) {


        for (int i = 0; i < rows; i++)
            for (int j = 0; j < columns; j++)
                aux[i][j] = get(i, j);

        deallocate();

    }


    cell = aux;
    columns = c;
    rows = r;


    aux = nullptr;
}

void Tiles::deallocate() {
    for (int i = 0; i < rows; i++)
        delete [] cell[i];

    delete [] cell;
    cell = nullptr;
    rows = 0;
    columns = 0;

}

void Tiles::setSize(int r, int c) {
    allocate(r, c);
}

void Tiles::set(int r, int c, char l) {
    assert(r < rows && c < columns && r >= 0 && c >= 0);
    cell[r][c] = l;
}

void Tiles::copy(const Tiles& t) {


    if (cell != nullptr)
        deallocate();

    allocate(t.getHeight(), t.getWidth());

    for (int i = 0; i < t.getHeight(); i++)
        for (int j = 0; j < t.getWidth(); j++)
            set(i, j, t.get(i, j));

}

void Tiles::add(const Move& m) {

    string aux = m.getLetters();
    int correction = 0;

    if (get(m.getRow() - 1, m.getCol() - 1) == EMPTY) {

        //HORIZONTAL   
        if (m.isHorizontal()) {
            for (int i = 0; i < aux.size() + correction; i++) {
                if (get(m.getRow() - 1, m.getCol() + i - 1) == EMPTY) {//vacia
                    set(m.getRow() - 1, m.getCol() + i - 1, aux[i - correction ]);
                } else
                    correction++;
            }
        }//VERTICAL
        else {
            for (int i = 0; i < aux.size() + correction; i++) {
                if (get(m.getRow() + i - 1, m.getCol() - 1) == EMPTY) {//vacia
                    set(m.getRow() + i - 1, m.getCol() - 1, aux[i - correction ]);
                } else
                    correction++;
            }
        }
    }

}

void Tiles::print(std::ostream& os) const {
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < columns; j++) {
            os << toUTF(get(i, j)) << " ";
            if (j == columns - 1)
                os << endl;
        }
}

bool Tiles::read(std::istream& is) {
    int r, c;
    char a;

    is >> r>>c;

    setSize(r, c);

    for (int i = 0; i < r; i++)
        for (int j = 0; j < c; j++) {
            is>>a;
            set(i, j, a);
        }
}

char Tiles::get(int r, int c) const {
    assert(r < rows && c < columns && r >= 0 && c >= 0);
    return cell[r][c];
}

int Tiles::getHeight() const {
    return rows;
}

int Tiles::getWidth() const {
    return columns;
}

Tiles::Tiles() {
    cell = nullptr;
    columns = 0;
    rows = 0;
}

Tiles::Tiles(int r, int c) {
    cell = nullptr;
    columns = 0;
    rows = 0;

    allocate(r, c);

}

Tiles::Tiles(const Tiles & orig) {
    cell = nullptr;
    columns = 0;
    rows = 0;
    *this = orig;
}

Tiles &Tiles::operator=(const Tiles& orig) {
    if (&orig != this)
        copy(orig);
}

Tiles::~Tiles() {
    deallocate();
}

Move Tiles::findMaxWord(int r, int c, bool hrz) const {

    bool stop = false;
    Move m;
    string maxword = "", word;
    int min, max;


    if (!hrz) {
        for (int i = r; i >= 0 && !stop; i--) {
            min = i;
            if (get(i, c) == EMPTY) {
                stop = true;
                min++;
            }


        }
        max = getHeight();
    } else {
        for (int i = c; i >= 0 && !stop; i--) {
            min = i;
            if (get(r, i) == EMPTY) {
                stop = true;
                min++;
            }

        }
        max = getWidth();

    }
    stop = false;
    for (int i = min; i < max && !stop; i++) {

        if (!hrz) {
            if (get(i, c) != EMPTY)
                maxword += get(i, c);
            else
                stop = true;
        } else {
            if (get(r, i) != EMPTY)
                maxword += get(r, i);
            else
                stop = true;
        }
    }

    if (!hrz)
        m.set(min + 1, c + 1, 'v', maxword);
    else
        m.set(r + 1, min + 1, 'h', maxword);

    return m;
}

Movelist Tiles::findCrosswords(const Move &m, const Language &l) const {
    Movelist Lista;
    Move Movimiento, mov_auxiliar, mov_auxiliar2;
    bool Continue = true;
    int punta = 0;
    char aux = 'h';
    int i = 0;

    if (!m.isHorizontal())
        aux = 'v';

    Movimiento = m;


    //..............1.CHECKS IF EXITS BOARD_OVERFLOW IN THE COORDENADES.........................
    if (m.getRow() > rows || m.getCol() > columns || m.getRow() <= 0 || m.getCol() <= 0) {
        return Lista;
        //..............2.CHECKS IF THE FIRST POSITION IS FREE...........
    } else if (get(m.getRow() - 1, m.getCol() - 1) != EMPTY) {
        Movimiento.setScore(NOT_FREE);
        //..............HORIZONTAL......................................................
    } else if (m.isHorizontal()) {

        //checks is exits BoardOverflow before add letters
        do {
            if (m.getCol() + i - 1 < columns) { //valida
                if (get(m.getRow() - 1, m.getCol() + i - 1) == EMPTY)//vacia
                    punta++;
            } else {
                Movimiento.setScore(BOARD_OVERFLOW);
                Continue = false;
            }

            i++;
        } while (punta < m.getLetters().size() && Continue);

        if (!Continue) {
            Lista.add(Movimiento);


            //CHECKS FOR CROSWORDS IN CASE OF NOT OVER FLOW
        } else {


            //That will be the Future tiles in case of no fails
            Tiles Futuro(*this);
            Futuro.add(Movimiento);

            mov_auxiliar = Futuro.findMaxWord(m.getRow() - 1, m.getCol() - 1, true);
            mov_auxiliar.findScore(l);
            if (mov_auxiliar.getLetters() != "\0" && mov_auxiliar.getLetters().size() >= 1) {
                //3. CHECKS IF THE CROSSWORD EXIST
                if (!l.query(mov_auxiliar.getLetters()))
                    mov_auxiliar.setScore(NONEXISTENT_WORD);
                //ADDS THE CROSSWORD
                Lista.add(mov_auxiliar);

            }


            for (int i = 0; i < mov_auxiliar.getLetters().size(); i++) {
                //FIND A CROSSWORD ( but discard everyone that isn't new )
                if (this->get(mov_auxiliar.getRow() - 1, mov_auxiliar.getCol() + i - 1) == EMPTY) {
                    mov_auxiliar2 = Futuro.findMaxWord(mov_auxiliar.getRow() - 1, mov_auxiliar.getCol() + i - 1, false);
                    mov_auxiliar2.findScore(l);

                    if (mov_auxiliar2.getLetters() != "\0" && mov_auxiliar2.getLetters().size() > 1) {
                        //3. CHECKS IF THE CROSSWORD EXIST
                        if (!l.query(mov_auxiliar2.getLetters()))
                            mov_auxiliar2.setScore(NONEXISTENT_WORD);
                        //ADDS THE CROSSWORD
                        Lista.add(mov_auxiliar2);
                    }
                }
            }
        } //..............VERTICAL...........................................................
    } else {

        //checks is exits BoardOverflow before add letters
        do {
            if (m.getRow() + i - 1 < rows) { //valida
                if (get(m.getRow() + i - 1, m.getCol() - 1) == EMPTY)//vacia
                    punta++;
            } else {
                Movimiento.setScore(BOARD_OVERFLOW);
                Continue = false;
            }

            i++;
        } while (punta < m.getLetters().size() && Continue);

        if (!Continue)
            Lista.add(Movimiento);

            //CHECKS FOR CROSWORDS IN CASE OF NOT OVER FLOW
        else {

            Tiles Futuro(*this);
            Futuro.add(Movimiento);

            mov_auxiliar = Futuro.findMaxWord(m.getRow() - 1, m.getCol() - 1, false);
            mov_auxiliar.findScore(l);
            if (mov_auxiliar.getLetters() != "\0" && mov_auxiliar.getLetters().size() >= 1) {
                //3. CHECK IF THE CROSSWORD EXIST
                if (!l.query(mov_auxiliar.getLetters()))
                    mov_auxiliar.setScore(NONEXISTENT_WORD);
                //ADDS THE CROSSWORD
                Lista.add(mov_auxiliar);

            }


            for (int i = 0; i < mov_auxiliar.getLetters().size(); i++) {
                //FIND A CROSSWORD ( but discard everyone that isn't new )
                if (this->get(mov_auxiliar.getRow() - 1 + i, mov_auxiliar.getCol() - 1) == EMPTY) {

                    mov_auxiliar2 = Futuro.findMaxWord(mov_auxiliar.getRow() + i - 1, mov_auxiliar.getCol() - 1, true);
                    mov_auxiliar2.findScore(l);

                    if (mov_auxiliar2.getLetters() != "\0" && mov_auxiliar2.getLetters().size() > 1) {
                        //3. CHECKS IF THE CROSSWORD EXIST
                        if (!l.query(mov_auxiliar2.getLetters()))
                            mov_auxiliar2.setScore(NONEXISTENT_WORD);
                        //ADDS THE CROSSWORD
                        Lista.add(mov_auxiliar2);
                    }
                }
            }
        }
    }

    return Lista;
}

std::ostream& operator<<(std::ostream& os, const Tiles &t) {
    t.print(os);
}

std::istream& operator>>(std::istream &is, Tiles &t) {
    t.read(is);
}