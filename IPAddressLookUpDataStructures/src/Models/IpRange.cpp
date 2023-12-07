//
// Created by Abhijith  K A on 16/07/23.
//

#include <IPAddressLookUpDataStructures/src/Models/IpRange.h>
#include <limits>
#include <stdexcept>

IpRange::IpRange(IPAddress startIp, IPAddress endIp): startIp_(startIp), endIp_(endIp) {
    if(startIp_.GetValue() >= 0 && endIp_.GetValue() >= 0 && endIp_.GetValue() < std::numeric_limits<ip_t>::max()){
       //pass
    }
    else{
        throw std::invalid_argument("Ip Ranges Not valid\n");
    }
}

ip_t IpRange::GetCapacity() const{
    return (endIp_.GetValue() + 1) - startIp_.GetValue();
}

ip_t IpRange::GetStartIP() const {
    return startIp_.GetValue();
}

ip_t IpRange::GetEndIP() const{
    return endIp_.GetValue();
}

IpRange::IpRange(const IpRange &other) = default;

IpRange &IpRange::operator=(const IpRange &other) = default;

IpRange::IpRange(IpRange &&other) = default;

IpRange &IpRange::operator=(IpRange &&other) noexcept =default;
