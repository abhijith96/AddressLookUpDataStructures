//
// Created by Abhijith  K A on 11/07/23.
//

#include "TreeMapValueObject.h"


TreeMapValueObject::TreeMapValueObject(int capacity, MacID macId): capacity_(capacity),
                                                                   macId_(macId){

}

int TreeMapValueObject::GetCapacity() const {
    return capacity_;
}


MacID TreeMapValueObject::GetMacAddress() const {
    return macId_;
}
