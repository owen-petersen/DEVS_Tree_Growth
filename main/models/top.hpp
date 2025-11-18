#ifndef TOP_HPP
#define TOP_HPP

#include "cadmium/modeling/devs/coupled.hpp"

#include "models/growth_atomic.hpp"
#include "enums/tree_type_en.hpp"
#include "util/json_iestream.hpp"


using namespace cadmium;
struct topSystem : public Coupled {

    // The Ports

    topSystem(const std::string& id) : Coupled(id) {
        // Initialize ports


        // Components of this coupled model
        auto growth_atomic_model = addComponent<growth_atomic>("growth_atomic");
        auto json_input_file = addComponent<lib::JsonIEStream<TreeTypeEn>>("tree_type_in file", "/home/owen/DEVS/DEVS_Tree_Growth/test_input_json.json");

        // Internal Couplings
        addCoupling(json_input_file->out, growth_atomic_model->tree_type_in);

        // External Input Couplings
        
        // External Output Couplings
        
    }
};

#endif