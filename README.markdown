# 🌌 **Memory Matching Championship** 🧠

*Flip, match, and conquer in this electrifying memory showdown!*

**Memory Matching Championship** is a dazzling, modern memory card game built with **SFML** and **C++** for the **Spring 2025 OOP Hackathon**. Packed with neon visuals, pulsating particle effects, and thrilling modes like AI battles and time challenges, it’s a test of wit and reflexes. Ready to claim the crown? Let’s match some cards! 💥

---

## 🎨 **Why It’s a Masterpiece**

- **Neon-Charged UI** ✨: Glowing cards, animated flips, and drifting neon circles/triangles for a futuristic vibe.
- **Three Epic Modes** 🎮: Face off in **Classic (2P)**, outsmart the **AI**, or race the clock in **Time Challenge**.
- **Smart AI Opponent** 🤖: Remembers cards and strategizes to keep you on your toes.
- **Dynamic Visuals** 🌃: Scalable, responsive card grid with smooth animations and hover effects.
- **Audio Immersion** 🎵: Card flips, matches, and epic win/lose sounds synced with background music.
- **Persistent Stats** 📊: Tracks player scores, wins, and leaderboard via file handling.
- **Pause & Forfeit** ⏸️: Flexible gameplay with pause (`P`) and forfeit (`Esc`) options.

---

## 🎲 **Game Modes**

| Mode | Challenge |
| --- | --- |
| **Classic** | 2-player showdown to match all pairs. |
| **AI** | Battle a cunning AI with three difficulty levels. |
| **Time Challenge** | Race against time (30s/60s/120s) to max your score. |

### AI Mode Difficulty Levels 🤖

Challenge the AI at your skill level:

| Difficulty | Strategy |
| --- | --- |
| **Easiest** | Randomly guesses card positions. No memory. 🎲 |
| **Beginner** | Remembers its own flipped cards for smarter picks. 🧩 |
| **Advanced** | Tracks *both* its own and player’s moves for ruthless strategy. 😈 |

- **Easiest**: Perfect for casual play, the AI flips cards blindly, relying on luck.
- **Beginner**: The AI stores its own card reveals, choosing known matches when possible but ignoring your moves.
- **Advanced**: A memory master, the AI recalls every card you flip, prioritizing matches and blocking your strategy.

---

## 🕹️ **Controls**

| Action | Keys/Mouse |
| --- | --- |
| Flip Card | Left Mouse Click |
| Pause Game | `P` |
| Forfeit Game | `Esc` |
| Return to Menu | `Enter` (on game over) |

---

## 🚀 **Get Matching!**

### Requirements

- **SFML 2.6.1+**
- **C++17** compatible compiler
- **CMake** (optional for builds)
- Assets: `arial.ttf`, `card_back.png`, audio files (`*.wav`, `background_music.mp3`)

### Installation

1. **Clone the Repo**:

   ```bash
   git clone https://github.com/Team404/MemoryMatchingChampionship.git
   cd MemoryMatchingChampionship
   ```
2. **Build the Game**:

   ```bash
   mkdir build && cd build
   cmake .. && make
   ```
3. **Launch the Fun**:

   ```bash
   ./MemoryMatchingChampionship
   ```

---

## 👨‍💻 **The Code Crafters**

- **Nisar Ahmad**
- **Rana Hanan Shafique** 
- **Mohib Ahmad** 
- 

---

## 🧩 **Game Architecture**

Here’s the modular brilliance behind the game, visualized with Mermaid:

```mermaid
classDiagram
    class GameManager {
        -board: GameBoard
        -players: Player**
        -currentPlayer: int
        -window: RenderWindow
        -font: Font
        -currentState: GameState
        -neonCircles: vector<NeonCircle>
        -neonTriangles: vector<NeonTriangle>
        +run()
        +processGUIMove()
        +switchPlayer()
        +isGameOver()
    }
    class GameBoard {
        -cards: Card[MAX_SIZE][MAX_SIZE]
        -rows: int
        -cols: int
        -pairCount: int
        +initializeBoard()
        +shuffleCards()
        +getCard()
        +draw()
        +checkWin()
    }
    class Card {
        -value: int
        -isFlipped: bool
        -isMatched: bool
        -pos: Pair
        +flip()
        +checkMatch()
        +draw()
        +getCardFace()
    }
    class Player {
        -name: string
        -score: int
        -board: GameBoard*
        +updateScore()
        +chooseCard()
        +setName()
    }
    class AI {
        -memory: vector<Card*>
        -difficulty: int
        +chooseCard()
        +pushToMemory()
    }
    class FileHandler {
        -playerCount: int
        -playerArr: Player*
        +readPlayerData()
        +writePlayerData()
        +appendScores()
        +getTopScores()
    }
    class GameMenu {
        -window: RenderWindow
        -font: Font
        -currentScreen: ScreenState
        -mainButtons: vector<MenuButton>
        +run()
        +handleMainMenuClick()
        +setupBackgroundGrid()
    }
    GameManager o--> GameBoard
    GameManager o--> "2" Player
    GameBoard o--> "many" Card
    Player <|-- AI
    GameManager o--> FileHandler
    GameMenu o--> GameManager
    style GameManager fill:#ff4d4d,stroke:#fff,stroke-width:2px
    style GameBoard fill:#4dff4d,stroke:#fff,stroke-width:2px
    style Card fill:#4d4dff,stroke:#fff,stroke-width:2px
    style Player fill:#ffaa4d,stroke:#fff,stroke-width:2px
    style AI fill:#ff4dff,stroke:#fff,stroke-width:2px
    style FileHandler fill:#4dffff,stroke:#fff,stroke-width:2px
    style GameMenu fill:#ffff4d,stroke:#fff,stroke-width:2px
```

- **GameManager**: Orchestrates gameplay, SFML rendering, and state transitions.
- **GameBoard**: Manages card grid, shuffling, and match logic.
- **Card**: Handles individual card state and flip animations.
- **Player/AI**: Encapsulates human and AI move logic with memory for AI.
- **FileHandler**: Persists player stats and leaderboard data.
- **GameMenu**: Drives the neon-themed menu and mode selection.

---

## ⚙️ **Tech Highlights**

### Performance

- **Frame-Rate Independence** ⏱️: Smooth animations with delta-time updates.
- **Efficient Rendering** 🖼️: Optimized SFML draw calls for cards and particles.
- **Memory Management** ♻️: Dynamic allocation for players and AI memory.

### Design

- **OOP Excellence**: Clean, modular classes with inheritance for Player/AI.
- **Scalable UI**: Adapts to any screen resolution via SFML.
- **Robust AI**: Memorizes cards for strategic gameplay.

---

## 💖 **Why You’ll Be Addicted**

**Memory Matching Championship** fuses retro memory fun with a futuristic neon glow. Whether you’re outwitting the AI, racing the clock, or battling a friend, the vibrant visuals and crisp mechanics keep you hooked. The pulsating particles and satisfying card flips make every match a thrill.

**Ready to test your memory?** Clone the repo and start flipping! 🚀

---