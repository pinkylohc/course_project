#ifndef __CORE_H__
#define __CORE_H__

#include <iostream>

class Player {
    private:
        char* name;
        int elo;
        int score;

    public:
        Player(const char* const name, const int elo);
        ~Player();

        // The following copy constructor and assignment operator are deleted
        Player(const Player& player) = delete;
        Player& operator=(const Player&) = delete;

        // The accessor, mutator and print functions are given
        int getELO() const { return elo; }
        int getScore() const { return score; }

        void addScore(const int points) { score += points; }

        void print() const {
            std::cout << name << " (" << elo << ")";
        }
};

class PlayerList {
    private:  
        int numPlayers;
        Player** players;

    public:
        PlayerList();
        PlayerList(const PlayerList& list);
        ~PlayerList();

        int getNumPlayers() const { return numPlayers; }
        Player* getPlayer(const int index) const { return players[index]; }

        void addPlayer(Player* const player);
        void sort();
        PlayerList* splice(const int startIndex, const int endIndex) const;

};

#endif // __CORE_H__
