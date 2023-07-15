//
// Created by Abhijith  K A on 13/07/23.
//

#include "DSModelVebTreeImpl.h"

DSModelVebTreeImpl::DSModelVebTreeImpl() {

}

ip_t DSModelVebTreeImpl::InsertSubnet(MacID subNetMacId, int capacity) {
    return 0;
}

ip_t DSModelVebTreeImpl::InsertSubnetHost(MacID hostMacId, ip_t subnetIp) {
    return 0;
}

void DSModelVebTreeImpl::DeleteSubnet(ip_t start_ip) {

}

void DSModelVebTreeImpl::DeleteHostFromSubnet(ip_t host_ip) {

}

ip_t DSModelVebTreeImpl::GetNetWorkIP(ip_t hostIp) {
    return 0;
}

ip_t DSModelVebTreeImpl::GetIpAddress(MacID macId) {
    return 0;
}

MacID DSModelVebTreeImpl::GetMacAddressOfHost(ip_t hostIpAddress) {
    return MacID(0);
}

DSModelVebTreeImpl::~DSModelVebTreeImpl() {

}
