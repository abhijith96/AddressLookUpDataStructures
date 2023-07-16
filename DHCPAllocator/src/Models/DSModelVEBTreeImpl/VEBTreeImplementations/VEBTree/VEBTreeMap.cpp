//
// Created by Abhijith  K A on 14/07/23.
//

#include "VEBTreeMap.h"
#include <DHCPAllocator/src/Models/DSModelVEBTreeImpl/VEBTreeImplementations/VEBTreeUtil.h>

template <typename ValueType>
VEBTreeMap<ValueType>::VEBTreeMap(veb_key_t universe): root_node_(std::make_unique<VEBTreeNode<ValueType>>(
        std::bit_ceil(universe))) {

}

template <typename ValueType>
std::pair<bool, ValueType> VEBTreeMap<ValueType>::FindKey(veb_key_t key) {
    if(key > root_node_->GetUniverseSize()) return {false, ValueType{}};
    VEBTreeNode<ValueType>*  currentNode = root_node_.get();
    veb_key_t currentKey = key;
    while(true){
        if(currentNode->IsSet()){
            if(currentNode->GetMinKey() == currentKey){
                return {true, currentNode->GetMinValue()};
            }
            if(currentNode->GetMaxKey() == currentKey){
                return {true, currentNode->GetMaxValue()};
            }
            if(currentKey < currentNode->GetMinKey() || currentKey > currentNode->GetMaxKey()){
                return {false, 0};
            }
            if(currentNode->GetUniverseSize() > 2){
                veb_key_t clusterIndex = High(currentKey, currentNode->GetUniverseSize());
                veb_key_t nodeIndex = Low(currentKey, currentNode->GetUniverseSize());
                currentNode =  &currentNode->GetCluster(clusterIndex);
                currentKey = nodeIndex;
            }
            else{
                return {false, 0};
            }
        }
        else {
            return {false, 0};
        }
    }

}

template <typename ValueType>
veb_key_t VEBTreeMap<ValueType>::High(veb_key_t key, veb_key_t universe_size){
    auto [high, low] = VEBTreeUtils::SplitIntoHighAndLow(key, universe_size);
    return high;
//    return key/ static_cast<veb_key_t>( std::sqrt(universe_size));
}

template <typename ValueType>
veb_key_t VEBTreeMap<ValueType>::Low(veb_key_t key, veb_key_t universe_size) {
    auto [high, low] = VEBTreeUtils::SplitIntoHighAndLow(key, universe_size);
    return low;
    //return key %  static_cast<veb_key_t>(std::sqrt(universe_size));
}

template <typename ValueType>
veb_key_t VEBTreeMap<ValueType>::Index(veb_key_t high, veb_key_t low, veb_key_t universe_size) const {
    veb_key_t itemsCount = GetItemsCount(universe_size);
    return high* itemsCount + low;
}

template <typename ValueType>
veb_key_t VEBTreeMap<ValueType>::GetClusterCount(veb_key_t universe) const{
    auto [clusterCount, itemsCount] = VEBTreeUtils::SplitIntoSquareOfPowersOfTwo(universe);
    return clusterCount;
}
template <typename ValueType>
veb_key_t VEBTreeMap<ValueType>::GetItemsCount(veb_key_t universe) const{
    auto [clusterCount, itemsCount] = VEBTreeUtils::SplitIntoSquareOfPowersOfTwo(universe);
    return itemsCount;
}

template <typename ValueType>
void VEBTreeMap<ValueType>::InsertHelper(VEBTreeNode<ValueType>& currentNode, veb_key_t key, ValueType value) {
    if(key > currentNode.GetUniverseSize()) return;

    if(!currentNode.IsSet()){
        currentNode.SetMaxKey(key);
        currentNode.SetMinKey(key);
        currentNode.SetMinValue(value);
        currentNode.SetMaxValue(value);
        return;
    }
    else if(currentNode.GetMinKey() == currentNode.GetMaxKey()){
        if(key == currentNode.GetMinKey()) return;

        if(key > currentNode.GetMinKey()){
            currentNode.SetMaxKey(key);
            currentNode.SetMaxValue(value);
        }
        else{
            currentNode.SetMinKey(key);
            currentNode.SetMinValue(value);
        }
        return;
    }
    if(key < currentNode.GetMinKey()){
        veb_key_t  temp_key = currentNode.GetMinKey();
        ValueType temp_value = currentNode.GetMinValue();
        currentNode.SetMinKey(key);
        currentNode.SetMinValue(value);
        key = temp_key;
        value = temp_value;
    }
    else if(key > currentNode.GetMaxKey()){
        veb_key_t  temp_key = currentNode.GetMaxKey();
        ValueType temp_value = currentNode.GetMaxValue();
        currentNode.SetMaxKey(key);
        currentNode.SetMaxValue(value);
        key = temp_key;
        value = temp_value;
    }
    else if(key == currentNode.GetMinKey() || key == currentNode.GetMaxKey()){
        return;
    }

    veb_key_t clusterIndex = High(key, currentNode.GetUniverseSize());
    veb_key_t lowIndex = Low(key, currentNode.GetUniverseSize());

    if(!currentNode.GetCluster(clusterIndex).IsSet()){
        InsertHelper(currentNode.GetSummary(), clusterIndex, value);
    }
    InsertHelper(currentNode.GetCluster(clusterIndex), lowIndex, value);

}

template <typename ValueType>
void VEBTreeMap<ValueType>::Insert(veb_key_t key, ValueType value) {
    InsertHelper(*root_node_, key, value);
}

template <typename ValueType>
void VEBTreeMap<ValueType>::Delete(veb_key_t key) {
    DeleteHelper(*root_node_, key);
}

template <typename ValueType>
void VEBTreeMap<ValueType>::DeleteHelper(VEBTreeNode<ValueType> &currentNode, veb_key_t key) {

    if(!currentNode.IsSet()) return;
    if( key < currentNode.GetMinKey() || key > currentNode.GetMaxKey()){
        return;
    }
    if(currentNode.GetUniverseSize() == 2){
        if(currentNode.GetMinKey() == currentNode.GetMaxKey() && currentNode.GetMaxKey() == key){
            currentNode.UnSetMin();
            currentNode.UnSetMax();
            currentNode.UnSet();
        }
        else if(currentNode.GetMinKey() == key){
            currentNode.SetMinKey(currentNode.GetMaxKey());
            currentNode.SetMinValue(currentNode.GetMaxValue());
        }
        else if(currentNode.GetMaxKey() == key){
            currentNode.SetMaxKey(currentNode.GetMinKey());
            currentNode.SetMaxValue(currentNode.GetMinValue());
        }
        return;
    }
    if(currentNode.GetMinKey() == currentNode.GetMaxKey()){
        if(currentNode.GetMinKey() == key){
            currentNode.UnSetMin();
            currentNode.UnSetMax();
            currentNode.UnSet();
        }
        return;
    }
    if(!currentNode.GetSummary().IsSet()){
        if(currentNode.GetMinKey() == key){
            currentNode.SetMinKey(currentNode.GetMaxKey());
            currentNode.SetMinValue(currentNode.GetMaxValue());
        }
        else if(currentNode.GetMaxKey() == key){
            currentNode.SetMaxKey(currentNode.GetMinKey());
            currentNode.SetMaxValue(currentNode.GetMinValue());
        }
        return;
    }
    if(currentNode.GetMinKey() == key){

        veb_key_t minClusterIndex = currentNode.GetSummary().GetMinKey();
        veb_key_t minLowIndex = currentNode.GetCluster(minClusterIndex).GetMinKey();

        veb_key_t indexInCurrentUniverse = Index(minClusterIndex, minLowIndex, currentNode.GetUniverseSize());
        currentNode.SetMinKey(indexInCurrentUniverse);
        currentNode.SetMinValue(currentNode.GetCluster(minClusterIndex).GetMinValue());

        key = indexInCurrentUniverse;

    }
    else if(currentNode.GetMaxKey() == key){
        veb_key_t maxClusterIndex = currentNode.GetSummary().GetMaxKey();
        veb_key_t maxLowIndex = currentNode.GetCluster(maxClusterIndex).GetMaxKey();

        veb_key_t indexInCurrentUniverse = Index(maxClusterIndex, maxLowIndex, currentNode.GetUniverseSize());
        currentNode.SetMaxKey(indexInCurrentUniverse);
        currentNode.SetMaxValue(currentNode.GetCluster(maxClusterIndex).GetMaxValue());

        key = indexInCurrentUniverse;
    }

    veb_key_t clusterIndex = High(key, currentNode.GetUniverseSize());
    veb_key_t lowIndex = Low(key, currentNode.GetUniverseSize());

    DeleteHelper(currentNode.GetCluster(clusterIndex), lowIndex);
    if(!currentNode.GetCluster(clusterIndex).IsSet()){
        DeleteHelper(currentNode.GetSummary(), clusterIndex);
    }

}

template <typename ValueType>
std::tuple<VEBTreeNodeKeyType, veb_key_t, ValueType> VEBTreeMap<ValueType>::Successor(veb_key_t key) {
   return SuccessorHelper(*root_node_, key);
}

template <typename ValueType>
std::tuple<VEBTreeNodeKeyType, veb_key_t, ValueType> VEBTreeMap<ValueType>::SuccessorHelper(VEBTreeNode<ValueType>& currentNode, veb_key_t key) {

    if (!currentNode.IsSet()) {
        return {VEBTreeNodeKeyType::POSITIVE_INFINITY, {}, {}};
    }

    if (key < currentNode.GetMinKey()) {
        return {VEBTreeNodeKeyType::NORMAL, currentNode.GetMinKey(), currentNode.GetMinValue()};
    } else if (key > currentNode.GetMaxKey()) {
        return {VEBTreeNodeKeyType::POSITIVE_INFINITY, veb_key_t{}, ValueType{}};
    }


    if (key == currentNode.GetMinKey()) {
        return {VEBTreeNodeKeyType::NORMAL, currentNode.GetMinKey(), currentNode.GetMinValue()};
    } else if (key == currentNode.GetMaxKey()) {
        return {VEBTreeNodeKeyType::NORMAL, currentNode.GetMaxKey(), currentNode.GetMaxValue()};
    }
    veb_key_t clusterIndex = High(key, currentNode.GetUniverseSize());

    veb_key_t lowIndex = Low(key, currentNode.GetUniverseSize());
    if (currentNode.GetCluster(clusterIndex).IsSet() && currentNode.GetCluster(clusterIndex).GetMaxKey() >= lowIndex) {
        auto [node_type, clusterKey, value] = SuccessorHelper(currentNode.GetCluster(clusterIndex), lowIndex);
        if (node_type == VEBTreeNodeKeyType::NORMAL) {
            veb_key_t IndexInCurrentUniverse = Index(clusterIndex, clusterKey, currentNode.GetUniverseSize());
            return {node_type, IndexInCurrentUniverse, value};
        } else {
            throw std::invalid_argument("Error in successor call successor not found eventhough subcluster is set");
        }
    }
    else if(clusterIndex >= GetClusterCount(currentNode.GetUniverseSize())){
        return {VEBTreeNodeKeyType::NORMAL, currentNode.GetMaxKey(), currentNode.GetMaxValue()};
    }

    else {
        auto [node_type, nextClusterIndex, value] = SuccessorHelper(currentNode.GetSummary(), clusterIndex + 1);
        if (node_type == VEBTreeNodeKeyType::NORMAL) {
            assert(currentNode.GetCluster(nextClusterIndex).IsSet());
            veb_key_t minKeyInNextCluster = currentNode.GetCluster(nextClusterIndex).GetMinKey();
            veb_key_t IndexInCurrentUniverse = Index(nextClusterIndex, minKeyInNextCluster,
                                                     currentNode.GetUniverseSize());
            return {node_type, IndexInCurrentUniverse, currentNode.GetCluster(nextClusterIndex).GetMinValue()};
        } else {
            return {VEBTreeNodeKeyType::NORMAL, currentNode.GetMaxKey(), currentNode.GetMaxValue()};
        }
    }

}

template <typename ValueType>
std::tuple<VEBTreeNodeKeyType, veb_key_t, ValueType> VEBTreeMap<ValueType>::Predecessor(veb_key_t key) {
    return PredecessorHelper(*root_node_, key);
}

template <typename ValueType>
std::tuple<VEBTreeNodeKeyType, veb_key_t, ValueType>
VEBTreeMap<ValueType>::PredecessorHelper(VEBTreeNode<ValueType> &currentNode, veb_key_t key) {
    if(!currentNode.IsSet()){
        return {VEBTreeNodeKeyType::NEGATIVE_INFINITY, {}, {}};
    }
    if(key > currentNode.GetMaxKey()){
        return {VEBTreeNodeKeyType::NORMAL, currentNode.GetMaxKey(), currentNode.GetMaxValue()};
    }
    if(key < currentNode.GetMinKey()){
        return {VEBTreeNodeKeyType::NEGATIVE_INFINITY, {}, {}};
    }

    if(key == currentNode.GetMinKey()){
        return {VEBTreeNodeKeyType::NORMAL, currentNode.GetMinKey(), currentNode.GetMinValue()};
    }

    if(key == currentNode.GetMaxKey()){

        return {VEBTreeNodeKeyType::NORMAL, currentNode.GetMaxKey(), currentNode.GetMaxValue()};
    }

    veb_key_t clusterIndex = High(key, currentNode.GetUniverseSize());

    veb_key_t lowIndex = Low(key, currentNode.GetUniverseSize());

    if(currentNode.GetCluster(clusterIndex).IsSet() && currentNode.GetCluster(clusterIndex).GetMinKey() <= lowIndex){
        auto [nodeType, subUniverseKey, value] = PredecessorHelper(currentNode.GetCluster(clusterIndex), lowIndex);
        assert(nodeType == VEBTreeNodeKeyType::NORMAL);
        veb_key_t indexValueInCurrentUniverse = Index(clusterIndex, subUniverseKey, currentNode.GetUniverseSize());
        return {nodeType, indexValueInCurrentUniverse, value};
    }
    else if(clusterIndex == 0){
        return {VEBTreeNodeKeyType::NORMAL, currentNode.GetMinKey(), currentNode.GetMinValue()};
    }
    else{
        auto [nodeType, previousCluster, value] = PredecessorHelper(currentNode.GetSummary(), clusterIndex - 1);
        if(nodeType == VEBTreeNodeKeyType::NORMAL){
            veb_key_t maxKeyInPreviousCluster = currentNode.GetCluster(previousCluster).GetMaxKey();
            veb_key_t indexInCurrentUniverse = Index(previousCluster, maxKeyInPreviousCluster, currentNode.GetUniverseSize());
            return {nodeType, indexInCurrentUniverse, currentNode.GetCluster(previousCluster).GetMaxValue()};
        }
        else{
            return {VEBTreeNodeKeyType::NORMAL, currentNode.GetMinKey(), currentNode.GetMinValue()};
        }
    }
}

template class VEBTreeMap<uint32_t>;
