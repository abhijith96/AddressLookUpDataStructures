//
// Created by Abhijith  K A on 22/07/23.
//
//
// Created by Abhijith  K A on 18/07/23.
//

#include <chrono>

#include <IPAddressLookUpDataStructures/src/Models/DSModelVEBTreeImpl/DSModelVebTreeImpl.h>
#include <IPAddressLookUpDataStructures/src/Models/IPAddress.h>
#include <IPAddressLookUpDataStructures/src/Models/IpRange.h>
#include <iostream>
#include <bit>

std::unique_ptr<DSModelVebTreeImpl> GetDSModelWithCapacity(uint32_t capacityLog2){
    const ip_t capacity = static_cast<ip_t >(std::pow(2, capacityLog2)) - 1;
    const ip_t startIp = 0;
    const ip_t endIp = startIp + capacity;
    IpRange ipRange{IPAddress{startIp}, IPAddress{endIp}};
    return std::make_unique<DSModelVebTreeImpl>(ipRange);

}

void InsertSubnets(DSModelVebTreeImpl& dsModelVebTreeImpl, ip_t subNetCapacity){
    ip_t totalCapacity = dsModelVebTreeImpl.GetIpRange().GetCapacity();
    for(size_t start = 0; start < totalCapacity;start += subNetCapacity){
        MacID subnetMacId{start};
        ip_t startIp;
        bool isInserted;
        std::tie(isInserted, startIp) =  dsModelVebTreeImpl.InsertSubnet(subnetMacId, subNetCapacity);
    }
}

void InsertSubnetsAgain(DSModelVebTreeImpl& DSModelVebTreeImpl, ip_t subNetCapacity){
    ip_t totalCapacity = DSModelVebTreeImpl.GetIpRange().GetCapacity();
    ip_t  two = 2;
    for(size_t start = 0; start < totalCapacity;start +=  two * subNetCapacity){
        MacID subnetMacId{start};
        ip_t startIp;
        bool isInserted;
        std::tie(isInserted, startIp) =  DSModelVebTreeImpl.InsertSubnet(subnetMacId, subNetCapacity);
    }
}

void DeleteHalfOfSubnets(DSModelVebTreeImpl& DSModelVebTreeImpl, ip_t subNetCapacity){
    ip_t totalCapacity = DSModelVebTreeImpl.GetIpRange().GetCapacity();
    ip_t  two = 2;
    for(size_t start = 0; start < totalCapacity;start += two * subNetCapacity){
        MacID subnetMacId{start};
        ip_t startIp;
        bool isDeleted;
        DSModelVebTreeImpl.DeleteSubnet(start);
    }
}

ip_t QueryHostsInActiveSubNets(DSModelVebTreeImpl& DSModelVebTreeImpl, ip_t subNetCapacity){
    ip_t totalCapacity = DSModelVebTreeImpl.GetIpRange().GetCapacity();
    ip_t  two = 2;
    ip_t networkCount = 0;
    for(size_t start = subNetCapacity; start < totalCapacity;start += two * subNetCapacity){
        ip_t hostIp = start + 10;
        auto [isNetworkPresent, networkIp] = DSModelVebTreeImpl.GetNetWorkIP(hostIp);
        if(isNetworkPresent) ++ networkCount;
    }

    return networkCount;
}





int main(){

    ip_t capacityLog2 = 28;
    auto startTimeCreation = std::chrono::high_resolution_clock::now();

    std::unique_ptr<DSModelVebTreeImpl> DSModelVebTreeImpl =   GetDSModelWithCapacity(capacityLog2);

    auto endTimeCreation = std::chrono::high_resolution_clock::now();

    auto durationCreation = std::chrono::duration_cast<std::chrono::milliseconds>(endTimeCreation - startTimeCreation).count();

    std::cout << "TIme for creation of VEBTree " <<durationCreation << " milli seconds" << std::endl;

    auto IpRange = DSModelVebTreeImpl->GetIpRange();

    std::cout<<"current Total address space length : "<<IpRange.GetCapacity()<<"\n";


    const int numIterations = 3;

    // Record the start time
    auto startTime = std::chrono::high_resolution_clock::now();

    ip_t subnetCapacity = 1000;


    InsertSubnets(*DSModelVebTreeImpl, subnetCapacity);


    // Record the end time
    auto endTime = std::chrono::high_resolution_clock::now();

    // Calculate the elapsed time per iteration
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    // Calculate the average time per iteration


    ip_t networkIp ;
    bool isPresent;

    //ip_t testHostIp = 429496000;

    ip_t testHostIp = 1099;

    std::tie(isPresent, networkIp) = DSModelVebTreeImpl->GetNetWorkIP(testHostIp);
    if(!isPresent){
        std::cout<<"Bug in benchmarking\n";
    }


    std::cout << "TIme for insertion of Subnets " <<duration << " milli seconds" << std::endl;


    // Record the start time
    auto startTime_2 = std::chrono::high_resolution_clock::now();



    DeleteHalfOfSubnets(*DSModelVebTreeImpl, subnetCapacity);


    // Record the end time
    auto endTime_2 = std::chrono::high_resolution_clock::now();

    // Calculate the elapsed time per iteration
    auto duration_2 = std::chrono::duration_cast<std::chrono::milliseconds>(endTime_2 - startTime_2).count();


    std::tie(isPresent, networkIp) = DSModelVebTreeImpl->GetNetWorkIP(testHostIp);
    if(!isPresent){
        std::cout<<"Bug in benchmarking\n";
    }
    {
        std::tie(isPresent, networkIp) = DSModelVebTreeImpl->GetNetWorkIP(10);
        if(isPresent){
            std::cout<<"Bug in benchmarking 2\n";
        }
    }


    std::cout << "Time for Deleting Subnets " << duration_2 << " milli seconds" << std::endl;


    // Record the start time
    auto startTime_3 = std::chrono::high_resolution_clock::now();



    ip_t networkCount = QueryHostsInActiveSubNets(*DSModelVebTreeImpl, subnetCapacity);


    // Record the end time
    auto endTime_3 = std::chrono::high_resolution_clock::now();

    // Calculate the elapsed time per iteration
    auto duration_3 = std::chrono::duration_cast<std::chrono::milliseconds>(endTime_3 - startTime_3).count();



    std::cout<<"total networks count: "<<networkCount<<"\n";

    std::cout << "Time for Querying hosts " << duration_3 << " milli seconds" << std::endl;



    //record time for inserting again

    // Record the start time
    auto startTime_4 = std::chrono::high_resolution_clock::now();


    InsertSubnetsAgain(*DSModelVebTreeImpl, subnetCapacity);


    // Record the end time
    auto endTime_4 = std::chrono::high_resolution_clock::now();

    // Calculate the elapsed time per iteration
    auto duration_4 = std::chrono::duration_cast<std::chrono::milliseconds>(endTime_4 - startTime_4).count();





    std::cout << "Time for Inserting Hosts Again " << duration_4 << " milli seconds" << std::endl;



    return 0;

}