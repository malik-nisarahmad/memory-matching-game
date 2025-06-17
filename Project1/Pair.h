//#pragma once
//
//struct Pair
//{
//    int x, y;
//    Pair() : x(-1), y(-1) {}
//    Pair(int x1, int y1) :x(x1), y(y1) {}
//    Pair(const Pair& other)
//    {
//        x = other.x;
//        y = other.y;
//    }
//};

#pragma once
#include <iostream>
struct Pair
{
    int x, y;
    Pair() {}
    Pair(int x1, int y1) :x(x1), y(y1) {}
    Pair(const Pair& other)
    {
        x = other.x;
        y = other.y;
    }

    bool operator ==(const Pair& other)
    {
        if (x == other.x && y == other.y)
            return true;
        return false;
    }

    void display() const
    {
        std::cout << "( " << x << ", " << y << ")" << std::endl;
    }
};



//=----=-=--=------------------=======---------===
//full code impementation 1
//#include <SFML/Graphics.hpp>
//#include <iostream>
//#include <vector>
//#include <string>
//#include <cmath>
//#include <ctime>
//#include <cstdlib>
//#include <algorithm>
//#include <memory> // For std::unique_ptr
//
// // Use standard namespace for brevity in this combined file
//using namespace std;
//using namespace sf;
//
//// ===================================================================
////
//// BACKEND LOGIC CLASSES (From your .h files)
////
//// ===================================================================
//
//// --- From Pair.h ---
//struct Pair
//{
//    int x, y;
//    Pair() : x(-1), y(-1) {}
//    Pair(int x1, int y1) : x(x1), y(y1) {}
//    Pair(const Pair& other) : x(other.x), y(other.y) {}
//};
//
//// --- From Card.h ---
//class Card
//{
//private:
//    int value;
//    bool isFlipped;
//    bool isMatched;
//    Pair pos;
//
//public:
//    Card() : isFlipped(false), isMatched(false), value(-1) {}
//    Card(int val) : value(val), isFlipped(false), isMatched(false) {}
//
//    void setValue(int val) { this->value = val; }
//    int getValue() const { return value; }
//
//    void setFlipped(bool flipped) { isFlipped = flipped; }
//    bool getFlipped() const { return isFlipped; }
//
//    void setMatched(bool matched) { isMatched = matched; }
//    bool getMatched() const { return isMatched; }
//
//    void setPos(int x, int y) { pos.x = x; pos.y = y; }
//    Pair getPos() const { return pos; }
//
//    void flip() { isFlipped = !isFlipped; }
//
//    bool checkMatch(const Card& other) const {
//        return value == other.value;
//    }
//};
//
//// --- From GameBoard.h ---
//const int MAX_SIZE = 8;
//class GameBoard
//{
//private:
//    Card cards[MAX_SIZE][MAX_SIZE];
//    int rows, cols;
//    int pairsLeft;
//
//public:
//    GameBoard(int difficulty) : rows(difficulty), cols(difficulty) {
//        pairsLeft = (rows * cols) / 2;
//        initializeBoard();
//        shuffleCards();
//    }
//
//    void initializeBoard() {
//        vector<int> cardValues;
//        for (int i = 0; i < (rows * cols) / 2; ++i) {
//            cardValues.push_back(i);
//            cardValues.push_back(i);
//        }
//
//        random_shuffle(cardValues.begin(), cardValues.end());
//
//        int k = 0;
//        for (int i = 0; i < rows; i++) {
//            for (int j = 0; j < cols; j++) {
//                cards[i][j].setValue(cardValues[k++]);
//                cards[i][j].setPos(i, j);
//            }
//        }
//    }
//
//    // A more robust shuffle
//    void shuffleCards() {
//        vector<int> values;
//        for (int i = 0; i < rows * cols / 2; i++) {
//            values.push_back(i);
//            values.push_back(i);
//        }
//
//        unsigned seed = time(0);
//        std::srand(seed);
//        std::random_shuffle(values.begin(), values.end());
//
//        int k = 0;
//        for (int i = 0; i < rows; i++) {
//            for (int j = 0; j < cols; j++) {
//                cards[i][j].setValue(values[k++]);
//            }
//        }
//    }
//
//    Card& getCard(int r, int c) { return cards[r][c]; }
//    int getRows() const { return rows; }
//    int getCols() const { return cols; }
//
//    bool checkWin() { return pairsLeft <= 0; }
//
//    void pairFound() { pairsLeft--; }
//};
//
//// --- From Player.h ---
//class Player
//{
//protected:
//    string name;
//    int score;
//    int gamesWon;
//public:
//    Player(string n = "Player") : name(n), score(0), gamesWon(0) {}
//    virtual ~Player() {}
//
//    void updateScore(int points) { score += points; }
//    void incrementWins() { gamesWon++; }
//
//    string getName() const { return name; }
//    int getScore() const { return score; }
//
//    // This method will be used by the AI
//    virtual Pair chooseCard(GameBoard& board) { return Pair(-1, -1); } // Return invalid pair for human
//};
//
//// --- From AI.h (Modified for GUI) ---
//class AI : public Player
//{
//private:
//    vector<Card> memory;
//    int difficulty;
//public:
//    AI(int diff) : Player("AI"), difficulty(diff) {}
//
//    // The AI's logic to choose a card
//    Pair chooseCard(GameBoard& board) override {
//        // 1. Look for a known pair in memory
//        for (size_t i = 0; i < memory.size(); ++i) {
//            for (size_t j = i + 1; j < memory.size(); ++j) {
//                if (memory[i].getValue() == memory[j].getValue()) {
//                    Pair p = memory[i].getPos();
//                    // "Forget" these cards
//                    memory.erase(memory.begin() + j);
//                    memory.erase(memory.begin() + i);
//                    return p;
//                }
//            }
//        }
//
//        // 2. No known pair, so pick a random, unknown card
//        Pair randomPair;
//        bool cardIsKnown;
//        do {
//            cardIsKnown = false;
//            randomPair.x = rand() % difficulty;
//            randomPair.y = rand() % difficulty;
//
//            // Check if we've seen this card before
//            for (const auto& mem_card : memory) {
//                if (mem_card.getPos().x == randomPair.x && mem_card.getPos().y == randomPair.y) {
//                    cardIsKnown = true;
//                    break;
//                }
//            }
//        } while (board.getCard(randomPair.x, randomPair.y).getMatched() || cardIsKnown);
//
//        return randomPair;
//    }
//
//    // AI "remembers" a card it sees
//    void rememberCard(const Card& card) {
//        if (card.getMatched()) return;
//
//        for (const auto& mem_card : memory) {
//            if (mem_card.getPos().x == card.getPos().x && mem_card.getPos().y == card.getPos().y) {
//                return; // Already remembered
//            }
//        }
//        memory.push_back(card);
//    }
//};
//
//// --- From GameManager.h (Re-imagined for GUI interaction) ---
//class GameManager
//{
//private:
//    GameBoard board;
//    vector<unique_ptr<Player>> players;
//    int currentPlayerIndex;
//    bool isAIGame;
//
//public:
//    GameManager(int diff, bool isAI)
//        : board(diff), currentPlayerIndex(0), isAIGame(isAI)
//    {
//        players.push_back(make_unique<Player>("Player 1"));
//        if (isAI) {
//            players.push_back(make_unique<AI>(diff));
//        }
//        else {
//            players.push_back(make_unique<Player>("Player 2"));
//        }
//    }
//
//    GameBoard& getBoard() { return board; }
//    Player* getCurrentPlayer() { return players[currentPlayerIndex].get(); }
//    Player* getPlayer(int index) { return players[index].get(); }
//
//    void switchPlayer() {
//        currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
//    }
//
//    bool isAITurn() const {
//        return isAIGame && currentPlayerIndex == 1;
//    }
//
//    bool isGameOver() { return board.checkWin(); }
//};
//
//// ===================================================================
////
//// FRONTEND / GRAPHICS CLASSES
////
//// ===================================================================
//
//// --- Class to manage the actual Game Screen ---
//class GameScreen {
//private:
//    RenderWindow& window;
//    Font& font;
//    unique_ptr<GameManager> gameManager;
//
//    // Game state
//    enum class State { SelectingFirst, SelectingSecond, ShowingMismatch, AIThinking, GameOver };
//    State currentState;
//
//    // Visual elements
//    vector<vector<RectangleShape>> visualCards;
//    vector<vector<Text>> cardTexts;
//    Texture cardBackTexture;
//    bool textureLoaded;
//    Text hudText;
//    Text gameOverText;
//
//    // Logic variables
//    Card* firstSelection;
//    Card* secondSelection;
//    Clock delayTimer;
//
//public:
//    GameScreen(RenderWindow& win, Font& f, int gridSize, bool isAI)
//        : window(win), font(f) {
//        gameManager = make_unique<GameManager>(gridSize, isAI);
//        firstSelection = nullptr;
//        secondSelection = nullptr;
//        currentState = State::SelectingFirst;
//
//        // Try to load card back texture
//        if (cardBackTexture.loadFromFile("card_back.png")) {
//            textureLoaded = true;
//        }
//        else {
//            cout << "Warning: 'card_back.png' not found. Using solid color." << endl;
//            textureLoaded = false;
//        }
//
//        setupBoard();
//        setupHUD();
//    }
//
//    void setupBoard() {
//        GameBoard& board = gameManager->getBoard();
//        int rows = board.getRows();
//        int cols = board.getCols();
//
//        visualCards.resize(rows, vector<RectangleShape>(cols));
//        cardTexts.resize(rows, vector<Text>(cols));
//
//        float boardWidth = window.getSize().x * 0.8f;
//        float boardHeight = window.getSize().y * 0.8f;
//
//        float cardWidth = boardWidth / cols * 0.9f;
//        float cardHeight = boardHeight / rows * 0.9f;
//
//        float xSpacing = (boardWidth / cols) * 0.1f;
//        float ySpacing = (boardHeight / rows) * 0.1f;
//
//        float startX = (window.getSize().x - boardWidth) / 2.0f;
//        float startY = (window.getSize().y - boardHeight) / 2.0f;
//
//        for (int r = 0; r < rows; ++r) {
//            for (int c = 0; c < cols; ++c) {
//                // Card Shape
//                visualCards[r][c].setSize(Vector2f(cardWidth, cardHeight));
//                visualCards[r][c].setPosition(startX + c * (cardWidth + xSpacing), startY + r * (cardHeight + ySpacing));
//                visualCards[r][c].setOutlineThickness(3);
//                visualCards[r][c].setOutlineColor(Color(200, 200, 200));
//
//                // Card Text (its value)
//                char val = 'A' + board.getCard(r, c).getValue(); // Simple A-Z values
//                cardTexts[r][c].setFont(font);
//                cardTexts[r][c].setString(string(1, val));
//                cardTexts[r][c].setCharacterSize(cardHeight * 0.6);
//                cardTexts[r][c].setFillColor(Color::Black);
//                FloatRect textBounds = cardTexts[r][c].getLocalBounds();
//                cardTexts[r][c].setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
//                cardTexts[r][c].setPosition(visualCards[r][c].getPosition() + Vector2f(cardWidth / 2, cardHeight / 2));
//            }
//        }
//    }
//
//    void setupHUD() {
//        hudText.setFont(font);
//        hudText.setCharacterSize(24);
//        hudText.setFillColor(Color::White);
//        hudText.setPosition(20, 20);
//
//        gameOverText.setFont(font);
//        gameOverText.setCharacterSize(60);
//        gameOverText.setStyle(Text::Bold);
//        gameOverText.setFillColor(Color::Yellow);
//    }
//
//    // Main loop for the game screen
//    void run() {
//        while (window.isOpen()) {
//            handleEvents();
//            update();
//            render();
//
//            // Exit condition for the game screen loop
//            if (currentState == State::GameOver && Keyboard::isKeyPressed(Keyboard::Enter)) {
//                return;
//            }
//        }
//    }
//
//    void handleEvents() {
//        Event event;
//        while (window.pollEvent(event)) {
//            if (event.type == Event::Closed || (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)) {
//                window.close();
//            }
//            // Human player clicks a card
//            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
//                if (currentState == State::SelectingFirst || currentState == State::SelectingSecond) {
//                    if (!gameManager->isAITurn()) {
//                        handleCardClick(Mouse::getPosition(window));
//                    }
//                }
//            }
//        }
//    }
//
//    void handleCardClick(Vector2i mousePos) {
//        GameBoard& board = gameManager->getBoard();
//        for (int r = 0; r < board.getRows(); ++r) {
//            for (int c = 0; c < board.getCols(); ++c) {
//                if (visualCards[r][c].getGlobalBounds().contains(Vector2f(mousePos))) {
//                    Card& clickedCard = board.getCard(r, c);
//                    if (!clickedCard.getMatched() && !clickedCard.getFlipped()) {
//                        processSelection(&clickedCard);
//                    }
//                }
//            }
//        }
//    }
//
//    void processSelection(Card* selection) {
//        selection->flip();
//
//        if (currentState == State::SelectingFirst) {
//            firstSelection = selection;
//            currentState = State::SelectingSecond;
//        }
//        else if (currentState == State::SelectingSecond) {
//            secondSelection = selection;
//
//            // It's a match!
//            if (firstSelection->checkMatch(*secondSelection)) {
//                firstSelection->setMatched(true);
//                secondSelection->setMatched(true);
//                gameManager->getBoard().pairFound();
//                gameManager->getCurrentPlayer()->updateScore(1);
//
//                if (gameManager->isGameOver()) {
//                    currentState = State::GameOver;
//                }
//                else {
//                    currentState = State::SelectingFirst; // Player gets another turn
//                }
//            }
//            // It's a mismatch
//            else {
//                currentState = State::ShowingMismatch;
//                delayTimer.restart();
//            }
//        }
//    }
//
//    void update() {
//        // Handle mismatch timer
//        if (currentState == State::ShowingMismatch && delayTimer.getElapsedTime().asSeconds() > 1.0f) {
//            firstSelection->flip();
//            secondSelection->flip();
//            firstSelection = nullptr;
//            secondSelection = nullptr;
//            gameManager->switchPlayer();
//            currentState = State::SelectingFirst;
//        }
//
//        // Handle AI turn
//        if (gameManager->isAITurn() && currentState == State::SelectingFirst) {
//            currentState = State::AIThinking;
//            delayTimer.restart();
//        }
//
//        if (currentState == State::AIThinking && delayTimer.getElapsedTime().asSeconds() > 0.75f) { // AI "thinks"
//            AI* aiPlayer = static_cast<AI*>(gameManager->getCurrentPlayer());
//            if (aiPlayer) {
//                GameBoard& board = gameManager->getBoard();
//
//                // Remember cards if AI just saw a mismatch
//                if (firstSelection) aiPlayer->rememberCard(*firstSelection);
//                if (secondSelection) aiPlayer->rememberCard(*secondSelection);
//
//                // AI makes its move
//                Pair p1 = aiPlayer->chooseCard(board);
//                processSelection(&board.getCard(p1.x, p1.y));
//
//                // After selecting first card, remember it and choose second
//                if (currentState == State::SelectingSecond) {
//                    aiPlayer->rememberCard(*firstSelection);
//                    Pair p2 = aiPlayer->chooseCard(board);
//                    processSelection(&board.getCard(p2.x, p2.y));
//                }
//            }
//        }
//
//        // Update HUD text
//        string hudString = gameManager->getCurrentPlayer()->getName() + "'s Turn\n";
//        hudString += gameManager->getPlayer(0)->getName() + " Score: " + to_string(gameManager->getPlayer(0)->getScore()) + "\n";
//        hudString += gameManager->getPlayer(1)->getName() + " Score: " + to_string(gameManager->getPlayer(1)->getScore());
//        hudText.setString(hudString);
//    }
//
//    void render() {
//        window.clear(Color(10, 15, 30)); // Dark blue background
//
//        GameBoard& board = gameManager->getBoard();
//        for (int r = 0; r < board.getRows(); ++r) {
//            for (int c = 0; c < board.getCols(); ++c) {
//                Card& card = board.getCard(r, c);
//                RectangleShape& vCard = visualCards[r][c];
//
//                // Fade out matched cards
//                if (card.getMatched()) {
//                    Color c = vCard.getFillColor();
//                    if (c.a > 0) {
//                        vCard.setFillColor(Color(c.r, c.g, c.b, c.a - 5));
//                        vCard.setOutlineColor(Color(c.r, c.g, c.b, c.a - 5));
//                    }
//                }
//
//                if (card.getFlipped() || (card.getMatched() && vCard.getFillColor().a > 0)) {
//                    // Face up card
//                    vCard.setTexture(nullptr);
//                    vCard.setFillColor(card.getMatched() ? Color::Green : Color::White);
//                    window.draw(vCard);
//                    window.draw(cardTexts[r][c]);
//                }
//                else {
//                    // Face down card
//                    if (textureLoaded) {
//                        vCard.setTexture(&cardBackTexture);
//                    }
//                    else {
//                        vCard.setFillColor(Color(0, 50, 200)); // Default blue back
//                    }
//                    window.draw(vCard);
//                }
//            }
//        }
//
//        window.draw(hudText);
//
//        if (currentState == State::GameOver) {
//            Player* p1 = gameManager->getPlayer(0);
//            Player* p2 = gameManager->getPlayer(1);
//            string winnerStr;
//            if (p1->getScore() > p2->getScore()) winnerStr = p1->getName() + " Wins!";
//            else if (p2->getScore() > p1->getScore()) winnerStr = p2->getName() + " Wins!";
//            else winnerStr = "It's a Draw!";
//
//            gameOverText.setString(winnerStr + "\nPress ENTER to return to menu");
//            FloatRect textBounds = gameOverText.getLocalBounds();
//            gameOverText.setOrigin(textBounds.width / 2.0f, textBounds.height / 2.0f);
//            gameOverText.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
//
//            RectangleShape overlay(Vector2f(window.getSize()));
//            overlay.setFillColor(Color(0, 0, 0, 150));
//            window.draw(overlay);
//            window.draw(gameOverText);
//        }
//
//        window.display();
//    }
//};
//
//// --- Menu Classes (From your original file, slightly modified) ---
//// Note: GridCell background effect is removed for clarity, focusing on the game.
//class MenuButton {
//private:
//    RectangleShape buttonShape, glowShape;
//    Text buttonText;
//    bool isHovered;
//public:
//    MenuButton(Vector2f pos, Vector2f sz, const string& text, Font& font)
//        : isHovered(false) {
//        buttonShape.setPosition(pos);
//        buttonShape.setSize(sz);
//        buttonShape.setFillColor(Color(20, 30, 60, 180));
//
//        buttonText.setFont(font);
//        buttonText.setString(text);
//        buttonText.setCharacterSize(20);
//        buttonText.setFillColor(Color::White);
//        FloatRect textBounds = buttonText.getLocalBounds();
//        buttonText.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
//        buttonText.setPosition(pos.x + sz.x / 2.0f, pos.y + sz.y / 2.0f);
//    }
//
//    void update(Vector2f mousePos) {
//        isHovered = buttonShape.getGlobalBounds().contains(mousePos);
//        buttonShape.setFillColor(isHovered ? Color(0, 100, 150, 220) : Color(20, 30, 60, 180));
//    }
//
//    void draw(RenderWindow& window) const {
//        window.draw(buttonShape);
//        window.draw(buttonText);
//    }
//
//    bool isClicked(Vector2f mousePos) const {
//        return buttonShape.getGlobalBounds().contains(mousePos);
//    }
//};
//
//class GameMenu {
//private:
//    enum class ScreenState { MainMenu, ModeSelection, GameRunning };
//    ScreenState currentScreen;
//
//    RenderWindow& window;
//    Font& font;
//
//    Text title, subtitle;
//    vector<MenuButton> mainButtons, modeButtons;
//
//    string selectedModeName;
//    bool selectedModeIsAI;
//
//public:
//    GameMenu(RenderWindow& win, Font& f)
//        : window(win), font(f), currentScreen(ScreenState::MainMenu) {
//        setupMainMenu();
//    }
//
//    void setupMainMenu() {
//        title.setFont(font);
//        title.setString("MEMORY MATCHING CHAMPIONSHIP");
//        title.setCharacterSize(40);
//        title.setStyle(Text::Bold);
//        FloatRect titleBounds = title.getLocalBounds();
//        title.setOrigin(titleBounds.width / 2.0f, 0);
//        title.setPosition(window.getSize().x / 2.0f, 80);
//        title.setFillColor(Color::White);
//
//        subtitle.setFont(font);
//        subtitle.setString("An OOP Hackathon Project");
//        subtitle.setCharacterSize(24);
//        titleBounds = subtitle.getLocalBounds();
//        subtitle.setOrigin(titleBounds.width / 2.0f, 0);
//        subtitle.setPosition(window.getSize().x / 2.0f, 140);
//        subtitle.setFillColor(Color(0, 255, 255));
//
//        mainButtons.clear();
//        vector<string> labels = { "Classic Mode (2P)", "AI Mode (1P)", "Exit" };
//        for (size_t i = 0; i < labels.size(); ++i) {
//            mainButtons.emplace_back(
//                Vector2f(window.getSize().x / 2.0f - 125, 250 + i * 80),
//                Vector2f(250, 60),
//                labels[i], font
//            );
//        }
//    }
//
//    void setupModeSelectionScreen() {
//        title.setString(selectedModeName);
//
//        modeButtons.clear();
//        vector<string> labels = { "Beginner (4x4)", "Intermediate (6x6)", "Advanced (8x8)", "Back" };
//        for (size_t i = 0; i < labels.size(); ++i) {
//            modeButtons.emplace_back(
//                Vector2f(window.getSize().x / 2.0f - 125, 250 + i * 80),
//                Vector2f(250, 60),
//                labels[i], font
//            );
//        }
//    }
//
//    void run() {
//        while (window.isOpen()) {
//            handleEvents();
//            update();
//            render();
//        }
//    }
//
//    void handleEvents() {
//        Event event;
//        while (window.pollEvent(event)) {
//            if (event.type == Event::Closed || (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)) {
//                window.close();
//            }
//
//            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
//                Vector2f mousePos = Vector2f(Mouse::getPosition(window));
//                if (currentScreen == ScreenState::MainMenu) {
//                    handleMainMenuClick(mousePos);
//                }
//                else if (currentScreen == ScreenState::ModeSelection) {
//                    handleModeSelectionClick(mousePos);
//                }
//            }
//        }
//    }
//
//    void handleMainMenuClick(Vector2f mousePos) {
//        if (mainButtons[0].isClicked(mousePos)) { // Classic
//            selectedModeName = "Classic Mode (2P)";
//            selectedModeIsAI = false;
//            currentScreen = ScreenState::ModeSelection;
//            setupModeSelectionScreen();
//        }
//        else if (mainButtons[1].isClicked(mousePos)) { // AI
//            selectedModeName = "AI Mode (1P)";
//            selectedModeIsAI = true;
//            currentScreen = ScreenState::ModeSelection;
//            setupModeSelectionScreen();
//        }
//        else if (mainButtons[2].isClicked(mousePos)) { // Exit
//            window.close();
//        }
//    }
//
//    void handleModeSelectionClick(Vector2f mousePos) {
//        int gridSize = 0;
//        if (modeButtons[0].isClicked(mousePos)) gridSize = 4;
//        else if (modeButtons[1].isClicked(mousePos)) gridSize = 6;
//        else if (modeButtons[2].isClicked(mousePos)) gridSize = 8;
//        else if (modeButtons[3].isClicked(mousePos)) { // Back
//            currentScreen = ScreenState::MainMenu;
//            setupMainMenu();
//            return;
//        }
//
//        if (gridSize > 0) {
//            GameScreen game(window, font, gridSize, selectedModeIsAI);
//            game.run(); // This will block until the game is over
//            // After game, return to main menu
//            currentScreen = ScreenState::MainMenu;
//            setupMainMenu();
//        }
//    }
//
//    void update() {
//        Vector2f mousePos = Vector2f(Mouse::getPosition(window));
//        if (currentScreen == ScreenState::MainMenu) {
//            for (auto& button : mainButtons) button.update(mousePos);
//        }
//        else if (currentScreen == ScreenState::ModeSelection) {
//            for (auto& button : modeButtons) button.update(mousePos);
//        }
//    }
//
//    void render() {
//        window.clear(Color(10, 15, 30));
//
//        window.draw(title);
//        window.draw(subtitle);
//
//        if (currentScreen == ScreenState::MainMenu) {
//            for (const auto& button : mainButtons) button.draw(window);
//        }
//        else if (currentScreen == ScreenState::ModeSelection) {
//            for (const auto& button : modeButtons) button.draw(window);
//        }
//
//        window.display();
//    }
//};
//
//// ===================================================================
////
//// MAIN FUNCTION
////
//// ===================================================================
//
//int main() {
//    // Seeding the random number generator is crucial for shuffling
//    srand(static_cast<unsigned int>(time(0)));
//
//    RenderWindow window(VideoMode(1024, 768), "Memory Matching Championship", Style::Default);
//    window.setFramerateLimit(60);
//
//    Font font;
//    if (!font.loadFromFile("arial.ttf")) {
//        cerr << "Error: Could not load 'arial.ttf'. Make sure the file is in the same directory." << endl;
//        return 1;
//    }
//
//    try {
//        GameMenu menu(window, font);
//        menu.run();
//    }
//    catch (const exception& e) {
//        cerr << "An unexpected error occurred: " << e.what() << '\n';
//        return 1;
//    }
//
//    return 0;
//}





//===========================================


//full code implementation
//#include <SFML/Graphics.hpp>
//#include <iostream>
//#include <vector>
//#include <string>
//#include <cmath>
//#include <ctime>
//#include <cstdlib>
//#include <algorithm>
//#include <memory>
//
// // Use standard namespace for brevity in this single-file project
//using namespace std;
//using namespace sf;
//
//// ===================================================================
////
//// PROVIDED BACKEND CODE - INCLUDED AS IS
////
//// ===================================================================
//
//// --- From Pair.h ---
//struct Pair
//{
//    int x, y;
//    Pair() : x(-1), y(-1) {}
//    Pair(int x1, int y1) :x(x1), y(y1) {}
//    Pair(const Pair& other)
//    {
//        x = other.x;
//        y = other.y;
//    }
//};
//
//// --- From Card.h ---
//
//class Card
//{
//private:
//    int value;
//    bool isFlipped;
//    bool isMatched;
//    Pair pos;
//
//public:
//    Card() : isFlipped(false), isMatched(false), value(-1) {}
//
//    void setValue(int val) { this->value = val; }
//    int getValue() const { return value; }
//
//    void setFlipped(bool flipped) { isFlipped = flipped; }
//    bool getFlipped() const { return isFlipped; }
//
//    void setMatched(bool matched) { isMatched = matched; }
//    bool getMatched() const { return isMatched; }
//
//    void setPos(int x, int y) { pos.x = x; pos.y = y; }
//    Pair getPos() const { return pos; }
//
//    void flip() { isFlipped = !isFlipped; }
//
//    bool checkMatch(const Card& other) const {
//        return value == other.value;
//    }
//
//    // Add the draw method
//    void draw() {
//        isMatched = true; // Mark the card as matched
//    }
//
//    // Declare the static member
//    static char cardFace[];
//};
//
//
//
//// Static array of characters to display on cards
//char Card::cardFace[] = {
//    '1','2','3','4','5','6','7','8','9',
//    'A','B','C','D','E','F','G','H','I','J','K','L','M',
//    'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
//    '@','#','$','%','&','*','!','?','+'
//};
//
//// --- From GameBoard.h ---
//const int MAX_SIZE = 8;
//class GameBoard
//{
//private:
//    Card cards[MAX_SIZE][MAX_SIZE];
//    int rows, cols;
//    int pairCount;
//public:
//    // Constructor uses your original logic
//    GameBoard(int difficulty) : rows(difficulty), cols(difficulty), pairCount(difficulty* difficulty / 2)
//    {
//        initializeBoard();
//        shuffleCards();
//    }
//
//    void initializeBoard()
//    {
//        int k = 0;
//        vector<int> values;
//        for (int i = 0; i < (rows * cols) / 2; ++i) {
//            values.push_back(i);
//            values.push_back(i);
//        }
//        random_shuffle(values.begin(), values.end());
//
//        for (int i = 0; i < MAX_SIZE; i++) {
//            for (int j = 0; j < MAX_SIZE; j++) {
//                if (i < rows && j < cols) {
//                    cards[i][j].setValue(values[k++]);
//                }
//            }
//        }
//    }
//
//    void shuffleCards()
//    {
//        for (int i = 0; i < rows; i++) {
//            for (int j = 0; j < cols; j++) {
//                int randomRow = rand() % rows;
//                int randomColumn = rand() % cols;
//                // Using a temporary variable for swap as std::swap with array members can be tricky
//                int tempValue = cards[i][j].getValue();
//                cards[i][j].setValue(cards[randomRow][randomColumn].getValue());
//                cards[randomRow][randomColumn].setValue(tempValue);
//            }
//        }
//
//        for (int i = 0; i < rows; i++) {
//            for (int j = 0; j < cols; j++) {
//                cards[i][j].setPos(i, j);
//            }
//        }
//    }
//
//    Card& getCard(int i, int j) { return cards[i][j]; }
//    int getRows() const { return rows; }
//    int getCols() const { return cols; }
//
//    void display() { /* Original console display, unused by GUI */ }
//
//    bool checkWin()
//    {
//        if (pairCount <= 0)
//            return true;
//        return false;
//    }
//
//    void draw(Card* c1, Card* c2)
//    {
//        c1->draw(); // Marks as matched
//        c2->draw(); // Marks as matched
//        pairCount--;
//    }
//};
//
//// --- From Player.h ---
//class Player
//{
//private:
//    string name;
//    int score;
//    int gamesWon;
//    int gamesPlayed;
//protected:
//    GameBoard* board;
//public:
//    Player(string name = "", int score = 0, int gamesWon = 0, int gamesPlayed = 0) :name(name), score(score), gamesWon(gamesWon), gamesPlayed(gamesPlayed), board(nullptr) {}
//    Player(GameBoard& b, string name = "", int score = 0, int gamesWon = 0) :name(name), score(score), gamesWon(gamesWon), board(&b) {}
//    virtual ~Player() {}
//
//    void updateScore(int points) { score += points; }
//    void incrementWins() { gamesWon++; }
//    void displayStats() { /* Original console display, unused by GUI */ }
//
//    virtual Pair chooseCard(int& f)
//    {
//        // Original console input logic, bypassed by GUI
//        Pair temp;
//        cout << "Enter row: ";
//        cin >> temp.x;
//        cout << "Enter column: ";
//        cin >> temp.y;
//        return temp;
//    }
//    void setName(string name) { this->name = name; }
//    string getName() { return name; }
//    int getScore() const { return score; }
//};
//
//// --- From AI.h ---
//class AI : public Player
//{
//    vector<Card> memory;
//    int size;
//    Pair nextPair;
//    int difficulty;
//public:
//    AI(GameBoard& g, int diff) :Player(g, "AI"), nextPair(-1, -1), difficulty(diff)
//    {
//        size = diff * diff;
//    }
//    // Your original AI logic is called directly by the GUI
//    Pair chooseCard(int& f) override
//    {
//        f++;
//        Pair temp;
//        if (nextPair.x != -1 && nextPair.y != -1) {
//            temp.x = nextPair.x;
//            temp.y = nextPair.y;
//            nextPair.x = -1;
//            nextPair.y = -1;
//            return temp;
//        }
//
//        for (size_t i = 0; i < memory.size(); i++) {
//            for (size_t j = i + 1; j < memory.size(); j++) {
//                if (memory[i].getValue() == memory[j].getValue() && !board->getCard(memory[i].getPos().x, memory[i].getPos().y).getMatched()) {
//                    nextPair = memory[j].getPos();
//                    temp = memory[i].getPos();
//                    return temp;
//                }
//            }
//        }
//        do {
//            temp.x = rand() % difficulty;
//            temp.y = rand() % difficulty;
//        } while (board->getCard(temp.x, temp.y).getMatched() || board->getCard(temp.x, temp.y).getFlipped());
//        return temp;
//    }
//    // This is the only addition: a helper for the GUI to let the AI "see" cards
//    void rememberCard(const Card& card) {
//        if (card.getMatched()) return;
//        for (const auto& mem_card : memory) {
//            if (mem_card.getPos().x == card.getPos().x && mem_card.getPos().y == card.getPos().y) {
//                return; // Already in memory
//            }
//        }
//        memory.push_back(card);
//    }
//};
//
//// --- From GameManager.h ---
//class GameManager
//{
//private:
//    GameBoard board;
//    Player** players;
//    int currentPlayer;
//public:
//    GameManager(int diff, bool is2P) :board(diff), currentPlayer(0)
//    {
//        players = new Player * [2];
//        if (is2P) {
//            players[0] = new Player("Player 1");
//            players[1] = new Player("Player 2");
//        }
//        else {
//            players[0] = new Player("Player 1");
//            players[1] = new AI(board, diff);
//        }
//    }
//    ~GameManager() {
//        for (int i = 0; i < 2; i++) { delete players[i]; }
//        delete[] players;
//    }
//    // This is the bridge method for the GUI to interact with your backend
//    void processGUIMove(Card* c1, Card* c2) {
//        if (c1->checkMatch(*c2)) {
//            board.draw(c1, c2); // Your original method for a match
//            players[currentPlayer]->updateScore(1);
//            // Player gets another turn, so we don't switch.
//        }
//        else {
//            // On mismatch, switch player.
//            // Before switching, let an AI player remember the cards.
//            if (auto ai = dynamic_cast<AI*>(players[currentPlayer])) {
//                ai->rememberCard(*c1);
//                ai->rememberCard(*c2);
//            }
//            switchPlayer();
//        }
//    }
//    GameBoard& getBoard() { return board; }
//    Player* getCurrentPlayer() { return players[currentPlayer]; }
//    Player* getPlayer(int i) { return players[i]; }
//    void switchPlayer() { currentPlayer = (currentPlayer + 1) % 2; }
//    bool isGameOver() { return board.checkWin(); }
//
//    // --- Your original methods, kept but unused by the GUI ---
//    void startGame() { /* GUI has its own game loop */ }
//    void processMove() { /* Replaced by processGUIMove */ }
//};
//
//
//// ===================================================================
////
//// GRAPHICAL INTERFACE (SFML Frontend)
////
//// ===================================================================
//
//class GameScreen {
//private:
//    RenderWindow& window;
//    Font& font;
//    unique_ptr<GameManager> gameManager;
//
//    enum class GameState { AwaitingFirst, AwaitingSecond, ShowingMismatch, AIThinking, GameOver };
//    GameState currentState;
//
//    vector<vector<RectangleShape>> visualCards;
//    vector<vector<Text>> cardTexts;
//    Texture cardBackTexture;
//    bool textureLoaded;
//    Text hudText, gameOverText;
//
//    Card* firstSelection, * secondSelection;
//    Clock delayTimer;
//
//public:
//    GameScreen(RenderWindow& win, Font& f, int gridSize, bool isAI)
//        : window(win), font(f), firstSelection(nullptr), secondSelection(nullptr) {
//        gameManager = make_unique<GameManager>(gridSize, !isAI); // Your constructor expects is2P
//        currentState = GameState::AwaitingFirst;
//
//        if (!cardBackTexture.loadFromFile("card_back.png")) textureLoaded = false;
//        else textureLoaded = true;
//
//        setupVisuals();
//    }
//
//    void setupVisuals() {
//        GameBoard& board = gameManager->getBoard();
//        int rows = board.getRows();
//        int cols = board.getCols();
//        visualCards.resize(rows, vector<RectangleShape>(cols));
//        cardTexts.resize(rows, vector<Text>(cols));
//
//        float cardW = 100.0f, cardH = 140.0f, spacing = 20.0f;
//        float totalW = cols * cardW + (cols - 1) * spacing;
//        float totalH = rows * cardH + (rows - 1) * spacing;
//        float startX = (window.getSize().x - totalW) / 2.0f;
//        float startY = (window.getSize().y - totalH) / 2.0f + 30.0f;
//
//        for (int r = 0; r < rows; ++r) {
//            for (int c = 0; c < cols; ++c) {
//                visualCards[r][c].setSize(Vector2f(cardW, cardH));
//                visualCards[r][c].setPosition(startX + c * (cardW + spacing), startY + r * (cardH + spacing));
//                visualCards[r][c].setOutlineThickness(4);
//                visualCards[r][c].setOutlineColor(Color(200, 200, 200, 150));
//
//                cardTexts[r][c].setFont(font);
//                cardTexts[r][c].setString(string(1, Card::cardFace[board.getCard(r, c).getValue()]));
//                cardTexts[r][c].setCharacterSize(72);
//                cardTexts[r][c].setFillColor(Color::Black);
//                FloatRect tb = cardTexts[r][c].getLocalBounds();
//                cardTexts[r][c].setOrigin(tb.left + tb.width / 2.0f, tb.top + tb.height / 2.0f);
//                cardTexts[r][c].setPosition(visualCards[r][c].getPosition() + Vector2f(cardW / 2.0f, cardH / 2.0f));
//            }
//        }
//
//        hudText.setFont(font);
//        hudText.setCharacterSize(28);
//        hudText.setPosition(20, 20);
//        hudText.setStyle(Text::Bold);
//
//        gameOverText.setFont(font);
//        gameOverText.setCharacterSize(60);
//        gameOverText.setStyle(Text::Bold);
//    }
//
//    void run() {
//        if (dynamic_cast<AI*>(gameManager->getCurrentPlayer())) currentState = GameState::AIThinking;
//        while (window.isOpen()) {
//            handleEvents();
//            update();
//            render();
//            if (currentState == GameState::GameOver && Keyboard::isKeyPressed(Keyboard::Enter)) return;
//        }
//    }
//
//    void handleEvents() {
//        Event event;
//        while (window.pollEvent(event)) {
//            if (event.type == Event::Closed || (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)) window.close();
//            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
//                if (currentState == GameState::AwaitingFirst || currentState == GameState::AwaitingSecond) {
//                    if (!dynamic_cast<AI*>(gameManager->getCurrentPlayer())) {
//                        handleCardClick(Mouse::getPosition(window));
//                    }
//                }
//            }
//        }
//    }
//
//    void handleCardClick(Vector2i mousePos) {
//        GameBoard& board = gameManager->getBoard();
//        for (int r = 0; r < board.getRows(); ++r) {
//            for (int c = 0; c < board.getCols(); ++c) {
//                if (visualCards[r][c].getGlobalBounds().contains(Vector2f(mousePos))) {
//                    Card& clickedCard = board.getCard(r, c);
//                    if (!clickedCard.getMatched() && !clickedCard.getFlipped()) {
//                        clickedCard.flip();
//                        if (currentState == GameState::AwaitingFirst) {
//                            firstSelection = &clickedCard;
//                            currentState = GameState::AwaitingSecond;
//                        }
//                        else {
//                            secondSelection = &clickedCard;
//                            gameManager->processGUIMove(firstSelection, secondSelection);
//                            if (firstSelection->getMatched()) {
//                                if (gameManager->isGameOver()) currentState = GameState::GameOver;
//                                else currentState = GameState::AwaitingFirst; // Gets another turn
//                            }
//                            else {
//                                currentState = GameState::ShowingMismatch;
//                                delayTimer.restart();
//                            }
//                        }
//                    }
//                }
//            }
//        }
//    }
//
//    void update() {
//        if (currentState == GameState::ShowingMismatch && delayTimer.getElapsedTime().asSeconds() > 1.2f) {
//            firstSelection->flip();
//            secondSelection->flip();
//            if (dynamic_cast<AI*>(gameManager->getCurrentPlayer())) currentState = GameState::AIThinking;
//            else currentState = GameState::AwaitingFirst;
//        }
//
//        if (currentState == GameState::AIThinking && delayTimer.getElapsedTime().asSeconds() > 0.75f) {
//            AI* ai = static_cast<AI*>(gameManager->getCurrentPlayer());
//            int f = 0;
//            // First card pick
//            Pair p1 = ai->chooseCard(f);
//            firstSelection = &gameManager->getBoard().getCard(p1.x, p1.y);
//            firstSelection->flip();
//            ai->rememberCard(*firstSelection); // AI remembers its own pick
//            // Second card pick
//            Pair p2 = ai->chooseCard(f);
//            secondSelection = &gameManager->getBoard().getCard(p2.x, p2.y);
//            secondSelection->flip();
//
//            gameManager->processGUIMove(firstSelection, secondSelection);
//            if (firstSelection->getMatched()) {
//                if (gameManager->isGameOver()) currentState = GameState::GameOver;
//                else { delayTimer.restart(); currentState = GameState::AIThinking; } // Another turn
//            }
//            else {
//                currentState = GameState::ShowingMismatch;
//                delayTimer.restart();
//            }
//        }
//
//        hudText.setString(gameManager->getPlayer(0)->getName() + " Score: " + to_string(gameManager->getPlayer(0)->getScore()) +
//            "   |   " + gameManager->getPlayer(1)->getName() + " Score: " + to_string(gameManager->getPlayer(1)->getScore()));
//        hudText.setFillColor(Color::White);
//        if (!dynamic_cast<AI*>(gameManager->getCurrentPlayer())) {
//            hudText.setString("Your Turn: " + gameManager->getCurrentPlayer()->getName() + "\n" + hudText.getString());
//        }
//        else {
//            hudText.setString("AI's Turn\n" + hudText.getString());
//            hudText.setFillColor(Color::Cyan);
//        }
//    }
//
//    void render() {
//        window.clear(Color(10, 15, 30));
//        GameBoard& board = gameManager->getBoard();
//        for (int r = 0; r < board.getRows(); ++r) {
//            for (int c = 0; c < board.getCols(); ++c) {
//                Card& card = board.getCard(r, c);
//                RectangleShape& vCard = visualCards[r][c];
//
//                if (card.getMatched()) {
//                    vCard.setFillColor(Color(0, 255, 0, 100)); // Fade to green
//                    vCard.setOutlineColor(Color(0, 255, 0, 150));
//                    window.draw(vCard);
//                }
//                else if (card.getFlipped()) {
//                    vCard.setTexture(nullptr);
//                    vCard.setFillColor(Color::White);
//                    vCard.setOutlineColor(Color::Yellow);
//                    window.draw(vCard);
//                    window.draw(cardTexts[r][c]);
//                }
//                else {
//                    if (textureLoaded) vCard.setTexture(&cardBackTexture);
//                    else vCard.setFillColor(Color(30, 80, 200));
//                    vCard.setOutlineColor(Color(200, 200, 200, 150));
//                    window.draw(vCard);
//                }
//            }
//        }
//
//        window.draw(hudText);
//
//        if (currentState == GameState::GameOver) {
//            string winnerStr;
//            int s1 = gameManager->getPlayer(0)->getScore();
//            int s2 = gameManager->getPlayer(1)->getScore();
//            if (s1 > s2) winnerStr = gameManager->getPlayer(0)->getName() + " Wins!";
//            else if (s2 > s1) winnerStr = gameManager->getPlayer(1)->getName() + " Wins!";
//            else winnerStr = "It's a Draw!";
//
//            gameOverText.setString(winnerStr + "\nPress ENTER to return to menu");
//            FloatRect tb = gameOverText.getLocalBounds();
//            gameOverText.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
//            gameOverText.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);
//
//            RectangleShape overlay(Vector2f(window.getSize()));
//            overlay.setFillColor(Color(0, 0, 0, 180));
//            window.draw(overlay);
//            window.draw(gameOverText);
//        }
//        window.display();
//    }
//};
//
//// --- Main Menu System ---
//class MenuButton {
//private:
//    RectangleShape buttonShape; Text buttonText;
//public:
//    MenuButton(Vector2f pos, Vector2f sz, const string& text, Font& font) {
//        buttonShape.setPosition(pos);
//        buttonShape.setSize(sz);
//        buttonText.setFont(font); buttonText.setString(text);
//        buttonText.setCharacterSize(24);
//        FloatRect tb = buttonText.getLocalBounds();
//        buttonText.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
//        buttonText.setPosition(pos.x + sz.x / 2.f, pos.y + sz.y / 2.f);
//    }
//    void update(Vector2f mousePos) {
//        bool hovered = buttonShape.getGlobalBounds().contains(mousePos);
//        buttonShape.setFillColor(hovered ? Color(0, 100, 150, 220) : Color(20, 30, 60, 180));
//        buttonShape.setOutlineColor(hovered ? Color::Cyan : Color::Transparent);
//        buttonShape.setOutlineThickness(3);
//    }
//    void draw(RenderWindow& w) const { w.draw(buttonShape); w.draw(buttonText); }
//    bool isClicked(Vector2f p) const { return buttonShape.getGlobalBounds().contains(p); }
//};
//
//class GameMenu {
//private:
//    enum class Screen { Main, ModeSelect };
//    Screen currentScreen;
//    RenderWindow& window; Font& font;
//    Text title, subtitle;
//    vector<MenuButton> mainButtons, modeButtons;
//    bool selectedModeIsAI;
//public:
//    GameMenu(RenderWindow& win, Font& f) : window(win), font(f), currentScreen(Screen::Main) {
//        setupMain();
//    }
//    void setupMain() {
//        title.setFont(font); title.setString("MEMORY CHAMPIONSHIP"); title.setCharacterSize(50);
//        FloatRect tb = title.getLocalBounds(); title.setOrigin(tb.left + tb.width / 2.f, tb.top);
//        title.setPosition(window.getSize().x / 2.f, 80); title.setFillColor(Color::White);
//
//        subtitle.setFont(font); subtitle.setString("OOP Hackathon Project"); subtitle.setCharacterSize(28);
//        tb = subtitle.getLocalBounds(); subtitle.setOrigin(tb.left + tb.width / 2.f, tb.top);
//        subtitle.setPosition(window.getSize().x / 2.f, 150); subtitle.setFillColor(Color(0, 255, 255));
//
//        mainButtons.clear();
//        vector<string> labels = { "Player vs AI", "Player vs Player", "Exit" };
//        for (size_t i = 0; i < labels.size(); ++i)
//            mainButtons.emplace_back(Vector2f(window.getSize().x / 2.f - 150, 280 + i * 90), Vector2f(300, 70), labels[i], font);
//    }
//    void setupModeSelect() {
//        modeButtons.clear();
//        vector<string> labels = { "Beginner (4x4)", "Intermediate (6x6)", "Advanced (8x8)", "Back" };
//        for (size_t i = 0; i < labels.size(); ++i)
//            modeButtons.emplace_back(Vector2f(window.getSize().x / 2.f - 150, 280 + i * 90), Vector2f(300, 70), labels[i], font);
//    }
//    void run() {
//        while (window.isOpen()) {
//            Event event;
//            while (window.pollEvent(event)) {
//                if (event.type == Event::Closed || (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)) window.close();
//                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) handleMouseClick(Vector2f(Mouse::getPosition(window)));
//            }
//            update();
//            render();
//        }
//    }
//    void handleMouseClick(Vector2f pos) {
//        if (currentScreen == Screen::Main) {
//            if (mainButtons[0].isClicked(pos)) { selectedModeIsAI = true; currentScreen = Screen::ModeSelect; setupModeSelect(); }
//            if (mainButtons[1].isClicked(pos)) { selectedModeIsAI = false; currentScreen = Screen::ModeSelect; setupModeSelect(); }
//            if (mainButtons[2].isClicked(pos)) window.close();
//        }
//        else if (currentScreen == Screen::ModeSelect) {
//            int gridSize = 0;
//            if (modeButtons[0].isClicked(pos)) gridSize = 4;
//            if (modeButtons[1].isClicked(pos)) gridSize = 6;
//            if (modeButtons[2].isClicked(pos)) gridSize = 8;
//            if (modeButtons[3].isClicked(pos)) { currentScreen = Screen::Main; setupMain(); return; }
//            if (gridSize > 0) {
//                GameScreen game(window, font, gridSize, selectedModeIsAI);
//                game.run();
//                currentScreen = Screen::Main;
//                setupMain();
//            }
//        }
//    }
//    void update() {
//        Vector2f mPos = Vector2f(Mouse::getPosition(window));
//        if (currentScreen == Screen::Main) for (auto& b : mainButtons) b.update(mPos);
//        else for (auto& b : modeButtons) b.update(mPos);
//    }
//    void render() {
//        window.clear(Color(10, 15, 30));
//        window.draw(title); window.draw(subtitle);
//        if (currentScreen == Screen::Main) for (const auto& b : mainButtons) b.draw(window);
//        else for (const auto& b : modeButtons) b.draw(window);
//        window.display();
//    }
//};
//
//// ===================================================================
////
//// MAIN FUNCTION
////
//// ===================================================================
//
//int main() {
//    // Seeding the random number generator is crucial for shuffling and AI
//    srand(static_cast<unsigned int>(time(0)));
//
//    RenderWindow window(VideoMode(1280, 800), "Memory Matching Championship", Style::Default);
//    window.setFramerateLimit(60);
//
//    Font font;
//    if (!font.loadFromFile("arial.ttf")) {
//        cerr << "FATAL ERROR: Font file 'arial.ttf' not found." << endl;
//        return 1;
//    }
//
//    try {
//        GameMenu menu(window, font);
//        menu.run();
//    }
//    catch (const exception& e) {
//        cerr << "An unexpected error occurred: " << e.what() << '\n';
//        return 1;
//    }
//
//    return 0;
//}