//
// Created by Abhijith  K A on 11/07/23.
//

#include <DHCPAllocator/src/Models/DSModelSingleton.h>
#include <DHCPAllocator/src/Models/DSModelTreeImpl/DSModelTreeImpl.h>
#include <DHCPAllocator/src/Models/DSModelArrayImpl/DSModelArrayImpl.h>
#include <DHCPAllocator/src/Models/DSModelVEBTreeImpl/DSModelHashedVebTreeImpl.h>
#include <DHCPAllocator/src/Models/DSModelVEBTreeImpl/DSModelVebTreeImpl.h>
#include <DHCPAllocator/src/Models/IPAddress.h>
#include <DHCPAllocator/src/Models/IpRange.h>
#include <stdexcept>


DSModelSingleton* DSModelSingleton::dsModelSingleton_ = nullptr;

DSModelSingleton & DSModelSingleton::GetSingletonInstance(DSModelType modelType) {
    if(!dsModelSingleton_){
        dsModelSingleton_ = new DSModelSingleton(modelType);
    }
    return *dsModelSingleton_;
}

DSModelmpl & DSModelSingleton::GetDSModel() {
    return *dsModel_;
}

DSModelSingleton::DSModelSingleton(DSModelType dsModelType) {
    dsModelType_ = dsModelType;
    if(dsModelType == DSModelType::TREE)
        dsModel_ = new DSModelTreeImpl();
    else if(dsModelType == DSModelType::ARRAY)
        dsModel_ = new DSModelArrayImpl();
    else if(dsModelType == DSModelType::VEB_TREE){
        IpRange ipRange{IPAddress{0}, IPAddress{static_cast<ip_t >(std::pow(2,28)) -1}};
        dsModel_ = new DSModelVebTreeImpl(ipRange);
    }
    else if(dsModelType_ == DSModelType::HASHED_VEB_TREE){
        IpRange ipRange{IPAddress{0}, IPAddress{static_cast<ip_t >(std::pow(2,28)) -1}};
        dsModel_ = new DSModelHashedVebTreeImpl(ipRange);
    }
    else{
        throw std::invalid_argument("Not implemented");
    }

}

DSModelSingleton::~DSModelSingleton() {
    delete dsModel_;
}

DSModelType DSModelSingleton::GetDSModelType() {
    return dsModelType_;
}
