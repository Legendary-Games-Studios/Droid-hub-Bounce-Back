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
int ballDirX = 1;  // ball horizontal direction: 1=right, -1=left
int ballDirY = 1;  // ball vertical direction: 1=down, -1=up

int playerY;
int aiY;

int playerScore = 0;
int aiScore = 0;

void Draw() {
    system("cls");  // Clear console (Windows); on Linux use "clear"

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (x == 0 && (y >= playerY && y < playerY + 3))
                cout << "|"; // player paddle (left)
            else if (x == WIDTH - 1 && (y >= aiY && y < aiY + 3))
                cout << "|"; // AI paddle (right)
            else if (x == ballX && y == ballY)
                cout << "O"; // ball
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

// AI moves to predict ball's vertical position with some randomness
void AIMove() {
    int paddleCenter = aiY + 1;  // middle of AI paddle

    // Predict where the ball will be when it reaches AI's side
    if (ballDirX == 1) { // ball moving right
        int stepsToReachAI = WIDTH - 1 - ballX;
        int predictedY = ballY + ballDirY * stepsToReachAI;

        // Reflect predictedY off top/bottom walls
        while (predictedY < 0 || predictedY >= HEIGHT) {
            if (predictedY < 0) predictedY = -predictedY;
            if (predictedY >= HEIGHT) predictedY = 2 * (HEIGHT - 1) - predictedY;
        }

        // Move AI paddle slowly toward predicted position with some randomness
        if (paddleCenter < predictedY && aiY + 3 < HEIGHT) {
            if (rand() % 3 != 0) // 2/3 chance to move
                aiY++;
        }
        else if (paddleCenter > predictedY && aiY > 0) {
            if (rand() % 3 != 0)
                aiY--;
        }
    }
    else {
        // If ball moving away, center the paddle slowly
        if (paddleCenter < HEIGHT / 2 && aiY + 3 < HEIGHT)
            aiY++;
        else if (paddleCenter > HEIGHT / 2 && aiY > 0)
            aiY--;
    }
}

bool UpdateBall() {
    // Move ball position
    ballX += ballDirX;
    ballY += ballDirY;

    // Bounce on top/bottom
    if (ballY <= 0 || ballY >= HEIGHT - 1) {
        ballDirY = -ballDirY;
    }

    // Bounce on player paddle
    if (ballX == 1) {
        if (ballY >= playerY && ballY < playerY + 3) {
            ballDirX = -ballDirX;
        }
    }

    // Bounce on AI paddle
    if (ballX == WIDTH - 2) {
        if (ballY >= aiY && ballY < aiY + 3) {
            ballDirX = -ballDirX;
        }
    }

    // Check for scoring
    if (ballX <= 0) {
        // AI scores
        aiScore++;
        return true; // round over
    }
    else if (ballX >= WIDTH - 1) {
        // Player scores
        playerScore++;
        return true; // round over
    }

    return false; // round continues
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
            // flush input buffer
            while (getchar() != '\n');
            return true;
        }
        else if (answer == 'N' || answer == 'n') {
            return false;
        }
    }
}

int main() {
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
                // Ball passed player's side => AI scores => You lose
                if (!PlayAgainPrompt("😅 Nice try! Another go?")) {
                    gameRunning = false;
                }
            }
            else {
                // Ball passed AI's side => You score => You win
                if (!PlayAgainPrompt("🎉 You win! Play again?")) {
                    gameRunning = false;
                }
            }
            ResetPositions();
        }

        // Slow down loop for input time & smoother gameplay (~60fps)
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    cout << "Thanks for playing! 👋\n";
    return 0;
}