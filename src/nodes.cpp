//
// Created by smaket on 08.12.2019.
//

#include "nodes.hpp"

Worker::Worker(ElementID id_, TimeOffset to_, std::unique_ptr<IPackageQueue> q) :
       PackageSender(), IPackageReceiver(ReceiverType::Worker),id(id_),to(to_),processing_start_time(0), queue_pointer(std::move(q)){}


void Worker::receive_package(Package &&p) {


    queue_pointer->push(std::move(p));

    push_package(queue_pointer->pop());


}

void Worker::do_work(Time t) {
if (processing_start_time==0){
    processing_start_time=t;
}
if (to==t-processing_start_time){
    send_package();
    processing_start_time=0;
}


}

void Storehouse::receive_package(Package &&p) {
    queue_pointer->push(std::move(p));

}



void ReceiverPreferences::add_receiver(IPackageReceiver * r) {

   preferences_map.insert(std::make_pair(r,0));
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


}

void ReceiverPreferences::remove_receiver(IPackageReceiver * r) {
    preferences_map.erase(r);
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
}

IPackageReceiver *ReceiverPreferences::choose_receiver() {
    std::vector<double> benchmarks;
    benchmarks.push_back(0);
    double c=0;
    for (const auto& i: preferences_map){
        c+=i.second;
        benchmarks.push_back(c);
    }
    double receiverRandom=prob_rand();
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


void PackageSender::push_package(Package &&p) {
    if(!sending_buffer){

        sending_buffer.emplace(std::move(p));
    }

}

std::optional<Package> PackageSender::get_sending_buffer() {

    return std::move(sending_buffer);
}

void PackageSender::send_package() {
    Package p(std::move(sending_buffer.value()));


    IPackageReceiver* reciever=receiver_preferences.choose_receiver();



    reciever->receive_package(std::move(p));
    sending_buffer.reset();
}

void Ramp::deliver_goods(Time t) {
    TimeOffset interval=get_delivery_interval();
   if(sending_buffer.has_value()){
        if(t%interval==0){

            send_package();

        }
    }


}
// funkcje testyjace na szybkosci dystrybuante

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

    return int(mocked_prob);
}