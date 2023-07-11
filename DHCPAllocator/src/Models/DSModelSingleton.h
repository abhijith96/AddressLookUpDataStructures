//
// Created by Abhijith  K A on 11/07/23.
//

#ifndef DHCPALLOCATOR_DSMODELSINGLETON_H
#define DHCPALLOCATOR_DSMODELSINGLETON_H

#include <DHCPAllocator/src/Models/DSModel.h>
#include <string>
#include <memory>
#include <utility>
#include "DSModel.h"

class DSModelSingleton {

private:
   static DSModelSingleton* dsModelSingleton_;
   std::unique_ptr<DSModel> dsModel_;

   DSModelSingleton(std::string dsModelType);



public:

   static DSModelSingleton& GetSingletonInstance(std::string modelType);

  DSModel& GetDSModel();

};


#endif //DHCPALLOCATOR_DSMODELSINGLETON_H
