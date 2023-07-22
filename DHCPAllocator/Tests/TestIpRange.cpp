//
// Created by Abhijith  K A on 11/07/23.
//


#include <iostream>
#include <DHCPAllocator/src/Models/DSModelSingleton.h>
#include <DHCPAllocator/src/Models/MacID.h>
#include <boost/container/flat_map.hpp>
#include <DHCPAllocator/src/Models/Subnet.h>
#include <DHCPAllocator/src/Models/DSModelmpl.h>

void TestIPRangeFunc(){

    boost::container::flat_map<int,int> flatmap_;
    flatmap_.insert({2,2});

    std::cout<<flatmap_.at(2);

    Subnet subnet{MacID{1}, 10, 20};


}

int main(){
    DSModelType impl_type = DSModelType::TREE;
    DSModelSingleton &dsModelSingleton = DSModelSingleton::GetSingletonInstance(impl_type);
    DSModelmpl& dsModel = dsModelSingleton.GetDSModel();

    TestIPRangeFunc();



}