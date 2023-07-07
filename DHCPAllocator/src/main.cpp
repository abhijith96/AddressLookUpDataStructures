//
// Created by Abhijith  K A on 06/07/23.
//

#include <iostream>
#include <boost/icl/interval_map.hpp>
#include <string>
#include <DHCPAllocator/src/IpRange.h>




int main(){

    IpRange ipRange;


     boost::icl::interval_map<int,std::string> intervalMap;
     //open interval (10,20) excludes end points.
     intervalMap.insert({boost::icl::interval<int>::open(10,20), "map value"});

    intervalMap.insert({boost::icl::interval<int>::open(21,30), "map value2"});


     auto query_interval = boost::icl:: interval<int>::closed(5,5);
    auto query = intervalMap.lower_bound(query_interval);
     if(query != intervalMap.end()) {
         std::cout << "key " << query->first << " value " << query->second;
     }

     std::cout<<"printing all intervavls\n";
    for(auto & iter : intervalMap){
        auto key = iter.first;
        auto value = iter.second;

        std::cout<<"key : "<<key<<" value: "<<value<<"\n";
    }

     intervalMap.erase(boost::icl::interval<int>::open(21,30));

    std::cout<<"printing all intervavls again\n";

     for(auto & iter : intervalMap){
         auto key = iter.first;
         auto value = iter.second;

         std::cout<<"key : "<<key<<" value: "<<value<<"\n";
     }
    std::cout<<"hello world\n";
    return 0;
}