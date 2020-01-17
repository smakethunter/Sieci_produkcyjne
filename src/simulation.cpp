//
// Created by smaket on 17.01.2020.
//
#include "simulation.hpp"
void simulate( Factory& f, TimeOffset d,  std::function<void (Factory&, Time)>){


        for (int t=1;t<d+1;++t){

            f.do_deliveries(t);

            f.do_package_passing();
            f.do_work(t);



        }
    }




