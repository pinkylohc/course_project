#include "boardtree.h"
#include "const.h"


BoardTree::BoardTree(const Board& board){
    root = new BoardNode{board};
}

BoardTree::~BoardTree(){
    delete root;
}

BoardTree* BoardTree::getSubTree(const BoardCoordinate &coords){
    if((root->subTree[coords.row][coords.col]).root == nullptr){ //if sub-tree is empty, build the sub-tree
        Board temp = root->board; 
        if(temp.play(coords)){ //valid move, then build 
            (root->subTree[coords.row][coords.col]).root = new BoardNode(temp);
        }
    }

    return &((root->subTree[coords.row][coords.col]));
}

BoardOptimalMove BoardTree::getOptimalMove(const unsigned int depth){
    if(root == nullptr) return BoardOptimalMove(); //return a illegal move

    if (depth == 0 || root->board.isFinished()){ //no further search or finished board
    BoardCoordinate temp{0,0};
    return BoardOptimalMove(root->board.getBoardScore(), temp);}

    if(BoardHashTable::getInstance().getHashedMove(root->board.getID(), depth).score != ILLEGAL)
        return BoardHashTable::getInstance().getHashedMove(root->board.getID(), depth);
  
    //subtree
    int estimatedscore;
    if(root->board.getCurPlayer() == X) estimatedscore = -15000;
    else  estimatedscore = 15000;
    BoardOptimalMove bestMove;


    for(int i=0; i<BOARD_SIZE; i++){    //loop over each subtree
        for(int j=0; j<BOARD_SIZE; j++){
            BoardCoordinate temp{i,j};
            BoardOptimalMove childMove = getSubTree(temp)->getOptimalMove(depth-1);

            if(childMove.score == ILLEGAL){
                continue;
            
            } //invalid subtree
            
            //find optimal one
            if(root->board.getCurPlayer() == X){ //for maximizer
                if(childMove.score > estimatedscore){
                    estimatedscore = childMove.score;
                    bestMove = BoardOptimalMove(estimatedscore, temp);
                }
            }

            if(root->board.getCurPlayer() == O){ //for minier
                if(childMove.score < estimatedscore){
                    estimatedscore = childMove.score;
                    bestMove = BoardOptimalMove(estimatedscore, temp);
                }
            }

        }
    }

    BoardHashTable::getInstance().updateTable(root->board.getID(), depth, bestMove);
    return bestMove;

}

//******************************************************************************************************
//******************************************************************************************************
//******************************************************************************************************


BoardOptimalMove BoardTree::getOptimalMoveAlphaBeta(const unsigned int depth, int alpha, int beta){
    if(root == nullptr) return BoardOptimalMove(); //return a illegal move

    if (depth == 0 || root->board.isFinished()){ //no further search or finished board
    BoardCoordinate temp{0,0};
    return BoardOptimalMove(root->board.getBoardScore(), temp);}
  
    //subtree
    int estimatedscore;
    if(root->board.getCurPlayer() == X) estimatedscore = -15000;
    else  estimatedscore = 15000;
    BoardOptimalMove bestMove;


    for(int i=0; i<BOARD_SIZE; i++){    //loop over each subtree
        for(int j=0; j<BOARD_SIZE; j++){
            //modified

            BoardCoordinate temp{i,j};
            BoardOptimalMove childMove = getSubTree(temp)->getOptimalMoveAlphaBeta(depth-1, alpha, beta);

            if(childMove.score == ILLEGAL){
                continue;
            
            } //invalid subtree
            
            //find optimal one
            if(root->board.getCurPlayer() == X){ //for maximizer
                if(childMove.score > estimatedscore){
                    estimatedscore = childMove.score;
                    bestMove = BoardOptimalMove(estimatedscore, temp);
                }
                if(estimatedscore > beta) return bestMove;
                if(estimatedscore > alpha) alpha = estimatedscore;
            }

            if(root->board.getCurPlayer() == O){ //for minier
                if(childMove.score < estimatedscore){
                    estimatedscore = childMove.score;
                    bestMove = BoardOptimalMove(estimatedscore, temp);
                }
                if(estimatedscore < alpha) return bestMove;
                if(estimatedscore < beta) beta = estimatedscore;
            }
        }
    }
    return bestMove;
}


