//
// Created by Abhijith  K A on 22/07/23.
//
//
// Created by Abhijith  K A on 18/07/23.
//

#include <chrono>

#include <DHCPAllocator/src/Models/DSModelTreeImpl/DSModelTreeImpl.h>
#include <DHCPAllocator/src/Models/IPAddress.h>
#include <DHCPAllocator/src/Models/IpRange.h>
#include <iostream>
#include <bit>
#include <cmath>


const ip_t globalTotalCapacity = static_cast<ip_t>( std::pow(2,28));
std::unique_ptr<DSModelTreeImpl> GetDSModelWithCapacity(uint32_t capacityLog2){
    const ip_t capacity = static_cast<ip_t >(std::pow(2, capacityLog2)) - 1;
    const ip_t startIp = 0;
    const ip_t endIp = startIp + capacity;
    IpRange ipRange{IPAddress{startIp}, IPAddress{endIp}};
    DSModelTreeImpl dsModelTreeImpl{};
    return std::make_unique<DSModelTreeImpl>(std::move(dsModelTreeImpl));

}

void InsertSubnets(DSModelTreeImpl& DSModelTreeImpl, ip_t subNetCapacity){
    ip_t totalCapacity = globalTotalCapacity;
    for(size_t start = 0; start < totalCapacity;start += subNetCapacity){
        MacID subnetMacId{start};
        ip_t startIp;
        bool isInserted;
        std::tie(isInserted, startIp) =  DSModelTreeImpl.InsertSubnet(subnetMacId, subNetCapacity);
    }
}

void InsertSubnetsAgain(DSModelTreeImpl& DSModelTreeImpl, ip_t subNetCapacity){
    ip_t totalCapacity = globalTotalCapacity;
    for(size_t start = 1; start < totalCapacity;start += subNetCapacity){
        MacID subnetMacId{start};
        ip_t startIp;
        bool isInserted;
        std::tie(isInserted, startIp) =  DSModelTreeImpl.InsertSubnet(subnetMacId, subNetCapacity);
    }
}

void DeleteHalfOfSubnets(DSModelTreeImpl& DSModelTreeImpl, ip_t subNetCapacity){
    ip_t totalCapacity = globalTotalCapacity;
    ip_t  two = 2;
    for(size_t start = 0; start < totalCapacity;start += two * subNetCapacity){
        MacID subnetMacId{start};
        ip_t startIp;
        bool isDeleted;
        DSModelTreeImpl.DeleteSubnet(start);
    }
}

ip_t QueryHostsInActiveSubNets(DSModelTreeImpl& DSModelTreeImpl, ip_t subNetCapacity){
    ip_t totalCapacity = globalTotalCapacity;
    ip_t  two = 2;
    ip_t networkCount = 0;
    for(size_t start = subNetCapacity; start < totalCapacity;start += two * subNetCapacity){
        ip_t hostIp = start + 10;
        auto [isNetworkPresent, networkIp] = DSModelTreeImpl.GetNetWorkIP(hostIp);
        if(isNetworkPresent) ++ networkCount;
    }

    return networkCount;
}





int main(){

    ip_t capacityLog2 = 32;
    std::unique_ptr<DSModelTreeImpl> DSModelTreeImpl =   GetDSModelWithCapacity(capacityLog2);


    ip_t subnetCapacity = 1000;
    // Record the start time
    auto startTime = std::chrono::high_resolution_clock::now();




    InsertSubnets(*DSModelTreeImpl, subnetCapacity);


    // Record the end time
    auto endTime = std::chrono::high_resolution_clock::now();

    // Calculate the elapsed time per iteration
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    // Calculate the average time per iteration


    ip_t networkIp ;
    bool isPresent;

    //ip_t testHostIp = 429496000;

    ip_t testHostIp = 2000000;

    std::tie(isPresent, networkIp) = DSModelTreeImpl->GetNetWorkIP(testHostIp);
    if(!isPresent){
        std::cout<<"Bug in benchmarking\n";
    }
    auto averageTimePerIteration = static_cast<double>(duration);

    std::cout << "Average time per iteration: " << averageTimePerIteration << " milli seconds" << std::endl;


    // Record the start time
    auto startTime_2 = std::chrono::high_resolution_clock::now();



    DeleteHalfOfSubnets(*DSModelTreeImpl, subnetCapacity);


    // Record the end time
    auto endTime_2 = std::chrono::high_resolution_clock::now();

    // Calculate the elapsed time per iteration
    auto duration_2 = std::chrono::duration_cast<std::chrono::milliseconds>(endTime_2 - startTime_2).count();


    std::tie(isPresent, networkIp) = DSModelTreeImpl->GetNetWorkIP(testHostIp);
    if(!isPresent){
        std::cout<<"Bug in benchmarking\n";
    }
    {
        std::tie(isPresent, networkIp) = DSModelTreeImpl->GetNetWorkIP(10);
        if(isPresent){
            std::cout<<"Bug in benchmarking 2\n";
        }
    }
    double averageTimePerIteration_2 = static_cast<double>(duration_2);

    std::cout << "Time for Deleting Subnets " << averageTimePerIteration_2 << " milli seconds" << std::endl;


    // Record the start time
    auto startTime_3 = std::chrono::high_resolution_clock::now();



    ip_t networkCount = QueryHostsInActiveSubNets(*DSModelTreeImpl, subnetCapacity);


    // Record the end time
    auto endTime_3 = std::chrono::high_resolution_clock::now();

    // Calculate the elapsed time per iteration
    auto duration_3 = std::chrono::duration_cast<std::chrono::milliseconds>(endTime_3 - startTime_3).count();

    double averageTimePerIteration_3 = static_cast<double>(duration_3);

    std::cout<<"total networks count: "<<networkCount<<"\n";

    std::cout << "Time for Querying hosts " << averageTimePerIteration_3 << " milli seconds" << std::endl;


    return 0;

}