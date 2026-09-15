#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <windows.h>

// Windows Number-based Color System
#define RED     4
#define GREEN   2
#define YELLOW  6
#define CYAN    3
#define MAGENTA 5
#define BLUE    1
#define WHITE   7
#define RESET   7

void setColor(int colorCode) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorCode);
}

#define MAX_SIZE 20

int rows = 7, cols = 7;
char grid[MAX_SIZE][MAX_SIZE];
int startR = 0, startC = 0;
int endR = 6, endC = 6;

// BFS Direction Vectors (Up, Right, Down, Left)
int dr[] = {-1, 0, 1, 0};
int dc[] = {0, 1, 0, -1};

// DFS Direction Vectors (Down, Left, Up, Right)
int dfs_dr[] = {1, 0, -1, 0};
int dfs_dc[] = {0, -1, 0, 1};

typedef struct {
    int r, c;
} Point;

void hideCursor() {
    printf("\x1b[?25l");
}

void showCursor() {
    printf("\x1b[?25h");
}

// Function to clear input buffer safely after scanf
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Helper function to wait for enter cleanly
void waitForEnter() {
    char buf[100];
    if (fgets(buf, sizeof(buf), stdin) == NULL) return;
}

// Windows Console Cursor Position Helper for 100% Flicker-Free Redraw
void setCursorPosition(short x, short y) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = { x, y };
    SetConsoleCursorPosition(hOut, coord);
}

// Get Console Cursor Position
COORD getCursorPosition() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.dwCursorPosition;
}

int getConsoleWidth() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

int getConsoleHeight() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

void printPadding(int padding) {
    for (int i = 0; i < padding; i++) {
        putchar(' ');
    }
}

void showWelcomeAnimation() {
    system("cls");
    hideCursor();

    const char *bannerLines[] = {
        "==================================================",
        "          WELCOME TO MAZE SOLVER ENGINE           ",
        "        -- BFS vs DFS Path Visualizer --          ",
        "=================================================="
    };
    int totalBannerLines = 4;
    int bannerWidth = 50;

    int consoleWidth = getConsoleWidth();
    int consoleHeight = getConsoleHeight();

    int leftPadding = (consoleWidth - bannerWidth) / 2;
    if (leftPadding < 0) leftPadding = 0;

    int topPadding = (consoleHeight - totalBannerLines - 2) / 2;
    if (topPadding < 0) topPadding = 0;

    for (int i = 0; i < topPadding; i++) {
        printf("\n");
    }

    for (int i = 0; i < totalBannerLines; i++) {
        printPadding(leftPadding);
        setColor(GREEN);
        for (int j = 0; bannerLines[i][j] != '\0'; j++) {
            putchar(bannerLines[i][j]);
            fflush(stdout);
            Sleep(1);
        }
        setColor(RESET);
        printf("\n");
    }

    printf("\n");
    printPadding(leftPadding + (bannerWidth - 15) / 2);
    setColor(YELLOW);
    printf("Loading Menu...\n");
    setColor(RESET);
    fflush(stdout);
    Sleep(300);

    showCursor();
}

void printGrid(const char *title, int visitedCount, int stepCount) {
    int consoleWidth = getConsoleWidth();

    int gridWidth = cols * 5;
    int contentWidth = (gridWidth > 45) ? gridWidth : 45;
    int leftPadding = (consoleWidth - contentWidth) / 2;
    if (leftPadding < 0) leftPadding = 0;

    printPadding(leftPadding);
    setColor(CYAN);
    printf("=============================================\n");
    setColor(RESET);

    printPadding(leftPadding);
    setColor(MAGENTA);
    printf("   %-39s   \n", title);
    setColor(RESET);

    printPadding(leftPadding);
    setColor(CYAN);
    printf("=============================================\n");
    setColor(RESET);

    for (int i = 0; i < rows; i++) {
        printPadding(leftPadding);
        for (int j = 0; j < cols; j++) {
            if (i == startR && j == startC) {
                setColor(GREEN);
                printf(" [S] ");
                setColor(RESET);
            } else if (i == endR && j == endC) {
                setColor(RED);
                printf(" [E] ");
                setColor(RESET);
            } else if (grid[i][j] == '#') {
                setColor(YELLOW);
                printf(" [\xDB] "); // Solid Block Wall
                setColor(RESET);
            } else if (grid[i][j] == '*') {
                setColor(CYAN);
                printf(" [*] "); // Star Path Trail
                setColor(RESET);
            } else {
                printf("  .  ");
            }
        }
        printf("\n");
    }

    printPadding(leftPadding);
    setColor(YELLOW);
    printf("Visited Nodes: %-3d", visitedCount);
    setColor(RESET);
    printf(" | ");
    setColor(GREEN);
    printf("Path Length: %-2d steps\n", stepCount);
    setColor(RESET);

    printPadding(leftPadding);
    printf("---------------------------------------------\n");
}

void resetGrid() {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (grid[i][j] != '#') {
                grid[i][j] = '.';
            }
        }
    }
}

void generateRandomMaze() {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            grid[i][j] = '.';
        }
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if ((i == startR && j == startC) || (i == endR && j == endC)) continue;

            if ((rand() % 100) < 25) {
                grid[i][j] = '#';
            }
        }
    }
}

// BFS Algorithm
void runBFS() {
    resetGrid();

    // Edge case: Start and End are same point
    if (startR == endR && startC == endC) {
        printGrid("BFS FINISHED (Start is End)", 1, 0);
        return;
    }

    Point queue[MAX_SIZE * MAX_SIZE];
    Point parent[MAX_SIZE][MAX_SIZE];
    bool visited[MAX_SIZE][MAX_SIZE] = {false};

    int front = 0, rear = 0;

    queue[rear++] = (Point){startR, startC};
    visited[startR][startC] = true;

    bool found = false;
    int visitedCount = 0;

    while (front < rear) {
        Point curr = queue[front++];
        visitedCount++;

        if (curr.r == endR && curr.c == endC) {
            found = true;
            break;
        }

        for (int i = 0; i < 4; i++) {
            int nr = curr.r + dr[i];
            int nc = curr.c + dc[i];

            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                if (!visited[nr][nc] && grid[nr][nc] != '#') {
                    visited[nr][nc] = true;
                    parent[nr][nc] = curr;
                    queue[rear++] = (Point){nr, nc};
                }
            }
        }
    }

    if (found) {
        Point pathTemp[MAX_SIZE * MAX_SIZE];
        int pathLen = 0;
        Point curr = parent[endR][endC];

        while (!(curr.r == startR && curr.c == startC)) {
            pathTemp[pathLen++] = curr;
            curr = parent[curr.r][curr.c];
        }

        hideCursor();
        COORD startPos = getCursorPosition();

        // Render initial state
        printGrid("BFS: Moving Step-by-Step...", visitedCount, 0);
        Sleep(180);

        // Animate path movement
        for (int i = pathLen - 1; i >= 0; i--) {
            grid[pathTemp[i].r][pathTemp[i].c] = '*';
            setCursorPosition(startPos.X, startPos.Y);
            printGrid("BFS: Moving Step-by-Step...", visitedCount, (pathLen - i));
            Sleep(180);
        }

        setCursorPosition(startPos.X, startPos.Y);
        printGrid("BFS FINISHED (Shortest Path)", visitedCount, pathLen + 1);
        showCursor();

        int consoleWidth = getConsoleWidth();
        printPadding((consoleWidth - 38) / 2);
        setColor(GREEN);
        printf("[BFS Complete] Path Found in %d Steps!\n", pathLen + 1);
        setColor(RESET);
    } else {
        printGrid("BFS RESULT: No Path Found!", visitedCount, 0);
        showCursor();

        int consoleWidth = getConsoleWidth();
        printPadding((consoleWidth - 21) / 2);
        setColor(RED);
        printf("[BFS] No Path Exists!\n");
        setColor(RESET);
    }
}

// DFS Recursive Helper
bool dfsUtil(int r, int c, bool visited[MAX_SIZE][MAX_SIZE], Point parent[MAX_SIZE][MAX_SIZE], int *visitedCount) {
    visited[r][c] = true;
    (*visitedCount)++;

    if (r == endR && c == endC) return true;

    for (int i = 0; i < 4; i++) {
        int nr = r + dfs_dr[i];
        int nc = c + dfs_dc[i];

        if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
            if (!visited[nr][nc] && grid[nr][nc] != '#') {
                parent[nr][nc] = (Point){r, c};
                if (dfsUtil(nr, nc, visited, parent, visitedCount)) return true;
            }
        }
    }
    return false;
}

// DFS Algorithm
void runDFS() {
    resetGrid();

    // Edge case: Start and End are same point
    if (startR == endR && startC == endC) {
        printGrid("DFS FINISHED (Start is End)", 1, 0);
        return;
    }

    bool visited[MAX_SIZE][MAX_SIZE] = {false};
    Point parent[MAX_SIZE][MAX_SIZE];
    int visitedCount = 0;

    bool found = dfsUtil(startR, startC, visited, parent, &visitedCount);

    if (found) {
        Point pathTemp[MAX_SIZE * MAX_SIZE];
        int pathLen = 0;
        Point curr = parent[endR][endC];

        while (!(curr.r == startR && curr.c == startC)) {
            pathTemp[pathLen++] = curr;
            curr = parent[curr.r][curr.c];
        }

        hideCursor();
        COORD startPos = getCursorPosition();

        printGrid("DFS: Moving Step-by-Step...", visitedCount, 0);
        Sleep(180);

        for (int i = pathLen - 1; i >= 0; i--) {
            grid[pathTemp[i].r][pathTemp[i].c] = '*';
            setCursorPosition(startPos.X, startPos.Y);
            printGrid("DFS: Moving Step-by-Step...", visitedCount, (pathLen - i));
            Sleep(180);
        }

        setCursorPosition(startPos.X, startPos.Y);
        printGrid("DFS FINISHED", visitedCount, pathLen + 1);
        showCursor();

        int consoleWidth = getConsoleWidth();
        printPadding((consoleWidth - 38) / 2);
        setColor(GREEN);
        printf("[DFS Complete] Path Found in %d Steps!\n", pathLen + 1);
        setColor(RESET);
    } else {
        printGrid("DFS RESULT: No Path Found!", visitedCount, 0);
        showCursor();

        int consoleWidth = getConsoleWidth();
        printPadding((consoleWidth - 21) / 2);
        setColor(RED);
        printf("[DFS] No Path Exists!\n");
        setColor(RESET);
    }
}

// Main Menu
void showMenu() {
    int consoleWidth = getConsoleWidth();
    int menuWidth = 45;
    int leftPadding = (consoleWidth - menuWidth) / 2;
    if (leftPadding < 0) leftPadding = 0;

    printf("\n");
    printPadding(leftPadding);
    setColor(CYAN);
    printf("=============================================\n");
    setColor(RESET);

    printPadding(leftPadding);
    setColor(MAGENTA);
    printf("   ADVANCED MAZE SOLVER (BFS vs DFS)  \n");
    setColor(RESET);

    printPadding(leftPadding);
    setColor(CYAN);
    printf("=============================================\n\n");
    setColor(RESET);

    printPadding(leftPadding);
    printf("1. Auto Generate Random Maze (Preset)\n");
    printPadding(leftPadding);
    printf("2. Custom Maze Input (Row, Col, Walls)\n");
    printPadding(leftPadding);
    printf("3. Quit Program\n\n");

    printPadding(leftPadding);
    printf("Select Choice (1, 2 or 3): ");
}

int main() {
    SetConsoleOutputCP(65001); // Enable UTF-8 Output
    srand((unsigned int)time(NULL));

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    if (GetConsoleMode(hOut, &dwMode)) {
        SetConsoleMode(hOut, dwMode | 0x0004); // Enable VT processing
    }

    showWelcomeAnimation();
    system("cls");

    while (1) {
        showCursor();
        showMenu();

        char inputBuffer[100];
        int choice = -1;

        if (fgets(inputBuffer, sizeof(inputBuffer), stdin) != NULL) {
            if (sscanf(inputBuffer, "%d", &choice) != 1) {
                choice = -1;
            }
        }

        int consoleWidth = getConsoleWidth();
        int leftPadding = (consoleWidth - 45) / 2;
        if (leftPadding < 0) leftPadding = 0;

        if (choice == 3) {
            printf("\n");
            printPadding(leftPadding);
            printf("Exiting Maze Solver. Goodbye!\n");
            break;
        }

        if (choice == 2) {
            printf("\n");
            printPadding(leftPadding);
            printf("Enter Rows and Cols (Max 20, e.g. 5 5): ");
            scanf("%d %d", &rows, &cols);

            // Boundary validation
            if (rows < 2 || rows > MAX_SIZE) rows = 7;
            if (cols < 2 || cols > MAX_SIZE) cols = 7;

            printPadding(leftPadding);
            printf("Enter Start Point (row col, e.g. 0 0): ");
            scanf("%d %d", &startR, &startC);

            printPadding(leftPadding);
            printf("Enter End Point (row col, e.g. %d %d): ", rows - 1, cols - 1);
            scanf("%d %d", &endR, &endC);

            // Start/End Boundary validation
            if (startR < 0 || startR >= rows || startC < 0 || startC >= cols) { startR = 0; startC = 0; }
            if (endR < 0 || endR >= rows || endC < 0 || endC >= cols) { endR = rows - 1; endC = cols - 1; }

            int walls;
            printPadding(leftPadding);
            printf("Enter Number of Walls: ");
            scanf("%d", &walls);

            for (int i = 0; i < rows; i++)
                for (int j = 0; j < cols; j++) grid[i][j] = '.';

            if (walls > 0) {
                printPadding(leftPadding);
                printf("Enter Wall Coordinates (row col):\n");
                for (int i = 0; i < walls; i++) {
                    int wr, wc;
                    printPadding(leftPadding);
                    scanf("%d %d", &wr, &wc);
                    if (wr >= 0 && wr < rows && wc >= 0 && wc < cols) {
                        grid[wr][wc] = '#';
                    }
                }
            }
            clearInputBuffer(); // Clear newline left in stdin by scanf
        } else if (choice == 1) {
            rows = 7; cols = 7;
            startR = 0; startC = 0;
            endR = 6; endC = 6;
            generateRandomMaze();
        } else {
            printf("\n");
            printPadding(leftPadding);
            setColor(RED);
            printf("Invalid Choice! Please enter 1, 2, or 3.\n");
            setColor(RESET);
            Sleep(1200);
            system("cls");
            continue;
        }

        // Preview initial layout
        resetGrid();
        printf("\n");
        printGrid("INITIAL MAZE LAYOUT", 0, 0);

        printf("\n");
        printPadding(leftPadding);
        printf("Press Enter to run BFS...");
        waitForEnter();

        // 1. Run BFS Algorithm
        runBFS();

        printf("\n");
        printPadding(leftPadding);
        printf("Press Enter to run DFS...");
        waitForEnter();

        // 2. Run DFS Algorithm
        runDFS();

        printf("\n");
        printPadding(leftPadding);
        printf("=============================================\n");
        printPadding(leftPadding);
        printf(" Execution Completed Successfully!\n");
        printPadding(leftPadding);
        printf("=============================================\n");

        printf("\n");
        printPadding(leftPadding);
        printf("Press Enter to return to Menu...");
        waitForEnter();

        system("cls");
    }

    return 0;
}