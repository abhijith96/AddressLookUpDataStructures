//
// Created by Sreedevi R Warrier on 2023-07-13.
//

#include "FreeSlotObject.h"

FreeSlotObject::FreeSlotObject(ip_t start_ip, int capacity) : start_ip_(start_ip), capacity_(capacity) {

}

ip_t FreeSlotObject::GetStartIP() const {
    return start_ip_;
}

int FreeSlotObject::GetCapacity() const {
    return capacity_;
}
