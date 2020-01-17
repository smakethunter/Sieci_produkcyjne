//
// Created by smaket on 08.12.2019.
//

#include "nodes.hpp"

Worker::Worker(ElementID id_, TimeOffset to_, std::unique_ptr<IPackageQueue> q ) :
       PackageSender(), IPackageReceiver() ,id(id_),to(to_),processing_start_time(0), queue_pointer(std::move(q)),receiverType(ReceiverType::WORKER){

}


void Worker::receive_package(Package &&p) {


    queue_pointer->push(std::move(p));

    //push_package(queue_pointer->pop());
    if(!processing_buffer.has_value()) {
        start_process(queue_pointer->pop());
    }
}

void Worker::do_work(Time t) {
if (processing_start_time==0){
    processing_start_time=t;
}
if (to==t-processing_start_time){

    push_package(std::move(*processing_buffer));
    processing_start_time=0;
    processing_buffer.reset();
}


}

void Worker::start_process(Package&& p) {

    processing_buffer.emplace(std::move(p));

}

std::optional<Package> &Worker::get_processing_buffer() {
    return processing_buffer;
}

void Storehouse::receive_package(Package &&p) {
    queue_pointer->push(std::move(p));

}




void ReceiverPreferences::add_receiver(IPackageReceiver * r) {

   preferences_map.insert(std::make_pair(r,0));
    double len=double(std::size(preferences_map));
   for (auto& i : preferences_map){
       i.second=1/len;
   }
   /*int all=int(std::size(preferences_map));
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
}*/


}

void ReceiverPreferences::remove_receiver(IPackageReceiver* r) {
    preferences_map.erase(r);
    double len=double(std::size(preferences_map));
    for (auto& i : preferences_map){
        i.second=1/len;
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


    if (!sending_buffer.has_value()){
        sending_buffer = std::make_optional<Package>(std::move(p));
    }

}

std::optional<Package>& PackageSender::get_sending_buffer() {

    return sending_buffer;
}

void PackageSender::send_package() {
    if(sending_buffer.has_value()){
    Package p(std::move(sending_buffer.value()));


    IPackageReceiver* reciever=receiver_preferences_.choose_receiver();



    reciever->receive_package(std::move(p));
    sending_buffer.reset();
    }
}

void Ramp::deliver_goods(Time t) {
    TimeOffset interval=get_delivery_interval();

   if(!sending_buffer.has_value()) {
       if ((t-1) % interval == 0 || t==1) {

           push_package(Package());

       }
   }


}


// funkcje testyjace na szybkosci dystrybuante

