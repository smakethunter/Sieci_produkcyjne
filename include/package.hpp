//
// Created by smaket on 06.12.2019.
//
#ifndef SIECI_PRODUKCYJNE_PACKAGE_HPP
#define SIECI_PRODUKCYJNE_PACKAGE_HPP

#include "types.hpp"
#include <set>

class Package {
public:
    Package();

    Package (Package&& p);
    Package(int id_): product_id(id_){};
    ~Package();
    Package& operator=(Package&&)= default;
    ElementID get_id() const;
    static std::set<ElementID> freed_ids;
    static std::set<ElementID> assigned_ids;

private:

    ElementID product_id;
};
extern std::set<ElementID> freed_ids;
extern  std::set<ElementID> assigned_ids;

#endif //SIECI_PRODUKCYJNE_PACKAGE_HPP

