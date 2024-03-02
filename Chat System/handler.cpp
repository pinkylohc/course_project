#include "handler.h"

HandlerModule::~HandlerModule(){ 
    languageFilter = nullptr;
    toneFilter = nullptr;
    data.clear();

}

void HandlerModule::print() const{
    std::cout << "Module name = " << m_name << ", this is a Handler, can handle topic = " << topic << std::endl;
}

void HandlerModule::trainData(const string &question, const string &answer){ //input the qs and ans to map??
    data.insert(question, answer);
}

string HandlerModule::answer(const string &question) const{
    string qs = question;
    if(languageFilter != nullptr) qs = languageFilter->translatePreData(qs);
    if(toneFilter != nullptr) qs = toneFilter->translatePreData(qs); 
    //std::cout<<"question: "<<qs<<std::endl;
    string ans = inneranswer(qs);
    if(ans == "") return "Sorry I don't know";
    if(toneFilter != nullptr) ans = toneFilter->translatePostData(ans);
    if(languageFilter != nullptr) ans = languageFilter->translatePostData(ans);
    return ans;
    
}

string HandlerModule::inneranswer(const string &question) const{
    return data.find(question);
}

//-------------------------------------------HadlerMap function---------------------------------------------------//

HandlerMap::HandlerMap(){
    arraySize = 0;
    names = nullptr;
    handlerModules = nullptr;
}

HandlerMap::~HandlerMap(){clear();}

void HandlerMap::insert(string key, HandlerModule *value){
    if (arraySize == 0)
    {
        arraySize = 1;
        names = new string[1]; //for keys
        names[0] = key;
        handlerModules = new HandlerModule*[1]; //for values
        handlerModules[0] = value;
        return;
    }    
    arraySize++;
    string *tempk = new string[arraySize];
    HandlerModule** tempv = new HandlerModule*[arraySize];
    for (int i = 0; i < arraySize - 1; i++)
    {
        tempk[i] = names[i];
        tempv[i] = handlerModules[i];
    }
    tempk[arraySize - 1] = key;
    tempv[arraySize - 1] = value;

    for(int i=0; i<arraySize-1; i++){

        //delete handlerModules[i];
        //handlerModules[i] = nullptr;
    }
    delete[] names;
    delete[] handlerModules;
    names = tempk;
    handlerModules = tempv;
    return;
}

HandlerModule* HandlerMap::find(const string &key) const{
    if (arraySize == 0) return nullptr;
    bool flag = false;
    int i = 0;
    for (; i<arraySize; i++){
        if(names[i] == key){
            flag = true;
            break;
        }
    }
    if (flag) 
        return handlerModules[i];
    else
        return nullptr;
}

string HandlerMap::getNames(int index) const{
    return names[index];
}

void HandlerMap::clear(){
    arraySize = 0;
    for(int i=0; i<arraySize-1; i++){
        delete handlerModules[i];
        handlerModules[i] = nullptr;
    }
    delete[] handlerModules;
    delete[] names;
    names = nullptr;
    handlerModules = nullptr;
    //std::cout<<"handlermap clear done"<<std::endl;
}

