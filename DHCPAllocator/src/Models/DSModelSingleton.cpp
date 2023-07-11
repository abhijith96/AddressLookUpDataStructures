//
// Created by Abhijith  K A on 11/07/23.
//

#include "DSModelSingleton.h"
#include "DHCPAllocator/src/Models/DSModelTreeImpl/DSModelTreeImpl.h"


DSModelSingleton* DSModelSingleton::dsModelSingleton_ = nullptr;

DSModelSingleton & DSModelSingleton::GetSingletonInstance(std::string modelType) {
    if(!dsModelSingleton_){
        dsModelSingleton_ = new DSModelSingleton(modelType);
    }
    return *dsModelSingleton_;
}

DSModel &DSModelSingleton::GetDSModel() {
    return *dsModel_;
}

DSModelSingleton::DSModelSingleton(std::string dsModelType) {

    if(dsModelType == "tree")
        dsModel_ = std::make_unique<DSModel>(DSModelTreeImpl());
    else
        throw std::invalid_argument("Not implemented");
}
