# Treasure Hunt Game (C)

This C program implements a Treasure Hunt game where the player navigates a grid to find a hidden treasure while avoiding traps and collecting power-ups.

## Description

The player starts at a designated point on a grid and must find the treasure located at another random location. The grid also contains traps that the player must avoid, as well as power-ups that increase the player's score. The player can request hints about the treasure's location, but doing so will deduct points. The game has a time limit, and the player must find the treasure before time runs out.  A leaderboard is displayed at the end of the game.

## How to Compile and Run

1.  **Save the code:** Save the C code in a file named `treasure_hunt.c` (or any other name with a `.c` extension).

2.  **Compile:** Open a terminal or command prompt and use a C compiler (like GCC) to compile the code. Navigate to the directory where you saved the file and run the following command:

    ```bash
    gcc treasure_hunt.c -o treasure_hunt
    ```

    This will create an executable file named `treasure_hunt` (or `treasure_hunt.exe` on Windows).

3.  **Run:** Execute the compiled program by running the following command in the terminal:

    ```bash
    ./treasure_hunt  # On Linux/macOS
    treasure_hunt.exe # On Windows
    ```

4.  **Interaction:** The program will guide you through the game. You'll be asked to choose a difficulty level, enter your name, and then use the WASD keys to move your player (P) around the grid.  'H' or 'h' will give you a hint (at a score cost).  The game will end when you find the treasure (T), hit a trap (X), or run out of time.

## Game Controls

*   **W:** Move Up
*   **A:** Move Left
*   **S:** Move Down
*   **D:** Move Right
*   **H:** Get a Hint (costs 5 points)

## Game Logic

*   **Difficulty Levels:** The game has three difficulty levels (Easy, Medium, Hard) that affect the grid size, number of traps, number of power-ups, and the time limit.
*   **Score:** The player starts with a score of 100.  Points are deducted for each move and for using hints. Points are awarded for collecting power-ups. Hitting a trap results in game over.
*   **Traps (X):** Hitting a trap ends the game.
*   **Power-ups (+):** Collecting a power-up increases the player's score by 10.
*   **Treasure (T):** Finding the treasure ends the game and awards the player their current score.
*   **Hint:** The hint tells the player if they are "very close" to the treasure or if the treasure is "still far away."
*   **Time Limit:** The game has a time limit. If the player doesn't find the treasure within the time limit, the game ends.
*   **Leaderboard:** After the game ends, the leaderboard is displayed.
