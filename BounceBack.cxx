#include <iostream>
#include <conio.h>      // For _kbhit() and _getch()
#include <cstdlib>      // For rand()
#include <ctime>        // For time()
#include <thread>       // For sleep_for
#include <chrono>       // For milliseconds

using namespace std;

const int WIDTH = 40;
const int HEIGHT = 15;

int ballX, ballY;
int ballDirX = 1;
int ballDirY = 1;

int playerY;
int aiY;

int playerScore = 0;
int aiScore = 0;

void Draw() {
    system("cls");

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (x == 0 && (y >= playerY && y < playerY + 3))
                cout << "|";
            else if (x == WIDTH - 1 && (y >= aiY && y < aiY + 3))
                cout << "|";
            else if (x == ballX && y == ballY)
                cout << "O";
            else
                cout << " ";
        }
        cout << "\n";
    }
    cout << "\nYour side: Left  |  AI side: Right\n";
}

void Input() {
    if (_kbhit()) {
        char ch = _getch();
        if (ch == 'w' || ch == 'W') {
            if (playerY > 0) playerY--;
        }
        else if (ch == 's' || ch == 'S') {
            if (playerY + 3 < HEIGHT) playerY++;
        }
    }
}

void AIMove() {
    int paddleCenter = aiY + 1;

    if (ballDirX == 1) {
        int stepsToReachAI = WIDTH - 1 - ballX;
        int predictedY = ballY + ballDirY * stepsToReachAI;

        while (predictedY < 0 || predictedY >= HEIGHT) {
            if (predictedY < 0) predictedY = -predictedY;
            if (predictedY >= HEIGHT) predictedY = 2 * (HEIGHT - 1) - predictedY;
        }

        if (paddleCenter < predictedY && aiY + 3 < HEIGHT) {
            if (rand() % 3 != 0)
                aiY++;
        }
        else if (paddleCenter > predictedY && aiY > 0) {
            if (rand() % 3 != 0)
                aiY--;
        }
    }
    else {
        if (paddleCenter < HEIGHT / 2 && aiY + 3 < HEIGHT)
            aiY++;
        else if (paddleCenter > HEIGHT / 2 && aiY > 0)
            aiY--;
    }
}

bool UpdateBall() {
    ballX += ballDirX;
    ballY += ballDirY;

    if (ballY <= 0 || ballY >= HEIGHT - 1) {
        ballDirY = -ballDirY;
    }

    if (ballX == 1 && ballY >= playerY && ballY < playerY + 3) {
        ballDirX = -ballDirX;
    }

    if (ballX == WIDTH - 2 && ballY >= aiY && ballY < aiY + 3) {
        ballDirX = -ballDirX;
    }

    if (ballX <= 0) {
        aiScore++;
        return true;
    }
    else if (ballX >= WIDTH - 1) {
        playerScore++;
        return true;
    }

    return false;
}

void ResetPositions() {
    ballX = WIDTH / 2;
    ballY = HEIGHT / 2;
    ballDirX = (rand() % 2 == 0) ? 1 : -1;
    ballDirY = (rand() % 2 == 0) ? 1 : -1;

    playerY = HEIGHT / 2 - 1;
    aiY = HEIGHT / 2 - 1;
}

bool PlayAgainPrompt(const string& message) {
    cout << message << " (Y/N): ";
    while (true) {
        char answer = getchar();
        if (answer == 'Y' || answer == 'y') {
            while (getchar() != '\n');
            return true;
        }
        else if (answer == 'N' || answer == 'n') {
            return false;
        }
    }
}

int main() {
    // ======== Droid Hub Game Banner ========
    cout << "====================================\n";
    cout << "=== BounceBack 🏓               ===\n";
    cout << "=== Rating: E (Everyone)          ===\n";
    cout << "=== Developed by:                 ===\n";
    cout << "=== Legendary Games Studios       ===\n";
    cout << "====================================\n\n";

    srand(time(0));
    ResetPositions();

    bool gameRunning = true;

    while (gameRunning) {
        Draw();
        Input();
        AIMove();

        bool roundOver = UpdateBall();

        if (roundOver) {
            Draw();
            if (ballX <= 0) {
                if (!PlayAgainPrompt("😅 Nice try! Another go?")) {
                    gameRunning = false;
                }
            }
            else {
                if (!PlayAgainPrompt("🎉 You win! Play again?")) {
                    gameRunning = false;
                }
            }
            ResetPositions();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    cout << "Thanks for playing! 👋\n";
    return 0;
}