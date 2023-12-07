//
// Created by Abhijith  K A on 11/07/23.
//

#include <IPAddressLookUpDataStructures/src/Models/DSModelTreeImpl/TreeMapValueObject.h>


TreeMapValueObject::TreeMapValueObject(ip_t capacity, MacID macId): capacity_(capacity),
                                                                    macId_(macId){

}

ip_t TreeMapValueObject::GetCapacity() const {
    return capacity_;
}


MacID TreeMapValueObject::GetMacAddress() const {
    return macId_;
}
