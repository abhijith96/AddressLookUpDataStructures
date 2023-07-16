//
// Created by Abhijith  K A on 11/07/23.
//

#ifndef DHCPALLOCATOR_DSMODELTREEIMPL_H
#define DHCPALLOCATOR_DSMODELTREEIMPL_H

#include "DHCPAllocator/src/Models/Subnet.h"
#include "DHCPAllocator/src/Models/SubnetHost.h"
#include "DHCPAllocator/src/Models/DSModelmpl.h"
#include "DHCPAllocator/src/Models/MacID.h"
#include "DHCPAllocator/src/Models/IPAddress.h"

#include <DHCPAllocator/src/Models/DSModelTreeImpl/TreeMapValueObject.h>
#include <DHCPAllocator/src/Models/DSModelTreeImpl/TreeMapValueObjectForUnusedObjectInArray.h>

#include <map>
#include <vector>


class DSModelTreeImpl : public DSModelmpl {
std::map<ip_t,TreeMapValueObject> treemap_;
std::vector<TreeMapValueObjectForUnusedObjectInArray> free_slots_list_;

public:

    DSModelTreeImpl() = default;
    /**
    * Pure virtual function hence zero
    * @param subnet
    * returns starting ip address of subnet inserted
    */
    std::pair<bool, ip_t> InsertSubnet(MacID subNetMacId, int capacity) override;


    /**
     * Give an ip address for a host in the given subnet
     * @param hostMacId
     * @param subnetIp
     * @return
     */

    std::pair<bool, ip_t> InsertSubnetHost(MacID hostMacId, ip_t subnetIp);


    /**
     * Delete a subnet with ip  starting at start_ip
     * @param start_ip
     */

    /**
     * Delete the subnet with ip starting at start_ip
     * @param start_ip
     */

     void DeleteSubnet(ip_t start_ip);


    /**
     * Deletes a particular host from a subnet
     * @param host_ip
     */

    void DeleteHostFromSubnet(ip_t host_ip, ip_t subnet_ip);

    /**
     * For Router to find network ip of a host
     * @param hostIp
     * @return the network ip address
     */
     ip_t  GetNetWorkIP(ip_t hostIp);


    /**
     * Get a hosts's ip address when host provides their mac address.
     * @param macId
     * @return
     */

    ip_t GetHostIpAddress(MacID macId, ip_t subnet_ip);


    /**
     * For network admin to find host mac id using a specific host ip address
     * @param hostIpAddress
     * @return
     */

    std::pair<bool, MacID> GetMacAddressOfHost(ip_t hostIpAddress, ip_t subnet_ip);

    std::optional<std::vector<TreeMapValueObjectForUnusedObjectInArray>::iterator> GetBestFitIp(int requiredCapacity);
    void UpdateFreeSlotsList(std::vector<TreeMapValueObjectForUnusedObjectInArray>::iterator iter, int requiredCapacity);

    [[maybe_unused]] void SetFreeSlots(ip_t startIp, int freeCapacity);

    std::vector<TreeMapValueObjectForUnusedObjectInArray> GetFreeSlotsList();
};


#endif //DHCPALLOCATOR_DSMODELTREEIMPL_H
