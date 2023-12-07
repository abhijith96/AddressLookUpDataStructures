//
// Created by Abhijith  K A on 16/07/23.
//

#include <gtest/gtest.h>
#include <map>
#include <DHCPAllocator/src/Models/DSModelVEBTreeImpl/DSModelVebTreeImpl.h>
#include <cmath>
#include <vector>


using veb_value_t = VEBTreeValueObject;


class DSModelVebTreeImplTest : public ::testing::Test {
protected:
    void SetUp() override {

        DsImplVebTree = std::make_unique<DSModelVebTreeImpl>(ipRange);
    }

    std::vector<std::pair<ip_t,ip_t >> GenerateSeries(ip_t start, ip_t count, ip_t capacity, ip_t gap) {
        std::vector<std::pair<ip_t,ip_t >> result;
        result.resize(count);
        ip_t current_val = start;
        for(auto& i : result){
            i = {current_val, current_val + capacity - 1};
            current_val += capacity  + gap;

        }
        return result;
    }

    void InsertElements(std::vector<std::pair<ip_t,ip_t >>& subNetLists){
        for(auto & subNetList : subNetLists){
            ip_t  networkIp = subNetList.first;
            ip_t  broadcastIp = subNetList.second;
            ip_t  capacity = broadcastIp - networkIp + 1;
            DsImplVebTree->InsertSubnet(MacID{1}, capacity);
        }
    }

    void DeleteElements (std::vector<std::pair<ip_t,ip_t >>& subNetLists){
        for(auto & subNetList : subNetLists){
            DsImplVebTree->DeleteSubnet(subNetList.first);
        }
    }
    bool CompareElements(std::vector<std::pair<ip_t,ip_t >>& subNetList){
        for(auto & subNetRange : subNetList){
            ip_t  networkIp = subNetRange.first;
            ip_t  broadcastIp = subNetRange.second;
            ip_t  capacity = broadcastIp - networkIp + 1;
            for(ip_t i = networkIp; i <= broadcastIp; ++i){
                auto [status, networkIpFromDs] =DsImplVebTree->GetNetWorkIP(i);
                if(networkIpFromDs != networkIp){
                    return false;
                }
            }
        }
        return true;

    }

    bool CheckElementsAreNotPresent(std::vector<std::pair<ip_t,ip_t >>& subNetList) {
        for(auto & subNetRange : subNetList){
            ip_t  networkIp = subNetRange.first;
            ip_t  broadcastIp = subNetRange.second;
            ip_t  capacity = broadcastIp - networkIp + 1;
            for(ip_t i = networkIp; i <= broadcastIp; ++i){
                auto [status, networkIpFromDs] =DsImplVebTree->GetNetWorkIP(i);
                if(status){
                    return false;
                }
            }
        }
        return true;
    }

    void TearDown() override {
        DsImplVebTree.reset();
    }
    std::unique_ptr<DSModelVebTreeImpl> DsImplVebTree;
    IpRange ipRange {IPAddress{0}, IPAddress{1023}};
    

};

TEST_F(DSModelVebTreeImplTest, SimpleTest){

    {
        ip_t networkIp;
        bool isPresent;
        std::tie(isPresent, networkIp) = DsImplVebTree->GetNetWorkIP(std::numeric_limits<ip_t>::max() - 1);
        EXPECT_FALSE(isPresent);
    }





    {
        auto [isSubNetInserted, subNetIp ] = DsImplVebTree->InsertSubnet(MacID{0}, 10);
        ip_t networkIp;
        bool isPresent;
        std::tie(isPresent, networkIp) = DsImplVebTree->GetNetWorkIP(3);
        EXPECT_EQ(networkIp, 0);

    }

    {
        ip_t networkIp;
        bool isPresent;
        std::tie(isPresent, networkIp) = DsImplVebTree->GetNetWorkIP(9);
        EXPECT_EQ(networkIp, 0);
    }

    {
        ip_t networkIp;
        bool isPresent;
        std::tie(isPresent, networkIp) = DsImplVebTree->GetNetWorkIP(10);
        EXPECT_FALSE(isPresent);
    }

    auto [isSubNetInserted, subNetIp ] = DsImplVebTree->InsertSubnet(MacID{1}, 10);

    EXPECT_EQ(subNetIp, 10);

    {
        ip_t networkIp;
        bool isPresent;
        std::tie(isPresent, networkIp) = DsImplVebTree->GetNetWorkIP(10);
        EXPECT_EQ(networkIp, 10);
    }

    {
        ip_t networkIp;
        bool isPresent;
        std::tie(isPresent, networkIp) = DsImplVebTree->GetNetWorkIP(10);
        EXPECT_EQ(networkIp, 10);
    }

    DsImplVebTree->DeleteSubnet(subNetIp);

    {
        ip_t networkIp;
        bool isPresent;
        std::tie(isPresent, networkIp) = DsImplVebTree->GetNetWorkIP(11);
        EXPECT_FALSE(isPresent);
    }

}

TEST_F(DSModelVebTreeImplTest, TestTwentySubNetsOfSizeTen){

    ip_t  start = 0;
    ip_t capacity = 10;
    ip_t count = 20;
    ip_t  gap = 0;
    auto subNetList = GenerateSeries(start, count,capacity,gap);

    InsertElements(subNetList);

  bool status =  CompareElements(subNetList);
    EXPECT_TRUE(status);

    {
        ip_t  new_count = 10;
        auto subNetList_2 = GenerateSeries(start, new_count,capacity,gap);
        DeleteElements(subNetList_2);
        std::vector<std::pair<ip_t , ip_t >> subNetList3;
        subNetList3.insert(subNetList3.end(), subNetList.begin() + new_count, subNetList.end());
        bool status_2 = CompareElements(subNetList3);
        EXPECT_TRUE(status_2);
        bool status_3 = CheckElementsAreNotPresent(subNetList_2);
        EXPECT_TRUE(status_3);

    }

}


TEST_F(DSModelVebTreeImplTest, TestTwentySubNetsOfSizeTwenty){

    ip_t  start = 0;
    ip_t capacity = 20;
    ip_t count = 20;
    ip_t  gap = 0;
    auto subNetList = GenerateSeries(start, count,capacity,gap);

    InsertElements(subNetList);

    bool status =  CompareElements(subNetList);
    EXPECT_TRUE(status);

    {
        ip_t  new_count = 10;
        auto subNetList_2 = GenerateSeries(start, new_count,capacity,gap);
        DeleteElements(subNetList_2);
        std::vector<std::pair<ip_t , ip_t >> subNetList3;
        subNetList3.insert(subNetList3.end(), subNetList.begin() + new_count, subNetList.end());
        bool status_2 = CompareElements(subNetList3);
        EXPECT_TRUE(status_2);
        bool status_3 = CheckElementsAreNotPresent(subNetList_2);
        EXPECT_TRUE(status_3);

    }

}

TEST_F(DSModelVebTreeImplTest, TestTwentySubNetsOfSizeThrity){

    ip_t  start = 0;
    ip_t capacity = 100;
    ip_t count = 10;
    ip_t  gap = 0;
    auto subNetList = GenerateSeries(start, count,capacity,gap);

    InsertElements(subNetList);


    bool status =  CompareElements(subNetList);
    EXPECT_TRUE(status);

    {
        ip_t  new_count = 5;
        auto subNetList_2 = GenerateSeries(start, new_count,capacity,gap);
        DeleteElements(subNetList_2);
        std::vector<std::pair<ip_t , ip_t >> subNetList3;
        subNetList3.insert(subNetList3.end(), subNetList.begin() + new_count, subNetList.end());
        bool status_2 = CompareElements(subNetList3);
        EXPECT_TRUE(status_2);
        bool status_3 = CheckElementsAreNotPresent(subNetList_2);
        EXPECT_TRUE(status_3);

    }

}

TEST_F(DSModelVebTreeImplTest, WhenSubNetIsBiggerThanIpRange){

    ip_t  start = 0;
    ip_t capacity = 1000;
    ip_t count = 10;
    ip_t  gap = 0;
   auto [isInserted, startIP] =   DsImplVebTree->InsertSubnet(MacID{1}, capacity);
    EXPECT_EQ(startIP, 0);

    {

        ip_t newCapacity = 25;
        auto [isInserted_2, startIP_2] = DsImplVebTree->InsertSubnet(MacID{1}, newCapacity);
        EXPECT_FALSE(isInserted_2);
    }


}

TEST_F(DSModelVebTreeImplTest, RangeNotFromZero) {

IpRange ipRange1{IPAddress{100}, IPAddress{200}};
DsImplVebTree = std::make_unique<DSModelVebTreeImpl>(ipRange1);

    ip_t capacity = 10;
    auto [isInserted, startIP] = DsImplVebTree->InsertSubnet(MacID{1}, capacity);
    EXPECT_EQ(startIP, 100);

}



int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}