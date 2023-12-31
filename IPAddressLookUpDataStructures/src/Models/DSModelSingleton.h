//
// Created by Abhijith  K A on 11/07/23.
//

#ifndef DHCPALLOCATOR_DSMODELSINGLETON_H
#define DHCPALLOCATOR_DSMODELSINGLETON_H

#include <IPAddressLookUpDataStructures/src/Models/DSModelmpl.h>
#include <string>
#include <memory>
#include <utility>
#include <IPAddressLookUpDataStructures/src/Models/DSModelType.h>

class DSModelSingleton {

private:
   static DSModelSingleton* dsModelSingleton_;
   DSModelmpl* dsModel_;
   DSModelType dsModelType_;

 explicit  DSModelSingleton(DSModelType dsModelType);

 ~DSModelSingleton();

public:

   static DSModelSingleton& GetSingletonInstance(DSModelType modelType);

  DSModelmpl & GetDSModel();

  DSModelType GetDSModelType();

};


#endif //DHCPALLOCATOR_DSMODELSINGLETON_H
