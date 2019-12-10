#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "package.hpp"
#include "storage_types.hpp"
using ::testing::Property;

TEST(PackageTest, IDgeneration) {
    auto* first =new Package;
    int len= int(std::size(Package::assigned_ids));

    Package second;

    ASSERT_EQ(len,1);
    ASSERT_EQ(first->get_id(),1);
    ASSERT_EQ(second.get_id(),2);
    delete first;
    int len2 = int(std::size(Package::freed_ids));
    ASSERT_EQ(len2, 1);
    auto* third = new Package;
    EXPECT_EQ(third->get_id(),1);
    EXPECT_EQ(int(std::size(Package::freed_ids)),0);
    delete third;
}

TEST(StorageTypesTest,testFIFO) {
    PackageQueue queue(PackageQueueType::FIFO);
    Package p;

    Package q;

    Package r;

    queue.push(p);

    queue.push(q);

    queue.push(r);


    EXPECT_EQ(queue.size(), 3);
    Package package = queue.pop();
    EXPECT_EQ(package.get_id(), 1);

    EXPECT_EQ(queue.size(),2);

}
TEST(StorageTypesTest,testLIFO) {

    PackageQueue queue(PackageQueueType::LIFO);
    Package p;

    Package q;

    Package r;

    queue.push(p);

    queue.push(q);

    queue.push(r);


    EXPECT_EQ(queue.size(), 3);
    Package package = queue.pop();
    EXPECT_EQ(package.get_id(), 3);

    EXPECT_EQ(queue.size(),2);
}
