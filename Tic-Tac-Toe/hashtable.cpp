#include "hashtable.h"

BoardOptimalMove BoardHashTable::getHashedMove(const unsigned long long id, const int depth){
    int hash_index = id % TABLE_SIZE;
    BoardHashNode* list = table[hash_index];

    for(; list != nullptr; list = list->next){
       if(list->id == id){
            if(list->depth >= depth){
                return list->optimalMove;
            }else break;
        } 
    }
    
    return BoardOptimalMove(); // the table point to end / reach the end of list / not enough depth
}

void BoardHashTable::updateTable(const unsigned long long id, const int depth, const BoardOptimalMove &optimalMove){
    int hash_index = id % TABLE_SIZE;
    BoardHashNode* list = table[hash_index];

    if(list == nullptr){ //empty linked list
        table[hash_index] = new BoardHashNode(id, depth, optimalMove); 
        return;
    }

    //search if the id exist
    for(; list != nullptr; list = list->next){
       if(list->id == id){ 
            if(list->depth == depth){
                return;
            }else if(list->depth < depth){
                list->depth = depth;
                list->optimalMove = optimalMove;
                return;
            }else return;
        } 
    }

    //insert at the beginning
    BoardHashNode* insert = new BoardHashNode(id, depth, optimalMove); 
    insert->next = table[hash_index];
    table[hash_index] = insert;
    insert = nullptr; //need??
}

void BoardHashTable::clearTable(){
    for(int i = 0; i<TABLE_SIZE; i++){
        BoardHashNode* temp = table[i];
        while (temp != nullptr) {
            BoardHashNode* nextnode = temp->next;
            delete temp;
            temp = nextnode;
        }
        table[i] = nullptr;      
    }
}
