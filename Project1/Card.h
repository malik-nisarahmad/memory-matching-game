#pragma once


#include "Pair.h"

class Card
{
private:
	int value;
	bool isFlipped;
	bool isMatched;
	static char cardFace[];
	Pair pos;
public:
	Card() :isFlipped(0), isMatched(0), value(-1) {}
	Card(int& val) :value(val), isFlipped(0), isMatched(0) {}

	void setValue(int value) { this->value = value; }
	int getValue() const { return value; }
	bool getMatched() const { return isMatched; }
	void setPos(int x, int y) { pos.x = x; pos.y = y; }
	Pair getPos() { return pos; }
	bool getFlipped() const { return isFlipped; }
	void flip()
	{
		isFlipped = !isFlipped;
	}
	bool checkMatch(const Card& other)
	{
		if (value == other.value)
			return true;
		return false;
	}
	void draw()
	{
		value = -1;
		isMatched = true;
	}
	static char getCardFace(int index) {
		return cardFace[index];
	}
};

char Card::cardFace[] = {
		'1','2','3','4','5','6',
		'A','B','C','D','E','F','G','H','I','J','K','L','M',
		'N','O','P','Q','R','S','T','U','V','W','X','Y','Z'
};


