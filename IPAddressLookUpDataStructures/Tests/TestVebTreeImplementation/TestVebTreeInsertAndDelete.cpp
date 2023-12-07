//
// Created by Abhijith  K A on 13/07/23.
//

#include <gtest/gtest.h>
#include <map>
#include <IPAddressLookUpDataStructures/src/Models/DSModelVEBTreeImpl/VEBTreeImplementations/VEBTree/VEBTreeMap.h>

constexpr veb_key_t universe = 16;

using veb_value_t = uint32_t;


class DSModelVebTreeImplTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

   std::vector<veb_key_t> GenerateSeries(veb_key_t start, veb_key_t count, veb_key_t increment) {
       std::vector<veb_key_t> result;
       result.resize(count);
       veb_key_t current_val = start;
       for(auto& i : result){
           i = current_val;
           current_val += increment;
       }
       return result;
   }

    void InsertElements (const std::vector<veb_key_t>& keys, const std::vector<veb_value_t>& values){
        for(size_t i = 0; i < keys.size(); ++i){
            veb_key_t  key = keys[i];
            veb_key_t value = values.at(i);
            DsImplVebTree->Insert(key, value);
            map_.insert({key, value});
        }
    }

    void DeleteElements (const std::vector<veb_key_t>& keys, const std::vector<veb_value_t>& values){
        for(size_t i = 0; i < keys.size(); ++i){
            veb_key_t  key = keys[i];
            veb_key_t value = values.at(i);
            DsImplVebTree->Delete(key);
            map_.erase(key);
        }
    }
    bool CompareElements(const std::vector<veb_key_t>& keys, const std::vector<veb_value_t>& values){
        for(size_t i = 0; i < keys.size(); ++i){
            veb_key_t  key = keys[i];
            veb_key_t value = values.at(i);
            bool status;
            veb_value_t vebValue;
            std::tie (status, vebValue) = DsImplVebTree->FindKey(key);
            auto iter = map_.find(key);
            if(iter == map_.end()){
                return false;
            }
            else if(!status) return false;
            else if(vebValue != iter->second){
                return false;
            }
        }
        return true;
    }

    bool CheckElementsAreNotPresent(const std::vector<veb_key_t>& keys, const std::vector<veb_value_t>& values){
        for(size_t i = 0; i < keys.size(); ++i){
            veb_key_t  key = keys[i];
            veb_key_t value = values.at(i);
            bool status;
            veb_value_t vebValue;
            std::tie (status, vebValue) = DsImplVebTree->FindKey(key);
            auto iter = map_.find(key);
            if(iter != map_.end() || status){
                return false;
            }
        }
        return true;
    }

    void TearDown() override {
        DsImplVebTree.reset();
    }
    std::unique_ptr<VEBTreeMap<veb_value_t>> DsImplVebTree;
    std::map<veb_key_t , veb_value_t > map_;

};

TEST_F(DSModelVebTreeImplTest, Construction) {

    EXPECT_NO_THROW(
            DsImplVebTree = std::make_unique<VEBTreeMap<veb_value_t>>(universe)
    );

}

TEST_F(DSModelVebTreeImplTest, InsertOneElement) {

    DsImplVebTree = std::make_unique<VEBTreeMap<veb_value_t>>(universe);
    DsImplVebTree->Insert(2, 100);
    map_.insert({2,100});

    auto [status, value] = DsImplVebTree->FindKey(2);
    auto iter = map_.find(2);
    EXPECT_TRUE(iter != map_.end());
    EXPECT_TRUE(status);
    EXPECT_TRUE(value == iter->second);

}

TEST_F(DSModelVebTreeImplTest, InsertTwoElement) {

    DsImplVebTree = std::make_unique<VEBTreeMap<veb_value_t>>(universe);
    DsImplVebTree->Insert(2, 100);
    map_.insert({2,100});
    DsImplVebTree->Insert(3, 100);
    map_.insert({3,101});

    auto [status, value] = DsImplVebTree->FindKey(2);
    auto iter = map_.find(2);
    EXPECT_TRUE(iter != map_.end());
    EXPECT_TRUE(status);
    EXPECT_TRUE(value == iter->second);

    auto [status_2, value_2] = DsImplVebTree->FindKey(3);
    auto iter2 = map_.find(3);
    EXPECT_TRUE(iter != map_.end());
    EXPECT_TRUE(status);
    EXPECT_TRUE(value_2 == iter->second);

}

TEST_F(DSModelVebTreeImplTest, InsertEightElementEven) {

    DsImplVebTree = std::make_unique<VEBTreeMap<veb_value_t>>(universe);
    auto keys = GenerateSeries(0,8,2);
    auto values = GenerateSeries(100, 8, 1);
    InsertElements(keys, values);
    EXPECT_TRUE(CompareElements(keys, values));

}

TEST_F(DSModelVebTreeImplTest, InsertEightElementEvenTwice) {

    DsImplVebTree = std::make_unique<VEBTreeMap<veb_value_t>>(universe);
    auto keys = GenerateSeries(0,8,2);
    auto values = GenerateSeries(100, 8, 1);
    InsertElements(keys, values);
    InsertElements(keys, values);
    EXPECT_TRUE(CompareElements(keys, values));

}

TEST_F(DSModelVebTreeImplTest, InsertEightElementEvenFourElementsTwice) {

    // Checks that Insert does not overwrite

    DsImplVebTree = std::make_unique<VEBTreeMap<veb_value_t>>(universe);
    auto keys = GenerateSeries(0,8,2);
    auto values = GenerateSeries(100, 8, 1);
    InsertElements(keys, values);
    EXPECT_TRUE(CompareElements(keys, values));
    auto keys_2 = GenerateSeries(0, 4,4);
    auto values_2 = GenerateSeries(1000,4,1);
    InsertElements(keys_2, values_2);
    EXPECT_TRUE(CompareElements(keys_2, values_2));
}


TEST_F(DSModelVebTreeImplTest, InsertEightElementEvenReverse) {

    DsImplVebTree = std::make_unique<VEBTreeMap<veb_value_t>>(universe);
    auto keys = GenerateSeries(0,8,2);
    std::reverse(keys.begin(), keys.end());
    auto values = GenerateSeries(100, 8, 1);
    InsertElements(keys, values);
    EXPECT_TRUE(CompareElements(keys, values));

}

TEST_F(DSModelVebTreeImplTest, InsertEightElementOdd) {

    DsImplVebTree = std::make_unique<VEBTreeMap<veb_value_t>>(universe);
    auto keys = GenerateSeries(1,8,2);
    auto values = GenerateSeries(100, 8, 1);
    InsertElements(keys, values);
    EXPECT_TRUE(CompareElements(keys, values));

}

TEST_F(DSModelVebTreeImplTest, InsertEightElementOddReverse) {

    DsImplVebTree = std::make_unique<VEBTreeMap<veb_value_t>>(universe);
    auto keys = GenerateSeries(1,8,2);
    std::reverse(keys.begin(), keys.end());
    auto values = GenerateSeries(100, 8, 1);
    InsertElements(keys, values);
    EXPECT_TRUE(CompareElements(keys, values));

}

TEST_F(DSModelVebTreeImplTest, InsertSixteenElements) {

    DsImplVebTree = std::make_unique<VEBTreeMap<veb_value_t>>(universe);
    auto keys = GenerateSeries(0,16,1);
    auto values = GenerateSeries(100, 16, 1);
    InsertElements(keys, values);
    EXPECT_TRUE(CompareElements(keys, values));

}

TEST_F(DSModelVebTreeImplTest, InsertSixteenElementsReverse) {

    DsImplVebTree = std::make_unique<VEBTreeMap<veb_value_t>>(universe);
    auto keys = GenerateSeries(0,16,1);
    std::reverse(keys.begin(), keys.end());
    auto values = GenerateSeries(100, 16, 1);
    InsertElements(keys, values);
    EXPECT_TRUE(CompareElements(keys, values));

}

TEST_F(DSModelVebTreeImplTest, Insert256Elements) {

    DsImplVebTree = std::make_unique<VEBTreeMap<veb_value_t>>(256);
    auto keys = GenerateSeries(0,256,1);
    auto values = GenerateSeries(100, 256, 1);
    InsertElements(keys, values);
    EXPECT_TRUE(CompareElements(keys, values));

}

TEST_F(DSModelVebTreeImplTest, DeleteFromEmptyTree){

    DsImplVebTree = std::make_unique<VEBTreeMap<veb_value_t>>(universe);
    EXPECT_NO_THROW(DsImplVebTree->Delete(1));
}

TEST_F(DSModelVebTreeImplTest, DeleteOneElement){

    DsImplVebTree = std::make_unique<VEBTreeMap<veb_value_t>>(universe);
    DsImplVebTree->Insert(1, 100);
    auto [status, value] = DsImplVebTree->FindKey(1);
    EXPECT_TRUE(status);
    DsImplVebTree->Delete(1);
    auto [status_2, value_2] = DsImplVebTree->FindKey(1);
    EXPECT_FALSE(status_2);

}

TEST_F(DSModelVebTreeImplTest, InsertEightOddElementsAndDeleteThem){
    DsImplVebTree = std::make_unique<VEBTreeMap<veb_value_t>>(universe);
    auto keys = GenerateSeries(1,8,2);
    auto values = GenerateSeries(100, 8, 1);
    InsertElements(keys, values);
    EXPECT_TRUE(CompareElements(keys, values));
    DeleteElements(keys, values);
    EXPECT_TRUE(CheckElementsAreNotPresent(keys, values));
}

TEST_F(DSModelVebTreeImplTest, InsertEightEvenElementsAndDeleteThem){
    DsImplVebTree = std::make_unique<VEBTreeMap<veb_value_t>>(universe);
    auto keys = GenerateSeries(0,8,2);
    auto values = GenerateSeries(100, 8, 1);
    InsertElements(keys, values);
    EXPECT_TRUE(CompareElements(keys, values));
    DeleteElements(keys, values);
    EXPECT_TRUE(CheckElementsAreNotPresent(keys, values));
}

TEST_F(DSModelVebTreeImplTest, InsertSixteenElementsAndDeleteThem){
    DsImplVebTree = std::make_unique<VEBTreeMap<veb_value_t>>(universe);
    auto keys = GenerateSeries(0,16,1);
    auto values = GenerateSeries(100, 16, 1);
    InsertElements(keys, values);
    EXPECT_TRUE(CompareElements(keys, values));
    DeleteElements(keys, values);
    EXPECT_TRUE(CheckElementsAreNotPresent(keys, values));
}

TEST_F(DSModelVebTreeImplTest, InsertEightOddElementsAndDeleteThemInReverse){
    DsImplVebTree = std::make_unique<VEBTreeMap<veb_value_t>>(universe);
    auto keys = GenerateSeries(1,8,2);
    auto values = GenerateSeries(100, 8, 1);
    InsertElements(keys, values);
    EXPECT_TRUE(CompareElements(keys, values));
    std::reverse(keys.begin(), keys.end());
    DeleteElements(keys, values);
    EXPECT_TRUE(CheckElementsAreNotPresent(keys, values));
}

TEST_F(DSModelVebTreeImplTest, InsertSixteenElementsAndDeleteThemReverse){
    DsImplVebTree = std::make_unique<VEBTreeMap<veb_value_t>>(universe);
    auto keys = GenerateSeries(0,16,1);
    auto values = GenerateSeries(100, 16, 1);
    InsertElements(keys, values);
    EXPECT_TRUE(CompareElements(keys, values));
    std::reverse(keys.begin(), keys.end());
    DeleteElements(keys, values);
    EXPECT_TRUE(CheckElementsAreNotPresent(keys, values));
}

TEST_F(DSModelVebTreeImplTest, InsertSixteenElementsAndDeleteFour){
    DsImplVebTree = std::make_unique<VEBTreeMap<veb_value_t>>(universe);
    auto keys = GenerateSeries(0,16,1);
    auto values = GenerateSeries(100, 16, 1);
    InsertElements(keys, values);
    EXPECT_TRUE(CompareElements(keys, values));
    auto keys_2 = GenerateSeries(1,4,4);
    auto values_2 = GenerateSeries(101,4,4);
    DeleteElements(keys_2, values_2);
    EXPECT_TRUE(CheckElementsAreNotPresent(keys_2, values_2));
}

TEST_F(DSModelVebTreeImplTest, InsertSixteenElementsAndDeleteFourteen){
    DsImplVebTree = std::make_unique<VEBTreeMap<veb_value_t>>(universe);
    auto keys = GenerateSeries(0,16,1);
    auto values = GenerateSeries(100, 16, 1);
    InsertElements(keys, values);
    EXPECT_TRUE(CompareElements(keys, values));
    auto keys_2 = GenerateSeries(0,7,1);
    auto values_2 = GenerateSeries(100, 7, 1);
    auto keys_3 = GenerateSeries(9,7,1);
    auto values_3 = GenerateSeries(109, 7, 1);
    DeleteElements(keys_2, values_2);
    DeleteElements(keys_3, values_3);
    EXPECT_TRUE(CompareElements({7,8}, {107,108}));
    EXPECT_TRUE(CheckElementsAreNotPresent(keys_2,values_2));
    EXPECT_TRUE(CheckElementsAreNotPresent(keys_3,values_3));
}

TEST_F(DSModelVebTreeImplTest, InsertEightEvenElementsAndDeleteThemInReverse){
    DsImplVebTree = std::make_unique<VEBTreeMap<veb_value_t>>(universe);
    auto keys = GenerateSeries(0,8,2);
    auto values = GenerateSeries(100, 8, 1);
    InsertElements(keys, values);
    EXPECT_TRUE(CompareElements(keys, values));
    std::reverse(keys.begin(), keys.end());
    DeleteElements(keys, values);
    EXPECT_TRUE(CheckElementsAreNotPresent(keys, values));
}

TEST_F(DSModelVebTreeImplTest, Insert256ElementsAndDeleteThem){
    DsImplVebTree = std::make_unique<VEBTreeMap<veb_value_t>>(256);
    auto keys = GenerateSeries(0,256,1);
    auto values = GenerateSeries(100, 256, 1);
    InsertElements(keys, values);
    EXPECT_TRUE(CompareElements(keys, values));
    DeleteElements(keys, values);
    EXPECT_TRUE(CheckElementsAreNotPresent(keys, values));
}

TEST_F(DSModelVebTreeImplTest, Insert256ElementsAndDeleteThemInReverse){
    DsImplVebTree = std::make_unique<VEBTreeMap<veb_value_t>>(256);
    auto keys = GenerateSeries(0,256,1);
    auto values = GenerateSeries(100, 256, 1);
    InsertElements(keys, values);
    EXPECT_TRUE(CompareElements(keys, values));
    std::reverse(keys.begin(), keys.end());
    DeleteElements(keys, values);
    EXPECT_TRUE(CheckElementsAreNotPresent(keys, values));
}

TEST_F(DSModelVebTreeImplTest, Insert256ElementsAndDelete128OddOnes){
    DsImplVebTree = std::make_unique<VEBTreeMap<veb_value_t>>(256);
    auto keys = GenerateSeries(0,256,1);
    auto values = GenerateSeries(100, 256, 1);
    InsertElements(keys, values);
    EXPECT_TRUE(CompareElements(keys, values));
    auto keys_2 = GenerateSeries(1,128,2);
    auto values_2 = GenerateSeries(101,128,2);
    DeleteElements(keys_2, values_2);
    EXPECT_TRUE(CheckElementsAreNotPresent(keys_2, values_2));
    auto keys_not_deleted = GenerateSeries(0,128,2);
    auto values_not_deleted = GenerateSeries(100,128,2);
    EXPECT_TRUE(CompareElements(keys_not_deleted, values_not_deleted));

}

TEST_F(DSModelVebTreeImplTest, Insert256ElementsAndDelete128EvenOnes){
    DsImplVebTree = std::make_unique<VEBTreeMap<veb_value_t>>(256);
    auto keys = GenerateSeries(0,256,1);
    auto values = GenerateSeries(100, 256, 1);
    InsertElements(keys, values);
    EXPECT_TRUE(CompareElements(keys, values));
    auto keys_2 = GenerateSeries(0,128,2);
    auto values_2 = GenerateSeries(100,128,2);
    DeleteElements(keys_2, values_2);
    EXPECT_TRUE(CheckElementsAreNotPresent(keys_2, values_2));
    auto keys_not_deleted = GenerateSeries(1,128,2);
    auto values_not_deleted = GenerateSeries(101,128,2);
    EXPECT_TRUE(CompareElements(keys_not_deleted, values_not_deleted));

}

TEST_F(DSModelVebTreeImplTest, Universe32){
    DsImplVebTree = std::make_unique<VEBTreeMap<veb_value_t>>(32);
    auto keys = GenerateSeries(0,32,1);
    auto values = GenerateSeries(100, 32, 1);
    InsertElements(keys, values);
    EXPECT_TRUE(CompareElements(keys, values));
    auto keys_2 = GenerateSeries(0,16,2);
    auto values_2 = GenerateSeries(100,16,2);
    DeleteElements(keys_2, values_2);
    EXPECT_TRUE(CheckElementsAreNotPresent(keys_2, values_2));
    auto keys_not_deleted = GenerateSeries(1,16,2);
    auto values_not_deleted = GenerateSeries(101,16,2);
    EXPECT_TRUE(CompareElements(keys_not_deleted, values_not_deleted));
}

TEST_F(DSModelVebTreeImplTest, Universe32DeleteOdd){
    DsImplVebTree = std::make_unique<VEBTreeMap<veb_value_t>>(32);
    auto keys = GenerateSeries(0,32,1);
    auto values = GenerateSeries(100, 32, 1);
    InsertElements(keys, values);
    EXPECT_TRUE(CompareElements(keys, values));
    auto keys_2 = GenerateSeries(1,16,2);
    auto values_2 = GenerateSeries(101,16,2);
    DeleteElements(keys_2, values_2);
    EXPECT_TRUE(CheckElementsAreNotPresent(keys_2, values_2));
    auto keys_not_deleted = GenerateSeries(0,16,2);
    auto values_not_deleted = GenerateSeries(100,16,2);
    EXPECT_TRUE(CompareElements(keys_not_deleted, values_not_deleted));
}

TEST_F(DSModelVebTreeImplTest, Universe32DeleteOddReverse){
    DsImplVebTree = std::make_unique<VEBTreeMap<veb_value_t>>(32);
    auto keys = GenerateSeries(0,32,1);
    auto values = GenerateSeries(100, 32, 1);
    InsertElements(keys, values);
    EXPECT_TRUE(CompareElements(keys, values));
    auto keys_2 = GenerateSeries(1,16,2);
    auto values_2 = GenerateSeries(101,16,2);
    std::reverse(keys_2.begin(), keys_2.end());
    std::reverse(values_2.begin(), values_2.end());
    DeleteElements(keys_2, values_2);
    EXPECT_TRUE(CheckElementsAreNotPresent(keys_2, values_2));
    auto keys_not_deleted = GenerateSeries(0,16,2);
    auto values_not_deleted = GenerateSeries(100,16,2);
    EXPECT_TRUE(CompareElements(keys_not_deleted, values_not_deleted));
}

TEST_F(DSModelVebTreeImplTest, Universe128Delete){
    DsImplVebTree = std::make_unique<VEBTreeMap<veb_value_t>>(128);
    auto keys = GenerateSeries(0,128,1);
    auto values = GenerateSeries(100, 128, 1);
    InsertElements(keys, values);
    EXPECT_TRUE(CompareElements(keys, values));
    auto keys_2 = GenerateSeries(0,16,1);
    auto values_2 = GenerateSeries(100,16,1);
    std::reverse(keys_2.begin(), keys_2.end());
    std::reverse(values_2.begin(), values_2.end());
    DeleteElements(keys_2, values_2);
    EXPECT_TRUE(CheckElementsAreNotPresent(keys_2, values_2));

    auto keys_3 = GenerateSeries(112,16,1);
    auto values_3 = GenerateSeries(212,16,1);
    DeleteElements(keys_3, values_3);
    EXPECT_TRUE(CheckElementsAreNotPresent(keys_3, values_3));
    auto keys_not_deleted = GenerateSeries(16,96,1);
    auto values_not_deleted = GenerateSeries(116,96,1);
    EXPECT_TRUE(CompareElements(keys_not_deleted, values_not_deleted));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}