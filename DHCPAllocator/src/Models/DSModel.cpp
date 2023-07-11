//
// Created by Abhijith  K A on 11/07/23.
//

#include "DSModel.h"


DSModel::DSModel(DSModelTreeImpl dsModelmpl){
    dsModelmpl_ = std::make_unique<DSModelTreeImpl>(std::move(dsModelmpl));
}

DSModel::DSModel() {

}
