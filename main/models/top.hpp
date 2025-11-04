#ifndef TOP_HPP
#define TOP_HPP

#include "cadmium/modeling/devs/coupled.hpp"

#include "growth_atomic.hpp"


using namespace cadmium;
struct topSystem : public Coupled {

    // The Ports


    topSystem(const std::string& id) : Coupled(id) {
        // Initialize ports


        // Components of this coupled model
        auto growth_atomic_model = addComponent<growth_atomic>("growth_atomic");

        // Internal Couplings
        
        // External Input Couplings
        
        // External Output Couplings
        
    }
};

#endif