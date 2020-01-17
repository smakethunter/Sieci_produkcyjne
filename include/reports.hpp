//
// Created by smaket on 06.12.2019.
//

#ifndef SIECI_PRODUKCYJNE_REPORTS_HPP
#define SIECI_PRODUKCYJNE_REPORTS_HPP
#include <utility>

#include "types.hpp"
#include "factory.hpp"
class IntervalReportNotifier{
public:
    bool should_generate_report(Time t);
    IntervalReportNotifier(TimeOffset to):time(to){}
private:
    TimeOffset time;
};

class SpecificTurnsReportNotifier{
public:
    SpecificTurnsReportNotifier(std::set<Time> s):intervals(std::move(s)){}
    bool should_generate_report(Time t);
private:
    std::set<Time> intervals;
};
//funkcje, typy do poprawy
void generate_structure_report();

void generate_simulation_turn_report();

#endif //SIECI_PRODUKCYJNE_REPORTS_HPP
