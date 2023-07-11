//
// Created by Abhijith  K A on 11/07/23.
//

#include "TreeMapValueObjectForUnusedObjectInArray.h"

ip_t TreeMapValueObjectForUnusedObjectInArray::GetStartIP() const {
    return start_ip_;
}

TreeMapValueObjectForUnusedObjectInArray::TreeMapValueObjectForUnusedObjectInArray(ip_t start_ip, int capacity):start_ip_(start_ip),
capacity_(capacity){

}

int TreeMapValueObjectForUnusedObjectInArray::GetCapacity() const {
    return capacity_;
}
