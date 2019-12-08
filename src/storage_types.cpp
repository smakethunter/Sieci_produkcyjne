//
// Created by smaket on 07.12.2019.
//
#include "storage_types.hpp"



Package PackageQueue::pop() {
    Package ret;

    PackageQueueType p = get_queue_type();
    switch(p){
        case PackageQueueType::FIFO:
            ret = *package_queue.begin();
            package_queue.erase(package_queue.cbegin());
            break;
        case PackageQueueType::LIFO:
            ret = *package_queue.rbegin();
            package_queue.erase(package_queue.end());
            break;
    }
    return ret;
}

PackageQueueType PackageQueue::get_queue_type() {
    return queueType;
}

void PackageQueue::push(Package &p) {

    package_queue.push_back(p);

}


int IPackageStockpile::size() const {
    return int(std::size(package_queue));
}

bool IPackageStockpile::empty() const {
    return int(IPackageStockpile::size())!=0;
}



