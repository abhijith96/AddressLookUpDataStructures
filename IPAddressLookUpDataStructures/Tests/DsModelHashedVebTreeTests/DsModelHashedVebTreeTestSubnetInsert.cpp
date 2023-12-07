//
// Created by Abhijith  K A on 16/07/23.
//

#include <gtest/gtest.h>
#include <map>
#include <IPAddressLookUpDataStructures/src/Models/DSModelVEBTreeImpl/DSModelHashedVebTreeImpl.h>
#include <cmath>
#include <vector>


using veb_value_t = VEBTreeValueObject;


class DSModelHashedVebTreeImplTest : public ::testing::Test {
protected:
    void SetUp() override {

        DsImplHashedVebTree = std::make_unique<DSModelHashedVebTreeImpl>(ipRange);
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
            DsImplHashedVebTree->InsertSubnet(MacID{1}, capacity);
        }
    }

    void DeleteElements (std::vector<std::pair<ip_t,ip_t >>& subNetLists){
        for(auto & subNetList : subNetLists){
            DsImplHashedVebTree->DeleteSubnet(subNetList.first);
        }
    }
    bool CompareElements(std::vector<std::pair<ip_t,ip_t >>& subNetList){
        for(auto & subNetRange : subNetList){
            ip_t  networkIp = subNetRange.first;
            ip_t  broadcastIp = subNetRange.second;
            ip_t  capacity = broadcastIp - networkIp + 1;
            for(ip_t i = networkIp; i <= broadcastIp; ++i){
                auto [status, networkIpFromDs] =DsImplHashedVebTree->GetNetWorkIP(i);
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
                auto [status, networkIpFromDs] =DsImplHashedVebTree->GetNetWorkIP(i);
                if(status){
                    return false;
                }
            }
        }
        return true;
    }

    void TearDown() override {
        DsImplHashedVebTree.reset();
    }
    std::unique_ptr<DSModelHashedVebTreeImpl> DsImplHashedVebTree;
    IpRange ipRange {IPAddress{0}, IPAddress{1023}};


};

TEST_F(DSModelHashedVebTreeImplTest, SimpleTest){

    {
        ip_t networkIp;
        bool isPresent;
        std::tie(isPresent, networkIp) = DsImplHashedVebTree->GetNetWorkIP(std::numeric_limits<ip_t>::max() - 1);
        EXPECT_FALSE(isPresent);
    }





    {
        auto [isSubNetInserted, subNetIp ] = DsImplHashedVebTree->InsertSubnet(MacID{0}, 10);
        ip_t networkIp;
        bool isPresent;
        std::tie(isPresent, networkIp) = DsImplHashedVebTree->GetNetWorkIP(3);
        EXPECT_EQ(networkIp, 0);

    }

    {
        ip_t networkIp;
        bool isPresent;
        std::tie(isPresent, networkIp) = DsImplHashedVebTree->GetNetWorkIP(9);
        EXPECT_EQ(networkIp, 0);
    }

    {
        ip_t networkIp;
        bool isPresent;
        std::tie(isPresent, networkIp) = DsImplHashedVebTree->GetNetWorkIP(10);
        EXPECT_FALSE(isPresent);
    }

    auto [isSubNetInserted, subNetIp ] = DsImplHashedVebTree->InsertSubnet(MacID{1}, 10);

    EXPECT_EQ(subNetIp, 10);

    {
        ip_t networkIp;
        bool isPresent;
        std::tie(isPresent, networkIp) = DsImplHashedVebTree->GetNetWorkIP(10);
        EXPECT_EQ(networkIp, 10);
    }

    {
        ip_t networkIp;
        bool isPresent;
        std::tie(isPresent, networkIp) = DsImplHashedVebTree->GetNetWorkIP(10);
        EXPECT_EQ(networkIp, 10);
    }

    DsImplHashedVebTree->DeleteSubnet(subNetIp);

    {
        ip_t networkIp;
        bool isPresent;
        std::tie(isPresent, networkIp) = DsImplHashedVebTree->GetNetWorkIP(11);
        EXPECT_FALSE(isPresent);
    }

}

TEST_F(DSModelHashedVebTreeImplTest, TestTwentySubNetsOfSizeTen){

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


TEST_F(DSModelHashedVebTreeImplTest, TestTwentySubNetsOfSizeTwenty){

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

TEST_F(DSModelHashedVebTreeImplTest, TestTwentySubNetsOfSizeThrity){

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

TEST_F(DSModelHashedVebTreeImplTest, WhenSubNetIsBiggerThanIpRange){

    ip_t  start = 0;
    ip_t capacity = 1000;
    ip_t count = 10;
    ip_t  gap = 0;
    auto [isInserted, startIP] =   DsImplHashedVebTree->InsertSubnet(MacID{1}, capacity);
    EXPECT_EQ(startIP, 0);

    {

        ip_t newCapacity = 25;
        auto [isInserted_2, startIP_2] = DsImplHashedVebTree->InsertSubnet(MacID{1}, newCapacity);
        EXPECT_FALSE(isInserted_2);
    }


}

TEST_F(DSModelHashedVebTreeImplTest, RangeNotFromZero) {

    IpRange ipRange1{IPAddress{100}, IPAddress{200}};
    DsImplHashedVebTree = std::make_unique<DSModelHashedVebTreeImpl>(ipRange1);

    ip_t capacity = 10;
    auto [isInserted, startIP] = DsImplHashedVebTree->InsertSubnet(MacID{1}, capacity);
    EXPECT_EQ(startIP, 100);

}

TEST_F(DSModelHashedVebTreeImplTest, BigRange) {

    IpRange ipRange1{IPAddress{0}, IPAddress{std::numeric_limits<ip_t>::max() - 1}};
    DsImplHashedVebTree = std::make_unique<DSModelHashedVebTreeImpl>(ipRange1);

    {
        ip_t capacity = std::numeric_limits<ip_t>::max() - 1;
        auto [isInserted, startIP] = DsImplHashedVebTree->InsertSubnet(MacID{1}, capacity);
        EXPECT_EQ(startIP, 0);

    }

    {
        ip_t capacity =  3;
        auto [isInserted, startIP] = DsImplHashedVebTree->InsertSubnet(MacID{2}, capacity);
        EXPECT_FALSE(isInserted);
    }


}

TEST_F(DSModelHashedVebTreeImplTest, BigRange2) {

    IpRange ipRange1{IPAddress{0}, IPAddress{std::numeric_limits<ip_t>::max()}};
    DsImplHashedVebTree = std::make_unique<DSModelHashedVebTreeImpl>(ipRange1);

    {
        ip_t capacity = std::numeric_limits<ip_t>::max();
        EXPECT_ANY_THROW(
       DsImplHashedVebTree->InsertSubnet(MacID{1}, capacity));

    }


}


TEST_F(DSModelHashedVebTreeImplTest, BigRangeAndLittleRange) {

    IpRange ipRange1{IPAddress{0}, IPAddress{std::numeric_limits<ip_t>::max() - 1}};
    DsImplHashedVebTree = std::move(std::make_unique<DSModelHashedVebTreeImpl>(ipRange1));

    std::cout<<ipRange1.GetCapacity()<<"\n";

    {
        ip_t capacity = std::numeric_limits<ip_t>::max() - 11;
        auto [isInserted, startIP] = DsImplHashedVebTree->InsertSubnet(MacID{1}, capacity);
        EXPECT_EQ(startIP, 0);

    }

    {
        ip_t expectedStartIp = std::numeric_limits<ip_t>::max() - 11;
        ip_t capacity =  10;
        auto [isInserted, startIP] = DsImplHashedVebTree->InsertSubnet(MacID{2}, capacity);
        EXPECT_TRUE(isInserted);
        EXPECT_EQ(expectedStartIp, startIP);
    }

    {
        ip_t capacity =  10;
        auto [isInserted, startIP] = DsImplHashedVebTree->InsertSubnet(MacID{2}, capacity);
        EXPECT_FALSE(isInserted);
    }

    {
        ip_t expectedStartIp = std::numeric_limits<ip_t>::max() - 11;
        DsImplHashedVebTree->DeleteSubnet(expectedStartIp);
    }

    {
        ip_t expectedStartIp = std::numeric_limits<ip_t>::max() - 11;
        ip_t capacity =  5;
        auto [isInserted, startIP] = DsImplHashedVebTree->InsertSubnet(MacID{2}, capacity);
        EXPECT_TRUE(isInserted);
        EXPECT_EQ(expectedStartIp, startIP);
    }

    {
        ip_t expectedStartIp = std::numeric_limits<ip_t>::max() - 6;
        ip_t capacity =  5;
        auto [isInserted, startIP] = DsImplHashedVebTree->InsertSubnet(MacID{2}, capacity);
        EXPECT_TRUE(isInserted);
        EXPECT_EQ(expectedStartIp, startIP);
    }



}



int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}