//
// Created by Abhijith  K A on 16/07/23.
//

#ifndef DHCPALLOCATOR_IPRANGE_H
#define DHCPALLOCATOR_IPRANGE_H

#include <DHCPAllocator/src/Models/IPAddress.h>

class IpRange {
private:
    IPAddress startIp_;
    IPAddress endIp_;
public:
    explicit IpRange(IPAddress startIp_, IPAddress endIp_);

    ip_t GetCapacity  () const;

    [[nodiscard]] ip_t GetStartIP() const;
};


#endif //DHCPALLOCATOR_IPRANGE_H
