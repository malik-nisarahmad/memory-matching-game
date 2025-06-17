#pragma once

#include "Card.h"
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <iostream>

using namespace std;

const int MAX_SIZE = 8;


class GameBoard
{
private:
	Card cards[MAX_SIZE][MAX_SIZE];
	int rows, cols;
	int pairCount;
public:

	void initializeBoard()
	{


		int k = 0;
		for (int i = 0; i < MAX_SIZE; i++)
		{
			for (int j = 0; j < MAX_SIZE; j++)
			{
				cards[i][j] = k;
				if (j % 2 == 1)
					k++;
			}
		}
	}

	void shuffleCards()
	{
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				int randomRow = rand() % rows;
				int randomColumn = rand() % cols;
				std::swap(cards[i][j], cards[randomRow][randomColumn]);
			}
		}

		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				cards[i][j].setPos(i, j);
			}
		}
	}

	GameBoard(int difficulty) : rows(difficulty), cols(difficulty), pairCount(difficulty* (difficulty / 2))
	{
		initializeBoard();
		shuffleCards();
	}

	Card& getCard(int i, int j)
	{
		return cards[i][j];
	}

	void display()
	{
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				cout << cards[i][j].getValue() << "  ";
			}
			cout << endl;
		}
	}


	bool checkWin()
	{
		if (pairCount <= 0)
			return true;
		return false;
	}
	int getRows() const {
		return rows;
	}

	int getCols() const {
		return cols;
	}


	void draw(Card* c1, Card* c2)
	{
		c1->draw();
		c2->draw();
		pairCount--;
	}
};