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

enum class ReceiverType{
Ramp,Worker,Storehouse
};

class IPackageReceiver{
public:
    IPackageReceiver(ReceiverType r):receiverType(r){};
    virtual void receive_package(Package&& p)=0;
    virtual ElementID get_id() const =0;
    ReceiverType get_receiver_type()const {return receiverType;}
private:
    ReceiverType receiverType;

};

class Storehouse : IPackageReceiver{
    Storehouse(ElementID id_, std::unique_ptr<IPackageStockpile> d):
    IPackageReceiver(ReceiverType::Storehouse), id(id_),queue_pointer(std::move(d)){}
    ElementID  get_id() const override { return id;};
    void receive_package(Package&& p) override;
private:
    ElementID id;
    std::unique_ptr<IPackageStockpile> queue_pointer;

};

class ReceiverPreferences {
public:
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
    std::optional<Package> sending_buffer;
    ReceiverPreferences receiver_preferences;
};

class Ramp :PackageSender{
    Ramp (ElementID id_, TimeOffset to_):PackageSender(),id(id_),to(to_){}
    void deliver_goods(Time t);
    TimeOffset get_delivery_interval()const{ return to;};
    ElementID get_id()const  {return id;};

private:
    ElementID id;
    TimeOffset to;

};

class Worker: public PackageSender, public IPackageReceiver {
    Worker(ElementID id_, TimeOffset to_,std::unique_ptr<IPackageQueue> q);

    ElementID  get_id() const override { return id;};

    void receive_package(Package&& p) override;

    void do_work();

    TimeOffset get_processing_duration(){ return  to;};

    Time get_package_processing_start_time(){ return processing_start_time;};
private:
    ElementID id;
    TimeOffset to;
    std::unique_ptr<IPackageQueue> queue_pointer;
    Time processing_start_time;

};
double generate_random();
#endif //SIECI_PRODUKCYJNE_NODES_HPP
