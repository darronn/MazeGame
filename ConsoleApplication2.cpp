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
    // Enemy coordinates
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

    

   

    void playGame() {
        while (true) {
            system("cls"); // Clear the screen (Windows-specific)

            // Redraw player and enemies
            maze[playerY][playerX] = 'P'; // Ensure the player is drawn
          

            displayMaze();

            if (isExitReached()) {
                
                continue;
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
            
        }
    }
};

int main() {
    MazeGame game(21, 11);  // You can adjust the maze dimensions
    game.playGame();
    return 0;
}
