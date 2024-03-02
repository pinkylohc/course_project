#include "board.h"

Board::Board(const int score[][BOARD_SIZE]):curPlayer(X), id(0){ 
    for(int i = 0; i<BOARD_SIZE; i++){
        for(int j=0; j<BOARD_SIZE; j++){
            cells[i][j] = EMPTY;
            this->score[i][j] = score[i][j];
        }
    }
}

bool Board::isFull() const{
    for(int i = 0; i<BOARD_SIZE; i++){
        for(int j=0; j<BOARD_SIZE; j++){
            if (cells[i][j] == EMPTY) return false;
        }
    }
    return true;
}

bool Board::isFinished() const{
    if(this->isFull()) return true;  // if all the cells are non-empty
    
    //row match
    for(int i = 0; i<BOARD_SIZE; i++){
        Cell rowstart = cells[i][0];
        bool match = (rowstart != EMPTY);
        for(int j=0; j<BOARD_SIZE; j++){
            if(cells[i][j] != rowstart) {match = false; break;}
        }
    if(match) return true;
    }

    //col match
    for(int i = 0; i<BOARD_SIZE; i++){
        Cell colstart = cells[0][i];
        bool match = (colstart != EMPTY);
        for(int j=0; j<BOARD_SIZE; j++){
            if(cells[j][i] != colstart) {match = false; break;}
        }
    if(match) return true;
    }

    //diagonal match
    Cell diastart1 = cells[0][0]; bool match1 = (diastart1 != EMPTY); 
    Cell diastart2 = cells[0][BOARD_SIZE-1]; bool match2 = (diastart2 != EMPTY);
    for(int i = 0; i<BOARD_SIZE; i++){
        if(cells[i][i] != diastart1) match1 = false;
        if(cells[i][BOARD_SIZE-1-i] != diastart2) match2 = false;
        if(!match1 && !match2) return false;
    }
    if(match1 || match2) return true;

    return false; //no match, game not finished
}

int Board::getBoardScore() const{
    if(this->isFinished()){   //check if their is a match
        //row match
        for(int i = 0; i<BOARD_SIZE; i++){
            Cell rowstart = cells[i][0];
            bool match = (rowstart != EMPTY);
            for(int j=0; j<BOARD_SIZE; j++){
                if(cells[i][j] != rowstart) {match = false; break;}
            }
        if(match) {
            if(rowstart == X) return WIN_SCORE; else return -WIN_SCORE;} 
        }

        //col match
        for(int i = 0; i<BOARD_SIZE; i++){
            Cell colstart = cells[0][i];
            bool match = (colstart != EMPTY);
            for(int j=0; j<BOARD_SIZE; j++){
                if(cells[j][i] != colstart) {match = false; break;}
            }
        if(match) {
            if(colstart == X) return WIN_SCORE; else return -WIN_SCORE;}
        }

        //diagonal match
        Cell diastart1 = cells[0][0]; bool match1 = (diastart1 != EMPTY); 
        Cell diastart2 = cells[0][BOARD_SIZE-1]; bool match2 = (diastart2 != EMPTY);
        for(int i = 0; i<BOARD_SIZE; i++){
            if(cells[i][i] != diastart1) match1 = false;
            if(cells[i][BOARD_SIZE-1-i] != diastart2) match2 = false;
            if(!match1 && !match2) break;
        }
        if(match1) {
            if(diastart1 == X) return WIN_SCORE; else return -WIN_SCORE;}
        
        if(match2) {
            if(diastart2 == X) return WIN_SCORE; else return -WIN_SCORE;}
    } //end checking for a metch

    //calculate the score base on the current cells on the board
    int xscore = 0;
    int oscore = 0;
    for(int i = 0; i<BOARD_SIZE; i++){
        for(int j=0; j<BOARD_SIZE; j++){
            if(cells[i][j] == X) xscore += score[i][j];
            else if(cells[i][j] == O) oscore += score[i][j];
        }
    }
    return xscore * getCellWeight(X) - oscore * getCellWeight(O);
}

bool Board::play(const BoardCoordinate& coords){
    if (!coords.isValid() || cells[coords.row][coords.col] != EMPTY) return false; //out of bound or already filled

    cells[coords.row][coords.col] = curPlayer; //update cells
    if(curPlayer == X){
         curPlayer = O;
    } else curPlayer = X;

    id = 0;
    for(int i = 0; i<BOARD_SIZE; i++){ //update id
        for(int j=0; j<BOARD_SIZE; j++){
            switch(cells[i][j]){
                case EMPTY:
                    break;
                case X:
                    id += 1 * (pow(3, (i* BOARD_SIZE + j)));
                    break;
                case O:
                    id += 2 * (pow(3, (i* BOARD_SIZE + j)));
                    break;
            }
        }
    } //update id done
    return true;

}
