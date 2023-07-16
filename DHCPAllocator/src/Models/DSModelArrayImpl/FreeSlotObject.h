//
// Created by Sreedevi R Warrier on 2023-07-13.
//

#ifndef DHCPALLOCATOR_FREESLOTOBJECT_H
#define DHCPALLOCATOR_FREESLOTOBJECT_H

#include <DHCPAllocator/src/Models/IPAddress.h>

class FreeSlotObject {
private:
    ip_t start_ip_;
    int capacity_;

public:
    FreeSlotObject(ip_t start_ip, int capacity);
    [[nodiscard]] ip_t GetStartIP() const;
    [[nodiscard]] int GetCapacity() const;

};


#endif //DHCPALLOCATOR_FREESLOTOBJECT_H
