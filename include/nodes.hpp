//
// Created by smaket on 06.12.2019.
//

#ifndef SIECI_PRODUKCYJNE_NODES_HPP
#define SIECI_PRODUKCYJNE_NODES_HPP
#include "storage_types.hpp"
#include <map>
#include <memory>
#include <ctime>
#include <iomanip>
#include <random>
#include <cstdlib>
#include <vector>
#include <functional>
#include "helpers.hpp"
enum class ReceiverType{
Ramp,Worker,Storehouse
};

class IPackageReceiver{
public:
    explicit IPackageReceiver(ReceiverType r):receiverType(r){};
    virtual void receive_package(Package&& p)=0;
    virtual ElementID get_id() const =0;
    ReceiverType get_receiver_type()const {return receiverType;}
private:
    ReceiverType receiverType;

};

class Storehouse : public IPackageReceiver{
public:
    Storehouse(ElementID id_, std::unique_ptr<IPackageStockpile> d):
    IPackageReceiver(ReceiverType::Storehouse), id(id_),queue_pointer(std::move(d)){}
    ~Storehouse()= default;
public:
    ElementID  get_id() const override { return id;};
public:
    void receive_package(Package&& p) override;
    std::deque<Package>::iterator begin(){ return queue_pointer->begin();}
    std::deque<Package>::iterator end(){ return queue_pointer->end();}
    std::deque<Package>::reverse_iterator rbegin(){ return queue_pointer->rbegin();}
    std::deque<Package>::const_iterator cbegin(){ return queue_pointer->cbegin();}
    std::deque<Package>::const_iterator cend(){ return queue_pointer->cend();}
    int size(){return queue_pointer->size();}

private:
    ElementID id;
    std::unique_ptr<IPackageStockpile> queue_pointer;

};

class ReceiverPreferences {
public:
    using preferences_t = std::map<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator;
    using iterator = preferences_t::iterator;
    iterator begin(){ return preferences_map.begin();}
    iterator end(){return preferences_map.end();}
    std::map<IPackageReceiver*, double >::reverse_iterator rbegin(){ return preferences_map.rbegin();}
    void add_receiver(IPackageReceiver*);
    void remove_receiver(IPackageReceiver*);
    IPackageReceiver* choose_receiver();
    ProbabilityGenerator prob_rand = [](){ return (double)rand() / RAND_MAX; };
    preferences_t& get_preferences() { return preferences_map;};
private:
    preferences_t preferences_map;
};

class PackageSender {
public:

    void send_package();
    std::optional<Package>& get_sending_buffer();
    void push_package(Package&&);
    std::optional<Package> sending_buffer;
public:
    ReceiverPreferences receiver_preferences;
};

class Ramp : public PackageSender{
public:
    Ramp (ElementID id_, TimeOffset to_):  id(id_),to(to_){}
    void deliver_goods(Time t);
    TimeOffset get_delivery_interval()const{ return to;};
    ElementID get_id()const  {return id;};

private:
    ElementID id;
    TimeOffset to;

};

class Worker: public PackageSender, public IPackageReceiver {
public:
    Worker(ElementID id_, TimeOffset to_,std::unique_ptr<IPackageQueue> q);
    ~Worker()= default;
    ElementID  get_id() const override { return id;};

    void receive_package(Package&& p) override;

    void do_work(Time);

    TimeOffset get_processing_duration(){ return  to;};

    Time get_package_processing_start_time(){ return processing_start_time;};
    std::deque<Package>::iterator begin(){ return queue_pointer->begin();}
    std::deque<Package>::iterator end(){ return queue_pointer->end();}
    std::deque<Package>::reverse_iterator rbegin(){ return queue_pointer->rbegin();}
    std::deque<Package>::const_iterator cbegin(){ return queue_pointer->cbegin();}
    std::deque<Package>::const_iterator cend(){ return queue_pointer->cend();}
    int size(){return queue_pointer->size();}


private:
    ElementID id;
    TimeOffset to;
    Time processing_start_time;
public:
    std::unique_ptr<IPackageQueue> queue_pointer;

};
void add_nr(int nr, std::map<int,double>& map);
int remove_nr(int nr, std::map<int,double> &preferences_map);
int choose_nr(double mocked_prob,std::map<int,double> &preferences_map);
#endif //SIECI_PRODUKCYJNE_NODES_HPP
