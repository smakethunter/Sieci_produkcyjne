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
#include <utility>
#include <vector>
#include <functional>
#include "helpers.hpp"
#include "config.hpp"


enum class ReceiverType{
    WORKER,STOREHOUSE
};

class IPackageReceiver{
public:
    IPackageReceiver(){};


    virtual void receive_package(Package&& p)=0;
    virtual ElementID get_id() const =0;
    virtual ReceiverType get_receiver_type()const =0;
    virtual IPackageStockpile::const_iterator begin() const =0;
    virtual IPackageStockpile::const_iterator end() const =0;
    //virtual std::deque<Package>::reverse_iterator rbegin()=0;
    virtual IPackageStockpile::const_iterator cbegin() const=0;
    virtual IPackageStockpile::const_iterator cend() const =0;
    virtual IPackageStockpile* get_queue() const =0;


};


class ReceiverPreferences {
public:
    ReceiverPreferences(ProbabilityGenerator p = global_probability_generator): prob_rand(std::move(p)){};
    using preferences_t = std::map<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator;
    using iterator = preferences_t::iterator;
    iterator begin(){ return preferences_map.begin();}
    iterator end(){return preferences_map.end();}

    std::map<IPackageReceiver*, double >::reverse_iterator rbegin(){ return preferences_map.rbegin();}

    const_iterator cbegin() const { return  preferences_map.cbegin();}
    const_iterator cend() const {return preferences_map.cend();}
    void add_receiver(IPackageReceiver*);
    void remove_receiver(IPackageReceiver*);
    IPackageReceiver* choose_receiver();
    //ProbabilityGenerator prob_rand = [](){ return (double)rand() / RAND_MAX; };
    preferences_t get_preferences() const { return preferences_map;};
    void set_preferences(preferences_t p){preferences_map=p;}
private:
    preferences_t preferences_map;
    ProbabilityGenerator prob_rand;
};

class PackageSender {
public:

    //PackageSender(PackageSender&&)=default;
    PackageSender()= default;
    void send_package();
    const std::optional<Package>& get_sending_buffer() const;
    void push_package(Package&&);
    std::optional<Package> sending_buffer;
    virtual ElementID get_id() const=0;
    ReceiverPreferences get_receiver_preferences() const { return receiver_preferences_;}
public:
    ReceiverPreferences receiver_preferences_;
};

class Ramp : public PackageSender{
public:
    Ramp (ElementID id_, TimeOffset to_):  id(id_),to(to_){}

    void deliver_goods(Time t);
    TimeOffset get_delivery_interval()const{ return to;};
    ElementID get_id() const {return id;};

private:
    ElementID id;
    TimeOffset to;

};

class Worker: public PackageSender, public IPackageReceiver {
public:
    Worker(ElementID id_, TimeOffset to_,std::unique_ptr<IPackageQueue> q);
    Worker()= default;
    ElementID  get_id() const override { return id;};

    void receive_package(Package&& p) override;

    void do_work(Time);

    TimeOffset get_processing_duration()const{ return  to;} ;

    Time get_package_processing_start_time() const { return processing_start_time;};
    IPackageStockpile::const_iterator begin() const override { return queue_pointer->begin();}
    IPackageStockpile::const_iterator end() const override { return queue_pointer->end();}
    //std::deque<Package>::reverse_iterator rbegin() override { return queue_pointer->rbegin();}
    [[nodiscard]] IPackageStockpile::const_iterator cbegin() const override { return queue_pointer->cbegin();}
    [[nodiscard]] IPackageStockpile::const_iterator cend() const override { return queue_pointer->cend();}
    int size() const {return queue_pointer->size();}
    void start_process(Package&& p);
    const std::optional<Package>& get_processing_buffer() const;
    ReceiverType get_receiver_type() const override { return receiverType;}
    PackageQueueType get_queue_type()const { return queue_pointer->get_queue_type();};
    IPackageStockpile* get_queue() const override { return &(*queue_pointer);}
private:
    ElementID id;
    TimeOffset to;
    Time processing_start_time;

protected:
    std::optional<Package> processing_buffer;
public:
    std::unique_ptr<IPackageQueue> queue_pointer;
private:
    ReceiverType receiverType;
};
class Storehouse : public IPackageReceiver{
public:
    Storehouse(ElementID id_, std::unique_ptr<IPackageStockpile> d):
            IPackageReceiver(), id(id_),queue_pointer(std::move(d)), receiverType(ReceiverType::STOREHOUSE){}


    Storehouse()= default;
    Storehouse(ElementID id):id(id),queue_pointer(std::make_unique<PackageQueue>(PackageQueue(PackageQueueType::FIFO))),receiverType(ReceiverType::STOREHOUSE){}
    ReceiverType get_receiver_type()const override { return receiverType;}
public:
    ElementID  get_id() const override { return id;};
public:
    void receive_package(Package&& p) override;
    IPackageStockpile::const_iterator begin() const  override { return queue_pointer->begin();}
    IPackageStockpile::const_iterator end() const override { return queue_pointer->end();}
    //std::deque<Package>::reverse_iterator rbegin() override { return queue_pointer->rbegin();}
    IPackageStockpile::const_iterator cbegin() const override { return queue_pointer->cbegin();}
    IPackageStockpile::const_iterator cend() const override { return queue_pointer->cend();}
    int size() const {return queue_pointer->size();}
    IPackageStockpile* get_queue() const override { return &(*queue_pointer);}
private:
    ElementID id;
    std::unique_ptr<IPackageStockpile> queue_pointer;
    ReceiverType receiverType;

};

#endif //SIECI_PRODUKCYJNE_NODES_HPP
