

#include "factory.hpp"

void Factory::add_ramp(Ramp && ramp) { ramps_.add(ramp); }

void Factory::remove_ramp(ElementID id) { ramps_.remove_by_id(id); }

NodeCollection<Ramp>::iterator Factory::find_ramp_by_id(ElementID id){ return ramps_.find_by_id(id); }

NodeCollection<Ramp>::const_iterator Factory::find_ramp_by_id(ElementID id) const { return ramps_.find_by_id(id); }

NodeCollection<Ramp>::const_iterator Factory::ramp_cbegin() const { return ramps_.cbegin(); }

NodeCollection<Ramp>::const_iterator Factory::ramp_cend() const { return ramps_.cend(); }


void Factory::add_worker(Worker && worker) { workers_.add(worker); }

void Factory::remove_worker(ElementID id) {
    remove_receiver(workers_, id);
    workers_.remove_by_id(id);
}

NodeCollection<Worker>::iterator Factory::find_worker_by_id(ElementID id) { return workers_.find_by_id(id); }

NodeCollection<Worker>::const_iterator Factory::find_worker_by_id(ElementID id) const { return workers_.find_by_id(id); }

NodeCollection<Worker>::const_iterator Factory::worker_cbegin() const { return workers_.cbegin(); }

NodeCollection<Worker>::const_iterator Factory::worker_cend() const { return workers_.cend(); }



void Factory::add_storehouse(Storehouse && storehouse) { storehouses_.add(storehouse); }

void Factory::remove_storehouse(ElementID id) {
    remove_receiver(storehouses_, id);
    storehouses_.remove_by_id(id);
}

NodeCollection<Storehouse>::iterator Factory::find_storehouse_by_id(ElementID id) { return storehouses_.find_by_id(id); }

NodeCollection<Storehouse>::const_iterator Factory::find_storehouse_by_id(ElementID id) const { return storehouses_.find_by_id(id); }

NodeCollection<Storehouse>::const_iterator Factory::storehouse_cbegin() const { return storehouses_.cbegin(); }

NodeCollection<Storehouse>::const_iterator Factory::storehouse_cend() const { return storehouses_.cend(); }



bool Factory::has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors) const {
    if (node_colors[sender] == NodeColor::VERIFIED) { return true; }

    node_colors[sender] = NodeColor::VISITED;

    if (sender->receiver_preferences_.get_preferences().empty()) {
        throw std::logic_error("Sender has no defined receivers");
    }

    bool if_sender_has_at_least_one_receiver_other_than_himself = false;

    Worker* worker_ptr;
    PackageSender* sendrecv_ptr;


        for (auto receiver: sender->receiver_preferences_.get_preferences()) {
            if (receiver.first->get_receiver_type() == ReceiverType::STOREHOUSE) {
                if_sender_has_at_least_one_receiver_other_than_himself = true;
            } else if (receiver.first->get_receiver_type() == ReceiverType::WORKER) {
                worker_ptr = dynamic_cast<Worker *>(receiver.first);


                sendrecv_ptr = dynamic_cast<PackageSender *>(worker_ptr);

                if (worker_ptr == sender) {

                    continue;
                }

                if_sender_has_at_least_one_receiver_other_than_himself = true;

                if (node_colors[sendrecv_ptr] == NodeColor::UNVISITED) {
                    Factory::has_reachable_storehouse(sendrecv_ptr, node_colors);
                }
            }
        }

        node_colors[sender] = NodeColor::VERIFIED;

        if (if_sender_has_at_least_one_receiver_other_than_himself) { return true; }

        else { throw std::logic_error("Storehouse is not reachable"); }


}


bool Factory::is_consistent() const {
    std::map<const PackageSender*, NodeColor> colour;
    std::for_each(ramps_.cbegin(),ramps_.cend(),[&colour](const auto& ramp){ colour[dynamic_cast<const PackageSender*> (&ramp)] = NodeColor::UNVISITED;} );
    std::for_each(workers_.cbegin(),workers_.cend(),[&colour](const auto& worker){ colour[dynamic_cast<const PackageSender*> (&worker)] = NodeColor::UNVISITED;} );

    try {
        for(const auto& ramp: ramps_) { has_reachable_storehouse(dynamic_cast<const PackageSender*> (&ramp), colour); }
    }

    catch(std::logic_error& string) {
        std::cout <<  string.what() << "\n";
        return false; }

    return true;
}


void Factory::do_deliveries(Time time) {
    for(auto& ramps: ramps_) { ramps.deliver_goods(time); }
}


void Factory::do_package_passing() {
    for(auto& ramp: ramps_) {
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
        for(auto& ramp: ramps_) {
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
std::pair<std::string,std::string> split_string(std::string s){
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream token_stream(s);
    char delimiter='-';
    while (std::getline(token_stream, token, delimiter)) {
        tokens.push_back(token);
    }
    return std::make_pair(*tokens.begin(),*tokens.rbegin());
}
ParsedLineData parse_line(std::string line) {
    std::vector<std::string> tokens;
    std::string token;
    ParsedLineData object_data = ParsedLineData();
    std::istringstream token_stream(line);
    char delimiter = ' ';
    char delimiter2 = '=';
    std::string ramp ("LOADIG_RAMP");
    std::string worker ("WORKER");
    std::string link ("LINK");
    std::string storehuse ("STOREHOUSE");

    while (std::getline(token_stream, token, delimiter)) {
        tokens.push_back(token);
    }
    for (int i=0;i<int(std::size(tokens));i++) {
        std::istringstream i_token(tokens[i]);
        std::string element;
        std::vector<std::string> data;
        if (i==0){
            while (std::getline(i_token, element, delimiter2)){
                std::cout<<element<<',';
                data.push_back(element);
            }
            if(*data.begin()==ramp){
                object_data.element_type = ElementType::RAMP;
            }
            else if(*data.begin()==worker){
                object_data.element_type = ElementType::WORKER;
            }
            else if(*data.begin()==storehuse){
                object_data.element_type = ElementType::STOREHOUSE;
            }

            else if(*data.begin()==link){
                object_data.element_type = ElementType::LINK;
            }
        }
        else{

            while (std::getline(i_token, element, delimiter2)) {
                data.push_back(element);

            }
            if (object_data.element_type!=ElementType::LINK) {
                object_data.parameters[*data.begin()] = *data.rbegin();
            }


        }
    }
return object_data;
}


PackageQueueType str_queue(std::string s){
    std::string LIFO ("LIFO");
    std::string FIFO("FIFO");
    if (s==LIFO){
        return PackageQueueType::LIFO;
    }
    else{
        return PackageQueueType::FIFO;
    }
}
Factory load_factory_structure(std::istream &is) {
    Factory factory;
    std::string id ("id");
    std::string pt("processing-time");
    std::string qt("queue-type");
    std::string line;
    std::string di ("delivery-interval");
    std::string src ("src");
    std::string dest ("dest");
    std::string ramp ("ramp");
    std::string worker ("worker");
    std::string storehouse("storehouse");
    while (std::getline(is, line)) {
        ParsedLineData object=parse_line(line);
        if (object.element_type==ElementType::RAMP){
            factory.add_ramp(Ramp(std::stoi(object.parameters.find(id)->second), std::stoi(object.parameters.find(di)->second)));
        }
        else if (object.element_type==ElementType::WORKER){
            factory.add_worker(Worker(std::stoi(object.parameters.find(id)->second),
                    std::stoi(object.parameters.find(pt)->second),std::make_unique<PackageQueue>(PackageQueue(str_queue(object.parameters.find(qt)->second)))));
        }
        else if (object.element_type==ElementType::STOREHOUSE){
            factory.add_storehouse(Storehouse(std::stoi(object.parameters.find(id)->second)));
        }
        else if (object.element_type==ElementType::LINK){
            std::pair<std::string,std::string> sender= split_string(object.parameters.find(src)->second);
            std::pair<std::string,std::string> receiver= split_string(object.parameters.find(dest)->second);
            if (sender.first==worker){
                factory.find_worker_by_id(std::stoi(sender.second))->receiver_preferences_.add_receiver(&(*factory.find_storehouse_by_id(std::stoi(receiver.second))));

            }
            else if(sender.first==ramp){
                factory.find_ramp_by_id(std::stoi(sender.second))->receiver_preferences_.add_receiver(&(*factory.find_worker_by_id(std::stoi(sender.second))));
            }
        }
    }
    return factory;



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
std::string worker_to_str(Worker w) {
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
                      [&os](const auto &w) { os << "   worker " << "#" << w.first->get_id() << std::endl; });
    });


    os << "== WORKERS ==" << std::endl << std::endl;
    std::for_each(factory.worker_cbegin(), factory.worker_cend(), [&os](const auto &r) {
        os << "WORKER" << " " << "#" << r.get_id() << std::endl <<
           "  Processing time: " << " " << r.get_processing_duration() << std::endl <<
           "  Queue type: " << queue_type_to_str(r.get_queue_type())<<std::endl
           << "  Receivers:" << std::endl;
        std::for_each(r.get_receiver_preferences().begin(), r.get_receiver_preferences().end(),
                      [&os](const auto &w) { os << "   storehouse " << "#" << w.first->get_id() << std::endl; });
    });
    os << "== STOREHOUSES ==" << std::endl << std::endl;
    std::for_each(factory.worker_cbegin(), factory.worker_cend(),
                  [&os](const auto &r) { os << "WORKER" << " " << "#" << r.get_id() << std::endl; });

}

void generate_simulation_turn_report(Factory& f, std::ostream& os, Time t){

}