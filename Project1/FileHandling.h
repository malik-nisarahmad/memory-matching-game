#pragma once

#include <fstream>
#include "Player.h"
#include <string>
#include <iostream>
using namespace std;

int strToNum(string str) { //function to do strToNum
    int scope = 1, numtemp, num = 0;
    for (int i = 1;i < str.length();i++)
        scope *= 10; //finds the maximum scope of the number, (if it is in hundreds, ones or thousansands
    for (int i = 0;i < str.length();i++) { //traversing through the length of the str
        for (int j = 48;j <= 57;j++) { //to check ASCII of the digits
            if ((str[i]) == j) { //if the ASCII equals the char which is extracted from the string
                numtemp = (j - 48) * scope; //store the digit equalent to the char after multiplying with it's scope
                scope /= 10; //decrease the scope
                break; //break in case the ASCII matches
            }
        }
        num += numtemp; //add the numbers into one
    }
    return num;
}

string numToStr(int num) {
    int exit = 1, digit;
    string str = "";
    while (1) {
        digit = num % 10; //string the last digit in the number
        switch (digit) { //apending the string with appropriate str based on the digit
        case 0: str = "0" + str;	break;
        case 1: str = "1" + str;	break;
        case 2: str = "2" + str;	break;
        case 3: str = "3" + str;	break;
        case 4: str = "4" + str;	break;
        case 5: str = "5" + str;	break;
        case 6: str = "6" + str;	break;
        case 7: str = "7" + str;	break;
        case 8: str = "8" + str;	break;
        case 9: str = "9" + str;
        }
        num /= 10; //decreasing the number 
        if (exit == 2) //breaking the loop
            break;
        if (num / 10 == 0) //if this is true, the loop should work only one more time
            exit++;
    }
    return str;
}


class FileHandler {
private:
    int playerCount;
    Player* playerArr = new Player[playerCount];
    string mode, grid;
    int leaderBoard[10] = { 0,0,0,0,0,0,0,0,0,0 };
public:
    void readPlayerData() {
        string line;
        fstream PlayerData("players.txt", ios::in);
        if (PlayerData.is_open()) {
            getline(PlayerData, line);
            playerCount = strToNum(line);
            for (int i = 0;i < playerCount;i++) {
                getline(PlayerData, line);
                playerArr[i].setName(line);
                getline(PlayerData, line);
                playerArr[i].setGamesPlayed(strToNum(line));
                getline(PlayerData, line);
                playerArr[i].setGamesWon(strToNum(line));
            }
            PlayerData.close();
        }
        else {
            cout << "Couldn't open the file; it doesn't exist yet\n";
        }
    }

    void writePlayerData() {
        fstream PlayerData("players.txt", ios::out);
        if (PlayerData.is_open()) {
            PlayerData << numToStr(playerCount) << endl;
            for (int i = 0;i < playerCount;i++) {
                PlayerData << playerArr[i].getName() << endl;
                PlayerData << numToStr(playerArr[i].getGamesPlayed()) << endl;
                PlayerData << numToStr(playerArr[i].getGamesWon()) << endl;
            }
            PlayerData.close();
        }
        else {
            cout << "Error while opeining the file; new file created\n";
        }
    }

    void appendPlayer(string userName) {
        Player* newArr = new Player[++playerCount];
        for (int i = 0;i < playerCount - 1;i++) {
            newArr[i] = playerArr[i];
        }
        newArr[playerCount - 1].setName(userName);
        delete[] playerArr;
        playerArr = newArr;
    }

    void updatePlayer(string userName, bool isWinner) {
        for (int i = 0;i < playerCount;i++) {
            if (userName == playerArr[i].getName()) {
                if (isWinner)
                    playerArr[i].setGamesWon(playerArr[i].getGamesWon() + 1);
                playerArr[i].setGamesPlayed(playerArr[i].getGamesPlayed() + 1);
            }
        }
    }

    void appendScores(string time, string highScore) {
        fstream ScoreData("scores.txt", ios::out | ios::app);
        if (ScoreData.is_open()) {
            ScoreData << time << endl;
            ScoreData << highScore << endl;
            ScoreData.close();
        }
        else {
            cout << "Error while opeining the file; new file created\n";
        }
    }

    void writeSetting(string mode, string grid) {
        fstream SettingData("settings.txt", ios::out);
        if (SettingData.is_open()) {
            SettingData << mode << endl;
            SettingData << grid << endl;
            SettingData.close();
        }
        else {
            cout << "Error while opeining the file; new file created\n";
        }
    }

    void readSetting() {
        string line;
        fstream SettingData("settings.txt", ios::in);
        if (SettingData.is_open()) {
            getline(SettingData, mode);
            getline(SettingData, grid);
            SettingData.close();
        }
        else {
            cout << "Couldn't open the file; it doesn't exist yet\n";
        }
    }

    void getTopScores() {
        string line;
        int total;
        fstream ScoresData("scores.txt", ios::in);
        if (ScoresData.is_open()) {
            getline(ScoresData, line);
            total = strToNum(line);
            for (int i = 0; i < total; i++) {
                getline(ScoresData, line);
                getline(ScoresData, line);
                int score = strToNum(line);
                if (score > 0) {
                    for (int j = 0; j < 10; j++) {
                        if (score >= leaderBoard[j]) {
                            for (int k = 9; k > j; k--)
                                leaderBoard[k] = leaderBoard[k - 1];
                            leaderBoard[j] = score;
                            break;
                        }
                    }
                }
            }
            ScoresData.close();
        }
        else {
            cout << "Couldn't open the file; it doesn't exist yet\n";
        }
    }

    ~FileHandler() {
        delete[] playerArr;
    }
};