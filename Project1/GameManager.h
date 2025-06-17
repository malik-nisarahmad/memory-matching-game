


#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include "GameBoard.h"
#include "Player.h"
#include <cmath>
#include <random>

using namespace sf;
using namespace std;

struct NeonCircle {
    CircleShape shape;
    Vector2f velocity;
    float lifetime;
    float maxLifetime;
    float pulsePeriod;
    Color baseColor;
    bool isActive;

    NeonCircle(Vector2f pos, float radius, Color color) {
        shape.setRadius(radius);
        shape.setPosition(pos);
        shape.setOrigin(radius, radius);
        shape.setFillColor(Color::Transparent);
        shape.setOutlineThickness(2.0f);

        baseColor = color;
        shape.setOutlineColor(color);

       
        float angle = static_cast<float>(rand()) / RAND_MAX * 2 * 3.14159f;
        float speed = 20.0f + static_cast<float>(rand()) / RAND_MAX * 40.0f;
        velocity = Vector2f(cos(angle) * speed, sin(angle) * speed);

        maxLifetime = 3.0f + static_cast<float>(rand()) / RAND_MAX * 2.0f;
        lifetime = maxLifetime;
        pulsePeriod = 1.0f + static_cast<float>(rand()) / RAND_MAX * 2.0f;
        isActive = true;
    }

    void update(float dt, Vector2u windowSize) {
        if (!isActive) return;

        lifetime -= dt;
        if (lifetime <= 0) {
            isActive = false;
            return;
        }

        // Move
        Vector2f pos = shape.getPosition();
        pos += velocity * dt;

        // Bounce off edges
        if (pos.x < 0 || pos.x > windowSize.x) velocity.x *= -1;
        if (pos.y < 0 || pos.y > windowSize.y) velocity.y *= -1;

        pos.x = max(0.0f, min(static_cast<float>(windowSize.x), pos.x));
        pos.y = max(0.0f, min(static_cast<float>(windowSize.y), pos.y));
        shape.setPosition(pos);

        // Pulsing effect
        float pulseTime = fmod(maxLifetime - lifetime, pulsePeriod) / pulsePeriod;
        float alpha = 100 + 100 * sin(pulseTime * 2 * 3.14159f);
        float fadeAlpha = (lifetime / maxLifetime) * alpha;

        Color color = baseColor;
        color.a = static_cast<Uint8>(fadeAlpha);
        shape.setOutlineColor(color);
    }
};

struct NeonTriangle {
    ConvexShape shape;
    Vector2f velocity;
    float lifetime;
    float maxLifetime;
    float rotationSpeed;
    Color baseColor;
    bool isActive;

    NeonTriangle(Vector2f pos, float size, Color color) {
        shape.setPointCount(3);
        shape.setPoint(0, Vector2f(0, -size));
        shape.setPoint(1, Vector2f(-size * 0.866f, size * 0.5f));
        shape.setPoint(2, Vector2f(size * 0.866f, size * 0.5f));

        shape.setPosition(pos);
        shape.setOrigin(0, 0);
        shape.setFillColor(Color::Transparent);
        shape.setOutlineThickness(1.5f);

        baseColor = color;
        shape.setOutlineColor(color);

        // Random velocity and rotation
        float angle = static_cast<float>(rand()) / RAND_MAX * 2 * 3.14159f;
        float speed = 15.0f + static_cast<float>(rand()) / RAND_MAX * 30.0f;
        velocity = Vector2f(cos(angle) * speed, sin(angle) * speed);

        rotationSpeed = 30.0f + static_cast<float>(rand()) / RAND_MAX * 60.0f;
        if (rand() % 2) rotationSpeed *= -1;

        maxLifetime = 4.0f + static_cast<float>(rand()) / RAND_MAX * 3.0f;
        lifetime = maxLifetime;
        isActive = true;
    }

    void update(float dt, Vector2u windowSize) {
        if (!isActive) return;

        lifetime -= dt;
        if (lifetime <= 0) {
            isActive = false;
            return;
        }

        // Move and rotate
        Vector2f pos = shape.getPosition();
        pos += velocity * dt;
        shape.setPosition(pos);
        shape.rotate(rotationSpeed * dt);

        // Wrap around edges
        if (pos.x < -50) pos.x = windowSize.x + 50;
        if (pos.x > windowSize.x + 50) pos.x = -50;
        if (pos.y < -50) pos.y = windowSize.y + 50;
        if (pos.y > windowSize.y + 50) pos.y = -50;
        shape.setPosition(pos);

        // Fade effect
        float fadeAlpha = (lifetime / maxLifetime) * 150;
        Color color = baseColor;
        color.a = static_cast<Uint8>(fadeAlpha);
        shape.setOutlineColor(color);
    }
};


class GameManager
{
private:
    GameBoard board;
    Player** players;
    int currentPlayer;

    RenderWindow& window;
    Font& font;

    enum class GameState {
        AwaitingFirst, AwaitingSecond, AnimatingFlip, ShowingMismatch,
        AI_ChooseFirstCard, AI_ChooseSecondCard, GameOver, TimeUp
    };
    GameState currentState;

    vector<vector<RectangleShape>> visualCards;
    vector<vector<Text>> cardTexts;
    vector<vector<bool>> cardMatched;
    Texture cardBackTexture;
    bool textureLoaded;

    Text hudText;
    Text timeText;
    Text gameOverText;
    Text pausedText; 

    static constexpr float FLIP_DURATION = 0.4f;
    Card* animatingCard;
    Card* hoveredCard;
    Clock animationClock;
    Clock delayTimer;
    Clock gameTimer;
    Clock frameClock; 
    GameState stateAfterAnimation;
    Card* firstSelection, * secondSelection;


    enum class GameMode { Classic, AI, TimeChallenge };
    GameMode currentMode;
    float timeLimit;
    bool isPaused; 


    Music backgroundMusic;
    SoundBuffer flipBuffer, matchBuffer, failBuffer, winBuffer, loseBuffer;
    Sound flipSound, matchSound, failSound, winSound, loseSound;
    bool gameOverSoundPlayed;
    bool wasForfeited;


    vector<NeonCircle> neonCircles;
    vector<NeonTriangle> neonTriangles;
    Clock animationSpawnTimer;
    mt19937 rng;

public:
    GameManager(RenderWindow& win, Font& f, int diff, bool is2P, bool isTimeChallenge = false, float timeLimitSeconds = 120.0f) :
        board(diff),
        currentPlayer(0),
        window(win),
        font(f),
        animatingCard(nullptr),
        hoveredCard(nullptr),
        firstSelection(nullptr),
        secondSelection(nullptr),
        timeLimit(timeLimitSeconds),
        isPaused(false),
        gameOverSoundPlayed(false),
        wasForfeited(false),
        rng(static_cast<unsigned int>(time(nullptr)))
    {
        players = new Player * [2];

        if (isTimeChallenge) {
            currentMode = GameMode::TimeChallenge;
            players[0] = new Player("Player 1");
            players[1] = nullptr;
            gameTimer.restart();
        }
        else if (is2P) {
            currentMode = GameMode::Classic;
            players[0] = new Player("Player 1");
            players[1] = new Player("Player 2");
        }
        else {
            currentMode = GameMode::AI;
            players[0] = new Player("Player 1");
            players[1] = new AI(board, diff);
        }

        if (currentMode != GameMode::TimeChallenge && dynamic_cast<AI*>(players[currentPlayer])) {
            currentState = GameState::AI_ChooseFirstCard;
            delayTimer.restart();
        }
        else {
            currentState = GameState::AwaitingFirst;
        }

        if (!cardBackTexture.loadFromFile("card_back.png")) {
            textureLoaded = false;
        }
        else {
            textureLoaded = true;
        }

        cardMatched.resize(board.getRows(), vector<bool>(board.getCols(), false));

        setupVisuals();
        setupAudio();
        animationSpawnTimer.restart();
    }

    ~GameManager() {
        for (int i = 0; i < 2; i++) {
            if (players[i]) delete players[i];
        }
        delete[] players;
    }



    void run() {
        backgroundMusic.play();
        frameClock.restart(); 
        while (window.isOpen()) {
            handleEvents();
            update();
            render();
            if ((currentState == GameState::GameOver || currentState == GameState::TimeUp) &&
                Keyboard::isKeyPressed(Keyboard::Enter)) {
                backgroundMusic.stop();
                return;
            }
        }
    }

private:
    void setupAudio() {
        // Load background music
        if (!backgroundMusic.openFromFile("background_music.mp3")) {
            cout << "Error: Could not load background.mp3" << endl;
        }
        backgroundMusic.setLoop(true);
        backgroundMusic.setVolume(40);

        // Load sound effect buffers
        if (!flipBuffer.loadFromFile("card_flip.wav")) cout << "Error loading card_flip.wav" << endl;
        if (!matchBuffer.loadFromFile("card_match.wav")) cout << "Error loading card_match.wav" << endl;
        if (!failBuffer.loadFromFile("card_fail.wav")) cout << "Error loading card_fail.wav" << endl;
        if (!winBuffer.loadFromFile("player_wins.wav")) cout << "Error loading player_wins.wav" << endl;
        if (!loseBuffer.loadFromFile("player_loses.wav")) cout << "Error loading player_loses.wav" << endl;

        // Assign buffers to sounds
        flipSound.setBuffer(flipBuffer);
        matchSound.setBuffer(matchBuffer);
        failSound.setBuffer(failBuffer);
        winSound.setBuffer(winBuffer);
        loseSound.setBuffer(loseBuffer);
    }
    void setupVisuals() {
        // ... (existing setup code is unchanged)
        visualCards.resize(board.getRows(), vector<RectangleShape>(board.getCols()));
        cardTexts.resize(board.getRows(), vector<Text>(board.getCols()));

        float topMargin = 100.0f;
        float bottomMargin = 40.0f;
        float sideMargins = 0.0f;

        float boardAreaWidth = window.getSize().x - (sideMargins * 2);
        float boardAreaHeight = window.getSize().y - (topMargin + bottomMargin);

        float cardAspectRatio = 1.2f;
        float spacingRatio = 0.15f;

        float calculatedCardW = boardAreaWidth / (board.getCols() + (board.getCols() - 1) * spacingRatio);
        float calculatedCardH = boardAreaHeight / (board.getRows() + (board.getRows() - 1) * spacingRatio);

        float finalCardW;
        if (calculatedCardW * cardAspectRatio > calculatedCardH) {
            finalCardW = calculatedCardH / cardAspectRatio;
        }
        else {
            finalCardW = calculatedCardW;
        }

        float cardW = finalCardW;
        float cardH = cardW * cardAspectRatio;
        float spacing = cardW * spacingRatio;

        float totalGridW = board.getCols() * cardW + (board.getCols() - 1) * spacing;
        float totalGridH = board.getRows() * cardH + (board.getRows() - 1) * spacing;

        float startX = (window.getSize().x - totalGridW) / 2.0f;
        float startY = topMargin + (boardAreaHeight - totalGridH) / 2.0f;

        for (int r = 0; r < board.getRows(); ++r) {
            for (int c = 0; c < board.getCols(); ++c) {
                float posX = startX + c * (cardW + spacing) + cardW / 2.0f;
                float posY = startY + r * (cardH + spacing) + cardH / 2.0f;

                visualCards[r][c].setOrigin(cardW / 2.0f, cardH / 2.0f);
                visualCards[r][c].setSize(Vector2f(cardW, cardH));
                visualCards[r][c].setPosition(posX, posY);
                visualCards[r][c].setOutlineThickness(4);

                cardTexts[r][c].setFont(font);
                cardTexts[r][c].setString(string(1, Card::getCardFace(board.getCard(r, c).getValue())));
                cardTexts[r][c].setCharacterSize(cardH * 0.6f);
                cardTexts[r][c].setFillColor(Color::Black);
                FloatRect tb = cardTexts[r][c].getLocalBounds();
                cardTexts[r][c].setOrigin(tb.left + tb.width / 2.0f, tb.top + tb.height / 2.0f);
                cardTexts[r][c].setPosition(posX, posY);
            }
        }

        hudText.setFont(font);
        hudText.setCharacterSize(32);
        hudText.setStyle(Text::Bold);

        if (currentMode == GameMode::TimeChallenge) {
            timeText.setFont(font);
            timeText.setCharacterSize(28);
            timeText.setStyle(Text::Bold);
            timeText.setFillColor(Color::Red);
        }

        gameOverText.setFont(font);
        gameOverText.setCharacterSize(60);
        gameOverText.setStyle(Text::Bold);

        // for the paused text
        pausedText.setFont(font);
        pausedText.setString("PAUSED");
        pausedText.setCharacterSize(80);
        pausedText.setStyle(Text::Bold);
        pausedText.setFillColor(Color::Yellow);
        FloatRect tb = pausedText.getLocalBounds();
        pausedText.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
        pausedText.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);
    }


    void spawnBackgroundAnimations() {
        if (animationSpawnTimer.getElapsedTime().asSeconds() > 0.1f) {
            animationSpawnTimer.restart();

            // Spawn circles
            if (neonCircles.size() < 15 && rand() % 3 == 0) {
                Vector2f pos(
                    static_cast<float>(rand() % window.getSize().x),
                    static_cast<float>(rand() % window.getSize().y)
                );
                Color colors[] = {
                    Color(0, 255, 255, 150),    // Cyan
                    Color(255, 0, 255, 150),    // Magenta  
                    Color(0, 255, 0, 150),      // Green
                    Color(255, 255, 0, 150)     // Yellow
                };
                Color color = colors[rand() % 4];
                float radius = 10.0f + static_cast<float>(rand()) / RAND_MAX * 30.0f;
                neonCircles.emplace_back(pos, radius, color);
            }

            // Spawn triangles
            if (neonTriangles.size() < 10 && rand() % 4 == 0) {
                Vector2f pos(
                    static_cast<float>(rand() % window.getSize().x),
                    static_cast<float>(rand() % window.getSize().y)
                );
                Color colors[] = {
                    Color(255, 100, 0, 120),
                    Color(100, 100, 255, 120),
                    Color(255, 100, 255, 120),
                    Color(100, 255, 100, 120)
                };
                Color color = colors[rand() % 4];
                float size = 20.0f + static_cast<float>(rand()) / RAND_MAX * 15.0f;
                neonTriangles.emplace_back(pos, size, color);
            }
        }
    }

    void updateBackgroundAnimations(float dt) {
        // Update circles
        for (auto it = neonCircles.begin(); it != neonCircles.end();) {
            it->update(dt, window.getSize());
            if (!it->isActive) {
                it = neonCircles.erase(it);
            }
            else {
                ++it;
            }
        }

        // Update triangles
        for (auto it = neonTriangles.begin(); it != neonTriangles.end();) {
            it->update(dt, window.getSize());
            if (!it->isActive) {
                it = neonTriangles.erase(it);
            }
            else {
                ++it;
            }
        }
    }

    void handleEvents() {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) window.close();

          
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Escape) {
                    wasForfeited = true;
                    currentState = GameState::GameOver;
                    return;
                }
                if (event.key.code == Keyboard::P) {
                    isPaused = !isPaused;
                }
            }

          
            if (isPaused) continue;

            Vector2f mousePos = Vector2f(Mouse::getPosition(window));
            updateHover(mousePos);
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                if (hoveredCard != nullptr && (currentState == GameState::AwaitingFirst || currentState == GameState::AwaitingSecond)) {
                    handleCardClick();
                }
            }
        }
    }


    void updateHover(const Vector2f& mousePos) {
        if (currentState != GameState::AwaitingFirst && currentState != GameState::AwaitingSecond) {
            hoveredCard = nullptr; return;
        }
        hoveredCard = nullptr;
        for (int r = 0; r < board.getRows(); ++r) {
            for (int c = 0; c < board.getCols(); ++c) {
                if (visualCards[r][c].getGlobalBounds().contains(mousePos)) {
                    Card& card = board.getCard(r, c);
                    if (!card.getMatched() && !card.getFlipped()) {
                        hoveredCard = &card;
                    }
                }
            }
        }
    }

    void handleCardClick() {
        if (!hoveredCard) return;
        if (currentState == GameState::AwaitingFirst) {
            firstSelection = hoveredCard;
            startFlipAnimation(firstSelection, GameState::AwaitingSecond);
        }
        else if (currentState == GameState::AwaitingSecond) {
            if (hoveredCard == firstSelection) return;
            secondSelection = hoveredCard;
            startFlipAnimation(secondSelection, GameState::AwaitingSecond);
        }
    }

    void startFlipAnimation(Card* card, GameState nextState) {
        flipSound.play(); 
        animatingCard = card;
        stateAfterAnimation = nextState;
        currentState = GameState::AnimatingFlip;
        animationClock.restart();
        hoveredCard = nullptr;
    }


    void update() {
      
        if (isPaused) {
            frameClock.restart();
           
            if (currentMode == GameMode::TimeChallenge) {
                
            }
            return;
        }

        
        float dt = frameClock.restart().asSeconds();

        
        if (currentMode == GameMode::TimeChallenge && currentState != GameState::GameOver && currentState != GameState::TimeUp) {
            if (gameTimer.getElapsedTime().asSeconds() >= timeLimit) {
                currentState = GameState::TimeUp;
            }
        }

        
        spawnBackgroundAnimations();
        updateBackgroundAnimations(dt);

        
        if (currentState == GameState::AnimatingFlip) {
            if (animationClock.getElapsedTime().asSeconds() >= FLIP_DURATION) {
                animatingCard->flip();
                animatingCard = nullptr;
                if (secondSelection && secondSelection->getFlipped()) {
                    processGUIMove(firstSelection, secondSelection);
                }
                else {
                    currentState = stateAfterAnimation;
                }
            }
            return;
        }

        if (currentState == GameState::ShowingMismatch && delayTimer.getElapsedTime().asSeconds() > 1.2f) {
            firstSelection->flip();
            secondSelection->flip();

            firstSelection = nullptr;
            secondSelection = nullptr;

            if (currentMode != GameMode::TimeChallenge) {
                switchPlayer();
                if (players[currentPlayer] && dynamic_cast<AI*>(players[currentPlayer])) {
                    currentState = GameState::AI_ChooseFirstCard;
                    delayTimer.restart();
                }
                else {
                    currentState = GameState::AwaitingFirst;
                }
            }
            else {
                currentState = GameState::AwaitingFirst;
            }
        }

        // AI logic 
        if (currentMode != GameMode::TimeChallenge) {
            if (currentState == GameState::AI_ChooseFirstCard && delayTimer.getElapsedTime().asSeconds() > 0.5f) {
                AI* ai = static_cast<AI*>(players[currentPlayer]);
                int f = 0;
                Pair p1 = ai->chooseCard(f);
                firstSelection = &board.getCard(p1.x, p1.y);
                startFlipAnimation(firstSelection, GameState::AI_ChooseSecondCard);
            }

            if (currentState == GameState::AI_ChooseSecondCard) {
                AI* ai = static_cast<AI*>(players[currentPlayer]);
                int f = 0;
                Pair p2;
                do {
                    p2 = ai->chooseCard(f);
                } while (p2.x == firstSelection->getPos().x && p2.y == firstSelection->getPos().y);
                secondSelection = &board.getCard(p2.x, p2.y);
                startFlipAnimation(secondSelection, GameState::AwaitingSecond);
            }
        }

        // Update HUD
        string status_string;
        if (currentMode == GameMode::TimeChallenge) {
            status_string = "Time Challenge";
            hudText.setFillColor(Color::Yellow);

            // Update timer display
            float remainingTime = timeLimit - gameTimer.getElapsedTime().asSeconds();
            if (remainingTime < 0) remainingTime = 0;
            int minutes = static_cast<int>(remainingTime) / 60;
            int seconds = static_cast<int>(remainingTime) % 60;
            timeText.setString("Time: " + to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + to_string(seconds));

            if (remainingTime < 30) {
                timeText.setFillColor(Color::Red);
            }
            else if (remainingTime < 60) {
                timeText.setFillColor(Color::Yellow);
            }
            else {
                timeText.setFillColor(Color::Green);
            }

            FloatRect timeBounds = timeText.getLocalBounds();
            timeText.setOrigin(timeBounds.left + timeBounds.width / 2.0f, timeBounds.top + timeBounds.height / 2.0f);
            timeText.setPosition(window.getSize().x / 2.0f, 70.0f);
        }
        else if (currentMode != GameMode::TimeChallenge && players[currentPlayer] && dynamic_cast<AI*>(players[currentPlayer]) &&
            (currentState == GameState::AI_ChooseFirstCard || currentState == GameState::AI_ChooseSecondCard)) {
            status_string = "AI's Turn";
            hudText.setFillColor(Color::Cyan);
        }
        else if (players[currentPlayer]) {
            status_string = players[currentPlayer]->getName() + "'s Turn";
            hudText.setFillColor(Color::Yellow);
        }

        if (currentMode == GameMode::TimeChallenge) {
            hudText.setString(status_string + "   |   Score: " + to_string(players[0]->getScore()));
        }
        else {
            hudText.setString(status_string + "   |   P1 Score: " + to_string(players[0]->getScore()) +
                "   |   P2 Score: " + (players[1] ? to_string(players[1]->getScore()) : "0"));
        }

        FloatRect tb = hudText.getLocalBounds();
        hudText.setOrigin(tb.left + tb.width / 2.0f, tb.top + tb.height / 2.0f);
        hudText.setPosition(window.getSize().x / 2.0f, 40.0f);
    }

    
    void processGUIMove(Card* c1, Card* c2) {
        if (c1->checkMatch(*c2)) {
            matchSound.play();
            board.draw(c1, c2);
            players[currentPlayer]->updateScore(1);

            // Mark cards as matched for shadow effect
            for (int r = 0; r < board.getRows(); ++r) {
                for (int c = 0; c < board.getCols(); ++c) {
                    if (&board.getCard(r, c) == c1 || &board.getCard(r, c) == c2) {
                        cardMatched[r][c] = true;
                    }
                }
            }

            firstSelection = nullptr;
            secondSelection = nullptr;

            if (isGameOver()) {
                currentState = GameState::GameOver;
            }
            else {
                // Player gets another turn
                if (currentMode != GameMode::TimeChallenge && players[currentPlayer] && dynamic_cast<AI*>(players[currentPlayer])) {
                    delayTimer.restart();
                    currentState = GameState::AI_ChooseFirstCard;
                }
                else {
                    currentState = GameState::AwaitingFirst;
                }
            }
        }
        else {
            failSound.play();
            currentState = GameState::ShowingMismatch;
            delayTimer.restart();
        }
    }


    void render() {
        window.clear(Color(10, 15, 30));

        
        for (const auto& circle : neonCircles) {
            window.draw(circle.shape);
        }
        for (const auto& triangle : neonTriangles) {
            window.draw(triangle.shape);
        }

        // Draw cards
        for (int r = 0; r < board.getRows(); ++r) {
            for (int c = 0; c < board.getCols(); ++c) {
                Card& card = board.getCard(r, c);
                RectangleShape& vCard = visualCards[r][c];
                Text& vText = cardTexts[r][c];
                vCard.setScale(1, 1);
                vText.setScale(1, 1);
                vCard.setOutlineColor(Color(200, 200, 200, 150));

                if (&card == hoveredCard) {
                    vCard.setScale(1.05f, 1.05f);
                    vCard.setOutlineColor(Color::White);
                }

                if (animatingCard == &card) {
                    float progress = animationClock.getElapsedTime().asSeconds() / FLIP_DURATION;
                    float scaleX = cos(progress * 3.14159f);
                    vCard.setScale(scaleX, 1.0f);
                    if (scaleX < 0) {
                        vCard.setTexture(nullptr);
                        vCard.setFillColor(Color::White);
                        window.draw(vCard);
                        vText.setScale(abs(scaleX), 1.0f);
                        window.draw(vText);
                    }
                    else {
                        if (textureLoaded) vCard.setTexture(&cardBackTexture);
                        else vCard.setFillColor(Color(30, 80, 200));
                        window.draw(vCard);
                    }
                }
                else {
                    if (cardMatched[r][c]) {
                        // Draw shadow/empty card for matched cards
                        vCard.setTexture(nullptr);
                        vCard.setFillColor(Color(20, 20, 20, 100)); 
                        vCard.setOutlineColor(Color(50, 50, 50, 150)); 
                        RectangleShape shadowCard = vCard;
                        shadowCard.setFillColor(Color::Transparent);
                        shadowCard.setOutlineColor(Color(0, 255, 0, 100)); 
                        shadowCard.setOutlineThickness(2);
                        window.draw(vCard);
                        window.draw(shadowCard);
                    }
                    else if (card.getFlipped()) {
                        vCard.setTexture(nullptr);
                        vCard.setFillColor(Color::White);
                        vCard.setOutlineColor(Color::Yellow);
                        window.draw(vCard);
                        window.draw(vText);
                    }
                    else {
                        if (textureLoaded) vCard.setTexture(&cardBackTexture);
                        else vCard.setFillColor(Color(30, 80, 200));
                        window.draw(vCard);
                    }
                }
            }
        }

        window.draw(hudText);
        if (currentMode == GameMode::TimeChallenge) {
            window.draw(timeText);
        }


        if (currentState == GameState::GameOver || currentState == GameState::TimeUp) {
            string winnerStr;

            
            if (!gameOverSoundPlayed) {
                backgroundMusic.stop();

                bool playerHasWon = false;
                
                if (currentState == GameState::GameOver && !wasForfeited) {
                    if (currentMode == GameMode::TimeChallenge) {
                        playerHasWon = true; 
                    }
                    else {
                        int s1 = players[0]->getScore();
                        int s2 = players[1] ? players[1]->getScore() : 0;
                        if (s1 > s2) {
                            playerHasWon = true; // Win in Classic or AI mode
                        }
                    }
                }

                // Any other game-over scenario (Time Up, Forfeit, Score Loss) is a loss.
                if (playerHasWon) {
                    winSound.play();
                }
                else {
                    loseSound.play();
                }

                gameOverSoundPlayed = true;
            }

            
            int s1 = players[0]->getScore();
            int s2 = players[1] ? players[1]->getScore() : 0;

            if (wasForfeited) {
                winnerStr = "Game Forfeited";
            }
            else if (currentState == GameState::TimeUp) {
                winnerStr = "Time's Up!\nFinal Score: " + to_string(s1);
            }
            else if (currentMode == GameMode::TimeChallenge) {
                winnerStr = "Congratulations!\nFinal Score: " + to_string(s1);
            }
            else {
                if (s1 > s2) winnerStr = players[0]->getName() + " Wins!";
                else if (s2 > s1) winnerStr = (players[1] ? players[1]->getName() : "Player 2") + " Wins!";
                else winnerStr = "It's a Draw!";
            }

            gameOverText.setString(winnerStr + "\nPress ENTER to return to menu");
            FloatRect tb = gameOverText.getLocalBounds();
            gameOverText.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
            gameOverText.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);
            RectangleShape overlay(Vector2f(window.getSize()));
            overlay.setFillColor(Color(0, 0, 0, 180));
            window.draw(overlay);
            window.draw(gameOverText);
        }

        window.display();
    }


    void switchPlayer() {
        if (currentMode != GameMode::TimeChallenge && players[1]) {
            currentPlayer = (currentPlayer + 1) % 2;
        }
    }

    bool isGameOver() {
        for (int r = 0; r < board.getRows(); ++r) {
            for (int c = 0; c < board.getCols(); ++c) {
                if (!board.getCard(r, c).getMatched()) {
                    return false;
                }
            }
        }
        return true;
    }
};