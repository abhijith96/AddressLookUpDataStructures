//
// Created by Abhijith  K A on 17/07/23.
//

#include <IPAddressLookUpDataStructures/src/Models/DSModelVEBTreeImpl/DSModelHashedVebTreeImpl.h>

ip_t DSModelHashedVebTreeImpl::ConvertIpAddressFromIpRangeAddressSpaceToVebTreeAddressSpace(ip_t ipInIpRange) {
    return ipInIpRange - ipRange_.GetStartIP();
}

ip_t DSModelHashedVebTreeImpl::ConvertIpAddressFromVebTreeToIpRangeAddressSpace(ip_t ipInVebTree) {
    return ipInVebTree + ipRange_.GetStartIP();
}

std::pair<bool, ip_t> DSModelHashedVebTreeImpl::InsertSubnet(MacID subNetMacId, ip_t capacity) {
    if(std::numeric_limits<ip_t>::max() - capacity > (freeIpStartRange_)) {
        ip_t startIP = freeIpStartRange_;
        ip_t endIP = startIP + (capacity - 1);
        if (endIP <= ipRange_.GetEndIP()) {
            ip_t startIPInVebTree = ConvertIpAddressFromIpRangeAddressSpaceToVebTreeAddressSpace(startIP);
            vebTreeWithHashMap_.Insert(startIPInVebTree, VEBTreeValueObject{static_cast<uint32_t>(capacity)});
            subnetToHostsMap_.emplace(startIP, SubnetHosts(startIP, capacity));
            freeIpStartRange_ = endIP + 1;
            return {true, startIP};
        }
    }
    if(!freeSlotsList_.IsEmpty()){
            auto [isFreeSlotPresent, freeSlotObjectRef] = FindBestFit(capacity);
            if(isFreeSlotPresent){
                ip_t hostIp = freeSlotObjectRef.get().GetIpListHavingCapacity().back();
                ip_t currentCapacity =  freeSlotObjectRef.get().getCapacity();
                freeSlotObjectRef.get().GetIpListHavingCapacity().pop_back();
                if(freeSlotObjectRef.get().GetIpListHavingCapacity().empty()){
                    freeSlotsList_.Delete(currentCapacity);
                }
                ip_t remainingCapacity = currentCapacity - capacity;
                if(remainingCapacity > 0){
                    ip_t  newStartIp = hostIp + capacity;
                    UpdateFreeSlotList(newStartIp, remainingCapacity);
                }
                subnetToHostsMap_.emplace(hostIp, SubnetHosts(hostIp, capacity));
                return {true, hostIp};
            }
    }

    return {false,std::numeric_limits<ip_t>::max()};
}

std::pair<bool, ip_t> DSModelHashedVebTreeImpl::InsertSubnetHost(MacID hostMacId, ip_t subnetIp) {
    auto iter = subnetToHostsMap_.find(subnetIp);
    if(iter != subnetToHostsMap_.end()){
        return iter->second.InsertHost(subnetIp, hostMacId);
    }
    else{
        return {false, subnetIp};
    }
}

bool DSModelHashedVebTreeImpl::DeleteSubnet(ip_t start_ip) {
    if(start_ip < std::numeric_limits<ip_t>::max()){
        ip_t startIpInVEBTree = ConvertIpAddressFromIpRangeAddressSpaceToVebTreeAddressSpace(start_ip);
        vebTreeWithHashMap_.Delete(startIpInVEBTree);
        auto iter = subnetToHostsMap_.find(start_ip);
        if(iter != subnetToHostsMap_.end()){
            auto & subnetHosts = iter->second;
            ip_t capacity = subnetHosts.GetCapacity();
            UpdateFreeSlotList(start_ip, capacity);
            subnetToHostsMap_.erase(iter);
        }
        return true;
    }
    return false;

}

bool DSModelHashedVebTreeImpl::DeleteHostFromSubnet(ip_t host_ip, ip_t subnet_ip) {
    return false;
}

std::pair<bool, ip_t>  DSModelHashedVebTreeImpl::DeleteHostFromSubnet(MacID host_mac, ip_t subnet_ip) {
    auto iter = subnetToHostsMap_.find(subnet_ip);
    if(iter != subnetToHostsMap_.end()){
        return iter->second.DeleteHost(subnet_ip, host_mac);
    }
    return {false, subnet_ip};
}

std::pair<bool, ip_t> DSModelHashedVebTreeImpl::GetNetWorkIP(ip_t hostIp) {
    ip_t hostIpInVEBTree = ConvertIpAddressFromIpRangeAddressSpaceToVebTreeAddressSpace(hostIp);
    auto [vebTreeNodeType, networkIPInVebTree, capacity] = vebTreeWithHashMap_.Predecessor(hostIpInVEBTree);
    if(vebTreeNodeType == VEBTreeNodeKeyType::NORMAL){
        ip_t networkIP = ConvertIpAddressFromVebTreeToIpRangeAddressSpace(networkIPInVebTree);
        ip_t lastIpInNetwork = networkIP + capacity.GetCapacity() - 1;
        if(hostIp <= lastIpInNetwork)
            return  {true, networkIP};
        else
            return {false, std::numeric_limits<ip_t>::max()};
    }
    else{
        return  {false, std::numeric_limits<ip_t>::max()};
    }
}

std::pair<bool, ip_t> DSModelHashedVebTreeImpl::GetHostIpAddress(MacID macId, ip_t subnet_ip) {
    auto iter = subnetToHostsMap_.find(subnet_ip);
    if(iter != subnetToHostsMap_.end()){
        return iter->second.GetHostIp(subnet_ip, macId);
    }
    return {false, 0};
}

std::pair<bool, MacID> DSModelHashedVebTreeImpl::GetMacAddressOfHost(ip_t hostIpAddress, ip_t subnet_ip) {
    auto iter = subnetToHostsMap_.find(subnet_ip);
    if(iter != subnetToHostsMap_.end()){
        return iter->second.GetHostMacId(subnet_ip, hostIpAddress);
    }
    return {false, MacID{0}};
}

DSModelHashedVebTreeImpl::~DSModelHashedVebTreeImpl() {

}

DSModelHashedVebTreeImpl::DSModelHashedVebTreeImpl(IpRange ipRange): ipRange_(ipRange), vebTreeWithHashMap_(ipRange.GetCapacity()) ,freeSlotsList_(ipRange.GetCapacity()) {
    freeIpStartRange_ = ipRange_.GetStartIP();

}

std::pair<bool, std::reference_wrapper<VebTreeWithHashMapFreeSlotObject>> DSModelHashedVebTreeImpl::FindBestFit(ip_t requiredCapacity) {
   auto [nodeType, key, valueType] = freeSlotsList_.Successor(requiredCapacity);
   if(nodeType == VEBTreeNodeKeyType::NORMAL){
       auto [isPresent, valueRef] = freeSlotsList_.GetValue(key);
       return {isPresent, valueRef};
   }
   else{
       return {false, std::reference_wrapper<VebTreeWithHashMapFreeSlotObject>{valueType}};
   }
}

void DSModelHashedVebTreeImpl::UpdateFreeSlotList(ip_t newStartIp, ip_t remainingCapacity) {
    auto [isPresent, valueRef] = freeSlotsList_.GetValue(remainingCapacity);
    if(isPresent){
        valueRef.get().GetIpListHavingCapacity().push_back(newStartIp);
    }
    else{
        freeSlotsList_.Insert(remainingCapacity, VebTreeWithHashMapFreeSlotObject{remainingCapacity, newStartIp});

    }
}

const IpRange &DSModelHashedVebTreeImpl::GetIpRange() const {
    return ipRange_;
}

DSModelHashedVebTreeImpl::DSModelHashedVebTreeImpl(DSModelHashedVebTreeImpl &&other)  noexcept = default;

DSModelHashedVebTreeImpl& DSModelHashedVebTreeImpl::operator=(DSModelHashedVebTreeImpl &&other) noexcept = default;
