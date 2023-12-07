//
// Created by Abhijith  K A on 11/07/23.
//

#ifndef DHCPALLOCATOR_IPALLOCATOR_H
#define DHCPALLOCATOR_IPALLOCATOR_H

#include <IPAddressLookUpDataStructures/src/Models/Subnet.h>
#include <IPAddressLookUpDataStructures/src/Models/SubnetHosts.h>
#include <IPAddressLookUpDataStructures/src/Models/MacID.h>
#include <IPAddressLookUpDataStructures/src/Models/IPAddress.h>



class IpAllocator {
private:

public:
    /** TODO
     * Return stuff handle later
     * @param macId
     * @param capacity
     */
    void AllocateSubNet(MacID macId, int capacity);

    void AllocateHost(MacID hostMacId, IPAddress networkIp);








};


#endif //DHCPALLOCATOR_IPALLOCATOR_H
