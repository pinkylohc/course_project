#include "filter.h"

FilterModule::~FilterModule(){
    preData.clear();
    postData.clear();
}
    
void FilterModule::print() const{
    std::cout << "Module name = " << m_name << ", this is a filter, filter type = " << getFilterType(type) << std::endl;
}

void FilterModule::trainPreData(const string &source, const string &target){
    preData.insert(source, target);
}

void FilterModule::trainPostData(const string &source, const string &target){
    postData.insert(source, target);
}

string FilterModule::translatePreData(string question) const{
    if(preData.find(question) == ""){
        return question;
    } else return preData.find(question);

}

string FilterModule::translatePostData(string question) const{
    if(postData.find(question) == ""){
        return question;
    } else return postData.find(question);
}

//---------------------------------------Filterarray func----------------------------------------//
void FilterArray::insert(FilterModule *input){
    if (0 == arraySize)
    {
        arraySize = 1;
        array = new FilterModule*[1];
        array[0] = input;
        return;
    }
    arraySize++;
    FilterModule** temp = new FilterModule*[arraySize];
    for (int i = 0; i < arraySize - 1; i++)
    {
        temp[i] = array[i];
    }
    temp[arraySize - 1] = input;
    for(int i=0; i<arraySize-1; i++){
        //delete array[i];
        //array[i] = nullptr;
    }
    delete[] array;
    array = temp;
    return;
}

FilterModule* FilterArray::get(int index) const{
    if (index < 0){return nullptr;}

    if (index < arraySize){
        return array[index];
    }else{
        return nullptr;
    }
}