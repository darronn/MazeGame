#include <iostream>
#include <vector>
#include <conio.h>   // For _getch() on Windows
#include <cstdlib>   // For rand(), srand()
#include <ctime>     // For time()
#include <stack>

using namespace std;

class MazeGame {
private:
    vector<vector<char>> maze;
    int playerX, playerY;        // Player's coordinates
    int exitX, exitY;            // Exit coordinates
    vector<pair<int, int>> enemies; // Enemy coordinates
    int width, height;
    const int WALL = '#';
    const int PASSAGE = ' ';
    int currentLevel;

public:
    MazeGame(int w = 21, int h = 11) : width(w), height(h) {
        currentLevel = 1;
        srand(static_cast<unsigned>(time(0))); // Seed random number generator
        generateRandomMaze();  // Generate the first maze
    }

    void generateRandomMaze() {
        // Initialize maze with walls
        maze = vector<vector<char>>(height, vector<char>(width, WALL));

        // Depth-First Search (DFS) algorithm with backtracking to generate a random maze
        stack<pair<int, int>> cellStack;
        playerX = 1; playerY = 1; // Start position
        maze[playerY][playerX] = PASSAGE;  // Mark starting cell as passage
        cellStack.push({ playerX, playerY });

        // Direction vectors (up, down, left, right)
        vector<pair<int, int>> directions = { {0, -2}, {0, 2}, {-2, 0}, {2, 0} };

        // DFS to carve the maze
        while (!cellStack.empty()) {
            int cx = cellStack.top().first;
            int cy = cellStack.top().second;

            // Find all unvisited neighbors
            vector<pair<int, int>> unvisitedNeighbors;
            for (auto& dir : directions) {
                int nx = cx + dir.first;
                int ny = cy + dir.second;

                if (nx > 0 && nx < width - 1 && ny > 0 && ny < height - 1 && maze[ny][nx] == WALL) {
                    unvisitedNeighbors.push_back({ nx, ny });
                }
            }

            if (!unvisitedNeighbors.empty()) {
                // Choose a random unvisited neighbor
                int randIndex = rand() % unvisitedNeighbors.size();
                int nx = unvisitedNeighbors[randIndex].first;
                int ny = unvisitedNeighbors[randIndex].second;

                // Carve a path between current cell and chosen neighbor
                maze[(cy + ny) / 2][(cx + nx) / 2] = PASSAGE;
                maze[ny][nx] = PASSAGE;

                // Move to the neighbor and push it onto the stack
                cellStack.push({ nx, ny });
            }
            else {
                // Backtrack if no unvisited neighbors
                cellStack.pop();
            }
        }

        // Set the exit at the bottom-right corner
        exitX = width - 2;
        exitY = height - 2;
        maze[exitY][exitX] = 'E';  // Mark exit

        // Add enemies
        placeEnemies();
    }

    void placeEnemies() {
        enemies.clear();
        int enemyCount = 2 + currentLevel; // Increase enemy count with each level

        while (enemyCount > 0) {
            int ex = rand() % width;
            int ey = rand() % height;

            // Ensure the enemy is placed on a passage, and not where the player or exit is
            if (maze[ey][ex] == PASSAGE && (ex != playerX || ey != playerY) && (ex != exitX || ey != exitY)) {
                enemies.push_back({ ex, ey });
                maze[ey][ex] = 'X';  // Mark enemy on the maze
                enemyCount--;
            }
        }
    }

    void displayMaze() {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                cout << maze[y][x];
            }
            cout << endl;
        }
    }

    void movePlayer(int dx, int dy) {
        int newX = playerX + dx;
        int newY = playerY + dy;

        if (maze[newY][newX] != WALL) {
            maze[playerY][playerX] = PASSAGE;  // Clear old position
            playerX = newX;
            playerY = newY;
            maze[playerY][playerX] = 'P';  // Place player in new position
        }
    }

    bool isExitReached() {
        return playerX == exitX && playerY == exitY;
    }

    bool isCaughtByEnemy() {
        for (auto& enemy : enemies) {
            if (playerX == enemy.first && playerY == enemy.second) {
                return true;
            }
        }
        return false;
    }

    void moveEnemies() {
        for (auto& enemy : enemies) {
            int dx = 0, dy = 0;
            int direction = rand() % 4; // Random movement: 0 = up, 1 = down, 2 = left, 3 = right

            switch (direction) {
            case 0: dy = -1; break; // Move up
            case 1: dy = 1; break;  // Move down
            case 2: dx = -1; break; // Move left
            case 3: dx = 1; break;  // Move right
            }

            int newEnemyX = enemy.first + dx;
            int newEnemyY = enemy.second + dy;

            if (maze[newEnemyY][newEnemyX] != WALL && maze[newEnemyY][newEnemyX] != 'E') {
                maze[enemy.second][enemy.first] = PASSAGE;  // Clear old enemy position
                enemy.first = newEnemyX;
                enemy.second = newEnemyY;
                maze[enemy.second][enemy.first] = 'X';  // Place enemy in new position
            }
        }
    }

 

    void playGame() {
        while (true) {
            system("cls"); // Clear the screen (Windows-specific)

            // Redraw player and enemies
            maze[playerY][playerX] = 'P'; // Ensure the player is drawn
            for (auto& enemy : enemies) {
                maze[enemy.second][enemy.first] = 'X'; // Ensure enemies are drawn
            }

            displayMaze();

            if (isExitReached()) {
                
                continue;//problem with the maze being completed
            }

            if (isCaughtByEnemy()) {
                cout << "Game Over! You were caught by an enemy!" << endl;
                break;
            }

            // Get user input for movement
            char input = _getch();
            switch (input) {
            case 'w': movePlayer(0, -1); break; // Up
            case 's': movePlayer(0, 1); break;  // Down
            case 'a': movePlayer(-1, 0); break; // Left
            case 'd': movePlayer(1, 0); break;  // Right
            default: break;
            }

            // Move enemies after player moves
            moveEnemies();
        }
    }
};

int main() {
    MazeGame game(21, 11);  // You can adjust the maze dimensions
    game.playGame();
    return 0;
}

