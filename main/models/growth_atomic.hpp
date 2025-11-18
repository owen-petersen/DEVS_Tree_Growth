#ifndef GROWTH_HPP
#define GROWTH_HPP

#include <iostream>
#include <iomanip>
#include <limits>
#include "cadmium/modeling/devs/atomic.hpp"

#include "enums/tree_type_en.hpp"

using namespace cadmium;
using limits = std::numeric_limits<double>;

enum class GrowthStateEn : int {
    passive = 0,
    growing = 1
};

struct growth_atomic_state_st {
    GrowthStateEn state;
    double sigma;
    double base_size; // width & depth use this
    double height;
    double rate;      // growth rate (units/sec) (I think we can get rid of this? Not sure though)
    double scale;     // height scale multiplier
    TreeTypeEn type;
    std::string shape;

    // constructor
    explicit growth_atomic_state_st():
        state(GrowthStateEn::passive), 
        sigma(limits::infinity()),
        base_size(1.0),
        height(0.0),
        rate(0.0),
        scale(1.0),
        type(TreeTypeEn::oak),
        shape("rect_prism")
        {}
};

std::ostream& operator<<(std::ostream &out, const growth_atomic_state_st& state) {
    out  << "<" << "next state=" << (int)state.state << "," 
    << "type=" << (int)state.type << ","
    << "base_size=" << state.base_size << ","
    << "height=" << state.height << ">";
    return out;
}

class growth_atomic : public Atomic<growth_atomic_state_st> {
    private:
        
    public:
    // Input ports
    Port<TreeTypeEn> tree_type_in;
    
    // Output ports
    

    growth_atomic(const std::string id) : Atomic<growth_atomic_state_st>(id, growth_atomic_state_st())
    {
        // Initialize ports
        tree_type_in = addInPort<TreeTypeEn>("tree_type_in");
    }

    void internalTransition(growth_atomic_state_st& state) const override {
        switch(state.state) {
        case GrowthStateEn::passive:
            state.scale = state.scale + 0.5;

            state.state = GrowthStateEn::growing;
            state.sigma = 2.0;
            break;
        case GrowthStateEn::growing:
            state.height = state.base_size * state.scale;

            state.state = GrowthStateEn::passive;
            state.sigma = 1.0;
            break;
        default:
            break;
        }
    }

    // external transition
    void externalTransition(growth_atomic_state_st& state, double e) const override {
        if (!tree_type_in->empty()) {
            for (const auto &input : tree_type_in->getBag()) {
                state.type = input;
                state.state = GrowthStateEn::growing;
            }
        }
        state.sigma = 1.0;
    }
    
    // output function
    void output(const growth_atomic_state_st& state) const override {
        // No outputs right now
    }

    // time_advance function
    [[nodiscard]] double timeAdvance(const growth_atomic_state_st& state) const override {
        return state.sigma;
    }
};

#endif