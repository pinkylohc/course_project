#include "swiss.h" 
#include "match.h"
#include <iostream>

Swiss::Swiss(const int numRounds, const PlayerList& list)
: numRounds(numRounds), curRound(0), list(list){ //deep copy on playerlist???

    this->list.sort();

};


Swiss::~Swiss(){ //all static, need delete??? //empty???


};

void Swiss::play(){

    while(curRound < numRounds){

        curRound += 1;

        int poss_scores = 2*curRound - 1; // length of the playlist array in each round

        PlayerList* arrlist = new PlayerList[poss_scores]; //creating the array of PlayerList
        //std::cout<<"done new";
        //sorting the list base on the score first, then use splice to sepearte each one

        list.sort();
        int j = 0; int a = 0;
        arrlist[j].addPlayer(list.getPlayer(a));
        int curr_score = list.getPlayer(a)->getScore();
        a+=1;

        while (j<poss_scores && a <list.getNumPlayers()){
            if(list.getPlayer(a)->getScore() == curr_score){
                arrlist[j].addPlayer(list.getPlayer(a));
                a+=1;
            }
            else{
                j+=1;
                arrlist[j].addPlayer(list.getPlayer(a));
                a+=1;
                
            }                           

        }
 

        //start the match

        for (int i=0; i< poss_scores; i++){
            //deal with odd no. player
            arrlist[i].sort();
            float isodd = (arrlist[i].getNumPlayers())%2;
            if (isodd != 0){ //odd player enter
                if(i == (poss_scores - 1)){ //reach the last Playerlist of the array
                  arrlist[i].getPlayer(arrlist[i].getNumPlayers()-1)->addScore(2);
                    //cout<<"add two points to "; arrlist[i].getPlayer(arrlist[i].getNumPlayers()-1)->print();
                }
                else{
                    arrlist[i+1].addPlayer(arrlist[i].getPlayer(arrlist[i].getNumPlayers()-1)); //adding the player to the next list
                    arrlist[i+1].sort();

                    //arrlist[i] = *(arrlist[i].splice(0,arrlist[i].getNumPlayers()-2)); //remove from the old list

                }

            }

            int midindex = (arrlist[i].getNumPlayers())/2; //trunication

            for(int k=0; k<midindex; k++){

                Match pairmatch(arrlist[i].getPlayer(k), arrlist[i].getPlayer(midindex+k));

                pairmatch.play();

            }

        }

 

        list.sort();

        printLeaderboard();

        // deleting the arrlist
        for (int i=0; i< poss_scores; i++) {

            arrlist[i].~PlayerList();
        
        }        
        delete [] arrlist;
    }
    

};
