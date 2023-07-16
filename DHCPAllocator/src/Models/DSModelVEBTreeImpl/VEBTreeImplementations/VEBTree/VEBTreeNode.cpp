//
// Created by Abhijith  K A on 14/07/23.
//


#include "VEBTreeNode.h"
#include <DHCPAllocator/src/Models/DSModelVEBTreeImpl/VEBTreeImplementations/VEBTreeUtil.h>


template <typename ValueType>
VEBTreeNode<ValueType>::VEBTreeNode(veb_key_t universe) : universe_(universe), min_(0), max_(0), is_set_(false){
    auto [clusterCount, itemsCount] = VEBTreeUtils::SplitIntoSquareOfPowersOfTwo(universe);
    if(itemsCount >= 2 && clusterCount >= 2){
        summary_ = std::move(std::make_unique<VEBTreeNode>(clusterCount));
        sub_clusters_.reserve(clusterCount);
        for(veb_key_t i = 0; i < clusterCount;++i){
            sub_clusters_.emplace_back(std::make_unique<VEBTreeNode>(itemsCount));
        }
    }
}

template <typename ValueType>
void VEBTreeNode<ValueType>::SetMinKey(veb_key_t min) {
    min_ =  min;
    is_set_ = true;
}

template <typename ValueType>
void VEBTreeNode<ValueType>::SetMaxKey(veb_key_t max) {
    max_ = max;
    is_set_ = true;
}

template <typename ValueType>
void VEBTreeNode<ValueType>::UnSetMin() {
    min_ = 0;
    if(min_value_) min_value_.reset();
}

template <typename ValueType>
void VEBTreeNode<ValueType>::UnSetMax() {
    max_ = 0;
    if(max_value_) max_value_.reset();
}


template <typename ValueType>
veb_key_t VEBTreeNode<ValueType>::GetMinKey() const {
    return min_;
}

template <typename ValueType>
veb_key_t VEBTreeNode<ValueType>::GetMaxKey() const {
    return max_;
}


template <typename ValueType>
bool VEBTreeNode<ValueType>::IsSet() const {
    return is_set_;
}

template <typename ValueType>
void VEBTreeNode<ValueType>::SetMinValue(ValueType value) {
    min_value_ = std::move(std::make_unique<ValueType>(value));
}

template <typename ValueType>
void VEBTreeNode<ValueType>::SetMaxValue(ValueType value) {
    max_value_ = std::move(std::make_unique<ValueType>(value));
}

template <typename ValueType>
const ValueType &VEBTreeNode<ValueType>::GetMinValue() {
    if(min_value_.get()) return *min_value_;
    throw std::invalid_argument("Min Key is Not Set");
}

template <typename ValueType>
const ValueType &VEBTreeNode<ValueType>::GetMaxValue() {
    if(max_value_.get()) return * max_value_;
    throw std::invalid_argument("Max Key is Not Set");
}

template <typename ValueType>
void VEBTreeNode<ValueType>::UnSet() {
    is_set_ = false;
    if(min_value_.get()) min_value_.reset();
    if(max_value_.get()) max_value_.reset();
}

template <typename ValueType>
veb_key_t VEBTreeNode<ValueType>::GetUniverseSize() const {
    return universe_;
}

template <typename ValueType>
VEBTreeNode<ValueType> & VEBTreeNode<ValueType>::GetCluster(veb_key_t clusterIndex) {
    return *sub_clusters_.at(clusterIndex);
}
template <typename ValueType>
VEBTreeNode<ValueType> &VEBTreeNode<ValueType>::GetSummary() {
    return *summary_;
}

template class VEBTreeNode<uint32_t>;

