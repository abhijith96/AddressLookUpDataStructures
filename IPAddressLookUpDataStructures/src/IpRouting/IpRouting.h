//
// Created by Abhijith  K A on 11/07/23.
//

#ifndef DHCPALLOCATOR_IPROUTING_H
#define DHCPALLOCATOR_IPROUTING_H

#include <IPAddressLookUpDataStructures/src/Models/Subnet.h>
#include <IPAddressLookUpDataStructures/src/Models/SubnetHosts.h>
#include <IPAddressLookUpDataStructures/src/Models/MacID.h>
#include <IPAddressLookUpDataStructures/src/Models/IPAddress.h>
#include <optional>

class IpRouting {

private:

public:
     explicit IpRouting();
     std::optional<IPAddress> FindNetworkIp(IPAddress hostIpAddress);






};


#endif //DHCPALLOCATOR_IPROUTING_H
