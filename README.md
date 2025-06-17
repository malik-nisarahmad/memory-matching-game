# Memory Match Pro - SFML Card Game

![GitHub release](https://img.shields.io/github/v/release/malik-nisarahmad/memory-matching-game)
![SFML](https://img.shields.io/badge/SFML-2.5.1+-brightgreen)
![License](https://img.shields.io/badge/License-MIT-blue)

## Table of Contents
- [Project Overview](#-project-overview)
- [Key Features](#-key-features)
- [Installation](#-installation)
- [Gameplay Guide](#-gameplay-guide)
- [Technical Specifications](#-technical-specifications)
- [Contributors](#-contributors)
- [License](#-license)

---

## 🏆 Project Overview
A high-performance memory card matching game featuring:
- **Cross-platform** compatibility (Windows/Linux/macOS)
- **Three distinct game modes** with progressive difficulty
- **Neon-themed visual effects** with smooth animations
- **Persistent score tracking** via file serialization

---

## ✨ Key Features

### Game Modes
| Mode | Players | Description |
|------|---------|-------------|
| Classic | 2 | Turn-based competitive play |
| AI Challenge | 1 | Adaptive AI with memory system |
| Time Attack | 1 | Race against the clock |

### Technical Highlights
- **60 FPS rendering** with SFML hardware acceleration
- **Custom memory allocator** for card objects
- **State machine architecture** for game flow control
- **Modular design** for easy feature expansion

---

## 📥 Installation

### Prerequisites
```bash
# Ubuntu/Debian
sudo apt install libsfml-dev cmake g++

# macOS (Homebrew)
brew install sfml cmake