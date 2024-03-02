# Tic-Tac-Toe
This project build a Tic-Tac-Toe game with two major functions, one is the player mode and other is the optimal move

# Functions
- `task1`: function allows the user to play a board game. It creates a Board object, accepts user input for selecting rows and columns, plays the game accordingly, and displays the board state, score, and ID. The game continues until a player wins or the board is full.

- `task2`:  function is similar to task1(), but it incorporates a BoardTree data structure. It uses the BoardTree to find an optimal move based on a specified depth. It then plays the game accordingly and displays the board state, score, and ID. The game continues until a player wins or the board is full. The function also measures the time taken to complete the task.

- `task3`:  function demonstrates the usage of a BoardHashTable to store and retrieve optimal moves for a given board state. It adds moves to the hash table, retrieves them, and clears the table.

- `task4`:  function is similar to task2(), but it uses the alpha-beta pruning technique (getOptimalMoveAlphaBeta()) to improve the efficiency of finding the optimal move.
