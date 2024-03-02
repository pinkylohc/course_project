#include <cstring> 
#include "player.h"

 
Player::Player(const char* const name, const int elo): elo(elo), score(0){
    int namelen = strlen(name);
    this->name = new char[30];
    strcpy(this->name, name);

}

Player::~Player(){

    //std::cout<<"deleting: "<<name;

    delete [] name;

}

PlayerList::PlayerList():numPlayers(0), players(nullptr){}; //default constructor

 

PlayerList::PlayerList(const PlayerList& list){ //copy constructor

    this->numPlayers = list.numPlayers;

    if(players!= nullptr){

        this->players = new Player*[numPlayers];

        for (int i=0; i<numPlayers; i++)

            this->players[i] = list.players[i];  
    }
    else this->players = nullptr;

 

};

 

PlayerList::~PlayerList(){ //do the pointer array list need to point to nullptr before delete????

    if(players != nullptr){
        
        for (int i=0; i<numPlayers; i++)

        players[i] = nullptr;   //avoid deleting the player object!!!!!

         delete [] players;
         players = nullptr;

    } else delete players; players = nullptr;

   
};

 

void PlayerList::addPlayer(Player* const player){ //try to adding many player!!!

    numPlayers += 1;

    Player** temp = players;  // for storing the original array of pointer

    players = new Player*[numPlayers]; //assign a new array, with addional player

    for (int i=0; i<numPlayers-1; i++)

        players[i] = temp[i];  

    players[numPlayers-1] = player;

     if(temp != nullptr){
        for (int i=0; i<numPlayers-1; i++) temp[i] = nullptr;
        delete [] temp;

    } else delete temp;
    
    

 

}

 

void PlayerList::sort(){ //bubble sort??

    for(int i=0; i<numPlayers-1; i++){

 

        for(int j=0; j< numPlayers-1-i; j++){

            if (players[j]->getScore() < players[j+1]->getScore()){ //smaller score, swap to the back

                Player* temp = players[j];

                players[j] = players[j+1];

                players[j+1] = temp;

                temp = nullptr;

            } else if(players[j]->getScore() == players[j+1]->getScore()){ //same scores, compare ELO

                if(players[j]->getELO() < players[j+1]->getELO()){ //Smaller elo, to the back

                    Player* temp = players[j];

                    players[j] = players[j+1];

                    players[j+1] = temp;

                    temp = nullptr;

                }

            }

        }

    }   

}

 

PlayerList* PlayerList::splice(const int startIndex, const int endIndex) const{ //delete handle??

    PlayerList* spliced_list = new PlayerList();

    if((startIndex < endIndex) && (startIndex>=0 && startIndex<=numPlayers-1) && (endIndex<=numPlayers-1 && endIndex>=0)){

        for (int i=startIndex; i<=endIndex; i++)

            spliced_list->addPlayer(players[i]); 

        spliced_list->numPlayers = endIndex - startIndex; //problem!!!!!!    

    }

    return spliced_list;

}

 
