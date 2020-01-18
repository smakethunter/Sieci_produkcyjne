

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
    PackageSender* sendrecv_ptr;


        for (auto receiver: sender->receiver_preferences_.get_preferences()) {
            if (receiver.first->get_receiver_type() == ReceiverType::STOREHOUSE) {
                node_has_link = true;
            } else if (receiver.first->get_receiver_type() == ReceiverType::WORKER) {
                workerCollectionptr = dynamic_cast<Worker *>(receiver.first);


                sendrecv_ptr = dynamic_cast<PackageSender *>(workerCollectionptr);

                if (workerCollectionptr == sender) {

                    continue;
                }

                node_has_link = true;

                if (NodeStatus[sendrecv_ptr] == NodeColor::UNVISITED) {
                    Factory::has_reachable_storehouse(sendrecv_ptr, NodeStatus);
                }
            }
        }

        NodeStatus[sender] = NodeColor::VERIFIED;

        if (node_has_link) { return true; }

        else { throw std::logic_error("Storehouse is not reachable"); }


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


enum class ObjectType{WORKER, STOREHOUSE, RAMP, LINK};

std::pair<ObjectType, std::map<std::string,std::string>>parse_line(std::string& line) {
    std::vector<std::string> tokens;
    std::string token;

    std::istringstream token_stream(line);
    char delimiter = ' ';

    while (std::getline(token_stream, token, delimiter)) {
        tokens.push_back(token);
    }

    ObjectType type;

    if (tokens[0] == "WORKER")
        type = ObjectType::WORKER;
    else if (tokens[0] == "LINK")
        type = ObjectType::LINK;
    else if (tokens[0] == "STOREHOUSE")
        type = ObjectType::STOREHOUSE;
    else
        type = ObjectType::RAMP;

    std::string temp1;
    std::string temp2;
    std::istringstream temp_string;
    std::map<std::string,std::string> map_of_tokens;
    for(auto iterator = tokens.begin() + 1; iterator!=tokens.end(); iterator++){
        temp_string = std::istringstream(*iterator);
        std::getline(temp_string, temp1, '=');
        std::getline(temp_string, temp2, '=');
        map_of_tokens[temp1]=temp2;
    }

    return std::make_pair(type, map_of_tokens);
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
    std::string temp_dest_id;
    std::string temp_dest_type;

    int temp_id;
    int temp_target_id;
    int temp_processing_time;
    PackageQueueType queue_type;

    IPackageReceiver* receiver_pointer;

    for(auto& elem: nodes_vector){
        if(elem.first == ObjectType::WORKER){
            temp_id = std::stoi(elem.second["id"]);
            temp_processing_time = std::stoi(elem.second["processing-time"]);
            if(elem.second["queue-type"] == "LIFO") queue_type = PackageQueueType::LIFO;
            else queue_type = PackageQueueType::FIFO;
            factory_.add_worker(Worker(temp_id, temp_processing_time, std::make_unique<PackageQueue>(queue_type)));
        }
        if(elem.first == ObjectType::RAMP){
            temp_id = std::stoi(elem.second["id"]);
            temp_processing_time = std::stoi(elem.second["delivery-interval"]);
            factory_.add_ramp(Ramp(temp_id, temp_processing_time));
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

            std::getline(o_string, temp_dest_type, '-');
            std::getline(o_string, temp_dest_id, '-');
            temp_target_id = std::stoi(temp_dest_id);

            if(temp_dest_type == "worker"){
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
    os<<"; == WORKERS =="<<std::endl<<std::endl;
    std::for_each(factory.worker_cbegin(),factory.worker_cend(),[&os](const auto& r ){os<< "WORKER"<<" "<< "id=" << r.get_id() << " " <<"processing-time="<<r.get_processing_duration()<<std::endl;});
    os<<"; == STOREHOUSES =="<<std::endl<<std::endl;
    std::for_each(factory.storehouse_cbegin(),factory.storehouse_cend(),[&os](const auto& r ){os<<"STOREHOUSE"<<" "<< "id=" << r.get_id()<<std::endl ;});

    os<<"; == LINKS =="<<std::endl<<std::endl;
    std::for_each(factory.ramp_cbegin(),factory.ramp_cend(),[&os](const auto& r ){std::ostringstream oss;
        ReceiverPreferences pref=r.receiver_preferences_;
        for (const auto& p : pref) {
            os << "LINK" << " " << "src=" << r.get_id() << " " <<"dest="<<p.first->get_id()<<std::endl;
        }});
    std::for_each(factory.worker_cbegin(),factory.worker_cend(),[&os](const auto& r ){std::ostringstream oss;
        ReceiverPreferences pref=r.receiver_preferences_;
        for (const auto& p : pref) {
            os << "LINK" << " " << "src=" << r.get_id() << " " <<"dest="<<p.first->get_id()<<std::endl;
        };});
}





std::string ramp_to_str(Ramp r) {
    std::ostringstream oss;
    oss << "LOADING_RAMP"<<" "<< "id=" << r.get_id() << " " <<"delivery-interval="<<r.get_delivery_interval()<<std::endl ;
    return oss.str();
}
std::string workerCollectionto_str(Worker w) {
    std::ostringstream oss;
    oss << "WORKER"<<" "<< "id=" << w.get_id() << " " <<"processing-time="<<w.get_processing_duration()<<std::endl ;
    return oss.str();
}
std::string storehouse_to_str(Storehouse s){
    std::ostringstream oss;
    oss << "STOREHOUSE"<<" "<< "id=" << s.get_id()<<std::endl ;
    return oss.str();
}
std::string link_to_str(Ramp& r){
    std::ostringstream oss;
    ReceiverPreferences pref=r.receiver_preferences_;
    for (const auto& p : pref) {
        oss << "LINK" << " " << "src=" << r.get_id() << " " <<"dest="<<p.first->get_id()<<std::endl;
    }
    return oss.str();
}
std::string link_to_str(Worker& w){
    std::ostringstream oss;
    ReceiverPreferences pref=w.receiver_preferences_;
    for (const auto& p : pref) {
        oss << "LINK" << " " << "src=" << w.get_id() << " " <<"dest="<<p.first->get_id()<<std::endl;
    }
    return oss.str();
}
std::string queue_type_to_str(PackageQueueType p){
    return p==PackageQueueType ::FIFO? "FIFO" : "LIFO";
}
void generate_structure_report(Factory& factory,  std::ostream& os) {




    os << "== LOADING RAMPS ==" << std::endl << std::endl;
    std::for_each(factory.ramp_cbegin(), factory.ramp_cend(), [&os](const auto &r) {
        os << "LOADING_RAMP" << " " << "#" << r.get_id() << std::endl <<
           "  Delivery interval: " << " " << r.get_delivery_interval() << std::endl <<
           "  Receivers:" << std::endl;
        std::for_each(r.get_receiver_preferences().begin(), r.get_receiver_preferences().end(),
                      [&os](const auto &w) { os << "    worker " << "#" << w.first->get_id() << std::endl; });
    });


    os << "== WORKERS ==" << std::endl << std::endl;
    std::for_each(factory.worker_cbegin(), factory.worker_cend(), [&os](const auto &r) {
        os << "WORKER" << " " << "#" << r.get_id() << std::endl <<
           "  Processing time: " << " " << r.get_processing_duration() << std::endl <<
           "  Queue type: " << queue_type_to_str(r.get_queue_type())<<std::endl
           << "  Receivers:" << std::endl;
        std::for_each(r.get_receiver_preferences().begin(), r.get_receiver_preferences().end(),
                      [&os](const auto &w) { os << "    storehouse " << "#" << w.first->get_id() << std::endl; });
    });
    os << "== STOREHOUSES ==" << std::endl << std::endl;
    std::for_each(factory.worker_cbegin(), factory.worker_cend(),
                  [&os](const auto &r) { os << "STOREHOUSE" << " " << "#" << r.get_id() << std::endl; });

}

void generate_simulation_turn_report(Factory& factory, std::ostream& os, Time t){
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
            os << "  PBuffer: " << " #" << r.get_processing_buffer()->get_id() << " (pt = "
               << r.get_package_processing_start_time() << ")" << std::endl;
        } else{
            os<<"  PBuffer: "<<"(empty)"<<std::endl;
        }
          os<< "  Queue: ";
        if (r.get_queue()){
            os<<r.get_queue()->begin()->get_id();
            std::for_each(r.get_queue()->begin()+1, r.get_queue()->end(),
                          [&os](const auto &w) { os <<", "<< "#" << w.get_id() ; });
        }
        else{
            os<<"(empty)";
        }


        os <<std::endl;
        if(r.get_sending_buffer().has_value()){
        os  << "  SBuffer: #" <<r.get_sending_buffer()->get_id();
        }
        else{
            os<<"  SBuffer: (empty)";

        }
        os<<std::endl;


    });
    os<<std::endl<<std::endl;
    os << "== STOREHOUSES ==" << std::endl << std::endl;
    std::for_each(factory.worker_cbegin(), factory.worker_cend(),
                  [&os](const auto &r) { os << "STOREHOUSE" << " " << "#" << r.get_id() << std::endl;
                      os<< "  Stock: ";
                      if (r.get_queue()){
                          os<<r.get_queue()->begin()->get_id();
                          std::for_each(r.get_queue()->begin()+1, r.get_queue()->end(),
                                        [&os](const auto &w) { os <<", "<< "#" << w.get_id() ; });
                      }
                      else{
                          os<<"(empty)";
                      }


                      os <<std::endl;

    });

}
