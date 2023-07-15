//
// Created by Abhijith  K A on 13/07/23.
//

#include <gtest/gtest.h>
#include <map>
#include <DHCPAllocator/src/Models/DSModelVEBTreeImpl/VEBTree/VEBTreeMap.h>

constexpr veb_key_t universe = 16;


class VEBTest : public ::testing::Test {
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
            veb_key_t value = values[i];
            vebTreeMap->Insert(key, value);
            map_.insert({key, value});
        }
    }

    void DeleteElements (const std::vector<veb_key_t>& keys, const std::vector<veb_value_t>& values){
        for(size_t i = 0; i < keys.size(); ++i){
            veb_key_t  key = keys[i];
            veb_key_t value = values[i];
            vebTreeMap->Delete(key);
            map_.erase(key);
        }
    }
    bool CompareElements(const std::vector<veb_key_t>& keys, const std::vector<veb_value_t>& values){
        for(size_t i = 0; i < keys.size(); ++i){
            veb_key_t  key = keys[i];
            veb_key_t value = values[i];
            bool status;
            veb_value_t vebValue;
            std::tie (status, vebValue) = vebTreeMap->FindKey(key);
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
            veb_key_t value = values[i];
            bool status;
            veb_value_t vebValue;
            std::tie (status, vebValue) = vebTreeMap->FindKey(key);
            auto iter = map_.find(key);
            if(iter != map_.end() || status){
                return false;
            }
        }
        return true;
    }

    void TearDown() override {
        vebTreeMap.reset();
    }
    std::unique_ptr<VEBTreeMap<veb_value_t>> vebTreeMap;
    std::map<veb_key_t , veb_value_t > map_;

};

TEST_F(VEBTest, Construction) {

    EXPECT_NO_THROW(
            vebTreeMap = std::make_unique<VEBTreeMap<veb_value_t>>(universe)
    );

}

TEST_F(VEBTest, InsertOneElement) {

    vebTreeMap = std::make_unique<VEBTreeMap<veb_value_t>>(universe);
    vebTreeMap->Insert(2, 100);
    map_.insert({2,100});

    auto [status, value] = vebTreeMap->FindKey(2);
    auto iter = map_.find(2);
    EXPECT_TRUE(iter != map_.end());
    EXPECT_TRUE(status);
    EXPECT_TRUE(value == iter->second);

}

TEST_F(VEBTest, InsertTwoElement) {

    vebTreeMap = std::make_unique<VEBTreeMap<veb_value_t>>(universe);
    vebTreeMap->Insert(2, 100);
    map_.insert({2,100});
    vebTreeMap->Insert(3, 100);
    map_.insert({3,101});

    auto [status, value] = vebTreeMap->FindKey(2);
    auto iter = map_.find(2);
    EXPECT_TRUE(iter != map_.end());
    EXPECT_TRUE(status);
    EXPECT_TRUE(value == iter->second);

    auto [status_2, value_2] = vebTreeMap->FindKey(3);
    auto iter2 = map_.find(3);
    EXPECT_TRUE(iter != map_.end());
    EXPECT_TRUE(status);
    EXPECT_TRUE(value_2 == iter->second);

}

TEST_F(VEBTest, InsertEightElementEven) {

    vebTreeMap = std::make_unique<VEBTreeMap<veb_value_t>>(universe);
    auto keys = GenerateSeries(0,8,2);
    auto values = GenerateSeries(100, 8, 1);
    InsertElements(keys, values);
    EXPECT_TRUE(CompareElements(keys, values));

}

TEST_F(VEBTest, InsertEightElementEvenTwice) {

    vebTreeMap = std::make_unique<VEBTreeMap<veb_value_t>>(universe);
    auto keys = GenerateSeries(0,8,2);
    auto values = GenerateSeries(100, 8, 1);
    InsertElements(keys, values);
    InsertElements(keys, values);
    EXPECT_TRUE(CompareElements(keys, values));

}

TEST_F(VEBTest, InsertEightElementEvenFourElementsTwice) {

    // Checks that Insert does not overwrite

    vebTreeMap = std::make_unique<VEBTreeMap<veb_value_t>>(universe);
    auto keys = GenerateSeries(0,8,2);
    auto values = GenerateSeries(100, 8, 1);
    InsertElements(keys, values);
    EXPECT_TRUE(CompareElements(keys, values));
    auto keys_2 = GenerateSeries(0, 4,4);
    auto values_2 = GenerateSeries(1000,4,1);
    InsertElements(keys_2, values_2);
    EXPECT_TRUE(CompareElements(keys_2, values_2));
}


TEST_F(VEBTest, InsertEightElementEvenReverse) {

    vebTreeMap = std::make_unique<VEBTreeMap<veb_value_t>>(universe);
    auto keys = GenerateSeries(0,8,2);
    std::reverse(keys.begin(), keys.end());
    auto values = GenerateSeries(100, 8, 1);
    InsertElements(keys, values);
    EXPECT_TRUE(CompareElements(keys, values));

}

TEST_F(VEBTest, InsertEightElementOdd) {

    vebTreeMap = std::make_unique<VEBTreeMap<veb_value_t>>(universe);
    auto keys = GenerateSeries(1,8,2);
    auto values = GenerateSeries(100, 8, 1);
    InsertElements(keys, values);
    EXPECT_TRUE(CompareElements(keys, values));

}

TEST_F(VEBTest, InsertEightElementOddReverse) {

    vebTreeMap = std::make_unique<VEBTreeMap<veb_value_t>>(universe);
    auto keys = GenerateSeries(1,8,2);
    std::reverse(keys.begin(), keys.end());
    auto values = GenerateSeries(100, 8, 1);
    InsertElements(keys, values);
    EXPECT_TRUE(CompareElements(keys, values));

}

TEST_F(VEBTest, InsertSixteenElements) {

    vebTreeMap = std::make_unique<VEBTreeMap<veb_value_t>>(universe);
    auto keys = GenerateSeries(0,16,1);
    auto values = GenerateSeries(100, 16, 1);
    InsertElements(keys, values);
    EXPECT_TRUE(CompareElements(keys, values));

}

TEST_F(VEBTest, InsertSixteenElementsReverse) {

    vebTreeMap = std::make_unique<VEBTreeMap<veb_value_t>>(universe);
    auto keys = GenerateSeries(0,16,1);
    std::reverse(keys.begin(), keys.end());
    auto values = GenerateSeries(100, 16, 1);
    InsertElements(keys, values);
    EXPECT_TRUE(CompareElements(keys, values));

}

TEST_F(VEBTest, Insert256Elements) {

    vebTreeMap = std::make_unique<VEBTreeMap<veb_value_t>>(256);
    auto keys = GenerateSeries(0,256,1);
    auto values = GenerateSeries(100, 256, 1);
    InsertElements(keys, values);
    EXPECT_TRUE(CompareElements(keys, values));

}

TEST_F(VEBTest, DeleteFromEmptyTree){

    vebTreeMap = std::make_unique<VEBTreeMap<veb_value_t>>(universe);
    EXPECT_NO_THROW(vebTreeMap->Delete(1));
}

TEST_F(VEBTest, DeleteOneElement){

    vebTreeMap = std::make_unique<VEBTreeMap<veb_value_t>>(universe);
    vebTreeMap->Insert(1, 100);
    auto [status, value] = vebTreeMap->FindKey(1);
    EXPECT_TRUE(status);
    vebTreeMap->Delete(1);
    auto [status_2, value_2] = vebTreeMap->FindKey(1);
    EXPECT_FALSE(status_2);

}

TEST_F(VEBTest, InsertEightOddElementsAndDeleteThem){
    vebTreeMap = std::make_unique<VEBTreeMap<veb_value_t>>(universe);
    auto keys = GenerateSeries(1,8,2);
    auto values = GenerateSeries(100, 8, 1);
    InsertElements(keys, values);
    EXPECT_TRUE(CompareElements(keys, values));
    DeleteElements(keys, values);
    EXPECT_TRUE(CheckElementsAreNotPresent(keys, values));
}

TEST_F(VEBTest, InsertEightEvenElementsAndDeleteThem){
    vebTreeMap = std::make_unique<VEBTreeMap<veb_value_t>>(universe);
    auto keys = GenerateSeries(0,8,2);
    auto values = GenerateSeries(100, 8, 1);
    InsertElements(keys, values);
    EXPECT_TRUE(CompareElements(keys, values));
    DeleteElements(keys, values);
    EXPECT_TRUE(CheckElementsAreNotPresent(keys, values));
}

TEST_F(VEBTest, InsertSixteenElementsAndDeleteThem){
    vebTreeMap = std::make_unique<VEBTreeMap<veb_value_t>>(universe);
    auto keys = GenerateSeries(0,16,1);
    auto values = GenerateSeries(100, 16, 1);
    InsertElements(keys, values);
    EXPECT_TRUE(CompareElements(keys, values));
    DeleteElements(keys, values);
    EXPECT_TRUE(CheckElementsAreNotPresent(keys, values));
}

TEST_F(VEBTest, InsertEightOddElementsAndDeleteThemInReverse){
    vebTreeMap = std::make_unique<VEBTreeMap<veb_value_t>>(universe);
    auto keys = GenerateSeries(1,8,2);
    auto values = GenerateSeries(100, 8, 1);
    InsertElements(keys, values);
    EXPECT_TRUE(CompareElements(keys, values));
    std::reverse(keys.begin(), keys.end());
    DeleteElements(keys, values);
    EXPECT_TRUE(CheckElementsAreNotPresent(keys, values));
}

TEST_F(VEBTest, InsertSixteenElementsAndDeleteThemReverse){
    vebTreeMap = std::make_unique<VEBTreeMap<veb_value_t>>(universe);
    auto keys = GenerateSeries(0,16,1);
    auto values = GenerateSeries(100, 16, 1);
    InsertElements(keys, values);
    EXPECT_TRUE(CompareElements(keys, values));
    std::reverse(keys.begin(), keys.end());
    DeleteElements(keys, values);
    EXPECT_TRUE(CheckElementsAreNotPresent(keys, values));
}

TEST_F(VEBTest, InsertSixteenElementsAndDeleteFour){
    vebTreeMap = std::make_unique<VEBTreeMap<veb_value_t>>(universe);
    auto keys = GenerateSeries(0,16,1);
    auto values = GenerateSeries(100, 16, 1);
    InsertElements(keys, values);
    EXPECT_TRUE(CompareElements(keys, values));
    auto keys_2 = GenerateSeries(1,4,4);
    auto values_2 = GenerateSeries(101,4,4);
    DeleteElements(keys_2, values_2);
    EXPECT_TRUE(CheckElementsAreNotPresent(keys_2, values_2));
}

TEST_F(VEBTest, InsertSixteenElementsAndDeleteFourteen){
    vebTreeMap = std::make_unique<VEBTreeMap<veb_value_t>>(universe);
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

TEST_F(VEBTest, InsertEightEvenElementsAndDeleteThemInReverse){
    vebTreeMap = std::make_unique<VEBTreeMap<veb_value_t>>(universe);
    auto keys = GenerateSeries(0,8,2);
    auto values = GenerateSeries(100, 8, 1);
    InsertElements(keys, values);
    EXPECT_TRUE(CompareElements(keys, values));
    std::reverse(keys.begin(), keys.end());
    DeleteElements(keys, values);
    EXPECT_TRUE(CheckElementsAreNotPresent(keys, values));
}

TEST_F(VEBTest, Insert256ElementsAndDeleteThem){
    vebTreeMap = std::make_unique<VEBTreeMap<veb_value_t>>(256);
    auto keys = GenerateSeries(0,256,1);
    auto values = GenerateSeries(100, 256, 1);
    InsertElements(keys, values);
    EXPECT_TRUE(CompareElements(keys, values));
    DeleteElements(keys, values);
    EXPECT_TRUE(CheckElementsAreNotPresent(keys, values));
}

TEST_F(VEBTest, Insert256ElementsAndDeleteThemInReverse){
    vebTreeMap = std::make_unique<VEBTreeMap<veb_value_t>>(256);
    auto keys = GenerateSeries(0,256,1);
    auto values = GenerateSeries(100, 256, 1);
    InsertElements(keys, values);
    EXPECT_TRUE(CompareElements(keys, values));
    std::reverse(keys.begin(), keys.end());
    DeleteElements(keys, values);
    EXPECT_TRUE(CheckElementsAreNotPresent(keys, values));
}

TEST_F(VEBTest, Insert256ElementsAndDelete128OddOnes){
    vebTreeMap = std::make_unique<VEBTreeMap<veb_value_t>>(256);
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

TEST_F(VEBTest, Insert256ElementsAndDelete128EvenOnes){
    vebTreeMap = std::make_unique<VEBTreeMap<veb_value_t>>(256);
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

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}