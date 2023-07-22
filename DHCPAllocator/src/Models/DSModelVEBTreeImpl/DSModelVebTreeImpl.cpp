//
// Created by Abhijith  K A on 13/07/23.
//

#include <DHCPAllocator/src/Models/DSModelVEBTreeImpl/DSModelVebTreeImpl.h>

DSModelVebTreeImpl::DSModelVebTreeImpl(IpRange ipRange) : ipRange_(ipRange), vebTreeMap_(ipRange_.GetCapacity()),
freeIpStartRange_(ipRange.GetStartIP()){

}

std::pair<bool, ip_t>  DSModelVebTreeImpl::InsertSubnet(MacID subNetMacId, ip_t capacity) {
    if(std::numeric_limits<ip_t>::max() - capacity > (freeIpStartRange_)){
        ip_t startIP = freeIpStartRange_;
        ip_t endIP = startIP + (capacity - 1);
        if(endIP <= ipRange_.GetEndIP()) {
            ip_t startIPInVebTree = ConvertIpAddressFromIpRangeAddressSpaceToVebTreeAddressSpace(startIP);
            vebTreeMap_.Insert(startIPInVebTree, VEBTreeValueObject{static_cast<uint32_t>(capacity)});
            subnetToHostsMap_.emplace(startIP, SubnetHosts(startIP, capacity));
            freeIpStartRange_ = endIP + 1;
            return {true, startIP};
        }
        else if(!freeSubNetSlots_.empty()){
            auto [isSlotFound, iter] = FindBestFit(capacity);
            if(isSlotFound){
                VebTreeFreeSlotsObject freeSlotsObject = *iter;
                ip_t  hostIp = freeSlotsObject.GetStartIP();
                endIP = hostIp + (capacity - 1);
                ip_t hostIPInVebTree = ConvertIpAddressFromIpRangeAddressSpaceToVebTreeAddressSpace(hostIp);
                vebTreeMap_.Insert(hostIPInVebTree, VEBTreeValueObject{capacity});
                subnetToHostsMap_.emplace(hostIp, SubnetHosts{hostIp, capacity});
                ip_t  remainingCapacity = freeSlotsObject.GetCapacity() - capacity;
                freeSubNetSlots_.erase(iter);
                if(remainingCapacity > 0) {
                    freeSubNetSlots_.emplace(VebTreeFreeSlotsObject{endIP + 1, remainingCapacity});
                }
                return {true, hostIp};
            }
        }
    }

        return {false,std::numeric_limits<ip_t>::max()};

}

std::pair<bool, ip_t>  DSModelVebTreeImpl::InsertSubnetHost(MacID hostMacId, ip_t subnetIp) {
    auto iter = subnetToHostsMap_.find(subnetIp);
    if(iter != subnetToHostsMap_.end()){
        return iter->second.InsertHost(subnetIp, hostMacId);
    }
    else{
        return {false, subnetIp};
    }
}

bool DSModelVebTreeImpl::DeleteSubnet(ip_t start_ip) {
    if(start_ip < std::numeric_limits<ip_t>::max()){
        ip_t startIpInVEBTree = ConvertIpAddressFromIpRangeAddressSpaceToVebTreeAddressSpace(start_ip);
        vebTreeMap_.Delete(startIpInVEBTree);
        auto iter = subnetToHostsMap_.find(start_ip);
        if(iter != subnetToHostsMap_.end()){
            auto & subnetHosts = iter->second;
            ip_t capacity = subnetHosts.GetCapacity();
            subnetToHostsMap_.erase(iter);
            freeSubNetSlots_.emplace(VebTreeFreeSlotsObject{start_ip, capacity});
        }
        return true;
    }
    return false;
}



std::pair<bool, ip_t>  DSModelVebTreeImpl::GetNetWorkIP(ip_t hostIp) {
    ip_t hostIpInVEBTree = ConvertIpAddressFromIpRangeAddressSpaceToVebTreeAddressSpace(hostIp);
    auto [vebTreeNodeType, networkIPInVebTree, capacity] = vebTreeMap_.Predecessor(hostIpInVEBTree);
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



DSModelVebTreeImpl::~DSModelVebTreeImpl() {

}

ip_t DSModelVebTreeImpl::ConvertIpAddressFromIpRangeAddressSpaceToVebTreeAddressSpace(ip_t ipInIpRange) {
    return ipInIpRange - ipRange_.GetStartIP();
}

ip_t DSModelVebTreeImpl::ConvertIpAddressFromVebTreeToIpRangeAddressSpace(ip_t ipInVebTree) {
    return ipInVebTree + ipRange_.GetStartIP();
}

bool DSModelVebTreeImpl::DeleteHostFromSubnet(ip_t host_ip, ip_t subnet_ip) {
    return false;
}

std::pair<bool, ip_t> DSModelVebTreeImpl::GetHostIpAddress(MacID macId, ip_t subnet_ip) {
   auto iter = subnetToHostsMap_.find(subnet_ip);
   if(iter != subnetToHostsMap_.end()){
       return iter->second.GetHostIp(subnet_ip, macId);
   }
   return {false, 0};
}

std::pair<bool, MacID> DSModelVebTreeImpl::GetMacAddressOfHost(ip_t hostIpAddress, ip_t subnet_ip) {
    auto iter = subnetToHostsMap_.find(subnet_ip);
    if(iter != subnetToHostsMap_.end()){
        return iter->second.GetHostMacId(subnet_ip, hostIpAddress);
    }
    return {false, MacID{0}};
}

std::pair<bool, ip_t> DSModelVebTreeImpl::DeleteHostFromSubnet(MacID host_mac, ip_t subnet_ip) {
    auto iter = subnetToHostsMap_.find(subnet_ip);
    if(iter != subnetToHostsMap_.end()){
        return iter->second.DeleteHost(subnet_ip, host_mac);
    }
    return {false, subnet_ip};
}

std::pair<bool, boost::container::flat_set<VebTreeFreeSlotsObject>::iterator>   DSModelVebTreeImpl::FindBestFit(ip_t requiredCapacity) {
    VebTreeFreeSlotsObject vebTreeFreeSlotsObject{0, requiredCapacity};

    auto iter = freeSubNetSlots_.lower_bound(vebTreeFreeSlotsObject);
    if(iter == freeSubNetSlots_.end()){
        return {false, iter};
    }
    return  {true, iter};
}
