

#include "factory.hpp"

void Factory::add_ramp(Ramp && ramp) { rampCollection.add(ramp); }

void Factory::remove_ramp(ElementID id) { rampCollection.remove_by_id(id); }

NodeCollection<Ramp>::iterator Factory::find_ramp_by_id(ElementID id){ return rampCollection.find_by_id(id); }

NodeCollection<Ramp>::const_iterator Factory::find_ramp_by_id(ElementID id) const { return rampCollection.find_by_id(id); }

NodeCollection<Ramp>::const_iterator Factory::ramp_cbegin() const { return rampCollection.cbegin(); }

NodeCollection<Ramp>::const_iterator Factory::ramp_cend() const { return rampCollection.cend(); }


void Factory::add_worker(Worker && worker) { workers_.add(worker); }

void Factory::remove_worker(ElementID id) {
    remove_receiver(workers_, id);
    workers_.remove_by_id(id);
}

NodeCollection<Worker>::iterator Factory::find_worker_by_id(ElementID id) { return workers_.find_by_id(id); }

NodeCollection<Worker>::const_iterator Factory::find_worker_by_id(ElementID id) const { return workers_.find_by_id(id); }

NodeCollection<Worker>::const_iterator Factory::worker_cbegin() const { return workers_.cbegin(); }

NodeCollection<Worker>::const_iterator Factory::worker_cend() const { return workers_.cend(); }



void Factory::add_storehouse(Storehouse && storehouse) { storehouseCollection.add(storehouse); }

void Factory::remove_storehouse(ElementID id) {
    remove_receiver(storehouseCollection, id);
    storehouseCollection.remove_by_id(id);
}

NodeCollection<Storehouse>::iterator Factory::find_storehouse_by_id(ElementID id) { return storehouseCollection.find_by_id(id); }

NodeCollection<Storehouse>::const_iterator Factory::find_storehouse_by_id(ElementID id) const { return storehouseCollection.find_by_id(id); }

NodeCollection<Storehouse>::const_iterator Factory::storehouse_cbegin() const { return storehouseCollection.cbegin(); }

NodeCollection<Storehouse>::const_iterator Factory::storehouse_cend() const { return storehouseCollection.cend(); }



bool Factory::has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& NodeStatus) const {
    if (NodeStatus[sender] == NodeColor::VERIFIED) { return true; }

    NodeStatus[sender] = NodeColor::VISITED;

    if (sender->receiver_preferences_.get_preferences().empty()) {
        throw std::logic_error("Sender has no defined receivers");
    }

    bool node_has_link = false;

    Worker* workerCollectionptr;
    PackageSender* recveiver;


        for (auto receiver: sender->receiver_preferences_.get_preferences()) {
            if (receiver.first->get_receiver_type() == ReceiverType::STOREHOUSE) {
                node_has_link = true;
            } else if (receiver.first->get_receiver_type() == ReceiverType::WORKER) {
                workerCollectionptr = dynamic_cast<Worker *>(receiver.first);


                recveiver = dynamic_cast<PackageSender *>(workerCollectionptr);

                if (workerCollectionptr == sender) {

                    continue;
                }

                node_has_link = true;

                if (NodeStatus[recveiver] == NodeColor::UNVISITED) {
                    Factory::has_reachable_storehouse(recveiver, NodeStatus);
                }
            }
        }

        NodeStatus[sender] = NodeColor::VERIFIED;

        if (node_has_link) { return true; }

        else { throw std::logic_error("Storehouse disconnected"); }


}


bool Factory::is_consistent() const {
    std::map<const PackageSender*, NodeColor> colour;
    std::for_each(rampCollection.cbegin(),rampCollection.cend(),[&colour](const auto& ramp){ colour[dynamic_cast<const PackageSender*> (&ramp)] = NodeColor::UNVISITED;} );
    std::for_each(workers_.cbegin(),workers_.cend(),[&colour](const auto& worker){ colour[dynamic_cast<const PackageSender*> (&worker)] = NodeColor::UNVISITED;} );

    try {
        for(const auto& ramp: rampCollection) { has_reachable_storehouse(dynamic_cast<const PackageSender*> (&ramp), colour); }
    }

    catch(std::logic_error& string) {
        std::cout <<  string.what() << "\n";
        return false; }

    return true;
}


void Factory::do_deliveries(Time time) {
    for(auto& ramps: rampCollection) { ramps.deliver_goods(time); }
}


void Factory::do_package_passing() {
    for(auto& ramp: rampCollection) {
        if(ramp.get_sending_buffer().has_value()) { ramp.send_package(); }
    }
    for(auto& worker: workers_) {
        if(worker.get_sending_buffer().has_value()) { worker.send_package(); }
    }
}


void Factory::do_work(Time time) {
    for(auto& worker: workers_) {
        worker.do_work(time);
    }
}


template<typename Node>
void Factory::remove_receiver( NodeCollection<Node>& collection, ElementID id) {

    if (collection.cbegin()->get_receiver_type() == ReceiverType::WORKER) {
        auto it = find_worker_by_id(id);
        for(auto& worker: workers_) {
            worker.receiver_preferences_.remove_receiver(&(*it));
        }
        for(auto& ramp: rampCollection) {
            ramp.receiver_preferences_.remove_receiver(&(*it));
        }
    }
    else if (collection.cbegin()->get_receiver_type() == ReceiverType::STOREHOUSE) {
        auto it = find_storehouse_by_id(id);
        for(auto& worker: workers_) {
            worker.receiver_preferences_.remove_receiver(&(*it));
        }
    }
}

std::string queue_type_to_str(PackageQueueType p){
    return p==PackageQueueType ::FIFO? "FIFO" : "LIFO";
}
enum class ObjectType{
    WORKER, STOREHOUSE, RAMP, LINK
};
std::string object_type_to_str(ReceiverType r){
    if (r==ReceiverType::WORKER){
        return "worker-";

    }
    else if(r==ReceiverType::STOREHOUSE){
        return "store-";
    }
    return "empty";
}

std::pair<ObjectType, std::map<std::string,std::string>>parse_line(std::string& line) {
    std::vector<std::string> ObjectData;
    std::string token;

    std::istringstream token_stream(line);
    char delimiter = ' ';

    while (std::getline(token_stream, token, delimiter)) {
        ObjectData.push_back(token);
    }

    ObjectType type;

    if (ObjectData[0] == "WORKER")
        type = ObjectType::WORKER;
    else if (ObjectData[0] == "LINK")
        type = ObjectType::LINK;
    else if (ObjectData[0] == "STOREHOUSE")
        type = ObjectType::STOREHOUSE;
    else
        type = ObjectType::RAMP;

    std::string parameter;
    std::string value;
    std::istringstream temp_string;
    std::map<std::string,std::string> map_of_ObjectData;
    for(auto iterator = ObjectData.begin() + 1; iterator!=ObjectData.end(); iterator++){
        temp_string = std::istringstream(*iterator);
        std::getline(temp_string, parameter, '=');
        std::getline(temp_string, value, '=');
        map_of_ObjectData[parameter]=value;
    }

    return std::make_pair(type, map_of_ObjectData);
}


Factory load_factory_structure(std::istream& is){
    Factory factory_;
    std::string line;
    std::vector<std::pair<ObjectType, std::map<std::string,std::string>>> nodes_vector;
    while(std::getline(is, line)) {
        if(line[0] != ';' and line[0] != ' ' and !line.empty()){
            nodes_vector.push_back(parse_line(line));
        }
    }

    std::istringstream i_string;
    std::istringstream o_string;

    std::string source_id;
    std::string source_type;
    std::string dest_id;
    std::string dest_type;

    int temp_id;
    int temp_target_id;
    int processing_time;
    PackageQueueType queue_type;

    IPackageReceiver* receiver_pointer;

    for(auto& elem: nodes_vector){
        if(elem.first == ObjectType::WORKER){
            temp_id = std::stoi(elem.second["id"]);
            processing_time = std::stoi(elem.second["processing-time"]);
            if(elem.second["queue-type"] == "LIFO") queue_type = PackageQueueType::LIFO;
            else queue_type = PackageQueueType::FIFO;
            factory_.add_worker(Worker(temp_id, processing_time, std::make_unique<PackageQueue>(queue_type)));
        }
        if(elem.first == ObjectType::RAMP){
            temp_id = std::stoi(elem.second["id"]);
            processing_time = std::stoi(elem.second["delivery-interval"]);
            factory_.add_ramp(Ramp(temp_id, processing_time));
        }
        if(elem.first == ObjectType::STOREHOUSE){
            temp_id = std::stoi(elem.second["id"]);
            factory_.add_storehouse(Storehouse(temp_id));
        }
        if(elem.first == ObjectType::LINK){
            i_string = std::istringstream(elem.second["src"]);
            o_string = std::istringstream(elem.second["dest"]);

            std::getline(i_string, source_type, '-');
            std::getline(i_string, source_id, '-');
            temp_id = std::stoi(source_id);

            std::getline(o_string, dest_type, '-');
            std::getline(o_string, dest_id, '-');
            temp_target_id = std::stoi(dest_id);

            if(dest_type == "worker"){
                receiver_pointer = &(*factory_.find_worker_by_id(temp_target_id));
            }
            else{
                receiver_pointer = &(*factory_.find_storehouse_by_id(temp_target_id));
            }

            if(source_type == "ramp"){
                (*factory_.find_ramp_by_id(temp_id)).receiver_preferences_.add_receiver(receiver_pointer);
            }
            else{
                (*factory_.find_worker_by_id(temp_id)).receiver_preferences_.add_receiver(receiver_pointer);
            }
        }
    }

    return factory_;
}
void save_factory_structure(Factory& factory, std::ostream& os){
    //WORKER id=<worker-id> processing-time=<processing-time> queue-type=<queue-type>
    //LOADING_RAMP id=<ramp-id> delivery-interval=<delivery-interval>
    //STOREHOUSE id=<storehouse-id>
    os<<"; == LOADING RAMPS =="<<std::endl<<std::endl;
    std::for_each(factory.ramp_cbegin(),factory.ramp_cend(),[&os](const auto& r ){os << "LOADING_RAMP"<<" "<< "id=" << r.get_id() << " " <<"delivery-interval="<<r.get_delivery_interval()<<std::endl ;});
    os<<std::endl<<std::endl;
    os<<"; == WORKERS =="<<std::endl<<std::endl;
    std::for_each(factory.worker_cbegin(),factory.worker_cend(),[&os](const auto& r ){os<< "WORKER"<<" "<< "id=" << r.get_id() << " " <<"processing-time="<<r.get_processing_duration()<<" queue-type="<<queue_type_to_str(r.get_queue_type())<<std::endl;});
    os<<std::endl<<std::endl;
    os<<"; == STOREHOUSES =="<<std::endl<<std::endl;
    std::for_each(factory.storehouse_cbegin(),factory.storehouse_cend(),[&os](const auto& r ){os<<"STOREHOUSE"<<" "<< "id=" << r.get_id()<<std::endl ;});

    os<<"; == LINKS =="<<std::endl<<std::endl;
    std::for_each(factory.ramp_cbegin(),factory.ramp_cend(),[&os](const auto& r ){
        ReceiverPreferences pref=r.receiver_preferences_;
        for (const auto& p : pref) {
            os << "LINK" << " " << "src="<<"ramp-" << r.get_id() << " " <<"dest="<<object_type_to_str(p.first->get_receiver_type())<<p.first->get_id()<<std::endl;
        }
    os<<std::endl;});
    std::for_each(factory.worker_cbegin(),factory.worker_cend(),[&os](const auto& r ){
        ReceiverPreferences pref=r.receiver_preferences_;
        for (const auto& p : pref) {
            os << "LINK" << " " << "src=" <<"worker-"<< r.get_id() << " " <<"dest="<<object_type_to_str(p.first->get_receiver_type())<<p.first->get_id()<<std::endl;
        }
        os<<std::endl;
        });


}





void ramp_to_str(Ramp r,std::ostream& oss) {

    oss << "LOADING_RAMP"<<" "<< "id=" << r.get_id() << " " <<"delivery-interval="<<r.get_delivery_interval()<<std::endl ;

}
void workerCollectionto_str(Worker w,std::ostream& oss) {

    oss << "WORKER"<<" "<< "id=" << w.get_id() << " " <<"processing-time="<<w.get_processing_duration()<<" queue-type="<<queue_type_to_str(w.get_queue_type())<<std::endl ;

}
void storehouse_to_str(Storehouse s, std::ostream &oss){

    oss << "STOREHOUSE"<<" "<< "id=" << s.get_id()<<std::endl ;

}
void link_to_str(Ramp& r,std::ostream& oss){

    ReceiverPreferences pref=r.receiver_preferences_;
    for (const auto& p : pref) {
        oss << "LINK" << " " << "src=" << r.get_id() << " " <<"dest="<<p.first->get_id()<<std::endl;
    }

}
void link_to_str(Worker& w,std::ostream &oss){

    ReceiverPreferences pref=w.receiver_preferences_;
    for (const auto& p : pref) {
        oss << "LINK" << " " << "src=" << w.get_id() << " " <<"dest="<<p.first->get_id()<<std::endl;
    }

}

void generate_structure_report(const Factory& factory,  std::ostream& os) {




   os<<std::endl<< "== LOADING RAMPS ==" << std::endl << std::endl;
    std::for_each(factory.ramp_cbegin(), factory.ramp_cend(), [&os](const auto &r) {
        os << "LOADING RAMP" << " " << "#" << r.get_id() << std::endl <<
           "  Delivery interval:" << " " << r.get_delivery_interval() << std::endl <<
           "  Receivers:" << std::endl;

        for(auto w=r.get_receiver_preferences().cbegin(); w!= r.get_receiver_preferences().cend();++w)
                      { os << "    worker " << "#" << w->first->get_id() << std::endl; }
        os<<std::endl;
    });

    os<<std::endl;
    os << "== WORKERS ==" << std::endl << std::endl;
    std::for_each(factory.worker_cbegin(), factory.worker_cend(), [&os](auto &r) {
        os << "WORKER" << " " << "#" << r.get_id() << std::endl <<
           "  Processing time:" << " " << r.get_processing_duration() << std::endl <<
           "  Queue type: " << queue_type_to_str(r.get_queue_type())<<std::endl
           << "  Receivers:" << std::endl;

      for(auto w=r.get_receiver_preferences().cbegin(); w!= r.get_receiver_preferences().cend();++w)
                   { os << "    storehouse " << "#" << w->first->get_id() << std::endl; }
                      os<<std::endl;
    }

    );
    os<<std::endl;
    os << "== STOREHOUSES ==" << std::endl << std::endl;
    std::for_each(factory.worker_cbegin(), factory.worker_cend(),
                  [&os](const auto &r) { os << "STOREHOUSE" << " " << "#" << r.get_id() << std::endl<<std::endl; });

}

void generate_simulation_turn_report(const Factory& factory, std::ostream& os, Time t){
//    === [ Turn: 2 ] ===
//
//    == WORKERS ==
//
//       WORKER #1
//    PBuffer: #1 (pt = 1)
//    Queue: #5
//    SBuffer: (empty)
//
//        WORKER #2
//    PBuffer: (empty)
//        Queue: #6, #7
//    SBuffer: #3
//
//
//              == STOREHOUSES ==
//
//              STOREHOUSE #1
//    Stock: #2, #4
    os<<"=== [ Turn: "<<t<<" ] ==="<<std::endl<<std::endl;
    os << "== WORKERS ==" << std::endl << std::endl;

    std::for_each(factory.worker_cbegin(), factory.worker_cend(), [&os](const auto &r) {


        os << "WORKER" << " " << "#" << r.get_id() << std::endl;
        if(r.get_processing_buffer().has_value()) {
            os << "  PBuffer:" << " #" << r.get_processing_buffer()->get_id() << " (pt = "
               << r.get_package_processing_start_time() << ")" << std::endl;
        } else{
            os<<"  PBuffer: "<<"(empty)"<<std::endl;
        }
          os<< "  Queue: ";
        if (r.size()){
            os<<"#"<<r.get_queue()->begin()->get_id();
            std::for_each(r.get_queue()->begin()+1, r.get_queue()->end(),
                          [&os](const auto &w) { os <<", "<< "#" << w.get_id() ; });

        }
        else{
            os<<"(empty)";

        }


        os <<std::endl;
        if(r.get_sending_buffer().has_value()){
        os  << "  SBuffer: #" <<r.get_sending_buffer()->get_id();
            os<<std::endl;
            os<<std::endl;
        }
        else{
            os<<"  SBuffer: (empty)";
            os<<std::endl;
            os<<std::endl;
        }



    });

    os<<std::endl << "== STOREHOUSES ==" << std::endl << std::endl;
    std::for_each(factory.storehouse_cbegin(), factory.storehouse_cend(),
                  [&os](const auto &r) { os << "STOREHOUSE" << " " << "#" << r.get_id() << std::endl;
                      os<< "  Stock: ";
                      if (r.size()==1){
                          os<<"#"<<r.get_queue()->cbegin()->get_id();

                      }
                      else if(r.size()>1){
                          os<<r.get_queue()->begin()->get_id();
                          for(auto w=r.get_queue()->cbegin()+1; w!= r.get_queue()->cend();++w)
                          { os <<", "<< "#" << w->get_id() ; }
                      }
                      else{
                          os<<"(empty)";
                      }


                      os <<std::endl;
                      os <<std::endl;

    });

}
