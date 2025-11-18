#ifndef TOP_HPP
#define TOP_HPP

#include "cadmium/modeling/devs/coupled.hpp"
#include "growth_atomic.hpp"

using namespace cadmium;

struct topSystem : public Coupled {
    topSystem(const std::string& id) : Coupled(id) {

        // Add growth model
        auto growth_atomic_model = addComponent<growth_atomic>("growth_atomic");

        // No input/output ports or couplings yet
    }
};

#endif
