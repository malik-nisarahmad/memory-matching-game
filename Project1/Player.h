#pragma once
#include "Pair.h"
#include "Card.h"
#include <vector>
using namespace std;


class Player
{
private:

    string name;
    int score;
    int gamesWon;
    int gamesPlayed;
protected:
    GameBoard* board;
public:
    Player(string name = "", int score = 0, int gamesWon = 0, int gamesPlayed = 0) :name(name), score(score), gamesWon(gamesWon), gamesPlayed(gamesPlayed) {}

    Player(GameBoard& b, string name = "", int score = 0, int gamesWon = 0) :name(name), score(score), gamesWon(gamesWon), board(&b) {}
    void updateScore(int points)
    {
        score += points;
    };
    void incrementWins() {
        gamesWon++;
    };
    void displayStats() {
        cout << "\nName: " << name << ", Score: " << score << ", Games Won: " << gamesWon << endl;
    };

    virtual Pair chooseCard()
    {
        Pair temp;
        cout << "Enter row: ";
        cin >> temp.x;
        cout << "Enter column: ";
        cin >> temp.y;
        return temp;
    }
    void setName(string name) { this->name = name; }
    void setGamesWon(int gamesWon) { this->gamesWon = gamesWon; }
    void setGamesPlayed(int gamesPlayed) { this->gamesPlayed = gamesPlayed; }
    string getName() { return name; }
    int getGamesWon() { return gamesWon; }
    int getGamesPlayed() { return gamesPlayed; }

    virtual void pushToMemory(Card* card)
    {

    }
    int getScore() const {
        return score;
    }



};

class easyAI : public Player
{
    int difficulty;

public:
    easyAI(GameBoard& g, int d) :Player(g, "Easy AI"), difficulty(d) {}
    Pair chooseCard() override
    {
        Pair temp;
        do
        {
            temp.x = rand() % difficulty;
            temp.y = rand() % difficulty;
        } while (board->getCard(temp.x, temp.y).getMatched() || board->getCard(temp.x, temp.y).getFlipped());

        temp.display();
        return temp;
    }
};

class AI : public Player
{
    vector<Card*> memory;
    int size;
    Pair nextPair;
    int difficulty;
    int turn;
public:

    AI(GameBoard& g, int diff) :Player(g, "AI"), nextPair(-1, -1), difficulty(diff), turn(0)
    {
        size = diff * 2;
    }

    bool inMemory(const Pair& p)
    {
        for (int i = 0; i < memory.size(); i++)
        {
            if (p.x == memory[i]->getPos().x && p.y == memory[i]->getPos().y)
                return true;
        }
        return false;
    }

    void pushToMemory(Card* card) override
    {
        if (!inMemory(card->getPos()))
        {
            if (memory.size() < size)
            {
                memory.push_back(card);
            }
        }
    }


    virtual Pair chooseCard(int& f) 
    {
        turn = !turn;
        Pair temp;
        for (int i = 0; i < memory.size(); i++)
        {
            if (memory[i]->getMatched())
            {
                memory.erase(memory.begin() + i);
                --i;
            }
        }
        if (nextPair.x != -1 && nextPair.y != -1)
        {
            temp = nextPair;
            nextPair.x = -1;
            nextPair.y = -1;
            return temp;
        }

        if (memory.size() >= 2 && turn)
        {
            for (int i = 0; i < memory.size() - 1; i++)
            {
                for (int j = i + 1; j < memory.size(); j++)
                {
                    if (memory[i]->getValue() == memory[j]->getValue())
                    {
                        temp = memory[i]->getPos();
                        nextPair = memory[j]->getPos();
                        memory.erase(memory.begin() + j);
                        memory.erase(memory.begin() + i);
                        return temp;
                    }
                }
            }
        }
        do
        {
            temp.x = rand() % difficulty;
            temp.y = rand() % difficulty;
        } while (board->getCard(temp.x, temp.y).getMatched() || board->getCard(temp.x, temp.y).getFlipped() || inMemory(temp));
        if (turn)
        {
            for (int i = 0; i < memory.size(); i++)
            {
                if (board->getCard(temp.x, temp.y).getValue() == memory[i]->getValue())
                {
                    nextPair = memory[i]->getPos();
                    memory.erase(memory.begin() + i);
                    return temp;
                }
            }
        }
        if (memory.size() < size)
            memory.push_back(&board->getCard(temp.x, temp.y));
        return temp;
    }

};