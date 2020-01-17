//
// Created by smaket on 06.12.2019.
//

#ifndef SIECI_PRODUKCYJNE_SIMULATION_HPP
#define SIECI_PRODUKCYJNE_SIMULATION_HPP

#include "factory.hpp"


void simulate( Factory& f, TimeOffset d, std::function<void (Factory&, Time)>);



#endif //SIECI_PRODUKCYJNE_SIMULATION_HPP

// testing
