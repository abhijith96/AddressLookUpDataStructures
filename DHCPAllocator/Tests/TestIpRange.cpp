//
// Created by Abhijith  K A on 11/07/23.
//


#include <iostream>
#include <DHCPAllocator/src/Models/DSModelSingleton.h>
#include <DHCPAllocator/src/Models/MacID.h>

void TestIPRangeFunc(){



}

int main(){
    std::string impl_type = "tree";
    DSModelSingleton &dsModelSingleton = DSModelSingleton::GetSingletonInstance(impl_type);
    DSModel& dsModel = dsModelSingleton.GetDSModel();
    dsModel.GetIpAddress(MacID{10});



}