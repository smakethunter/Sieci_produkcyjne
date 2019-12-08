//
// Created by smaket on 07.12.2019.
//
#include "package.hpp"
#include "types.hpp"
#include <iostream>
std::set<ElementID > Package::freed_ids;
std::set<ElementID > Package::assigned_ids;
Package::Package() {
    if (std::size(freed_ids)) {

        product_id=*(freed_ids.begin());
        freed_ids.erase(product_id);
        assigned_ids.insert(product_id);
    }
    else{
       if (std::size(assigned_ids)) {
           product_id = *(--assigned_ids.end()) + 1;
           assigned_ids.insert(product_id);
       }
       else{
           product_id=1;
           assigned_ids.insert(product_id);
       }
    }
}


Package::~Package() {
    freed_ids.insert(product_id);
    assigned_ids.erase(product_id);
}



ElementID Package::get_id() {
    return product_id;
}



Package::Package(const Package &p) {
    product_id=p.product_id;
    Package::assigned_ids.erase(*assigned_ids.rbegin());

}

void print_ids(){
    for (const auto i: Package::freed_ids){
        std::cout<<i<<',';
    }
    std::cout<<std::endl;
    for (const auto i: Package::assigned_ids){
        std::cout<<i<<',';
    }
    std::cout<<std::endl;
}

