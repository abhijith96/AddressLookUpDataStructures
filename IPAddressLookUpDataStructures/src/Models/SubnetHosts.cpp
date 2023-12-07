//
// Created by Abhijith  K A on 11/07/23.
//

#include <DHCPAllocator/src/Models/SubnetHosts.h>

SubnetHosts::SubnetHosts(ip_t subnetIp, ip_t capacity) : nextFreeIpAvailable_(subnetIp + 1), capacity_(capacity) {

}

std::pair<bool, ip_t> SubnetHosts::InsertHost(ip_t subnetIp, MacID hostMacID) {
    ip_t startIp = subnetIp + 1;
    ip_t endIp = subnetIp + capacity_ - 2;
    if(!freeHostSlotsList_.empty() && freeHostSlotsList_.size() == capacity_ - 2){
        freeHostSlotsList_.clear();
        nextFreeIpAvailable_ = startIp;
    }
    if(nextFreeIpAvailable_ <= endIp){
        hostMacToIpMap_.emplace(hostMacID, nextFreeIpAvailable_);
        ip_t currentHostIp = nextFreeIpAvailable_;
        ++nextFreeIpAvailable_;
        return {true, currentHostIp};
    }
    else if(!freeHostSlotsList_.empty()){
        ip_t  currentHostIp = freeHostSlotsList_.back();
        freeHostSlotsList_.pop_back();
        hostMacToIpMap_.emplace(hostMacID, currentHostIp);
        return {true, currentHostIp};
    }
    else{
        return {false, subnetIp};
    }
}

std::pair<bool, ip_t> SubnetHosts::DeleteHost(ip_t subnetIp, MacID hostMacId) {
    auto itr = hostMacToIpMap_.find(hostMacId);
    if(itr == hostMacToIpMap_.end()){
        return {false, subnetIp};
    }
    ip_t  hostIp = itr->second;
    hostMacToIpMap_.erase(itr);
    freeHostSlotsList_.push_back(hostIp);
    return {true, hostIp};
}

std::pair<bool, ip_t> SubnetHosts::GetHostIp(ip_t subnetIp, MacID hostMacId) {
    auto itr = hostMacToIpMap_.find(hostMacId);
    if(itr != hostMacToIpMap_.end()){
        return {true, itr->second};
    }
    return {false, subnetIp};
}

std::pair<bool, MacID> SubnetHosts::GetHostMacId(ip_t subnetIp, ip_t hostIp) {

    for(auto & [macId, mappedHostIp] : hostMacToIpMap_){
        if(mappedHostIp == hostIp){
            return {true, macId};
        }
    }
    return {false, MacID{0}};
}

ip_t SubnetHosts::GetCapacity() const {
    return capacity_;
}




