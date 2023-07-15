//
// Created by Abhijith  K A on 14/07/23.
//

#ifndef DHCPALLOCATOR_VEBTREEWITHHASHMAPNODE_H
#define DHCPALLOCATOR_VEBTREEWITHHASHMAPNODE_H

#include <unordered_map>
#include <memory>
#include <DHCPAllocator/src/Models/DSModelVEBTreeImpl/VEBTreeNodeKeyType.h>

using veb_hm_t = uint32_t;


template <typename ValueType>
class VEBTreeWithHashMapNode {
private:
    veb_hm_t  universe_;
    bool is_set_;
    veb_hm_t min_;
    veb_hm_t max_;
    std::unique_ptr<ValueType> min_value_;
    std::unique_ptr<ValueType> max_value_;

    std::unique_ptr<VEBTreeWithHashMapNode> summary_;

    std::unordered_map<veb_hm_t, VEBTreeWithHashMapNode<ValueType>> cluster_map_;


public:
   explicit VEBTreeWithHashMapNode(veb_hm_t universe) :universe_(universe), is_set_(false),
   min_(), max_(){

   }
   [[nodiscard]] veb_hm_t GetUniverse() const{
       return universe_;
   }
   [[nodiscard]] veb_hm_t GetSubUniverse() const{
       return static_cast<veb_hm_t>(std::sqrt(universe_));
   }

   [[nodiscard]] bool IsSet() const{
       return is_set_;
   }

   void Unset(){
       is_set_ = false;
   }

   [[nodiscard]] bool IsClusterSet(veb_hm_t clusterIndex) const{
       return cluster_map_.find(clusterIndex) != cluster_map_.end();
   }

   void SetCluster(veb_hm_t clusterIndex){
       cluster_map_.emplace(clusterIndex,VEBTreeWithHashMapNode<ValueType>(GetSubUniverse()));
   }

   void UnsetCluster(veb_hm_t clusterIndex){
       cluster_map_.erase(clusterIndex);
   }

   void UnsetSummary(){
       summary_.reset();
   }

   VEBTreeWithHashMapNode<ValueType>& GetCluster(veb_hm_t clusterIndex){
       auto itr = cluster_map_.find(clusterIndex);
       assert(itr != cluster_map_.end());
       return itr->second;
   }

   VEBTreeWithHashMapNode<ValueType>& GetSummary(){
       assert(summary_);
       return *summary_;
   }

   void SetSummary(){
       summary_ = std::move(std::make_unique<VEBTreeWithHashMapNode<ValueType>>(GetSubUniverse()));
   }

   [[nodiscard]] bool IsSummarySet() const{
       return summary_.get() != nullptr;
   }

   void SetMinKey(veb_hm_t key) {
        min_ = key;
        is_set_ = true;
   }

   void SetMaxKey(veb_hm_t key){
       max_ = key;
       is_set_ = true;
   }

   void SetMinValue(ValueType valueType){
       min_value_ = std::move(std::make_unique<ValueType>(valueType));
   }

   void SetMaxValue(ValueType valueType){
       max_value_ = std::move(std::make_unique<ValueType>(valueType));
   }

   [[nodiscard]] veb_hm_t GetMinKey() const{
       return min_;
   }

   [[nodiscard]] veb_hm_t GetMaxKey() const{
       return max_;
   }

   [[nodiscard]] ValueType GetMinValue() const{
       return *min_value_;
   }

   [[nodiscard]] ValueType GetMaxValue() const{
       return *max_value_;
   }

};


#endif //DHCPALLOCATOR_VEBTREEWITHHASHMAPNODE_H
