//
// Created by Sreedevi R Warrier on 2023-07-13.
//

#include "Host.h"

 ip_t Host::GetIP() const {
    return host_ip_;
}

 MacID Host::GetMacID() const {
    return host_mac_id_;
}

 bool Host::GetRenewalFlag() const {
    return renewal_flag_;
}

void Host::setRenewalFlag(bool renewalFlag) {
    renewal_flag_ = renewalFlag;
}


Host::Host(ip_t hostIP, MacID macID, bool renew) : host_ip_(hostIP), host_mac_id_(macID), renewal_flag_(renew) {

}