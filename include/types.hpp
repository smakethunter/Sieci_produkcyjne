//
// Created by smaket on 06.12.2019.
//
// Na aliasy typów

#ifndef SIECI_PRODUKCYJNE_TYPES_HPP
#define SIECI_PRODUKCYJNE_TYPES_HPP

#include <functional>

using TimeOffset=int;
using Time=int;
using ElementID = int;
using ProbabilityGenerator =  std::function<double()>;
#endif //SIECI_PRODUKCYJNE_TYPES_HPP
