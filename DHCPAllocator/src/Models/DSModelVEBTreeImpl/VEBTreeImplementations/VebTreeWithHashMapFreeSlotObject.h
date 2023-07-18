//
// Created by Abhijith  K A on 17/07/23.
//

#ifndef DHCPALLOCATOR_VEBTREEWITHHASHMAPFREESLOTOBJECT_H
#define DHCPALLOCATOR_VEBTREEWITHHASHMAPFREESLOTOBJECT_H

#include <DHCPAllocator/src/Models/IPAddress.h>
#include <vector>


class VebTreeWithHashMapFreeSlotObject {
private:
    ip_t capacity_;
    std::vector<ip_t> startIpList_;

public:
    VebTreeWithHashMapFreeSlotObject();
    explicit VebTreeWithHashMapFreeSlotObject(ip_t capacity);
    explicit VebTreeWithHashMapFreeSlotObject(ip_t capacity, ip_t startIP);
    void AddIp(ip_t startIp);
    [[nodiscard]] ip_t getCapacity() const;
    [[nodiscard]] const std::vector<ip_t>& GetIpListHavingCapacity() const;
    std::vector<ip_t>& GetIpListHavingCapacity();

};


#endif //DHCPALLOCATOR_VEBTREEWITHHASHMAPFREESLOTOBJECT_H
