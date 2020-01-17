
#ifndef NETSIM_FACTORY_HPP
#define NETSIM_FACTORY_HPP

#include "nodes.hpp"
#include <list>
template<typename Node>

class NodeCollection {
public:

    explicit NodeCollection<Node>(): nodes_() {};

    using list = typename std::list<Node>;
    using iterator = typename list::iterator;
    using const_iterator = typename list::const_iterator;

    iterator begin()    { return nodes_.begin(); }
    iterator end()  { return nodes_.end(); }
    const_iterator begin() const  { return nodes_.cbegin(); }
    const_iterator end() const  { return nodes_.cend(); }
    const_iterator cbegin() const  { return nodes_.cbegin(); }
    const_iterator cend() const  { return nodes_.cend(); }

    void add(Node& node) { nodes_.push_back(std::move(node)); }

    void remove_by_id(ElementID id_) {
        auto del_it = find_by_id(id_);
        if(del_it != nodes_.cend()){
            nodes_.erase(del_it);
        }
    }

    NodeCollection<Node>::iterator find_by_id(ElementID id_) {
        return std::find_if(nodes_.begin(), nodes_.end(), [id_](const auto& it){ return it.get_id() == id_;} );
    }

    NodeCollection<Node>::const_iterator find_by_id(ElementID id_) const {
        return std::find_if(nodes_.cbegin(), nodes_.cend(),[id_](const auto& it){ return it.get_id() == id_;} );
    }

private:
    std::list<Node> nodes_;
};




template class NodeCollection<Ramp>;
template class NodeCollection<Worker>;
template class NodeCollection<Storehouse>;

enum class NodeColor { UNVISITED, VISITED, VERIFIED };

class Factory {
public:
    Factory() : ramps_(), workers_(), storehouses_() {};

    void add_ramp(Ramp && );
    void remove_ramp(ElementID id);

    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id);
    NodeCollection<Ramp>::const_iterator find_ramp_by_id(ElementID id) const;

    NodeCollection<Ramp>::const_iterator ramp_cbegin() const;
    NodeCollection<Ramp>::const_iterator ramp_cend() const;
    NodeCollection<Ramp>::iterator ramp_begin() { return ramps_.begin();};
    NodeCollection<Ramp>::iterator ramp_end() {return ramps_.end();};

    void add_worker(Worker && );
    void remove_worker(ElementID id);

    NodeCollection<Worker>::iterator find_worker_by_id(ElementID id);
    NodeCollection<Worker>::const_iterator find_worker_by_id(ElementID id) const;

    NodeCollection<Worker>::const_iterator worker_cbegin() const;
    NodeCollection<Worker>::const_iterator worker_cend() const;



    void add_storehouse(Storehouse && );
    void remove_storehouse(ElementID id);

    NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id);
    NodeCollection<Storehouse>::const_iterator find_storehouse_by_id(ElementID id) const;

    NodeCollection<Storehouse>::const_iterator storehouse_cbegin() const;
    NodeCollection<Storehouse>::const_iterator storehouse_cend() const;


    bool is_consistent() const;
    void do_deliveries(Time time);
    void do_package_passing();
    void do_work(Time time);


    bool has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors) const;

private:

    template<typename Node>
    void remove_receiver( NodeCollection<Node>& collection, ElementID id);

    NodeCollection<Ramp> ramps_;
    NodeCollection<Worker> workers_;
    NodeCollection<Storehouse> storehouses_;
};


//void save_factory_structure(Factory& factory, std::ostream& os);
enum class ElementType{
    WORKER,RAMP,STOREHOUSE,LINK
};
struct ParsedLineData {
    ElementType element_type;
    std::map< std::string,std::string> parameters;


};
Factory load_factory_structure(std::istream &is);
void save_factory_structure(Factory& factory, std::ostream& os);
std::string ramp_to_str(Ramp r);
std::string worker_to_str(Worker w);
std::string storehouse_to_str(Storehouse s);

std::string link_to_str(Ramp& r);
std::string link_to_str(Worker& w);
void generate_structure_report(Factory& f,  std::ostream& os);
void generate_simulation_turn_report(Factory& f, std::ostream& os, Time t);

#endif //NETSIM_FACTORY_HPP
