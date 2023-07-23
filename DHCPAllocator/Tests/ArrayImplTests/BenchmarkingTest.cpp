//
// Created by Sreedevi R Warrier on 2023-07-16.
//
#ifndef DHCPALLOCATOR_ARRAYIMPL_BENCHMARKING_TEST1_H
#define DHCPALLOCATOR_ARRAYIMPL_BENCHMARKING_TEST1_H

#include <chrono>
#include <DHCPAllocator/src/Models/IPAddress.h>
#include <iostream>
#include <DHCPAllocator/src/Models/MacID.h>
#include <DHCPAllocator/src/Models/DSModelArrayImpl/DSModelArrayImpl.h>


int capacityLog2 = 28;
const int total_capacity = static_cast<int >(std::pow(2, capacityLog2)) - 1;


void InsertSubnets(DSModelArrayImpl& dsModelArrayImpl, ip_t subNetCapacity){
    for (size_t start = 0; start < total_capacity; start += subNetCapacity) {
        MacID subnetMacId{start};
        ip_t startIp;
        bool isInserted;
        std::tie(isInserted, startIp) = dsModelArrayImpl.InsertSubnet(subnetMacId, subNetCapacity);
    }
}

void DeleteHalfOfSubnets(DSModelArrayImpl & dsModelArrayImpl, ip_t subNetCapacity){
    ip_t two = 2;
    for (size_t start = 0; start < total_capacity; start += two * subNetCapacity) {
        MacID subnetMacId{start};
        ip_t startIp;
        bool isDeleted;
        dsModelArrayImpl.DeleteSubnet(start);
    }
}

ip_t QueryHostsInActiveSubNets(DSModelArrayImpl& dsModelArrayImpl, ip_t subNetCapacity){
    ip_t  two = 2;
    ip_t networkCount = 0;
    for (size_t start = subNetCapacity; start < total_capacity; start += two * subNetCapacity) {

        ip_t hostIp = start + 10;
        auto [isNetworkPresent, networkIp] = dsModelArrayImpl.GetNetWorkIP(hostIp);
        if (isNetworkPresent) ++networkCount;
    }

    return networkCount;
}


int main(){

    DSModelArrayImpl dsModelArrayImpl;

    const int numIterations = 3;

    // Record the start time
    auto startTime = std::chrono::high_resolution_clock::now();

    ip_t subnetCapacity = 1000;


    InsertSubnets(dsModelArrayImpl, subnetCapacity);


    // Record the end time
    auto endTime = std::chrono::high_resolution_clock::now();

    // Calculate the elapsed time per iteration
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    // Calculate the average time per iteration


    ip_t networkIp ;
    bool isPresent;

    ip_t testHostIp = 1100;

    std::tie(isPresent, networkIp) = dsModelArrayImpl.GetNetWorkIP(testHostIp);
    if(!isPresent){
        std::cout<<"Bug in benchmarking\n";
    }
    double averageTimePerIteration = static_cast<double>(duration) / numIterations;

    std::cout << "Average time per iteration: " << averageTimePerIteration << " milli seconds" << std::endl;


    // Record the start time
    auto startTime_2 = std::chrono::high_resolution_clock::now();



    DeleteHalfOfSubnets(dsModelArrayImpl, subnetCapacity);


    // Record the end time
    auto endTime_2 = std::chrono::high_resolution_clock::now();

    // Calculate the elapsed time per iteration
    auto duration_2 = std::chrono::duration_cast<std::chrono::milliseconds>(endTime_2 - startTime_2).count();


    std::tie(isPresent, networkIp) = dsModelArrayImpl.GetNetWorkIP(testHostIp);
    if(!isPresent){
        std::cout<<"Bug in benchmarking\n";
    }
    {
        std::tie(isPresent, networkIp) = dsModelArrayImpl.GetNetWorkIP(10);
        if(isPresent){
            std::cout<<"Bug in benchmarking 2\n";
        }
    }
    double averageTimePerIteration_2 = static_cast<double>(duration_2);

    std::cout << "Time for Deleting Subnets " << averageTimePerIteration_2 << " milli seconds" << std::endl;


    // Record the start time
    auto startTime_3 = std::chrono::high_resolution_clock::now();



    ip_t networkCount = QueryHostsInActiveSubNets(dsModelArrayImpl, subnetCapacity);


    // Record the end time
    auto endTime_3 = std::chrono::high_resolution_clock::now();

    // Calculate the elapsed time per iteration
    auto duration_3 = std::chrono::duration_cast<std::chrono::milliseconds>(endTime_3 - startTime_3).count();

    double averageTimePerIteration_3 = static_cast<double>(duration_3);

    std::cout<<"total networks count: "<<networkCount<<"\n";

    std::cout << "Time for Querying hosts " << averageTimePerIteration_3 << " milli seconds" << std::endl;


    // Record the start time
    auto startTime_4 = std::chrono::high_resolution_clock::now();


    auto new_assignments = dsModelArrayImpl.optimizeSubnetAllocationSpace();
    std::cout << "size of subnets after optimizing " << new_assignments.size() << std::endl;
    // Record the end time
    auto endTime_4 = std::chrono::high_resolution_clock::now();
    // Calculate the elapsed time per iteration
    auto duration_4 = std::chrono::duration_cast<std::chrono::milliseconds>(endTime_4 - startTime_4).count();

    double averageTimePerIteration_4 = static_cast<double>(duration_4);

    std::cout << "Time for optimizing subnets free slots " << averageTimePerIteration_4 << " milli seconds" << std::endl;




    // Test case 1: Required capacity is not the exact match, but a capacity higher than the required capacity is available
    int requiredCapacity = 10000;
    auto result = dsModelArrayImpl.InsertSubnet(MacID(134218 * 3 + 1), requiredCapacity);
    if (result.first) {
        std::cout << "Insert subnet returned start IP: " << result.second << std::endl;
    } else {
        std::cout << "Insert subnet failed." << std::endl;
    }

    return 0;

}

#endif