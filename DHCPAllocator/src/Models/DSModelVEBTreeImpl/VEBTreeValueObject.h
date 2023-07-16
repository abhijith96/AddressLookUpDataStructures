//
// Created by Abhijith  K A on 16/07/23.
//

#ifndef DHCPALLOCATOR_VEBTREEVALUEOBJECT_H
#define DHCPALLOCATOR_VEBTREEVALUEOBJECT_H

#include <DHCPAllocator/src/Models/DSModelVEBTreeImpl/VEBTreeValueObject.h>
#include <DHCPAllocator/src/Models/IPAddress.h>

class VEBTreeValueObject {
private:
    ip_t capacity_;
public:
    explicit VEBTreeValueObject(ip_t capacity);
    VEBTreeValueObject();
    VEBTreeValueObject(const VEBTreeValueObject& other);
    VEBTreeValueObject & operator = (const VEBTreeValueObject& other);
};


#endif //DHCPALLOCATOR_VEBTREEVALUEOBJECT_H
