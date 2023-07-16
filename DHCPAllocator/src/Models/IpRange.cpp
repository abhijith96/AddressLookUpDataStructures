//
// Created by Abhijith  K A on 16/07/23.
//

#include <DHCPAllocator/src/Models/IpRange.h>
#include <limits>
#include <stdexcept>

IpRange::IpRange(IPAddress startIp, IPAddress endIp): startIp_(startIp), endIp_(endIp) {
    if(startIp.GetValue() > 0 && endIp_.GetValue() > 0 &&
     startIp_.GetValue() <= endIp.GetValue()){
       //pass
    }
    else{
        throw std::invalid_argument("Ip Ranges Not valid\n");
    }
}

ip_t IpRange::GetCapacity() const{
    return endIp_.GetValue() - startIp_.GetValue();
}

ip_t IpRange::GetStartIP() const {
    return startIp_.GetValue();
}
