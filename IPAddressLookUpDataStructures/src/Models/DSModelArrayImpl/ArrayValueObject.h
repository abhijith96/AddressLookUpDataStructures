//
// Created by Sreedevi R Warrier on 2023-07-13.
//

#ifndef DHCPALLOCATOR_ARRAYVALUEOBJECT_H
#define DHCPALLOCATOR_ARRAYVALUEOBJECT_H

#include <IPAddressLookUpDataStructures/src/Models/MacID.h>
#include <IPAddressLookUpDataStructures/src/Models/IPAddress.h>

class ArrayValueObject {

    private:
        int capacity_;
        MacID macId_;

public:
        ArrayValueObject(int capacity, MacID macId);
        [[nodiscard]] int GetCapacity() const;
        [[nodiscard]] MacID GetMacAddress() const;

};


#endif //DHCPALLOCATOR_ARRAYVALUEOBJECT_H
