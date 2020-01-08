//
// Created by smaket on 06.12.2019.
//
// Pomocnicze obiekty na funkcje globalne
#ifndef SIECI_PRODUKCYJNE_HELPERS_HPP
#define SIECI_PRODUKCYJNE_HELPERS_HPP
#include "types.hpp"
#include <functional>
#include <map>
extern ProbabilityGenerator probability_generator;




void add_nr(int nr, std::map<int,double>& map);
int remove_nr(int nr, std::map<int,double> &preferences_map);
int choose_nr(double mocked_prob,std::map<int,double> &preferences_map);

#endif //SIECI_PRODUKCYJNE_HELPERS_HPP
