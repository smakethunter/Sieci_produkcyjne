//
// Created by nsak on 11.12.2019.
//

#include "reports.hpp"

bool SpecificTurnsReportNotifier::should_generate_report(Time t) {
    return (intervals.find(t) != intervals.end());
}

bool IntervalReportNotifier::should_generate_report(Time t) {
    return t==time;
}
