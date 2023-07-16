//
// Created by Sreedevi R Warrier on 2023-07-13.
//

#include "ArrayValueObject.h"


ArrayValueObject::ArrayValueObject(int capacity, MacID macId): capacity_(capacity),
                                                                   macId_(macId) {

}

int ArrayValueObject::GetCapacity() const {
    return capacity_;
}

MacID ArrayValueObject::GetMacAddress() const {
    return macId_;
}
