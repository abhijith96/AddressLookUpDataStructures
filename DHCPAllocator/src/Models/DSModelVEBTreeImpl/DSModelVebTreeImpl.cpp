//
// Created by Abhijith  K A on 13/07/23.
//

#include <DHCPAllocator/src/Models/DSModelVEBTreeImpl/DSModelVebTreeImpl.h>

DSModelVebTreeImpl::DSModelVebTreeImpl(IpRange ipRange) : ipRange_(ipRange), vebTreeMap_(ipRange_.GetCapacity()),
freeIpStartRange_(0){

}

ip_t DSModelVebTreeImpl::InsertSubnet(MacID subNetMacId, int capacity) {
    if(std::numeric_limits<ip_t>::max() - capacity < (freeIpStartRange_)){
        ip_t startIP = freeIpStartRange_;
        ip_t startIPInVebTree = ConvertIpAddressFromIpRangeAddressSpaceToVebTreeAddressSpace(startIP);
        vebTreeMap_.Insert(startIPInVebTree, VEBTreeValueObject{static_cast<uint32_t>(capacity)});
        ip_t endIP = startIP + (capacity - 1);
        freeIpStartRange_ = endIP + 1;
        return startIP;
    }
    else{
        return std::numeric_limits<ip_t>::max();
    }
}

ip_t DSModelVebTreeImpl::InsertSubnetHost(MacID hostMacId, ip_t subnetIp) {
    return 0;
}

void DSModelVebTreeImpl::DeleteSubnet(ip_t start_ip) {
    if(start_ip < std::numeric_limits<ip_t>::max()){
        ip_t startIpInVEBTree = ConvertIpAddressFromIpRangeAddressSpaceToVebTreeAddressSpace(start_ip);
        DeleteSubnet(startIpInVEBTree);
    }
}

void DSModelVebTreeImpl::DeleteHostFromSubnet(ip_t host_ip) {

}

ip_t DSModelVebTreeImpl::GetNetWorkIP(ip_t hostIp) {
    ip_t hostIpInVEBTree = ConvertIpAddressFromIpRangeAddressSpaceToVebTreeAddressSpace(hostIp);
    auto [vebTreeNodeType, networkIPInVebTree, capacity] = vebTreeMap_.Predecessor(hostIpInVEBTree);
    if(vebTreeNodeType == VEBTreeNodeKeyType::NORMAL){
      ip_t networkIP = ConvertIpAddressFromVebTreeToIpRangeAddressSpace(networkIPInVebTree);
        return  networkIP;
    }
    else{
        return  std::numeric_limits<ip_t>::max();
    }

}

ip_t DSModelVebTreeImpl::GetIpAddress(MacID macId) {
    return 0;
}

MacID DSModelVebTreeImpl::GetMacAddressOfHost(ip_t hostIpAddress) {
    return MacID(0);
}

DSModelVebTreeImpl::~DSModelVebTreeImpl() {

}

ip_t DSModelVebTreeImpl::ConvertIpAddressFromIpRangeAddressSpaceToVebTreeAddressSpace(ip_t ipInIpRange) {
    return ipInIpRange - ipRange_.GetStartIP();
}

ip_t DSModelVebTreeImpl::ConvertIpAddressFromVebTreeToIpRangeAddressSpace(ip_t ipInVebTree) {
    return ipInVebTree + ipRange_.GetStartIP();
}
