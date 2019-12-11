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

    IPackageStockpile()= default;



    virtual void push(Package&&)=0;

    virtual bool empty() const;

    virtual int size() const=0;

protected:

};

class IPackageQueue : public IPackageStockpile{
public:
    IPackageQueue():IPackageStockpile(){}


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


private:

    PackageQueueType queueType;
    std::deque<Package> package_queue;
};

#endif //SIECI_PRODUKCYJNE_STORAGE_TYPES_HPP

