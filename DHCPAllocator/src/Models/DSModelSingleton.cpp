//
// Created by Abhijith  K A on 11/07/23.
//

#include "DSModelSingleton.h"
#include <DHCPAllocator/src/Models/DSModelTreeImpl.h>


DSModelSingleton & DSModelSingleton::GetSingletonInstance(std::string modelType) {
    if(!dsModelSingleton_){
        dsModelSingleton_ = new DSModelSingleton();
    }
    return *dsModelSingleton_;
}

DSModel &DSModelSingleton::GetDSModel() {
    return *dsModel_;
}

DSModelSingleton::DSModelSingleton() {
    dsModel_ = std::make_unique<DSModel>(DSModelTreeImpl());
}
