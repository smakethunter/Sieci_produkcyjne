//
// Created by smaket on 06.12.2019.
//



#ifndef SIECI_PRODUKCYJNE_STORAGE_TYPES_HPP
#define SIECI_PRODUKCYJNE_STORAGE_TYPES_HPP

#include <optional>
#include <set>
#include <deque>
#include <iostream>
#include <memory>
#include "types.hpp"
#include "package.hpp"
enum class PackageQueueType{
FIFO, LIFO
};


class IPackageStockpile{

public:
    using const_iterator = std::deque<Package>::const_iterator ;

    IPackageStockpile()= default;

    virtual void push(Package&&)=0;

    virtual bool empty() const;

    virtual int size() const=0;

    virtual std::deque<Package>::iterator begin()=0;
    virtual std::deque<Package>::iterator end()=0;
    virtual std::deque<Package>::reverse_iterator rbegin()=0;
    virtual const_iterator cbegin() const =0;
    virtual const_iterator cend() const =0;
};

class IPackageQueue : public IPackageStockpile{
public:
    IPackageQueue():IPackageStockpile(){}
    void push(Package&&) override=0;

    virtual Package pop()  = 0 ;

    virtual PackageQueueType get_queue_type() = 0;



};
class PackageQueue :public IPackageQueue{

public:
    PackageQueue(PackageQueueType qt):IPackageQueue(),queueType(qt){}

    void push(Package&&) override ;

    Package pop() override ;

    PackageQueueType get_queue_type() override ;
    int size() const override;
    std::deque<Package>::iterator begin() override {return package_queue.begin();}
    std::deque<Package>::iterator end() override {return package_queue.end();}
    std::deque<Package>::reverse_iterator rbegin() override {return package_queue.rbegin();}
    [[nodiscard]] const_iterator cbegin() const override { return package_queue.cbegin(); }
    [[nodiscard]] const_iterator cend() const override { return package_queue.cend();}
private:

    PackageQueueType queueType;
    std::deque<Package> package_queue;
};

#endif //SIECI_PRODUKCYJNE_STORAGE_TYPES_HPP

