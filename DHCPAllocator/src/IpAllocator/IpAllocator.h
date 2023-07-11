//
// Created by Abhijith  K A on 11/07/23.
//

#ifndef DHCPALLOCATOR_IPALLOCATOR_H
#define DHCPALLOCATOR_IPALLOCATOR_H

#include <DHCPAllocator/src/Models/Subnet.h>
#include <DHCPAllocator/src/Models/SubnetHost.h>
#include <DHCPAllocator/src/Models/MacID.h>
#include <DHCPAllocator/src/Models/IPAddress.h>



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
