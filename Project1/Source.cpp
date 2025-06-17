#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include "GameManager.h"
using namespace std;
using namespace sf;

struct GridCell {
    RectangleShape border;
    float lifetime;
    float maxLifetime;
    bool isActive;

    GridCell(Vector2f pos, float cellSize, float scale) {
        maxLifetime = 1.2f;
        lifetime = 0.0f;
        isActive = false;

        float thickness = 1.0f * scale;
        border.setSize(Vector2f(cellSize * scale, cellSize * scale));
        border.setPosition(pos);
        border.setFillColor(Color::Transparent);
        border.setOutlineThickness(thickness);
        border.setOutlineColor(Color(0, 255, 255, 0));
    }

    void activate() {
        isActive = true;
        lifetime = maxLifetime;
        border.setOutlineColor(Color(0, 255, 255, 255));
    }

    void update(float dt) {
        if (isActive) {
            lifetime -= dt;
            float t = lifetime / maxLifetime;
            float alpha = 255 * t * (0.8f + 0.2f * sin(t * 8.0f));
            Color color = border.getOutlineColor();
            color.a = static_cast<Uint8>(alpha);
            border.setOutlineColor(color);

            if (lifetime <= 0) {
                isActive = false;
                border.setOutlineColor(Color(0, 255, 255, 0));
            }
        }
    }

    Vector2f getPosition() const {
        return border.getPosition();
    }
};

class MenuButton {
private:
    RectangleShape buttonShape;
    RectangleShape glowShape;
    Text buttonText;
    Vector2f position;
    Vector2f size;
    bool isHovered;
    float hoverAnimation;
    Color baseColor;
    Color hoverColor;

public:
    MenuButton(Vector2f pos, Vector2f sz, const string& text, Font& font, float scalex, float scaley)
        : position(pos), size(sz), isHovered(false), hoverAnimation(0.0f) {
        baseColor = Color(20, 30, 60, 180);
        hoverColor = Color(0, 100, 150, 220);

        buttonShape.setSize(Vector2f(size.x * scalex, size.y * scaley));
        buttonShape.setPosition(Vector2f(position.x * scalex, position.y * scaley));
        buttonShape.setFillColor(baseColor);

        glowShape.setSize(Vector2f(size.x * scalex + 8 * scalex, size.y * scaley + 8 * scaley));
        glowShape.setPosition(Vector2f((position.x - 4) * scalex, (position.y - 4) * scaley));
        glowShape.setFillColor(Color::Transparent);
        glowShape.setOutlineThickness(2 * min(scalex, scaley));
        glowShape.setOutlineColor(Color::Transparent);

        buttonText.setFont(font);
        buttonText.setString(text);
        buttonText.setCharacterSize(static_cast<unsigned int>(20 * min(scalex, scaley)));
        buttonText.setFillColor(sf::Color::White);
        updateTextPosition(scalex, scaley, 1.0f);
    }

    void updateTextPosition(float scalex, float scaley, float buttonScale) {
        sf::FloatRect textBounds = buttonText.getLocalBounds();
        float scaledButtonWidth = size.x * scalex * buttonScale;
        float scaledButtonHeight = size.y * scaley * buttonScale;
        float buttonPosX = position.x * scalex - (scaledButtonWidth - size.x * scalex) / 2;
        float buttonPosY = position.y * scaley - (scaledButtonHeight - size.y * scaley) / 2;
        buttonText.setPosition(
            buttonPosX + (scaledButtonWidth - textBounds.width) / 2,
            buttonPosY + (scaledButtonHeight - textBounds.height) / 2 - 3 * scaley
        );
    }

    bool contains(Vector2f point) const {
        return buttonShape.getGlobalBounds().contains(point);
    }

    void update(Vector2f mousePos, float dt, float scalex, float scaley) {
        isHovered = buttonShape.getGlobalBounds().contains(mousePos);

        if (isHovered) {
            hoverAnimation = min(1.0f, hoverAnimation + dt * 5.0f);
        }
        else {
            hoverAnimation = max(0.0f, hoverAnimation - dt * 5.0f);
        }

        Uint8 r = static_cast<Uint8>(baseColor.r + (hoverColor.r - baseColor.r) * hoverAnimation);
        Uint8 g = static_cast<Uint8>(baseColor.g + (hoverColor.g - baseColor.g) * hoverAnimation);
        Uint8 b = static_cast<Uint8>(baseColor.b + (hoverColor.b - baseColor.b) * hoverAnimation);
        Uint8 a = static_cast<Uint8>(baseColor.a + (hoverColor.a - baseColor.a) * hoverAnimation);
        buttonShape.setFillColor(Color(r, g, b, a));

        Uint8 glowAlpha = static_cast<Uint8>(100 * hoverAnimation);
        glowShape.setOutlineColor(Color(0, 255, 255, glowAlpha));

        float scale = 1.0f + hoverAnimation * 0.05f;
        buttonShape.setScale(scale, scale);
        glowShape.setScale(scale, scale);

        Vector2f offset = Vector2f(buttonShape.getSize().x * (scale - 1.0f) / 2,
            buttonShape.getSize().y * (scale - 1.0f) / 2);
        buttonShape.setPosition(position.x * scalex - offset.x, position.y * scaley - offset.y);
        glowShape.setPosition(Vector2f((position.x - 4) * scalex - offset.x, (position.y - 4) * scaley - offset.y));

        updateTextPosition(scalex, scaley, scale);
    }

    bool isClicked(Vector2f mousePos) const {
        return isHovered && Mouse::isButtonPressed(Mouse::Left);
    }

    void draw(RenderWindow& window) const {
        window.draw(glowShape);
        window.draw(buttonShape);
        window.draw(buttonText);
    }
};

class ModeSelectionScreen {
private:
    vector<MenuButton> buttons;
    Text title;
    Font& font;
    float scalex, scaley;

public:
    ModeSelectionScreen(Font& f, float sx, float sy, const string& modeName)
        : font(f), scalex(sx), scaley(sy) {
        title.setFont(font);
        title.setString(modeName + " - Select Grid Size");
        title.setCharacterSize(static_cast<unsigned int>(36 * min(scalex, scaley)));
        title.setFillColor(sf::Color::White);
        title.setStyle(sf::Text::Bold);
        FloatRect titleBounds = title.getLocalBounds();
        title.setPosition((sx * 800.0f - titleBounds.width) / 2.0f, 80 * sy);

        vector<string> gridSizes = { "Beginner (4x4)", "Intermediate (6x6)", "Advanced (8x8)", "Back" };
        for (size_t i = 0; i < gridSizes.size(); ++i) {
            buttons.emplace_back(Vector2f(300, 220 + i * 70), Vector2f(200, 50),
                gridSizes[i], font, scalex, scaley);
        }
    }

    void update(Vector2f mousePos, float dt) {
        for (auto& button : buttons) {
            button.update(mousePos, dt, scalex, scaley);
        }
    }

    void draw(RenderWindow& window) const {
        window.draw(title);
        for (const auto& button : buttons) {
            button.draw(window);
        }
    }

    int handleClick(Vector2f mousePos) {
        for (size_t i = 0; i < buttons.size(); ++i) {
            if (buttons[i].isClicked(mousePos)) {
                return i; // returns index of clicked button (0: 4x4, 1: 6x6, 2: 8x8, 3: back)
            }
        }
        return -1;
    }
};

class GameMenu {
private:
    enum class ScreenState { MainMenu, ModeSelection };
    ScreenState currentScreen;
    string selectedMode;
    RenderWindow window;
    Font font;
    Text title;
    Text subtitle;
    Text pausedText; 
    vector<MenuButton> mainButtons;
    ModeSelectionScreen* modeSelection;
    vector<GridCell> gridCells;
    Clock clock;
    Vector2f lastMousePos;
    float gridTimer;
    float scalex, scaley;
    float baseWidth = 800.0f;
    float baseHeight = 600.0f;
    static constexpr float cellSize = 30.0f;
    bool isPaused = false;
    

public:
    GameMenu()
        : window(VideoMode::getDesktopMode(), "Memory Game Menu", Style::Fullscreen),
        currentScreen(ScreenState::MainMenu), modeSelection(nullptr) {
        window.setFramerateLimit(60);

        scalex = static_cast<float>(window.getSize().x) / baseWidth;
        scaley = static_cast<float>(window.getSize().y) / baseHeight;

        if (!font.loadFromFile("arial.ttf")) {
            cout << "Warning: Could not load arial.ttf, using default font\n";
        }

        setupMainMenu();
        setupBackgroundGrid();
        gridTimer = 0;
        lastMousePos = Vector2f(window.getSize().x / 2, window.getSize().y / 2);
    }

    ~GameMenu() {
        delete modeSelection;
    }

    void setupMainMenu() {
        title.setFont(font);
        title.setString("Memory Game");
        title.setCharacterSize(static_cast<unsigned int>(48 * min(scalex, scaley)));
        title.setFillColor(sf::Color::White);
        title.setStyle(sf::Text::Bold);
        FloatRect titleBounds = title.getLocalBounds();
        title.setPosition((window.getSize().x - titleBounds.width) / 2, 80 * scaley);

        subtitle.setFont(font);
        subtitle.setString("Neon Challenge");
        subtitle.setCharacterSize(static_cast<unsigned int>(24 * min(scalex, scaley)));
        subtitle.setFillColor(Color(0, 255, 255));
        FloatRect subtitleBounds = subtitle.getLocalBounds();
        subtitle.setPosition((window.getSize().x - subtitleBounds.width) / 2, 140 * scaley);

        mainButtons.clear();
        vector<string> buttonLabels = { "Classic Mode", "AI Mode", "Time Challenge", "Leaderboard", "Exit" };
        for (size_t i = 0; i < buttonLabels.size(); ++i) {
            mainButtons.emplace_back(Vector2f(300, 220 + i * 70), Vector2f(200, 50),
                buttonLabels[i], font, scalex, scaley);
        }

        // Setup the paused text
        pausedText.setFont(font);
        pausedText.setString("PAUSED");
        pausedText.setCharacterSize(static_cast<unsigned int>(60 * min(scalex, scaley)));
        pausedText.setFillColor(Color::Yellow);
        pausedText.setStyle(Text::Bold);
        FloatRect bounds = pausedText.getLocalBounds();
        pausedText.setOrigin(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2);
        pausedText.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
    }

    void setupBackgroundGrid() {
        gridCells.clear();
        float scaledCellSize = cellSize * min(scalex, scaley);
        int numCols = static_cast<int>(ceil(window.getSize().x / scaledCellSize));
        int numRows = static_cast<int>(ceil(window.getSize().y / scaledCellSize));

        for (int i = 0; i < numCols; ++i) {
            for (int j = 0; j < numRows; ++j) {
                gridCells.emplace_back(Vector2f(i * scaledCellSize, j * scaledCellSize), cellSize, min(scalex, scaley));
            }
        }
    }

    void handleEvents() {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed ||
                (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)) {
                window.close();
            }

            if (event.type == Event::KeyPressed && event.key.code == Keyboard::P) {
                isPaused = !isPaused;
            }

            // If paused, don't process other events
            if (isPaused) continue;

            if (event.type == Event::MouseButtonPressed) {
                Vector2f mousePos = Vector2f(Mouse::getPosition(window));
                if (currentScreen == ScreenState::MainMenu) {
                    for (size_t i = 0; i < mainButtons.size(); ++i) {
                        if (mainButtons[i].isClicked(mousePos)) {
                            handleMainMenuClick(i);
                        }
                    }
                }
                else if (currentScreen == ScreenState::ModeSelection) {
                    int index = modeSelection->handleClick(mousePos);
                    handleModeSelectionClick(index);
                }
            }

            if (event.type == Event::Resized) {
                FloatRect viewport(0, 0, event.size.width, event.size.height);
                window.setView(View(viewport));
                scalex = event.size.width / baseWidth;
                scaley = event.size.height / baseHeight;
                setupMainMenu();
                if (modeSelection) {
                    delete modeSelection;
                    modeSelection = new ModeSelectionScreen(font, scalex, scaley, selectedMode);
                }
                setupBackgroundGrid();
            }
        }
    }

    void handleMainMenuClick(int index) {
        switch (index) {
        case 0:
            selectedMode = "Classic Mode";
            currentScreen = ScreenState::ModeSelection;
            delete modeSelection;
            modeSelection = new ModeSelectionScreen(font, scalex, scaley, selectedMode);
            break;
        case 1:
            selectedMode = "AI Mode";
            currentScreen = ScreenState::ModeSelection;
            delete modeSelection;
            modeSelection = new ModeSelectionScreen(font, scalex, scaley, selectedMode);
            break;
        case 2:
            selectedMode = "Time Challenge";
            currentScreen = ScreenState::ModeSelection;
            delete modeSelection;
            modeSelection = new ModeSelectionScreen(font, scalex, scaley, selectedMode);
            break;
        case 3:
            cout << "Leaderboard!\n";

            break;
        case 4:
            window.close();
            break;
        }
    }

    void handleModeSelectionClick(int index) {
        if (index == -1) return;
        if (index == 3) {
            currentScreen = ScreenState::MainMenu;
            delete modeSelection;
            modeSelection = nullptr;
            return;
        }


        int gridSize = (index == 0) ? 4 : (index == 1) ? 6 : 8;
        bool isTwoPlayer = (selectedMode == "Classic Mode");
        bool isTimeChallenge = (selectedMode == "Time Challenge");


        float timeLimit = 120.0f; // default for 8x8
        if (isTimeChallenge) {
            if (gridSize == 4) timeLimit = 30.0f;
            else if (gridSize == 6) timeLimit = 60.0f;
            else if (gridSize == 8) timeLimit = 120.0f;
        }
        GameManager game(window, font, gridSize, isTwoPlayer, isTimeChallenge, timeLimit);


        game.run();
        window.setVisible(true);

        currentScreen = ScreenState::MainMenu;
        delete modeSelection;
        modeSelection = nullptr;
    }

    void update() {
        //Paused
        if (isPaused) {
            clock.restart();
            return;
        }

        float dt = clock.restart().asSeconds();
        Vector2f mousePos = Vector2f(Mouse::getPosition(window));

        if (currentScreen == ScreenState::MainMenu) {
            for (auto& button : mainButtons) {
                button.update(mousePos, dt, scalex, scaley);
            }
        }
        else if (currentScreen == ScreenState::ModeSelection) {
            modeSelection->update(mousePos, dt);
        }

        float mouseDistance = sqrt(pow(mousePos.x - lastMousePos.x, 2) + pow(mousePos.y - lastMousePos.y, 2));
        if (mouseDistance > 0) {
            gridTimer += dt;
            if (gridTimer >= 0.02f) {
                float activationRadius = 50 * min(scalex, scaley);
                float scaledCellSize = cellSize * min(scalex, scaley);
                for (auto& cell : gridCells) {
                    Vector2f cellCenter = cell.getPosition() + Vector2f(scaledCellSize / 2, scaledCellSize / 2);
                    float distance = sqrt(pow(mousePos.x - cellCenter.x, 2) + pow(mousePos.y - cellCenter.y, 2));
                    bool underButton = false;

                    if (currentScreen == ScreenState::MainMenu) {
                        for (size_t i = 0; i < mainButtons.size(); ++i) {
                            if (mainButtons[i].contains(cellCenter)) {
                                underButton = true;
                                break;
                            }
                        }
                    }
                    else if (modeSelection) {  
                    }

                    if (!underButton && distance < activationRadius && rand() % 2 == 0) {
                        cell.activate();
                    }
                }
                gridTimer = 0;
            }
            lastMousePos = mousePos;
        }

        for (auto& cell : gridCells) {
            cell.update(dt);
        }
    }

    void render() {
        window.clear(Color(10, 15, 30));

        // Draw background grid
        for (const auto& cell : gridCells) {
            window.draw(cell.border);
        }

        // Draw the current screen
        if (currentScreen == ScreenState::MainMenu) {
            window.draw(title);
            window.draw(subtitle);
            for (const auto& button : mainButtons) {
                button.draw(window);
            }
        }
        else if (currentScreen == ScreenState::ModeSelection) {
            modeSelection->draw(window);
        }

        // If paused, draw the overlay on top of everything
        if (isPaused) {
            RectangleShape overlay(Vector2f(window.getSize()));
            overlay.setFillColor(Color(0, 0, 0, 170)); // Semi-transparent black
            window.draw(overlay);
            window.draw(pausedText);
        }

        window.display();
    }

    void run() {
        while (window.isOpen()) {
            handleEvents();
            update();
            render();
        }
    }
};

int main() {

	Music backgroundMusic;
	if (!backgroundMusic.openFromFile("background_music.mp3")) {
		cerr << "Error loading background music\n";
		return 1;
	}
	backgroundMusic.setLoop(true);
	backgroundMusic.setVolume(40);
	backgroundMusic.play();

    try {
        GameMenu menu;
        menu.run();
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << '\n';
        return 1;
    }
    return 0;
}