#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <functional>
#include "nodes.hpp"
#include <map>


TEST(RandomTest, addtests) {

    std::map<int,double> map;
    std::vector<int> elements;

    elements.reserve(5);
for (auto i = 0; i < 5; i++) {
        elements.push_back(i);
    }
    for(auto& i: elements){
        add_nr(i, map);
    }

    ASSERT_EQ(2,choose_nr(0.4, map));


    ASSERT_EQ(4,remove_nr(0,map));

}
TEST(PackageSenderTest, TestBuffer){
    PackageQueue queue (PackageQueueType::FIFO);
    PackageQueue queue2(PackageQueueType::FIFO);
    Package p;
    ASSERT_EQ(p.get_id(),1);
    Package q;
    ASSERT_EQ(q.get_id(),2);
    Package r;
    ASSERT_EQ(r.get_id(),3);
    Package test;
    Package wtest;
    Package stest;
    queue.push(std::move(p));

    queue.push(std::move(q));

    queue.push(std::move(r));

    Storehouse s1(1,std::make_unique<PackageQueue>(std::move(queue2)));
    Worker w1(1, 2, std::make_unique<PackageQueue>(std::move(queue)));
    Ramp r1(1,1);
    ASSERT_EQ(r1.get_sending_buffer(),std::nullopt);
    r1.push_package(std::move(test));
    ASSERT_EQ(r1.sending_buffer->get_id(),4);
    ASSERT_EQ(r1.sending_buffer.has_value(),true);
    r1.receiver_preferences.add_receiver(&w1);

    ASSERT_EQ(r1.receiver_preferences.begin()->first->get_id(),1);
    ASSERT_EQ(r1.receiver_preferences.choose_receiver()->get_id(),1);
    ASSERT_EQ(r1.receiver_preferences.choose_receiver()->get_receiver_type(),ReceiverType::Worker);
    //r1.deliver_goods(1);
    w1.receive_package(std::move(wtest));
    for (auto& i: *(w1.queue_pointer)){
        std::cout << i.get_id() <<',';
    }
    w1.receiver_preferences.add_receiver(&s1);
    ASSERT_EQ(w1.sending_buffer.value().get_id(),1);
    w1.do_work(1);
    w1.do_work(2);
    //w1.do_work(3);
    //s1.receive_package(std::move(stest));
    ASSERT_EQ(s1.begin()->get_id(),0);





}





