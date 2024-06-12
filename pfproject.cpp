#include <SFML/Graphics.hpp>
#include <iostream>
#include<string.h>
#include<cstdlib>
#include <fstream>

using namespace std;
using namespace sf;

const int gridSize = 8;
const int cellSize = 64;
const int scoreFontSize = 20; // Font size for the score
const int timerFontSize = 20;
const int MAX_PLAYERS = 5;
const string SCORES_FILE = "scores.txt";
const string FONT_FILE = "arial.ttf";
const string MAIN_BACKGROUND = ".\\image\\main_background.png";
const string INSTRUCT_BACKGROUND = ".\\image\\instruct_background.jpg";
const string GAME_BACKGROUND= ".\\image\\background.jpg";
const string GEM_IMAGES_PATH= ".\\image";

void displayMatrix(int gemMatrix[gridSize][gridSize]);
void generateGem(int gemMatrix[gridSize][gridSize]);
void checkForThreeMatches(int gemMatrix[gridSize][gridSize], int& score);

void checkForFourMatches(int gemMatrix[gridSize][gridSize], int& score, int selectedX, int selectedY);
void checkForFiveMatches(int gemMatrix[gridSize][gridSize], int& score);
void fillEmptySpaces(int gemMatrix[gridSize][gridSize]);
void checkAndDestroyMatches(int gemMatrix[gridSize][gridSize], int& score, int selectedX, int selectedY);
void displayScore(sf::RenderWindow& window, sf::Font& font, int score);
void displayTimer(RenderWindow& window, sf::Font& font, int time);

void displayRemarks(RenderWindow& window, Font& font, string remarks);


void drawGemMatrixWindow(RenderWindow& window, Font& font, int gemMatrix[gridSize][gridSize],
    Texture gemTextures[],
    int space,
    int selectedX, int selectedY);


bool checkIfHorizontallySwapPossible(int gemMatrix[gridSize][gridSize]);
bool checkIfVerticallySwapPossible(int gemMatrix[gridSize][gridSize]);
bool isValidSwap(int gemMatrix[gridSize][gridSize], int cursorX, int cursorY, int newCursorY, int newCursorX);
void displayGameEnd(Font& font, int& score);


bool intializeGame(RenderWindow& window, Sprite&backgroundSprite, Texture &backgroundTextrue,TexturegemTexture[],int gemMatrix[gridSize][gridSize]);
void handleGameEvents(RenderWindow& window, int gemMatrix[gridSize][gridSize], int&cursorX, int&cursorY, bool&enterPressed);
void playGame(Font &font);
void showInstructions(Font& font);
void showCreditsWindow(Font& font);

void displayMainWindow(Font &font);
string getInputName( Font& font);
void saveScore(const string& name, int score);
void loadScores(string names[MAX_PLAYERS], int scores[MAX_PLAYERS]);
void sortAccordingToScore(string names[MAX_PLAYERS], int scores[MAX_PLAYERS]);
void displayTopScores(Font &font);



int main() {
    Font font;
    if (!font.loadFromFile(FONT_FILE)) {
cout<< "Failed to load font file." <<endl;
        return false;
    }
displayMainWindow(font);
    return 0;
}



void displayMainWindow(Font &font) {

RenderWindowwindow(VideoMode(800, 600), "Main Menu");
    Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile(MAIN_BACKGROUND)) {
         return;
    }

    Sprite backgroundSprite(backgroundTexture);
backgroundSprite.setScale(static_cast<float>(window.getSize().x) / backgroundTexture.getSize().x,
static_cast<float>(window.getSize().y) / backgroundTexture.getSize().y);



    // Button properties
    const int buttonWidth = 200;
    const int buttonHeight = 50;
    const Color buttonBackgroundColor = Color(200, 200, 200); 
    const Color buttonBorderColor = Color::Red;
    const Color textColor = Color::Black;

    // Play button
RectangleShapeplayButton(Vector2f(buttonWidth, buttonHeight));
playButton.setFillColor(buttonBackgroundColor);
playButton.setOutlineColor(buttonBorderColor);
playButton.setOutlineThickness(2); // Border thickness
playButton.setPosition((window.getSize().x - buttonWidth) / 2, 100);

    Text playText("Play", font, 30);
playText.setFillColor(textColor);
playText.setPosition((window.getSize().x - playText.getLocalBounds().width) / 2,
playButton.getPosition().y + (buttonHeight - playText.getLocalBounds().height) / 2);

    // Credits button
RectangleShapecreditsButton(Vector2f(buttonWidth, buttonHeight));
creditsButton.setFillColor(buttonBackgroundColor);
creditsButton.setOutlineColor(buttonBorderColor);
creditsButton.setOutlineThickness(2); // Border thickness
creditsButton.setPosition((window.getSize().x - buttonWidth) / 2, 200);

    Text creditsText("Credits", font, 30);
creditsText.setFillColor(textColor);
creditsText.setPosition((window.getSize().x - creditsText.getLocalBounds().width) / 2,
creditsButton.getPosition().y + (buttonHeight - creditsText.getLocalBounds().height) / 2);

    // Instructions button
RectangleShapeinstructionsButton(Vector2f(buttonWidth, buttonHeight));
instructionsButton.setFillColor(buttonBackgroundColor);
instructionsButton.setOutlineColor(buttonBorderColor);
instructionsButton.setOutlineThickness(2); // Border thickness
instructionsButton.setPosition((window.getSize().x - buttonWidth) / 2, 300);

    Text instructionsText("Instructions", font, 30);
instructionsText.setFillColor(textColor);
instructionsText.setPosition((window.getSize().x - instructionsText.getLocalBounds().width) / 2,
instructionsButton.getPosition().y + (buttonHeight - instructionsText.getLocalBounds().height) / 2);

RectangleShapetopScoreButton(Vector2f(buttonWidth, buttonHeight));
topScoreButton.setFillColor(buttonBackgroundColor);
topScoreButton.setOutlineColor(buttonBorderColor);
topScoreButton.setOutlineThickness(2); // Border thickness
topScoreButton.setPosition((window.getSize().x - buttonWidth) / 2, 400);

    Text topScoresText("Top Scores", font, 30);
topScoresText.setFillColor(textColor);
topScoresText.setPosition((window.getSize().x - topScoresText.getLocalBounds().width) / 2,
topScoreButton.getPosition().y + (buttonHeight - topScoresText.getLocalBounds().height) / 2);


    // Exit button
RectangleShapeexitButton(Vector2f(buttonWidth, buttonHeight));
exitButton.setFillColor(buttonBackgroundColor);
exitButton.setOutlineColor(buttonBorderColor);
exitButton.setOutlineThickness(2); // Border thickness
exitButton.setPosition((window.getSize().x - buttonWidth) / 2, 500);

    Text exitText("Exit", font, 30);
exitText.setFillColor(textColor);
exitText.setPosition((window.getSize().x - exitText.getLocalBounds().width) / 2,
exitButton.getPosition().y + (buttonHeight - exitText.getLocalBounds().height) / 2);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
window.close();
            }

            // Check for button clicks
            if (event.type == Event::MouseButtonPressed) {
                if (event.mouseButton.button == Mouse::Left) {
                    Vector2i mousePos = Mouse::getPosition(window);

                    // Check if Play button is clicked
                    if (playButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
window.clear();
window.close();
playGame(font);
                        break;
                    }

                    // Check if Credits button is clicked
                    else if (creditsButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
window.clear();
window.close();
showCreditsWindow(font);
                        break;
                        // Add logic for credits here
                    }

                    // Check if Instructions button is clicked
                    else if (instructionsButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
window.clear();
window.close();
showInstructions(font);
                        break;
                    }

                    else if (topScoreButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
window.clear();
window.close();
displayTopScores(font);
                        break;
                    }

                    // Check if Exit button is clicked
                    if (exitButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
window.close();
                    }
                }
            }
        }

        // Clear the window
window.clear();
window.draw(backgroundSprite);
        // Draw buttons
window.draw(playButton);
window.draw(playText);
window.draw(creditsButton);
window.draw(creditsText);
window.draw(instructionsButton);
window.draw(instructionsText);
window.draw(topScoreButton);
window.draw(topScoresText);
window.draw(exitButton);
window.draw(exitText);

        // Display the contents of the window
window.display();
    }

}



string getInputName( Font& font) {
RenderWindowwindow(VideoMode(300, 300), "User Input");


        Text promptText;
promptText.setFont(font);
promptText.setCharacterSize(30);
promptText.setFillColor(sf::Color::White);
promptText.setPosition(10, 10);
promptText.setString("Enter your name:");

        Text inputText;
inputText.setFont(font);
inputText.setCharacterSize(30);
inputText.setFillColor(sf::Color::White);
inputText.setPosition(10, 50);

        string playerName;

        while (window.isOpen()) {
sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) {
window.close();
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
cout<< "Enter pressed" <<endl;
window.clear();
window.close();
                    break;
                }
                else if (event.type == Event::TextEntered) {
                    if (event.text.unicode< 128) {
                        char inputChar = static_cast<char>(event.text.unicode);
                        if (inputChar == '\n') {
cout<< "entere pressed";
                            break;
                        }
                        else {
playerName += inputChar;
inputText.setString(playerName);
                        }
                    }
                }
            }

window.clear();
window.draw(promptText);
window.draw(inputText);
window.display();
        }
window.close();
        return playerName;
 }



void saveScore(const string& name, int score) {
ofstreamscoresFile(SCORES_FILE, ios::app);
    if (scoresFile.is_open()) {
scoresFile<< name << " " << score <<endl;
scoresFile.close();
    }
    else {
cout<< "Failed to open scores file for writing." <<endl;
    }
}

void loadScores(string names[MAX_PLAYERS], int scores[MAX_PLAYERS]) {
ifstreamscoresFile(SCORES_FILE);

    if (scoresFile.is_open()) {
        for (int i = 0; i< MAX_PLAYERS; ++i) {
            if (scoresFile>> names[i] >> scores[i]) {

            }
            else {
                break;
            }
        }
scoresFile.close();
    }
    else {
cout<< "Failed to open scores file for reading." <<endl;
    }
}

void sortAccordingToScore(string names[MAX_PLAYERS], int scores[MAX_PLAYERS]) {
    for (int i = 0; i< MAX_PLAYERS - 1; ++i) {
        for (int j = 0; j < MAX_PLAYERS - i - 1; ++j) {
            if (scores[j] <scores[j + 1]) {
                swap(scores[j], scores[j + 1]);
                swap(names[j], names[j + 1]);
            }
        }
    }
}

void displayTopScores(Font &font) {

RenderWindowwindow(VideoMode(800, 600), "Highest Score");

    string names[MAX_PLAYERS];
    int scores[MAX_PLAYERS];

loadScores(names, scores);
sortAccordingToScore(names, scores);

    string nameWithScores = "";
    for (int i = 0; i< MAX_PLAYERS; ++i) {
        if (scores[i] > 0) {
nameWithScores += to_string(i + 1) + ". " + names[i] + ": " + to_string(scores[i]) + "\n";
        }
    }


    Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile(INSTRUCT_BACKGROUND)) {
         return;
    }

    Sprite backgroundSprite(backgroundTexture);
backgroundSprite.setScale(static_cast<float>(window.getSize().x) / backgroundTexture.getSize().x,
static_cast<float>(window.getSize().y) / backgroundTexture.getSize().y);

    // Instructions text with line wrapping
    Text topScoresText;
topScoresText.setFont(font);
topScoresText.setCharacterSize(20);
topScoresText.setFillColor(Color::White);
topScoresText.setPosition(20, 20);

    // Set line spacing and wrap text
topScoresText.setLineSpacing(1.5);
topScoresText.setString(
        "\n\n\n\n\Hghest Scores\n"
        +nameWithScores

    );


    const int buttonWidth = 200;
    const int buttonHeight = 50;

RectangleShapebackButton(Vector2f(buttonWidth, buttonHeight));
backButton.setFillColor(Color::Cyan);
backButton.setOutlineThickness(2);
backButton.setPosition(250, 400);

    Text backText("Back", font, 30);
backText.setFillColor(Color::Black);
backText.setPosition((window.getSize().x - backText.getLocalBounds().width) / 2,
backButton.getPosition().y + (buttonHeight - backText.getLocalBounds().height) / 2);


    // Main loop for the instructions window
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
window.close();
            }
            if (event.type == Event::MouseButtonPressed) {
                if (event.mouseButton.button == Mouse::Left) {
                    Vector2i mousePos = Mouse::getPosition(window);

                    // Check if Play button is clicked
                    if (backButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
cout<< "Play button clicked! Game starts..." <<endl;
window.clear();
window.close();
                        break;

                    }
                }
            }
        }

        // Clear the instructions window
window.clear();

        // Draw instructions text
window.draw(backgroundSprite);
window.draw(topScoresText);

window.draw(backButton);
window.draw(backText);

        // Display the contents of the instructions window
window.display();
    }
displayMainWindow(font);

}



void playGame(Font &font) {


RenderWindowwindow(VideoMode(gridSize * cellSize + 200, gridSize * cellSize), "Bejeweled Blitz");

    string playerName = getInputName(font);

    Sprite backgroundSprite;
    Texture backgroundTexture;
    Texture gemTextures[8];
    Clock clock;


    int gemMatrix[gridSize][gridSize];
    int score = 1;
    float timeRemaining = 60;
    bool enterPressed = false;
    int cursorX = 1;
    int cursorY = 1;

    if (intializeGame(window, backgroundSprite, backgroundTexture, gemTextures, gemMatrix)) {
checkAndDestroyMatches(gemMatrix, score, cursorX, cursorY);
        score = 0;
        while (window.isOpen()) {

handleGameEvents(window, gemMatrix, cursorX, cursorY, enterPressed);


            Time elapsed = clock.restart();
timeRemaining -= (elapsed.asSeconds());
            if (timeRemaining<= 0) {
window.clear();
window.close();
saveScore(playerName, score);
displayGameEnd(font, score);
                break;
            }
            else {
                int prevScore = score;
checkAndDestroyMatches(gemMatrix, score, cursorX, cursorY);
                if (prevScore != score) {
                    sleep(milliseconds(500));
                }

window.clear();
window.draw(backgroundSprite);

                const int space = 5;

drawGemMatrixWindow(window, font, gemMatrix, gemTextures, space, cursorY, cursorX);
displayScore(window, font, score);
displayTimer(window, font, timeRemaining);
            }

window.display();
        }
    }
    else {
cout<< "could not load game due to error\n";
    }
}

bool intializeGame(RenderWindow&window, Sprite &backgroundSprite, Texture &backgroundTexture,TexturegemTextures[],int gemMatrix[gridSize][gridSize]) {
    if (!backgroundTexture.loadFromFile(GAME_BACKGROUND)) {

            return false;
    }
backgroundSprite.setTexture(backgroundTexture);
backgroundSprite.setScale(static_cast<float>(window.getSize().x) / backgroundTexture.getSize().x,
static_cast<float>(window.getSize().y) / backgroundTexture.getSize().y);

    for (int i = 0; i< 7; ++i) {
        if (!gemTextures[i].loadFromFile(GEM_IMAGES_PATH+"\\gem" + to_string(i) + ".png")) {
cout<< "Failed to load gem" <<i<< ".png" <<endl;
            return false;
        }
    }


generateGem(gemMatrix);
    return true;

}
void generateGem(int gemMatrix[gridSize][gridSize]) {
srand(42);
    for (int i = 0; i<gridSize; ++i) {
        for (int j = 0; j <gridSize; ++j) {
gemMatrix[i][j] = rand() % 7; // You can replace this with your logic for generating gem types
        }
    }
}

void handleGameEvents(RenderWindow& window, int gemMatrix[gridSize][gridSize], int&cursorX, int&cursorY, bool&enterPressed) {
    Event event;
    while (window.pollEvent(event)) {
        if (event.type == Event::Closed) {
window.close();
        }
        // Handle arrow key input for gem swapping
        else


            if (event.type == Event::KeyPressed) {
                int newCursorX = cursorX;
                int newCursorY = cursorY;
                if (event.key.code == Keyboard::Enter) {
enterPressed = true;
                }
                else if (event.key.code == Keyboard::Up &&cursorY> 0) {
newCursorY = cursorY - 1;

                }
                else if (event.key.code == Keyboard::Down &&cursorY<gridSize - 1) {
newCursorY = cursorY + 1;

                }
                else if (event.key.code == Keyboard::Left &&cursorX> 0) {
newCursorX = cursorX - 1;

                }
                else if (event.key.code == Keyboard::Right &&cursorX<gridSize - 1) {
newCursorX = cursorX + 1;
                }
                if (newCursorX != cursorX || newCursorY != cursorY) {
                    if (enterPressed) {
                        if (isValidSwap(gemMatrix, cursorX, cursorY, newCursorY, newCursorX)) {
                            swap(gemMatrix[cursorY][cursorX], gemMatrix[newCursorY][newCursorX]);
                        }
enterPressed = false;
                    }
cursorX = newCursorX;
cursorY = newCursorY;
                }
            }
    }

}
void drawGemMatrixWindow(RenderWindow& window, Font& font, int gemMatrix[gridSize][gridSize],
    Texture gemTextures[],
    int space,
    int selectedX, int selectedY) {

    for (int i = 0; i<gridSize; ++i) {
        for (int j = 0; j <gridSize; ++j) {
            Sprite gemSprite(gemTextures[gemMatrix[i][j]]);
gemSprite.setPosition(j * cellSize + space, i * cellSize + space);
            float scale = static_cast<float>(cellSize - 2 * space) / gemTextures[gemMatrix[i][j]].getSize().x;
gemSprite.setScale(scale, scale);
window.draw(gemSprite);
            if (i == selectedX&& j == selectedY) {
RectangleShapeboundingBox;
boundingBox.setSize(Vector2f(cellSize - 2 * space, cellSize - 2 * space));
boundingBox.setOutlineThickness(4);
boundingBox.setOutlineColor(Color(255, 215, 0));
boundingBox.setFillColor(Color(0, 0, 0, 0));
boundingBox.setPosition(j * cellSize + space, i * cellSize + space);
window.draw(boundingBox);

            }
        }

    }
}



void checkAndDestroyMatches(int gemMatrix[gridSize][gridSize], int& score, int selectedX, int selectedY) {

checkForFiveMatches(gemMatrix, score);
checkForFourMatches(gemMatrix, score, selectedX, selectedY);
checkForThreeMatches(gemMatrix, score);
fillEmptySpaces(gemMatrix);
}




void displayMatrix(int gemMatrix[gridSize][gridSize]) {
    for (int i = 0; i<gridSize; i++) {
        for (int j = 0; j <gridSize; j++) {
cout<<gemMatrix[i][j] << " ";
        }
cout<< "\n";
    }
}


void checkForThreeMatches(int gemMatrix[gridSize][gridSize], int& score) {
    for (int i = 0; i<gridSize; ++i) {
        int count = 1; // Count of consecutive matching gems
        for (int j = 1; j <gridSize; ++j) {
            if (gemMatrix[i][j] == gemMatrix[i][j - 1] &&gemMatrix[i][j] != -1) {
                // Increment the count for consecutive matching gems
                count++;
            }
            else {
                // Reset the count if the sequence is broken
                count = 1;
            }

            // Check if we have three or more consecutive matching gems
            if (count == 3) {
                // Destroy the matching gems
                for (int k = j - count + 1; k <= j; ++k) {
gemMatrix[i][k] = -1;
                }
                score += count * 10; // Increase score (adjust accordingly)
            }
        }
    }

    // Check vertically
    for (int j = 0; j <gridSize; ++j) {
        int count = 1; // Count of consecutive matching gems
        for (int i = 1; i<gridSize; ++i) {
            if (gemMatrix[i][j] == gemMatrix[i - 1][j] &&gemMatrix[i][j] != -1) {
                // Increment the count for consecutive matching gems
                count++;
            }
            else {
                // Reset the count if the sequence is broken
                count = 1;
            }

            // Check if we have three or more consecutive matching gems
            if (count >= 3) {
                // Destroy the matching gems
                for (int k = i - count + 1; k <= i; ++k) {
gemMatrix[k][j] = -1;
                }
                score += count * 10; // Increase score (adjust accordingly)
            }
        }
    }
}

void checkForFourMatches(int gemMatrix[gridSize][gridSize], int& score, int selectedX, int selectedY) {
    for (int i = 0; i<gridSize; ++i) {
        int count = 1; // Count of consecutive matching gems
        for (int j = 1; j <gridSize; ++j) {
            if (gemMatrix[i][j] == gemMatrix[i][j - 1] &&gemMatrix[i][j] != -1) {
                // Increment the count for consecutive matching gems
                count++;
            }
            else {
                // Reset the count if the sequence is broken
                count = 1;
            }
            if (count == 4) {
                score += 100;
                for (int i = 0; i<gridSize; i++) {
gemMatrix[selectedX][i] = -1;
                }
                for (int i = 0; i<gridSize; i++) {
gemMatrix[i][selectedY] = -1;
                }
            }
        }
    }

    // Check vertically
    for (int j = 0; j <gridSize; ++j) {
        int count = 1; // Count of consecutive matching gems
        for (int i = 1; i<gridSize; ++i) {
            if (gemMatrix[i][j] == gemMatrix[i - 1][j] &&gemMatrix[i][j] != -1) {
                // Increment the count for consecutive matching gems
                count++;
            }
            else {
                // Reset the count if the sequence is broken
                count = 1;
            }
            // Check if we have three or more consecutive matching gems
            if (count == 4) {
                score += 100;
                for (int i = 0; i<gridSize; i++) {
gemMatrix[selectedX][i] = -1;
                }
                for (int i = 0; i<gridSize; i++) {
gemMatrix[i][selectedY] = -1;
                }
            }
        }
    }

}

void checkForFiveMatches(int gemMatrix[gridSize][gridSize], int& score) {
    for (int i = 0; i<gridSize; ++i) {
        int count = 1; // Count of consecutive matching gems
        for (int j = 1; j <gridSize; ++j) {
            if (gemMatrix[i][j] == gemMatrix[i][j - 1] &&gemMatrix[i][j] != -1) {
                // Increment the count for consecutive matching gems
                count++;
            }
            else {
                // Reset the count if the sequence is broken
                count = 1;
            }
            // Check if we have three or more consecutive matching gems
            if (count == 5) {
generateGem(gemMatrix);
                score += 300; // Increase score (adjust accordingly)
                return;
            }
        }
    }
    // Check vertically
    for (int j = 0; j <gridSize; ++j) {
        int count = 1; // Count of consecutive matching gems
        for (int i = 1; i<gridSize; ++i) {
            if (gemMatrix[i][j] == gemMatrix[i - 1][j] &&gemMatrix[i][j] != -1) {
                // Increment the count for consecutive matching gems
                count++;
            }
            else {
                // Reset the count if the sequence is broken
                count = 1;
            }

            // Check if we have three or more consecutive matching gems
            if (count == 5) {
                // Destroy the matching gems
generateGem(gemMatrix);
                score += 300; // Increase score (adjust accordingly)
                return;
            }
        }
    }
}

void fillEmptySpaces(int gemMatrix[gridSize][gridSize]) {
    for (int j = 0; j <gridSize; ++j) {
        int k = gridSize - 1;
        for (int i = gridSize - 1; i>= 0; --i) {
            if (gemMatrix[i][j] != -1) {
gemMatrix[k--][j] = gemMatrix[i][j];
            }
        }
        // Fill the remaining empty spaces with new gems
        for (int i = k; i>= 0; --i) {
gemMatrix[i][j] = rand() % 7; // You can replace this with your logic for generating gem types
        }
    }
}




void displayScore(sf::RenderWindow& window, sf::Font& font, int score) {

    Text scoreText;
scoreText.setFont(font); // Use default font
scoreText.setCharacterSize(20);
scoreText.setFillColor(sf::Color::Red);
scoreText.setPosition(gridSize * cellSize + 10, 30);
scoreText.setString("Score  =>  " + to_string(score));
window.draw(scoreText);
}


void displayTimer(RenderWindow& window, sf::Font& font, int time) {
    Text timerText;
timerText.setFont(font);
timerText.setCharacterSize(20);
timerText.setFillColor(Color::Blue);
timerText.setPosition(gridSize * cellSize + 10, 130);
timerText.setString("Timer  =>  " + to_string(time) + "s");
window.draw(timerText);
}


void displayRemarks(RenderWindow& window, Font& font, string remarks) {
    Text timerText;
timerText.setFont(font);
timerText.setCharacterSize(timerFontSize);
timerText.setFillColor(Color::White);
timerText.setPosition(gridSize * cellSize + 10, 500);
timerText.setString("Remarks: \n" + remarks);
window.draw(timerText);
}







bool checkIfHorizontallySwapPossible(int gemMatrix[gridSize][gridSize]) {
    int count = 1;
    for (int i = 0; i<gridSize; i++) {
        for (int j = 0; j <gridSize - 1; j++) {
            if (gemMatrix[i][j] == gemMatrix[i][j + 1]) {
                count += 1;
                if (count >= 3) {
                    return true;
                }
            }
            else {
                count = 1;
            }
        }
    }
    return false;
}

bool checkIfVerticallySwapPossible(int gemMatrix[gridSize][gridSize]) {
    int count = 1;
    for (int i = 0; i<gridSize; i++) {
        for (int j = 0; j <gridSize - 1; j++) {
            if (gemMatrix[j][i] == gemMatrix[j + 1][i]) {
                count += 1;
                if (count >= 3) {
                    return true;
                }
            }
            else {
                count = 1;
            }
        }
    }
    return count >= 3;
}


bool isValidSwap(int gemMatrix[gridSize][gridSize], int cursorX, int cursorY, int newCursorY, int newCursorX) {
    swap(gemMatrix[cursorY][cursorX], gemMatrix[newCursorY][newCursorX]);
    bool isSwapPossible = false;
    bool horizontalSwapPossible = checkIfHorizontallySwapPossible(gemMatrix);
    bool verticalSwapPossible = checkIfVerticallySwapPossible(gemMatrix);

    if (horizontalSwapPossible || verticalSwapPossible) {
isSwapPossible = true;
    }
    swap(gemMatrix[newCursorY][newCursorX], gemMatrix[cursorY][cursorX]);
    return isSwapPossible;
}


void displayGameEnd(Font& font, int& score) {

RenderWindowwindow(VideoMode(gridSize * cellSize + 200, gridSize * cellSize), "Game Over!!");

    Text gameEnd;
gameEnd.setFont(font);
gameEnd.setCharacterSize(50);
gameEnd.setFillColor(Color::Red);
gameEnd.setPosition(250, 180);
gameEnd.setString("Game Over");

    Text scoreText;
scoreText.setFont(font);
scoreText.setCharacterSize(20);
scoreText.setFillColor(Color::Green);
scoreText.setPosition(350, 230);
scoreText.setString("Score : " + to_string(score));



    const int buttonWidth = 200;
    const int buttonHeight = 50;

RectangleShapequitButton(Vector2f(buttonWidth, buttonHeight));
quitButton.setFillColor(Color::Cyan);
quitButton.setOutlineThickness(2);
quitButton.setPosition(250, 400);

    Text quitText("Quit", font, 30);
quitText.setFillColor(Color::Black);
quitText.setPosition((window.getSize().x - quitText.getLocalBounds().width) / 2,
quitButton.getPosition().y + (buttonHeight - quitText.getLocalBounds().height) / 2);


    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
window.close();
            }

            // Check for button clicks
            if (event.type == Event::MouseButtonPressed) {
                if (event.mouseButton.button == Mouse::Left) {
                    Vector2i mousePos = Mouse::getPosition(window);

                    // Check if Play button is clicked
                    if (quitButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
cout<< "Play button clicked! Game starts..." <<endl;
window.clear();
window.close();
                        break;
                    }
                }
            }
        }

window.clear();
window.draw(gameEnd);
window.draw(scoreText);
window.draw(quitButton);
window.draw(quitText);
window.display();
    }   
displayMainWindow(font);
}


void showInstructions(Font& font) {

    // Create a new window for instructions
RenderWindowwindow(VideoMode(800, 600), "Instructions");


    Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile(INSTRUCT_BACKGROUND)) {
        // Handle error loading the background image
        return;
    }

    // Create a sprite for the background image
    Sprite backgroundSprite(backgroundTexture);
backgroundSprite.setScale(static_cast<float>(window.getSize().x) / backgroundTexture.getSize().x,
static_cast<float>(window.getSize().y) / backgroundTexture.getSize().y);
    // Font for buttons

    // Instructions text with line wrapping
    Text instructionsText;
instructionsText.setFont(font);
instructionsText.setCharacterSize(20);
instructionsText.setFillColor(Color::White);
instructionsText.setPosition(20, 20);

    // Set line spacing and wrap text
instructionsText.setLineSpacing(1.5);
instructionsText.setString(
        "\n\n\n\n\nInstructions:\n"
        "- Swap gems by clicking on two adjacent gems to match three or more of the same\n"
"  type horizontally or vertically.\n"
        "- Matching gems will be destroyed, and new gems will fall from above.\n"
        "- Try to make matches to score points within the given time limit.\n"
        "- Have fun playing the game!"
    );


    const int buttonWidth = 200;
    const int buttonHeight = 50;

RectangleShapebackButton(Vector2f(buttonWidth, buttonHeight));
backButton.setFillColor(Color::Cyan);
backButton.setOutlineThickness(2);
backButton.setPosition(250, 400);

    Text backText("Back", font, 30);
backText.setFillColor(Color::Black);
backText.setPosition((window.getSize().x - backText.getLocalBounds().width) / 2,
backButton.getPosition().y + (buttonHeight - backText.getLocalBounds().height) / 2);


    // Main loop for the instructions window
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
window.close();
            }
            if (event.type == Event::MouseButtonPressed) {
                if (event.mouseButton.button == Mouse::Left) {
                    Vector2i mousePos = Mouse::getPosition(window);

                    // Check if Play button is clicked
                    if (backButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
cout<< "Play button clicked! Game starts..." <<endl;
window.clear();
window.close();
                        break;

                    }
                }
            }
        }

        // Clear the instructions window
window.clear();

        // Draw instructions text
window.draw(backgroundSprite);
window.draw(instructionsText);

window.draw(backButton);
window.draw(backText);

        // Display the contents of the instructions window
window.display();
    }
displayMainWindow(font);
}


void showCreditsWindow(Font& font) {

    // Create a new window for instructions
RenderWindowwindow(VideoMode(800, 600), "Credits");


    Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile(INSTRUCT_BACKGROUND)) {
        // Handle error loading the background image
        return;
    }

    // Create a sprite for the background image
    Sprite backgroundSprite(backgroundTexture);
backgroundSprite.setScale(static_cast<float>(window.getSize().x) / backgroundTexture.getSize().x,
static_cast<float>(window.getSize().y) / backgroundTexture.getSize().y);
    // Font for buttons

    // Instructions text with line wrapping
    Text creditsText;
creditsText.setFont(font);
creditsText.setCharacterSize(20);
creditsText.setFillColor(Color::White);
creditsText.setPosition(20, 20);

    // Set line spacing and wrap text
creditsText.setLineSpacing(1.5);
creditsText.setString(
        "\n\n\n\n\nCredits:\n"
        "- Name: Uma e Rubab\n"
"  Roll No: 23L-0928.\n"
        "- Name: Ayesha Khalid.\n"
"  Roll No: 23L-0667.\n"
    );

    const int buttonWidth = 200;
    const int buttonHeight = 50;

RectangleShapebackButton(Vector2f(buttonWidth, buttonHeight));
backButton.setFillColor(Color::Cyan);
backButton.setOutlineThickness(2);
backButton.setPosition(250, 400);

    Text backButtonText("Back", font, 30);
backButtonText.setFillColor(Color::Black);
backButtonText.setPosition((window.getSize().x - backButtonText.getLocalBounds().width) / 2,
backButton.getPosition().y + (buttonHeight - backButtonText.getLocalBounds().height) / 2);


    // Main loop for the instructions window
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
window.close();
            }
            if (event.type == Event::MouseButtonPressed) {
                if (event.mouseButton.button == Mouse::Left) {
                    Vector2i mousePos = Mouse::getPosition(window);

                    // Check if Play button is clicked
                    if (backButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
cout<< "Play button clicked! Game starts..." <<endl;
window.clear();
window.close();
                        break;

                    }
                }
            }
        }

        // Clear the instructions window
window.clear();

        // Draw instructions text
window.draw(backgroundSprite);
window.draw(creditsText);
window.draw(backButton);
window.draw(backButtonText);

        // Display the contents of the instructions window
window.display();
    }
displayMainWindow(font);
}
