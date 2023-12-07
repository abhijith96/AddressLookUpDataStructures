//
// Created by Sreedevi R Warrier on 2023-07-13.
//

#ifndef DHCPALLOCATOR_HOST_H
#define DHCPALLOCATOR_HOST_H

#include <IPAddressLookUpDataStructures/src/Models/IPAddress.h>
#include <IPAddressLookUpDataStructures/src/Models/MacID.h>


class Host {

private:
    ip_t host_ip_;
    MacID host_mac_id_;
    bool renewal_flag_;

public:
    explicit Host(ip_t host_ip, MacID macId, bool renew);

    [[nodiscard]] ip_t GetIP() const;
    [[nodiscard]] MacID GetMacID() const;
    [[nodiscard]] bool GetRenewalFlag() const;

    void setRenewalFlag(bool renewalFlag);


};

#endif //DHCPALLOCATOR_HOST_H
