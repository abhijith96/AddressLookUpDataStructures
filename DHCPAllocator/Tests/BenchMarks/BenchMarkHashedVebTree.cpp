//
// Created by Abhijith  K A on 18/07/23.
//

#include <chrono>

#include <DHCPAllocator/src/Models/DSModelVEBTreeImpl/DSModelHashedVebTreeImpl.h>
#include <DHCPAllocator/src/Models/IPAddress.h>
#include <DHCPAllocator/src/Models/IpRange.h>
#include <iostream>
#include <bit>

std::unique_ptr<DSModelHashedVebTreeImpl> GetDSModelWithCapacity(uint32_t capacityLog2){
    const ip_t capacity = static_cast<ip_t >(std::pow(2, capacityLog2)) - 1;
    const ip_t startIp = 0;
    const ip_t endIp = startIp + capacity;
    IpRange ipRange{IPAddress{startIp}, IPAddress{endIp}};
    DSModelHashedVebTreeImpl dsModelHashedVebTree{ipRange};
    return std::make_unique<DSModelHashedVebTreeImpl>(std::move(dsModelHashedVebTree));

}

void InsertSubnets(DSModelHashedVebTreeImpl& dsModelHashedVebTreeImpl, ip_t subNetCapacity){
    ip_t totalCapacity = dsModelHashedVebTreeImpl.GetIpRange().GetCapacity();
    for(size_t start = 0; start < totalCapacity;start += subNetCapacity){
        MacID subnetMacId{start};
        ip_t startIp;
        bool isInserted;
      std::tie(isInserted, startIp) =  dsModelHashedVebTreeImpl.InsertSubnet(subnetMacId, subNetCapacity);
    }
}

void DeleteHalfOfSubnets(DSModelHashedVebTreeImpl& dsModelHashedVebTreeImpl, ip_t subNetCapacity){
    ip_t totalCapacity = dsModelHashedVebTreeImpl.GetIpRange().GetCapacity();
    ip_t  two = 2;
    for(size_t start = 0; start < totalCapacity;start += two * subNetCapacity){
        MacID subnetMacId{start};
        ip_t startIp;
        bool isDeleted;
        dsModelHashedVebTreeImpl.DeleteSubnet(start);
    }
}

ip_t QueryHostsInActiveSubNets(DSModelHashedVebTreeImpl& dsModelHashedVebTreeImpl, ip_t subNetCapacity){
    ip_t totalCapacity = dsModelHashedVebTreeImpl.GetIpRange().GetCapacity();
    ip_t  two = 2;
    ip_t networkCount = 0;
    for(size_t start = subNetCapacity; start < totalCapacity;start += two * subNetCapacity){
       ip_t hostIp = start + 10;
       auto [isNetworkPresent, networkIp] = dsModelHashedVebTreeImpl.GetNetWorkIP(hostIp);
       if(isNetworkPresent) ++ networkCount;
    }

    return networkCount;
}





int main(){

    ip_t capacityLog2 = 32;
    std::unique_ptr<DSModelHashedVebTreeImpl> dsModelHashedVebTreeImpl =   GetDSModelWithCapacity(capacityLog2);

    auto IpRange = dsModelHashedVebTreeImpl->GetIpRange();

    std::cout<<"current Total address space length : "<<IpRange.GetCapacity()<<"\n";


    const int numIterations = 3;

    // Record the start time
    auto startTime = std::chrono::high_resolution_clock::now();

    ip_t subnetCapacity = 1000;


        InsertSubnets(*dsModelHashedVebTreeImpl, subnetCapacity);


    // Record the end time
    auto endTime = std::chrono::high_resolution_clock::now();

    // Calculate the elapsed time per iteration
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    // Calculate the average time per iteration


    ip_t networkIp ;
    bool isPresent;

    ip_t testHostIp = 429496000;

    std::tie(isPresent, networkIp) = dsModelHashedVebTreeImpl->GetNetWorkIP(testHostIp);
    if(!isPresent){
        std::cout<<"Bug in benchmarking\n";
    }
    double averageTimePerIteration = static_cast<double>(duration) / numIterations;

    std::cout << "Average time per iteration: " << averageTimePerIteration << " milli seconds" << std::endl;


    // Record the start time
    auto startTime_2 = std::chrono::high_resolution_clock::now();



    DeleteHalfOfSubnets(*dsModelHashedVebTreeImpl, subnetCapacity);


    // Record the end time
    auto endTime_2 = std::chrono::high_resolution_clock::now();

    // Calculate the elapsed time per iteration
    auto duration_2 = std::chrono::duration_cast<std::chrono::milliseconds>(endTime_2 - startTime_2).count();


    std::tie(isPresent, networkIp) = dsModelHashedVebTreeImpl->GetNetWorkIP(testHostIp);
    if(!isPresent){
        std::cout<<"Bug in benchmarking\n";
    }
    {
        std::tie(isPresent, networkIp) = dsModelHashedVebTreeImpl->GetNetWorkIP(10);
        if(isPresent){
            std::cout<<"Bug in benchmarking 2\n";
        }
    }
    double averageTimePerIteration_2 = static_cast<double>(duration_2);

    std::cout << "Time for Deleting Subnets " << averageTimePerIteration_2 << " milli seconds" << std::endl;


    // Record the start time
    auto startTime_3 = std::chrono::high_resolution_clock::now();



    ip_t networkCount = QueryHostsInActiveSubNets(*dsModelHashedVebTreeImpl, subnetCapacity);


    // Record the end time
    auto endTime_3 = std::chrono::high_resolution_clock::now();

    // Calculate the elapsed time per iteration
    auto duration_3 = std::chrono::duration_cast<std::chrono::milliseconds>(endTime_3 - startTime_3).count();

    double averageTimePerIteration_3 = static_cast<double>(duration_3);

    std::cout<<"total networks count: "<<networkCount<<"\n";

    std::cout << "Time for Querying hosts " << averageTimePerIteration_3 << " milli seconds" << std::endl;


    return 0;

}