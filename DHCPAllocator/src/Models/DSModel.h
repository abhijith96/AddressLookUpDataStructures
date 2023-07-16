//
// Created by Abhijith  K A on 11/07/23.
//

#ifndef DHCPALLOCATOR_DSMODEL_H
#define DHCPALLOCATOR_DSMODEL_H

#include <variant>
#include <DHCPAllocator/src/Models/DSModelmpl.h>
#include "DHCPAllocator/src/Models/DSModelTreeImpl/DSModelTreeImpl.h"
#include  <DHCPAllocator/src/Models/SubnetHost.h>
#include  <DHCPAllocator/src/Models/Subnet.h>
#include <memory>


class DSModel {

private:
   std::unique_ptr<DSModelTreeImpl> dsModelmpl_;

public:

    DSModel();

    /**
     * Do this for all implementations
     * @param dsModelmpl
     */
    explicit DSModel(DSModelTreeImpl dsModelmpl) ;

    /**
     * Pure virtual function hence zero
     * @param subnet
     * returns starting ip address of subnet inserted
     */
    std::pair<bool, ip_t> InsertSubnet(MacID subNetMacId, int capacity){
        return dsModelmpl_->InsertSubnet(subNetMacId, capacity);
    }


    /**
     * Give an ip address for a host in the given subnet
     * @param hostMacId
     * @param subnetIp
     * @return
     */

    std::pair<bool, ip_t> InsertSubnetHost(MacID hostMacId, ip_t subnetIp){
        return dsModelmpl_->InsertSubnetHost(hostMacId, subnetIp);
    }


    /**
     * Delete a subnet with ip starting at start_ip
     * @param start_ip
     */



   void DeleteSubnet(ip_t start_ip){
        dsModelmpl_->DeleteSubnet(start_ip);
    }


    /**
     * Deletes a particular host from a subnet
     * @param host_ip
     */

    void DeleteHostFromSubnet(ip_t host_ip) {
        dsModelmpl_->DeleteHostFromSubnet(host_ip, 0);
    }

    /**
     * For Router to find network ip of a host
     * @param hostIp
     * @return the network ip address
     */
    std::pair<bool, ip_t>   GetNetWorkIP(ip_t hostIp){
        return dsModelmpl_->GetNetWorkIP(hostIp);
    }


    /**
     * Get a hosts's ip address when host provides their mac address.
     * @param macId
     * @return
     */

    std::pair<bool, ip_t>   GetIpAddress(MacID macId) {
       return dsModelmpl_->GetHostIpAddress(macId, 0);
    }


    /**
     * For network admin to find host mac id using a specific host ip address
     * @param hostIpAddress
     * @return
     */

    std::pair<bool, MacID> GetMacAddressOfHost(ip_t hostIpAddress) {
        return dsModelmpl_->GetMacAddressOfHost(hostIpAddress, 0);
    }


};


#endif //DHCPALLOCATOR_DSMODEL_H
