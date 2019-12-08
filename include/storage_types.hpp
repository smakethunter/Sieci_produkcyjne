//
// Created by smaket on 06.12.2019.
//
#include "package.hpp"
#include "types.hpp"
#include <set>
#include <deque>
#include <iostream>
#include <memory>
#ifndef SIECI_PRODUKCYJNE_STORAGE_TYPES_HPP
#define SIECI_PRODUKCYJNE_STORAGE_TYPES_HPP


enum class PackageQueueType{
FIFO, LIFO
};




class IPackageStockpile{

public:

    IPackageStockpile()= default;


    virtual void push(Package&);

    virtual bool empty() const;

    [[nodiscard]] virtual int size() const;

protected:
    std::deque<Package> package_queue;
};

class IPackageQueue : public IPackageStockpile{
public:
    IPackageQueue():IPackageStockpile(){}


    virtual Package pop();

    virtual PackageQueueType get_queue_type();



};
class PackageQueue :public IPackageQueue{

public:
    PackageQueue(PackageQueueType qt):IPackageQueue(),queueType(qt){}

    void push(Package&) override ;

    Package pop() override ;

    PackageQueueType get_queue_type() override ;


private:

    PackageQueueType queueType;

};

#endif //SIECI_PRODUKCYJNE_STORAGE_TYPES_HPP
