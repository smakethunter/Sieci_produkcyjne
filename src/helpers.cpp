//
// Created by smaket on 07.01.2020.
//
#include <random>
#include "helpers.hpp"

double prob_rand() {return (double)rand()/RAND_MAX;}
void add_nr(int nr, std::map<int,double>& map) {

    map.insert(std::make_pair(nr, 0));
    int all = int(std::size(map));
    std::vector<double> random_values;
    random_values.reserve(all);
    for (int i = 0; i < all; i++) {
        random_values.push_back(prob_rand());

    }

    double sum_of_elems = std::accumulate(random_values.begin(), random_values.end(), double(0));

    for (auto &i:random_values) {
        i = i / sum_of_elems;
    }
    int a = 0;
    for (auto &i: map) {
        i.second = random_values[a];
        a++;
    }


}

int remove_nr(int nr, std::map<int,double> &preferences_map) {
    preferences_map.erase(nr);
    int all=int(std::size(preferences_map));
    std::vector<double> random_values;
    random_values.reserve(all);
    for (int i = 0;i < all; i++){
        random_values.push_back(prob_rand());
    }
    double sum_of_elems = std::accumulate(random_values.begin(), random_values.end(),double(0));

    for (auto& i:random_values){
        i/=sum_of_elems;

    }
    int a=0;
    for (auto& i: preferences_map){
        i.second=random_values[a];
        a++;
    }
    return int(std::size(preferences_map));
}

int choose_nr(double mocked_prob,std::map<int,double> &preferences_map) {
    std::vector<double> benchmarks;
    benchmarks.push_back(0);
    double c = 0;
    for (auto &i: preferences_map) {
        c += i.second;
        benchmarks.push_back(c);

    }

    double receiverRandom=mocked_prob;
    for (int i=0;i<int(std::size(benchmarks));i++){
        if (benchmarks[i] < receiverRandom && receiverRandom <= benchmarks[i + 1]){
            int a=0;
            for (const auto& elem: preferences_map){
                if (a==i){

                    return elem.first;
                }

                else{
                    a++;
                }

            }
        }
    }
    return (preferences_map.begin())->first;


}