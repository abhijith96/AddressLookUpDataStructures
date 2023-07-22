//
// Created by Abhijith  K A on 18/07/23.
//



#include <gtest/gtest.h>
#include <map>
#include <DHCPAllocator/src/Models/DSModelVEBTreeImpl/DSModelHashedVebTreeImpl.h>





class DSModelHashedVebTreeImplTestSubnetHosts : public ::testing::Test {
protected:
    void SetUp() override {

//        vebTreeMap = std::make_unique<DSModelVebTreeImpl>(ipRange);
    }


    void TearDown() override {
        hashedVebTreeMap_.reset();
    }
    std::unique_ptr<DSModelHashedVebTreeImpl> hashedVebTreeMap_;
//    IpRange ipRange {IPAddress{0}, IPAddress{1023}};


};

TEST_F( DSModelHashedVebTreeImplTestSubnetHosts, SimpleTest) {

    IpRange ipRange{IPAddress{0}, IPAddress{1023}};
    hashedVebTreeMap_ = std::make_unique<DSModelHashedVebTreeImpl>(ipRange);

    auto [isInserted, networkIP] = hashedVebTreeMap_->InsertSubnet(MacID{1}, 1024);
    EXPECT_TRUE(isInserted);

    for(uint32_t i = 1; i <= 1022;++i){
        MacID macId{i};
        auto [isHostInserted, hostIp] = hashedVebTreeMap_->InsertSubnetHost(macId, networkIP);
        EXPECT_TRUE(isHostInserted);
        EXPECT_EQ(i, hostIp);
    }

    {

        MacID macIdLast{1023};
        auto [isHostInserted, hostIpLast] = hashedVebTreeMap_->InsertSubnetHost(macIdLast, networkIP);
        EXPECT_FALSE(isHostInserted);
    }

    for(uint32_t i = 1; i <= 1022;++i){
        MacID macId{i};
        auto [isHostPresent, hostIP] =  hashedVebTreeMap_->GetHostIpAddress(macId, networkIP);
        EXPECT_TRUE(isHostPresent);
        EXPECT_EQ(i, hostIP);
        auto [isHostPresent2, macIdOutput] = hashedVebTreeMap_->GetMacAddressOfHost(hostIP, networkIP);
        EXPECT_TRUE(isHostPresent2);
        EXPECT_EQ(macId.GetValue(), macIdOutput.GetValue());
    }

    {
        for (uint32_t i = 1; i <= 1022; ++i) {
            MacID macId{i};
            auto [isHostPresent, hostIP] = hashedVebTreeMap_->GetHostIpAddress(macId, networkIP);
            EXPECT_TRUE(isHostPresent);
            EXPECT_EQ(i, hostIP);
        }
    }

    {

        for (uint32_t i = 1; i <= 1022; ++i) {
            MacID macId{i};
            auto [isHostDeleted, hostIP] = hashedVebTreeMap_->DeleteHostFromSubnet(macId, networkIP);
            EXPECT_TRUE(isHostDeleted);
            EXPECT_EQ(i, hostIP);
        }
    }

    {
        for (uint32_t i = 1; i <= 1022; ++i) {
            MacID macId{i};
            auto [isHostPresent, hostIP] = hashedVebTreeMap_->GetHostIpAddress(macId, networkIP);
            EXPECT_FALSE(isHostPresent);
        }
    }




}

TEST_F( DSModelHashedVebTreeImplTestSubnetHosts, TestWithDeletionsAndReInsertions) {

    IpRange ipRange{IPAddress{0}, IPAddress{1023}};
    hashedVebTreeMap_ = std::make_unique<DSModelHashedVebTreeImpl>(ipRange);

    auto [isInserted, networkIP] = hashedVebTreeMap_->InsertSubnet(MacID{1}, 1024);
    EXPECT_TRUE(isInserted);

    {
        for(uint32_t i = 1; i <= 1022;++i){
            MacID macId{i};
            auto [isHostInserted, hostIp] = hashedVebTreeMap_->InsertSubnetHost(macId, networkIP);
            EXPECT_TRUE(isHostInserted);
            EXPECT_EQ(i, hostIp);
        }
    }

    {
        for(uint32_t i = 2; i <= 1022; i+=2){
            MacID macId{i};
            auto [isHostInserted, hostIp] = hashedVebTreeMap_->DeleteHostFromSubnet(macId, networkIP);
            EXPECT_TRUE(isHostInserted);
            EXPECT_EQ(i, hostIp);
        }
    }

    {
        int j = 1022;
        for(uint32_t i = 2; i <= 1022; i+=2){
            MacID macId{i};
            auto [isHostInserted, hostIp] = hashedVebTreeMap_->InsertSubnetHost(macId, networkIP);
            EXPECT_TRUE(isHostInserted);
            EXPECT_EQ(j, hostIp);
            j -= 2;
        }
    }

    {
        MacID macId{1203};
        auto [isHostInserted, hostIp] = hashedVebTreeMap_->InsertSubnetHost(macId, networkIP);
        EXPECT_FALSE(isHostInserted);


    }

    {
        MacID macId{1022};
        auto [isHostDeleted, hostIp] = hashedVebTreeMap_->DeleteHostFromSubnet(macId, networkIP);
        EXPECT_TRUE(isHostDeleted);

    }

    {
        MacID macId{1022};
        auto [isHostInserted, hostIp] = hashedVebTreeMap_->InsertSubnetHost(macId, networkIP);
        EXPECT_TRUE(isHostInserted);

    }

    {
        for(uint32_t i = 1; i < 1022; i+=1){
            MacID macId{i};
            auto [isHostDeleted, hostIp] = hashedVebTreeMap_->DeleteHostFromSubnet(macId, networkIP);
            EXPECT_TRUE(isHostDeleted);
        }
    }


}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}