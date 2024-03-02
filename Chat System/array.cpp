#include "array.h"
//#include <iostream> //delete later
//using namespace std; //delete later

DataMap::DataMap(){
    arraySize = 0;
    keys = nullptr;
    values = nullptr;
}

DataMap::~DataMap(){
    clear();
}

void DataMap::insert(const string &key, const string &value){
    if (0 == arraySize)
    {
        arraySize = 1;
        this->keys = new string[1]; //for keys
        this->keys[0] = key;
        this->values = new string[1]; //for values
        this->values[0] = value;
        return;
    }
    arraySize++;
    string *tempk = new string[arraySize];
    string *tempv = new string[arraySize];
    for (int i = 0; i < arraySize - 1; i++)
    {
        tempk[i] = this->keys[i];
        tempv[i] = this->values[i];
    }
    tempk[arraySize - 1] = key;
    tempv[arraySize - 1] = value;
    delete[] keys;
    delete[] values;
    this->keys = tempk;
    this->values = tempv;
    return;
}

string DataMap::find(const string &key)const{
    if (arraySize == 0) return "";

    bool flag = false;
    int i = 0;
    for (; i<arraySize; i++){
        if(this->keys[i] == key){
            flag = true;
            break;
        }
    }
    if (flag) 
        return this->values[i];
    else
        return "";
}

void DataMap::clear(){
    if (arraySize != 0)
        {
            arraySize = 0;
            delete[] keys;
            delete[] values;
            keys = nullptr;
            values = nullptr;
        }
}

/*
int main(){
    DataMap trial;
    trial.insert("1", "trial");
    trial.insert("abc", "trial2");
    trial.insert("ac", "trial3");
    trial.insert("ww", "trial4");
    cout<<trial.find("w");
    trial.clear();
    
}*/

