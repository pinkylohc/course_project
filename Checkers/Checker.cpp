#include <iostream>

using namespace std;

// Board Constants
const int NUM_ROWS = 6, NUM_COLS = 6;
const int NUM_PIECES = 6;

// Player Constants
const int RED = 0, BLACK = 1;

// Piece Constants
const int EMPTY = 0;
const int RED_MAN = 2;
const int BLACK_MAN = 1;
const int RED_KING = 4;
const int BLACK_KING = 3;

// Constants for the return of the function `int move()`, can also be used in your helper functions
const int INVALID = 0;
const int VALID = 1;
const int CAPTURE = 2;

// Additional Constants for your helper functions, just for your reference and it's not necessary to use
const int RIGHT_CAPTURE = 3;
const int LEFT_CAPTURE = 4;
const int UP_CAPTURE = 5;
const int DOWN_CAPTURE = 6;

// Constants for the return of the function `int checkEndGameConditions()`, you should reuse RED and BLACK for the case that Red or Black wins.
const int TIE = 2;
const int CONTINUE = 3;

// Solver Constant
const int SOLVER_ROUNDS = 3;

// Score for Minimax Algorithm
const int WIN_SCORE = 5;
const int TIE_SCORE = 3;
const int CONTINUE_SCORE = 0; // If you reach the end of the round, the score is CONTINUE_SCORE

// Add this score for each of the capture
const int CAPTURE_SCORE = 1;

// Global Variable to denote the player for current turn, start from RED first
int whoseTurn = RED;

// Starting map for the game
const int ORIGINAL_MAP[NUM_ROWS][NUM_COLS] = {{0, 0, 0, 0, 0, 0},
                                        {1, 1, 1, 1, 1, 1},
                                        {0, 0, 0, 0, 0, 0},
                                        {0, 0, 0, 0, 0, 0},
                                        {2, 2, 2, 2, 2, 2},
                                        {0, 0, 0, 0, 0, 0}};

// Default map for the recursive solver
const int DEFAULT_MAP[NUM_ROWS][NUM_COLS] = {{0, 0, 0, 0, 0, 2},
                                       {0, 0, 0, 0, 0, 1},
                                       {0, 0, 0, 0, 3, 0},
                                       {0, 0, 2, 0, 0, 0},
                                       {2, 0, 0, 1, 0, 0},
                                       {0, 0, 0, 0, 0, 0}};

/**
 * function findOpponent
 * @param player current player, should be RED or BLACK
 * @return RED or BLACK for the current player's opponent
 */
int findOpponent(int player)
{
    return (player + 1) % 2;
}

/**
 * function belongsToDifferentPlayer
 * @param piece1 First piece to be evaluated
 * @param piece2 Second piece to be evaluated
 * @return true if both pieces belongs to the different players, false if they belong to the same player.
 */
bool belongsToDifferentPlayer(int piece1, int piece2)
{
    // This is a small play in the integers - see above constants. Red  results in 0 mod 2,
    // while black always results in 1 mod 2
    if (piece1 != EMPTY && piece2 != EMPTY && piece1 % 2 != piece2 % 2)
    {
        return true;
    }
    return false;
}

/**
 * countBlackPieces counts the number of black pieces remaining on the map - this function is mainly being used for the
 * cases where the initial map given to the recursive_solver isn't a standard 6-piece map.
 * @param map Game map
 * @return # of black pieces (black and black king combined)
 */
int countBlackPieces(const int map[NUM_ROWS][NUM_COLS])
{
    int count = 0;
    // Just iterating over the map, no specific order required for this case.
    for (int i = 0; i < NUM_ROWS; ++i)
    {
        for (int j = 0; j < NUM_COLS; ++j)
        {
            // Check if the pieces are the required piece, if so, increment the count
            if (map[i][j] == BLACK_MAN || map[i][j] == BLACK_KING)
            {
                count++;
            }
        }
    }
    return count;
}

/**
 * countRedPieces counts the number of red pieces remaining on the map - this function is mainly being used for the
 * cases where the initial map given to the recursive_solver isn't a standard 6-piece map.
 * @param map Game map
 * @return # of red pieces (red and red king combined)
 */
int countRedPieces(const int map[NUM_ROWS][NUM_COLS])
{
    int count = 0;
    // Just iterating over the map, no specific order required for this case.
    for (int i = 0; i < NUM_ROWS; ++i)
    {
        for (int j = 0; j < NUM_COLS; ++j)
        {
            // Check if the pieces are the required piece, if so, increment the count
            if (map[i][j] == RED_MAN || map[i][j] == RED_KING)
            {
                count++;
            }
        }
    }
    return count;
}

/**
 * printMap prints the game map in the given format. UNDER NO CIRCUMSTANCES SHOULD YOU BE TOUCHING THIS FUNCTION,
 * OTHERWISE ZINC MIGHT MISGRADE YOUR ASSIGNMENT
 * @param map Game map
 */
void printMap(const int map[NUM_ROWS][NUM_COLS])
{
    cout << "   | A  | B  | C  | D  | E  | F  |" << endl;
    cout << "   -------------------------------" << endl;

    for (int i = 0; i < NUM_ROWS; ++i)
    {

        // See PA description on how rows are in a different order than the array order.
        cout << 6 - i;
        cout << "  |";

        for (int j = 0; j < NUM_COLS; ++j)
        {

            // See lines 10 onwards on what these constants mean.
            switch (map[i][j])
            {
            case EMPTY:
                cout << "    ";
                break;
            case BLACK_MAN:
                cout << "  B ";
                break;
            case RED_MAN:
                cout << "  R ";
                break;
            case BLACK_KING:
                cout << " BK ";
                break;
            case RED_KING:
                cout << " RK ";
                break;
            }
            cout << "|";
        }
        cout << endl;
        cout << "   -------------------------------" << endl;
    }
    cout << "   | A  | B  | C  | D  | E  | F  |" << endl;
}


void copyMap(const int map[NUM_ROWS][NUM_COLS], int map_copy[NUM_ROWS][NUM_COLS])
{
    for (int i = 0; i < NUM_ROWS; i++)
    {
        for (int j = 0; j < NUM_COLS; j++)
        {
            map_copy[i][j] = map[i][j];
        }
    }
}

/**
 * checkForCapturablePieces checks for the map if there exists a capturing opportunity for any given spot on the map
 * The function does not check if the cur[rent]Col[umn] and cur[rent]Row are empty, as we call this function only after
 * a certain move or if there already is a piece in other functions.
 * @param map
 * @param currentRow the current location's row (the location to be checked)
 * @param currentCol the current location's column (the location to be checked)
 * @return true if there exists a capturable piece, false if not.
 */
bool checkCapturablePieces(const int map[NUM_ROWS][NUM_COLS], int currentRow, int currentCol)
{
    // Check if there's a move opportunity i.e. from A3 to C3, capturing the piece at B3
    if (currentCol + 2 < NUM_ROWS && belongsToDifferentPlayer(map[currentRow][currentCol + 1], piece) &&
        map[currentRow][currentCol + 2] == EMPTY)
    {
        return true;
    }
    // Check if there's a move opportunity i.e. from C3 to A3, capturing the piece at B3
    if (currentCol - 2 >= 0 && belongsToDifferentPlayer(map[currentRow][currentCol - 1], piece) &&
        map[currentRow][currentCol - 2] == EMPTY)
    {
        return true;
    }
    // Check if there's a move opportunity i.e. from C1 to C3, capturing the piece at C2
    if (currentRow + 2 < NUM_ROWS && belongsToDifferentPlayer(map[currentRow + 1][currentCol], piece) &&
        map[currentRow + 2][currentCol] == EMPTY && (map[currentRow][currentCol] == BLACK_MAN || map[currentRow][currentCol] == BLACK_KING || map[currentRow][currentCol] == RED_KING))
    {
        return true;
    }

    // Check if there's a move opportunity i.e. from C3 to C1, capturing the piece at C2
    if (currentRow - 2 >= 0 && belongsToDifferentPlayer(map[currentRow - 1][currentCol], piece) &&
        map[currentRow - 2][currentCol] == EMPTY && (map[currentRow][currentCol] == RED_MAN || map[currentRow][currentCol] == RED_KING || map[currentRow][currentCol] == BLACK_KING))
    {
        return true;
    }
    return false;
}

/**
 * Checks if a move from @param currentCol and @param currentRow to
 * @param destinationCol and @param destinationRow is valid.
 *
 * @return INVALID if the move is invalid,
 *         VALID if the move is valid but no capturing exists
 *         RIGHT_CAPTURE if the move is a capture to the right (i.e. from E6 to G6, capturing F6)
 *         LEFT_CAPTURE if the move is a capture to the left (i.e. from G6 to E6. capturing F6)
 *         UP_CAPTURE if the move is a capture upwards (i.e. A3 to C3, capturing B3)
 *         DOWN_CAPTURE if the move is a capture downwards (i.e. C3 to A3, capturing B3)
 *
 */
int checkIfValid(const int map[NUM_ROWS][NUM_COLS], int currentRow, int currentCol, int destinationRow, int destinationCol)
{
    // Check if the current position is out of bounds
    if (currentCol >= NUM_ROWS || currentRow >= NUM_ROWS || currentCol < 0 || currentRow < 0)
    {
        return INVALID;
    }
    // Get the piece from the map
    int piece = map[currentRow][currentCol];

    // Check if the destination is out of bounds
    if (destinationCol >= NUM_ROWS || destinationRow >= NUM_ROWS || destinationCol < 0 || destinationRow < 0)
    {
        return INVALID;
    }
    // Check if there is a piece in the current position
    if (piece == EMPTY)
    {
        return INVALID;
    }

    // Check if there isn't any piece in the destination position.
    if (map[destinationRow][destinationCol] > 0)
    {
        return INVALID;
    }
  
    bool capturable = checkCapturablePieces(map, currentRow, currentCol);

    // If there's a piece that's capturable, the player MUST capture that piece first.
    if (!capturable)
    {
        // Check if the move is to the immediate right, if so, it's a valid move
        if (destinationCol == currentCol + 1 && destinationRow == currentRow)
        {
            return VALID;
        }
        // Check if the move is to the immediate left, if so, it's a valid move
        if (destinationCol == currentCol - 1 && destinationRow == currentRow)
        {
            return VALID;
        }
        // if it's directly to the front (A3 to B3) - This also is valid for BLACK_KINGs.
        if (destinationRow == currentRow - 1 && (piece == RED_MAN || piece == BLACK_KING || piece == RED_KING) && destinationCol == currentCol)
        {
            return VALID;
        }
        // if it's directly to the front (E3 to D3) - This also is valid for RED_KINGs.
        if (destinationRow == currentRow + 1 && (piece == BLACK_MAN || piece == RED_KING || piece == BLACK_KING) &&
            destinationCol == currentCol)
        {
            return VALID;
        }
    }

    // Capturing a piece, this is the helper feature for you
    if (destinationCol == currentCol + 2 && // Check if the move changes by 2 columnns (E to G)
        destinationRow == currentRow &&     // Check if the rows stays the same
        belongsToDifferentPlayer(piece, map[currentRow][currentCol + 1]) &&
        // Check if the piece to be moved is different than the one to be captured
        map[currentRow][currentCol + 1] != EMPTY) // and that the location to be captured isn't empty.)
    {
        return RIGHT_CAPTURE;
    }
    if (destinationCol == currentCol - 2 &&
        destinationRow == currentRow &&
        belongsToDifferentPlayer(piece, map[currentRow][currentCol - 1]) &&
        map[currentRow][currentCol - 1] != EMPTY)
    {
        return LEFT_CAPTURE;
    }
    if (destinationRow == currentRow - 2 &&
        (piece % 2 == 0 || piece == BLACK_KING) &&
        destinationCol == currentCol &&
        belongsToDifferentPlayer(piece, map[currentRow - 1][currentCol]) &&
        map[currentRow - 1][currentCol] != EMPTY)
    {
        return UP_CAPTURE;
    }
    if (destinationRow == currentRow + 2 &&
        (piece % 2 == 1 || piece == RED_KING) &&
        destinationCol == currentCol &&
        belongsToDifferentPlayer(piece, map[currentRow + 1][currentCol]) &&
        map[currentRow + 1][currentCol] != EMPTY)
    {
        return DOWN_CAPTURE;
    }

    // If nothing applies, return INVALID
    return INVALID;
}

/**
 * Moves the piece from @param currentCol (currentColumn and @param currentRow (currentRow) to
 * @param destinationCol (destinationColumn and @param destinationRow (destinationRow)
 * @return INVALID if it wasn't a valid move,
 *         VALID if it's valid but no capturing occurs,
 *         CAPTURE if capturing occurs
 */
int move(int map[NUM_ROWS][NUM_COLS], int currentRow, int currentCol, int destinationRow, int destinationCol)
{
    int move = checkIfValid(map, currentRow, currentCol, destinationRow, destinationCol);
    switch(move){
        case INVALID:
            cout<< "Please enter a valid move"<<endl;
            return INVALID;

        case RIGHT_CAPTURE:
            map[destinationRow][destinationCol] = map[currentRow][currentCol];
            map[currentRow][currentCol] = EMPTY;
            map[currentRow][currentCol+1] = EMPTY;
            return CAPTURE;

        case LEFT_CAPTURE:
            map[destinationRow][destinationCol] = map[currentRow][currentCol];
            map[currentRow][currentCol] = EMPTY;
            map[currentRow][currentCol-1] = EMPTY;
            return CAPTURE;

        case UP_CAPTURE: // promotion
            if ((destinationRow == 0) && (map[currentRow][currentCol] == RED_MAN)) map[currentRow][currentCol] = RED_KING;
            if ((destinationRow == 5) && (map[currentRow][currentCol] == BLACK_MAN)) map[currentRow][currentCol] = BLACK_KING;
            map[destinationRow][destinationCol] = map[currentRow][currentCol];
            map[currentRow][currentCol] = EMPTY;           
            map[currentRow-1][currentCol] = EMPTY;
            return CAPTURE;

        case DOWN_CAPTURE: //promotion
            if ((destinationRow == 0 )&& (map[currentRow][currentCol] == RED_MAN)) map[currentRow][currentCol] = RED_KING;
            if ((destinationRow == 5) && (map[currentRow][currentCol] == BLACK_MAN)) map[currentRow][currentCol] = BLACK_KING;
            map[destinationRow][destinationCol] = map[currentRow][currentCol];
            map[currentRow][currentCol] = EMPTY;
            map[currentRow+1][currentCol] = EMPTY;
            return CAPTURE;

        default : //promotion
            if ((destinationRow == 0) && (map[currentRow][currentCol] == RED_MAN)) map[currentRow][currentCol] = RED_KING;
            if ((destinationRow == 5) && (map[currentRow][currentCol] == BLACK_MAN)) map[currentRow][currentCol] = BLACK_KING;
            map[destinationRow][destinationCol] = map[currentRow][currentCol];
            map[currentRow][currentCol] = EMPTY;
            return VALID;
    }
}

/**
 * @param map
 * @param current
 * @param destination
 * @return
 */
bool move(int map[NUM_ROWS][NUM_COLS], char *current, char *destination)
{

    // Convert the input from string to necessary details
    int currentCol = current[0] - 65;
    int currentRow = NUM_ROWS - (int(current[1]) - 48);
    int destinationCol = destination[0] - 65;
    int destinationRow = NUM_ROWS - (int(destination[1]) - 48);

    // Get the piece from the map
    int piece = map[currentRow][currentCol];

    // Check if the owner of the piece has the turn, you still need to check the out-of-bound in checkIfValid()
    if (piece != EMPTY && piece % 2 != whoseTurn && currentCol < NUM_ROWS && currentRow < NUM_ROWS && currentCol >= 0 && currentRow >= 0)
    {
        cout << "This is not your turn now." << endl;
        return false;
    }

    // Execute the move
    int result = move(map, currentRow, currentCol, destinationRow, destinationCol);

    if (result == INVALID)
        return false;
    else if (result == CAPTURE)
        cout << "- Capture happens" << endl;

    whoseTurn = (whoseTurn + 1) % 2;
    return true;
}

/**
 * function checkIfValidMoveExists - just combines checkIfValid and checkCapturablePieces together.
 * @param map
 * @param currentRow the current location's row (the location to be checked)
 * @param currentCol the current location's column (the location to be checked)
 * @return INVALID if there is not valid movement exists for current piece in the location. Otherwise, it will 
 * return CAPTURE if this piece is available to capture. If it can just do a normal move, then return VALID.
 */
int checkIfValidMoveExists(const int map[NUM_ROWS][NUM_COLS], int currentRow, int currentCol)
{

    if (checkCapturablePieces(map, currentRow, currentCol))
    {
        return CAPTURE;
    }
    if (checkIfValid(map, currentRow, currentCol, currentRow + 1, currentCol))
    {
        return VALID;
    }
    if (checkIfValid(map, currentRow, currentCol, currentRow - 1, currentCol))
    {
        return VALID;
    }
    if (checkIfValid(map, currentRow, currentCol, currentRow, currentCol + 1))
    {
        return VALID;
    }
    if (checkIfValid(map, currentRow, currentCol, currentRow, currentCol - 1))
    {
        return VALID;
    }
    return INVALID;
}

/**
 * function checkIfValidMoveExistsForPlayer
 * Iterates all pieces and calls the function checkIfValidMoveExists)
 * @param map
 * @param player current player, should be RED or BLACK
 * @return INVALID if there is not valid movement exists for this player, that will be the end of the game. Otherwise, it will 
 * return CAPTURE if there exist a player's piece that is available to capture. If all the pieces of the player can just do a normal move, then return VALID.
 */
int checkIfValidMoveExistsForPlayer(const int map[NUM_ROWS][NUM_COLS], int player)
{
    int validFlag = INVALID;
    for (int i = 0; i < NUM_ROWS; ++i)
    {
        for (int j = 0; j < NUM_COLS; ++j)
        {
            if (map[i][j] != EMPTY && map[i][j] % 2 == player)
            {
                int valid = checkIfValidMoveExists(map, i, j);
                if (valid == CAPTURE)
                {
                    return CAPTURE;
                }
                if (valid == VALID)
                {
                    validFlag = VALID;
                }
            }
        }
    }
    return validFlag;
}

/**
 * Checks if the game ends (or not)
 * @param map
 * @return RED if red wins
 *         BLACK if black wins
 *         TIE if it's a tie
 *         CONTINUE if it doesn't end (
 */
int checkEndGameConditions(const int map[NUM_ROWS][NUM_COLS])
{

    if(countRedPieces(map) == 0 || checkIfValidMoveExistsForPlayer(map, RED) == INVALID)
    {
        cout<<"BLACK WINS"<<endl;
        return BLACK;
    }
    if(countBlackPieces(map) == 0 || checkIfValidMoveExistsForPlayer(map, BLACK) == INVALID)
    {
        cout<<"RED WINS"<<endl;
        return RED;
    }
    if((checkIfValidMoveExistsForPlayer(map, RED) == INVALID && checkIfValidMoveExistsForPlayer(map, BLACK) == INVALID)||
    (countRedPieces(map) == 1 && countBlackPieces(map) == 1 && checkIfValidMoveExistsForPlayer(map, RED) != CAPTURE && checkIfValidMoveExistsForPlayer(map, BLACK) != CAPTURE))
    {
        cout<<"TIE"<<endl;
        return TIE;
    }
    return CONTINUE;
}



/**
 * Recursively solve the given map
 * @param map
 * @param initialPlayer the player that wants to know the suggestion from this recursive solver, won't be changed in the recursion
 * @param player current player, should be changed everytime while do the recursion because players will take turns to play.
 * @param rounds the round number we are looking ahead. Round 0 is the original map. 
 * @return maxScore of its children
 */
int recursive_solver(const int map[NUM_ROWS][NUM_COLS], int initialPlayer, int player, int rounds)
{
    int maxScore = 0;
    int maxDistinationCol = -1;
    int maxDistinationRow = -1;
    int maxStartCol = -1;
    int maxStartRow = -1;

    if (rounds == 0)
    {
        cout << "--Round: " << rounds << " -- "
             << "Solve for " << (initialPlayer == RED ? "Red--" : "Black") << "-----" << endl;
        printMap(map);
        cout << "----------------------------------" << endl;
        cout << endl;
    }
    else
    {
        cout << "--Round: " << rounds << "--After " << (player != RED ? "Red's  " : "Black's") << " Action--" << endl;
        printMap(map);
        cout << "----------------------------------" << endl;
        cout << endl;
    }

    if (rounds >= SOLVER_ROUNDS) // case for round 3
    {
        cout << "Already reached the end of rounds we look ahead" << endl;
        // cout << "Score: " << 0 << endl;
        cout << "----------------------------------" << endl;
        cout << endl;
        return CONTINUE_SCORE;
    }
    //-------- DO NOT MODIFY CODE ABOVE. PUT ANY OF YOUR ANSWERS IN BETWEEN THESE TWO COMMENTS --------

    // Base conditions - check if the game ends
    int end = checkEndGameConditions(map);
    if (end == BLACK || end == RED) {
        cout << "----------------------------------" << endl;
        cout<<endl;
        if (end == initialPlayer) return WIN_SCORE; else return WIN_SCORE * -1;}
    if (end == TIE) {
        cout << "----------------------------------" << endl;
        cout<<endl;
        return TIE_SCORE;}

    int copy[NUM_ROWS][NUM_COLS];

    copyMap(map, copy);
    
    
    int startrow = 0, startcol = 0, destinationrow = 0, destinationcol = 0, score = 0; //value for this!!!

    int numberPiece = (player == RED? countRedPieces(map) : countBlackPieces(map));
     //for(int i = 0; i < numberPiece;i++){
        //cout<<rounds<<"round and i"<<i<<endl;
        for (int k = 0; k < NUM_ROWS; k++){
            //cout<<k;
            for (int j = 0; j < NUM_COLS; j++){
                if(player == RED)
                    if(copy[k][j] == RED_MAN || copy[k][j] == RED_KING) {startrow = k; startcol = j; }
                else continue;
                if(player == BLACK)
                    if(copy[k][j] == BLACK_MAN || copy[k][j] == BLACK_KING) {startrow = k; startcol = j; }
                else continue;
                
        
        
        if(checkCapturablePieces(copy, startrow, startcol) == true){
            if (checkIfValid(copy, startrow, startcol, startrow+2, startcol)){//move in position 1
                //cout<<"position 1"<<endl;
                int store = copy[startrow+1][startcol];
                int store2 = copy[startrow][startcol];
                move(copy, startrow, startcol, startrow+2, startcol);
                if(player == initialPlayer) score = CAPTURE_SCORE + recursive_solver(copy, initialPlayer, findOpponent(player), rounds+1);
                    else score = (-1 * CAPTURE_SCORE) + recursive_solver(copy, initialPlayer, findOpponent(player), rounds+1); 
                //cout<<score<<endl;
                if(score > maxScore) {
                    maxScore = score; 
                    maxStartRow = startrow; maxStartCol = startcol; 
                    maxDistinationRow = startrow+2; maxDistinationCol = startcol;} 
                copy[startrow][startcol] = store2;
                copy[startrow+1][startcol] = store;
                copy[startrow+2][startcol] = EMPTY;  
            }

            if (checkIfValid(copy, startrow, startcol, startrow-2, startcol)){//move in position 2
                //cout<<"position 2"<<endl;
                int store = copy[startrow-1][startcol];
                int store2 = copy[startrow][startcol];
                move(copy, startrow, startcol, startrow-2, startcol);
                if(player == initialPlayer) score = CAPTURE_SCORE + recursive_solver(copy, initialPlayer, findOpponent(player), rounds+1);
                    else score = (-1 * CAPTURE_SCORE) + recursive_solver(copy, initialPlayer, findOpponent(player), rounds+1);  
                //cout<<score<<endl;
                if(score > maxScore) {
                    maxScore = score; 
                    maxStartRow = startrow; maxStartCol = startcol; 
                    maxDistinationRow = startrow-2; maxDistinationCol = startcol;}         
                copy[startrow][startcol] = store2;
                copy[startrow-1][startcol] = store;
                copy[startrow-2][startcol] = EMPTY;
  
            }

            if (checkIfValid(copy, startrow, startcol, startrow, startcol+2)){//move in position 3
                //cout<<"position 3"<<endl;
                int store = copy[startrow][startcol+1];
                int store2 = copy[startrow][startcol];
                move(copy, startrow, startcol, startrow, startcol+2);
                if(player == initialPlayer) score = CAPTURE_SCORE + recursive_solver(copy, initialPlayer, findOpponent(player), rounds+1);
                    else score = (-1 * CAPTURE_SCORE) + recursive_solver(copy, initialPlayer, findOpponent(player), rounds+1); 
                //cout<<score<<endl;
                if(score > maxScore) {
                    maxScore = score; 
                    maxStartRow = startrow; maxStartCol = startcol; 
                    maxDistinationRow = startrow; maxDistinationCol = startcol+2;}
                copy[startrow][startcol] = store2;
                copy[startrow][startcol+1] = store;
                copy[startrow][startcol+2] = EMPTY;
                
                }

            if (checkIfValid(copy, startrow, startcol, startrow, startcol-2)){//move in position 4
                //cout<<"position 4"<<endl;
                int store = copy[startrow][startcol-1];
                int store2 = copy[startrow][startcol];
                move(copy, startrow, startcol, startrow, startcol-2);
                if(player == initialPlayer) score = CAPTURE_SCORE + recursive_solver(copy, initialPlayer, findOpponent(player), rounds+1);
                    else score = (-1 * CAPTURE_SCORE) + recursive_solver(copy, initialPlayer, findOpponent(player), rounds+1);  
                //cout<<score<<endl;
                if(score > maxScore) {
                    maxScore = score; 
                    maxStartRow = startrow; maxStartCol = startcol; 
                    maxDistinationRow = startrow; maxDistinationCol = startcol-2;}  
                copy[startrow][startcol] = store2;
                copy[startrow][startcol-1] = store;
                copy[startrow][startcol-2] = EMPTY;
      
                }

        } else{  //for valid move, no capture
            if (checkIfValid(copy, startrow, startcol, startrow+1, startcol)){//move in position 5
                //cout<<"position 5"<<endl;
                int store = copy[startrow][startcol];
                move(copy, startrow, startcol, startrow+1, startcol);
                score = recursive_solver(copy, initialPlayer, findOpponent(player), rounds+1); 
                //cout<<score<<endl;
                if(score > maxScore) {
                    maxScore = score; 
                    maxStartRow = startrow; maxStartCol = startcol; 
                    maxDistinationRow = startrow+1; maxDistinationCol = startcol;}
                copy[startrow][startcol] = store;
                copy[startrow+1][startcol] = EMPTY; 
                }

            if (checkIfValid(copy, startrow, startcol, startrow-1, startcol)){//move in position 6
                //cout<<"position 6"<<endl;
                int store = copy[startrow][startcol];
                move(copy, startrow, startcol, startrow-1, startcol);
                score =  recursive_solver(copy, initialPlayer, findOpponent(player), rounds+1); 
                //cout<<score<<endl;
                if(score > maxScore) {
                    maxScore = score; 
                    maxStartRow = startrow; maxStartCol = startcol; 
                    maxDistinationRow = startrow-1; maxDistinationCol = startcol;} 
                copy[startrow][startcol] = store;
                copy[startrow-1][startcol] = EMPTY; 
            }

            if (checkIfValid(copy, startrow, startcol, startrow, startcol+1)){//move in position 7
                //cout<<"position 7"<<endl;
                int store = copy[startrow][startcol];
                move(copy, startrow, startcol, startrow, startcol+1);
                score =  recursive_solver(copy, initialPlayer, findOpponent(player), rounds+1); 
                //cout<<score<<endl;
                if(score > maxScore) {
                    maxScore = score; 
                    maxStartRow = startrow; maxStartCol = startcol; 
                    maxDistinationRow = startrow; maxDistinationCol = startcol+1;} 
                copy[startrow][startcol] = store;
                copy[startrow][startcol+1] = EMPTY; 
                }

            if (checkIfValid(copy, startrow, startcol, startrow, startcol-1)){//move in position 8
                //cout<<"position 8"<<endl;
                int store = copy[startrow][startcol];
                move(copy, startrow, startcol, startrow, startcol-1);
                score = recursive_solver(copy, initialPlayer, findOpponent(player), rounds+1); 
                //cout<<score<<endl;
                if(score > maxScore) {
                    maxScore = score; 
                    maxStartRow = startrow; maxStartCol = startcol; 
                    maxDistinationRow = startrow; maxDistinationCol = startcol-1;}
                copy[startrow][startcol] = store;
                copy[startrow][startcol-1] = EMPTY; 
                }          
            }
            }
        }
    //}

    // cout << "round " << rounds << "Max Score: " << maxScore << endl;
    // cout << "round " << rounds << " end" << endl;
    // cout << "----------------------------------" << endl;
    // cout << endl;
    if (rounds == 0)
    {
        cout << "-----------Suggession-------------" << endl;
         //cout << " Max Score: " << maxScore << endl;
        char destinationCol = maxDistinationCol == -1 ? 'X' : maxDistinationCol + 65;
        int destinationRow = maxDistinationRow == -1 ? 0 : NUM_ROWS - maxDistinationRow;
        char startCol = maxStartCol == -1 ? 'X' : maxStartCol + 65;
        int startRow = maxStartRow == -1 ? 0 : NUM_ROWS - maxStartRow;
        if (maxScore == 0)
            cout << "Score can't be maximized. " << endl;
        else
            cout << "Score maximized from " << startCol << startRow << " to " << destinationCol << destinationRow << endl;
        cout << "----------------------------------" << endl;
        cout << endl;
    }
    return maxScore;
}

int main()
{
    bool Continue = true;

    // game_map will be used in the player mode (P)
    // At the start, it will copy the ORIGINAL_MAP to initiate the game.
    int game_map[NUM_ROWS][NUM_COLS];
    copyMap(ORIGINAL_MAP, game_map);

    // solver_map will be used in the solver mode (A)
    // What you set in the DEFAULT_MAP will be copied to the sover_map and solve by recursion
    int solver_map[NUM_ROWS][NUM_COLS];
    copyMap(DEFAULT_MAP, solver_map);

    cout << "Do you want to play (P) , or do you want to test the automatic solver? (A): put either P or A " << endl;
    char input[3];
    cin >> input;
    if (input[0] == 'A' || input[0] == 'a')
    {
        cout << "Would you like to use a (C)ustom map or a (D)efault map? (D/C)" << endl;
        cin >> input;
        if (input[0] == 'D')
        {
            recursive_solver(solver_map, 0, 0, 0);
        }
        else if (input[0] == 'C')
        {
            cout << "Please input your map, 6 numbers in a row, seperated by spaces. " << endl;
            for (int i = 0; i < NUM_ROWS; i++)
            {
                for (int j = 0; j < NUM_COLS; j++)
                {
                    cin >> solver_map[i][j];
                }
            }
            cout << "The map you gave me is this: " << endl;
            printMap(solver_map);
            cout << "Starting the solver now on red: " << endl;
            cout << endl;
            recursive_solver(solver_map, 0, 0, 0);
        }
    }
    else if (input[0] == 'P' || input[0] == 'p')
    {
        while (Continue)
        {
            printMap(game_map);
            if (checkEndGameConditions(game_map) != CONTINUE)
            {
                break;
            };
            char inputMove[3];
            cout << "Enter the piece that you want to move, S to ask suggestion, or Q to quit "
                 << "(player " << (whoseTurn == RED ? "Red" : "Black") << "'s turn)" << endl;
            cin >> inputMove;
            if (inputMove[0] == 'Q' || inputMove[0] == 'q')
                break;
            else if (inputMove[0] == 'S' || inputMove[0] == 's')
                recursive_solver(game_map, whoseTurn, whoseTurn, 0);
            else
            {
                char dest[3];
                cout << "Enter the destination. " << endl;
                cin >> dest;

                move(game_map, inputMove, dest);
            }
        }
    }
    else
    {
        cout << "invalid input" << endl;
    }
}
