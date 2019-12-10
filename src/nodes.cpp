//
// Created by smaket on 08.12.2019.
//

#include "nodes.hpp"

Worker::Worker(ElementID id_, TimeOffset to_, std::unique_ptr<IPackageQueue> q) :
        PackageSender(),id(id_),to(to_),queue_pointer(std::move(q)){}
double generate_random(){
    srand(time(nullptr));
    return double(rand()/RAND_MAX);
}

void ReceiverPreferences::add_receiver(IPackageReceiver * r) {

   preferences_map.insert(std::make_pair(r,0));
   int all=int(std::size(preferences_map));
   std::vector<double> random_values;
   random_values.reserve(all);
for (int i = 0;i < all; i++){
    random_values.push_back(generate_random());
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


}

void ReceiverPreferences::remove_receiver(IPackageReceiver * r) {
    preferences_map.erase(r);
    int all=int(std::size(preferences_map));
    std::vector<double> random_values;
    random_values.reserve(all);
    for (int i = 0;i < all; i++){
        random_values.push_back(generate_random());
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
}

IPackageReceiver *ReceiverPreferences::choose_receiver() {
    std::vector<double> benchmarks;
    benchmarks.push_back(0);
    double c=0;
    for (const auto& i: preferences_map){
        c+=i.second;
        benchmarks.push_back(c);
    }
    double receiverRandom=generate_random();
    for (int i=0;i<int(std::size(benchmarks));){
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
