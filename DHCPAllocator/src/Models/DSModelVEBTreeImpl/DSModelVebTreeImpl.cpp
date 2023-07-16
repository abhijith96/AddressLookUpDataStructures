//
// Created by Abhijith  K A on 13/07/23.
//

#include <DHCPAllocator/src/Models/DSModelVEBTreeImpl/DSModelVebTreeImpl.h>

DSModelVebTreeImpl::DSModelVebTreeImpl(IpRange ipRange) : ipRange_(ipRange), vebTreeMap_(ipRange_.GetCapacity()),
freeIpStartRange_(0){

}

std::pair<bool, ip_t>  DSModelVebTreeImpl::InsertSubnet(MacID subNetMacId, int capacity) {
    if(std::numeric_limits<ip_t>::max() - capacity < (freeIpStartRange_)){
        ip_t startIP = freeIpStartRange_;
        ip_t startIPInVebTree = ConvertIpAddressFromIpRangeAddressSpaceToVebTreeAddressSpace(startIP);
        vebTreeMap_.Insert(startIPInVebTree, VEBTreeValueObject{static_cast<uint32_t>(capacity)});
        ip_t endIP = startIP + (capacity - 1);
        freeIpStartRange_ = endIP + 1;
        return {true,startIP};
    }
    else{
        return {true,std::numeric_limits<ip_t>::max()};
    }
}

std::pair<bool, ip_t>  DSModelVebTreeImpl::InsertSubnetHost(MacID hostMacId, ip_t subnetIp) {
    return {false, 0};
}

void DSModelVebTreeImpl::DeleteSubnet(ip_t start_ip) {
    if(start_ip < std::numeric_limits<ip_t>::max()){
        ip_t startIpInVEBTree = ConvertIpAddressFromIpRangeAddressSpaceToVebTreeAddressSpace(start_ip);
        DeleteSubnet(startIpInVEBTree);
    }
}

void DSModelVebTreeImpl::DeleteHostFromSubnet(ip_t host_ip) {

}

std::pair<bool, ip_t>  DSModelVebTreeImpl::GetNetWorkIP(ip_t hostIp) {
    ip_t hostIpInVEBTree = ConvertIpAddressFromIpRangeAddressSpaceToVebTreeAddressSpace(hostIp);
    auto [vebTreeNodeType, networkIPInVebTree, capacity] = vebTreeMap_.Predecessor(hostIpInVEBTree);
    if(vebTreeNodeType == VEBTreeNodeKeyType::NORMAL){
      ip_t networkIP = ConvertIpAddressFromVebTreeToIpRangeAddressSpace(networkIPInVebTree);
        return  {true, networkIP};
    }
    else{
        return  {false, std::numeric_limits<ip_t>::max()};
    }

}

std::pair<bool, ip_t>  DSModelVebTreeImpl::GetIpAddress(MacID macId) {
    return {false, 0};
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

void DSModelVebTreeImpl::DeleteHostFromSubnet(ip_t host_ip, ip_t subnet_ip) {

}

std::pair<bool, ip_t> DSModelVebTreeImpl::GetHostIpAddress(MacID macId, ip_t subnet_ip) {
    return {false, 0};
}

std::pair<bool, MacID> DSModelVebTreeImpl::GetMacAddressOfHost(ip_t hostIpAddress, ip_t subnet_ip) {
    return {false, MacID{1}};
}
