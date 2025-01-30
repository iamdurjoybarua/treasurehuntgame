#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Struct to hold player data for leaderboard
typedef struct {
    char name[20];  // Player's name
    int score;      // Player's score
} Player;

// Function Prototypes
void initializeGrid(char grid[][10], int size, int *playerX, int *playerY, int *treasureX, int *treasureY, int traps[][2], int numTraps, int powerUps[][2], int numPowerUps);
void printGrid(char grid[][10], int size, int playerX, int playerY);
int movePlayer(char direction, int *playerX, int *playerY, int size);
int checkPosition(int playerX, int playerY, int treasureX, int treasureY, int traps[][2], int numTraps, int powerUps[][2], int numPowerUps, int *score);
void leaderboard(Player players[], int count);
void showHint(char grid[][10], int size, int playerX, int playerY, int treasureX, int treasureY);
void countdown(int timeLimit);

int main() {
    char grid[10][10];  // Game grid (10x10 maximum size)
    int gridSize, numTraps, numPowerUps;  // Grid size, number of traps, number of power-ups
    int playerX, playerY, treasureX, treasureY;  // Player position and treasure position
    int traps[20][2], powerUps[10][2];  // Trap and power-up coordinates
    Player players[5];  // Array to store leaderboard (max 5 players)
    int playerCount = 0;  // Number of players

    char playerName[20];  // Player's name
    int difficulty, score = 100, gameRunning = 1;  // Game difficulty, score, game state
    int timeLimit, remainingTime;  // Time limit and remaining time

    srand(time(NULL));  // Seed random number generator

    // Game welcome message and difficulty selection
    printf("Welcome to the Treasure Hunt Game!\n");
    printf("Choose a difficulty level:\n");
    printf("1. Easy (5x5 grid, few traps)\n");
    printf("2. Medium (7x7 grid, more traps)\n");
    printf("3. Hard (10x10 grid, max traps)\n");
    scanf("%d", &difficulty);

    // Set parameters based on difficulty level
    if (difficulty == 1) {
        gridSize = 5;
        numTraps = 5;
        numPowerUps = 2;
        timeLimit = 60;  // 60 seconds for Easy
    } else if (difficulty == 2) {
        gridSize = 7;
        numTraps = 10;
        numPowerUps = 3;
        timeLimit = 90;  // 90 seconds for Medium
    } else {
        gridSize = 10;
        numTraps = 20;
        numPowerUps = 5;
        timeLimit = 120;  // 120 seconds for Hard
    }

    // Get player's name
    printf("Enter your name: ");
    scanf("%s", playerName);

    // Initialize the grid with player, treasure, traps, and power-ups
    initializeGrid(grid, gridSize, &playerX, &playerY, &treasureX, &treasureY, traps, numTraps, powerUps, numPowerUps);

    // Inform the player about the objective
    printf("Find the treasure (T) while avoiding traps (X)!\n");

    // Start countdown timer in a separate thread or loop
    remainingTime = timeLimit;  // Initialize remaining time

    // Game loop
    while (gameRunning && remainingTime > 0) {
        // Print the current grid
        printGrid(grid, gridSize, playerX, playerY);
        printf("Score: %d\n", score);  // Display current score
        printf("Time remaining: %d seconds\n", remainingTime);  // Display remaining time
        printf("Enter your move (W/A/S/D) or H for a hint: ");
        char move;
        scanf(" %c", &move);  // Take player input for move or hint

        // If player asks for a hint
        if (move == 'H' || move == 'h') {
            showHint(grid, gridSize, playerX, playerY, treasureX, treasureY);
            score -= 5;  // Penalize player for using a hint
            continue;  // Skip the rest of the loop and go back to the start
        }

        // Move the player based on input
        if (!movePlayer(move, &playerX, &playerY, gridSize)) {
            printf("Invalid move! Try again.\n");  // Invalid move (out of bounds)
            continue;  // Skip the rest of the loop and go back to the start
        }

        score -= 1;  // Deduct score for every move

        // Check the player's current position
        int status = checkPosition(playerX, playerY, treasureX, treasureY, traps, numTraps, powerUps, numPowerUps, &score);

        if (status == 1) {
            // Player found the treasure
            printf("Congratulations, %s! You found the treasure with a score of %d.\n", playerName, score);
            players[playerCount].score = score;  // Store player score
            snprintf(players[playerCount].name, 20, "%s", playerName);  // Store player name
            playerCount++;  // Increment player count for leaderboard
            gameRunning = 0;  // End the game
        } else if (status == -1) {
            // Player hit a trap
            printf("Oh no! You hit a trap. Game over.\n");
            gameRunning = 0;  // End the game
        }

        remainingTime--;  // Decrease remaining time after each move
    }

    if (remainingTime <= 0) {
        printf("Time's up! You didn't find the treasure in time.\n");
    }

    // Display the leaderboard after game ends
    leaderboard(players, playerCount);

    return 0;  // End the program
}

// Initialize grid with traps, power-ups, and treasure
void initializeGrid(char grid[][10], int size, int *playerX, int *playerY, int *treasureX, int *treasureY, int traps[][2], int numTraps, int powerUps[][2], int numPowerUps) {
    // Fill grid with empty spaces
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            grid[i][j] = '.';  // '.' represents an empty space
        }
    }

    // Set initial player position at (0, 0)
    *playerX = 0;
    *playerY = 0;

    // Place the treasure randomly on the grid
    *treasureX = rand() % size;
    *treasureY = rand() % size;
    grid[*treasureX][*treasureY] = 'T';  // 'T' represents the treasure

    // Place traps randomly, avoiding player and treasure positions
    for (int i = 0; i < numTraps; i++) {
        int x, y;
        do {
            x = rand() % size;
            y = rand() % size;
        } while ((x == *playerX && y == *playerY) || (x == *treasureX && y == *treasureY));
        traps[i][0] = x;
        traps[i][1] = y;
        grid[x][y] = 'X';  // 'X' represents a trap
    }

    // Place power-ups randomly, avoiding player and treasure positions
    for (int i = 0; i < numPowerUps; i++) {
        int x, y;
        do {
            x = rand() % size;
            y = rand() % size;
        } while ((x == *playerX && y == *playerY) || (x == *treasureX && y == *treasureY));
        powerUps[i][0] = x;
        powerUps[i][1] = y;
        grid[x][y] = '+';  // '+' represents a power-up
    }
}

// Print the grid, showing player position, traps, treasure, and power-ups
void printGrid(char grid[][10], int size, int playerX, int playerY) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (i == playerX && j == playerY) {
                printf("P ");  // 'P' represents the player
            } else {
                printf("%c ", grid[i][j]);  // Print grid element (trap, treasure, etc.)
            }
        }
        printf("\n");
    }
}

// Move the player based on input direction (W/A/S/D)
int movePlayer(char direction, int *playerX, int *playerY, int size) {
    int newX = *playerX;
    int newY = *playerY;

    // Update player position based on direction input
    switch (direction) {
        case 'W': case 'w': newX--; break;  // Move up
        case 'S': case 's': newX++; break;  // Move down
        case 'A': case 'a': newY--; break;  // Move left
        case 'D': case 'd': newY++; break;  // Move right
        default: return 0;  // Invalid input
    }

    // Check if the new position is within bounds of the grid
    if (newX >= 0 && newX < size && newY >= 0 && newY < size) {
        *playerX = newX;
        *playerY = newY;
        return 1;  // Valid move
    }
    return 0;  // Invalid move (out of bounds)
}

// Check the player's current position for treasure, traps, or power-ups
int checkPosition(int playerX, int playerY, int treasureX, int treasureY, int traps[][2], int numTraps, int powerUps[][2], int numPowerUps, int *score) {
    // Check if the player found the treasure
    if (playerX == treasureX && playerY == treasureY) {
        return 1;  // Treasure found
    }

    // Check if the player hit a trap
    for (int i = 0; i < numTraps; i++) {
        if (playerX == traps[i][0] && playerY == traps[i][1]) {
            *score -= 20;  // Deduct score for hitting a trap
            return -1;  // Trap hit
        }
    }

    // Check if the player found a power-up
    for (int i = 0; i < numPowerUps; i++) {
        if (playerX == powerUps[i][0] && playerY == powerUps[i][1]) {
            *score += 10;  // Bonus score for power-up
            printf("You found a power-up! Score increased.\n");
        }
    }

    return 0;  // Continue the game
}

// Show a hint to the player about the proximity of the treasure
void showHint(char grid[][10], int size, int playerX, int playerY, int treasureX, int treasureY) {
    if (abs(playerX - treasureX) + abs(playerY - treasureY) <= 2) {
        printf("You're very close to the treasure!\n");
    } else {
        printf("The treasure is still far away.\n");
    }
}

// Display the leaderboard of top players
void leaderboard(Player players[], int count) {
    printf("\nLeaderboard:\n");
    for (int i = 0; i < count; i++) {
        printf("%d. %s - Score: %d\n", i + 1, players[i].name, players[i].score);
    }
}
