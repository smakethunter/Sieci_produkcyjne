//
// Created by smaket on 06.12.2019.
//
#include "storage_types.hpp"
#include <map>
#include <memory>
#include <time.h>
#include <iomanip>
#include <random>
#include <stdlib.h>
#include <vector>
#ifndef SIECI_PRODUKCYJNE_NODES_HPP
#define SIECI_PRODUKCYJNE_NODES_HPP
using TimeOffset=int;
using Time=int;
enum class ReceiverType{
Ramp,Worker,Storehouse
};

class IPackageReceiver{
public:
    virtual void receive_package(Package&& p);
    virtual void do_work();
    ElementID  get_id();

};

class Storehouse : IPackageReceiver{
    Storehouse(ElementID id_, std::unique_ptr<IPackageStockpile> d):
    IPackageReceiver(), id(id_),queue_pointer(std::move(d)){}
private:
    ElementID id;
    std::unique_ptr<IPackageStockpile> queue_pointer;
};

class ReceiverPreferences {
    using preferences_t = std::map<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator;
    using iterator = preferences_t::iterator;
    void add_receiver(IPackageReceiver*);
    void remove_receiver(IPackageReceiver*);
    IPackageReceiver* choose_receiver();

private:
    preferences_t preferences_map;
};

class PackageSender {
public:
    void send_package();
    std::optional<Package> get_sending_buffer();
    void push_package(Package);

private:
ReceiverPreferences receiver_preferences;
};

class Ramp :PackageSender{
    Ramp (ElementID id_, TimeOffset to_):PackageSender(),id(id_),to(to_){}
    void deliver_goods(Time t);
    TimeOffset get_delivery_interval()const;
    ElementID get_id()const;

private:
    ElementID id;
    TimeOffset to;

};

class Worker: public PackageSender, public IPackageReceiver {
    Worker(ElementID id_, TimeOffset to_,std::unique_ptr<IPackageQueue> q);
private:
    ElementID id;
    TimeOffset to;
    std::unique_ptr<IPackageQueue> queue_pointer;
    Time procesing_start_time;

};
double generate_random();
#endif //SIECI_PRODUKCYJNE_NODES_HPP
