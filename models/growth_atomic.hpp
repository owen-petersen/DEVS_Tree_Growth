#ifndef GROWTH_ATOMIC_HPP
#define GROWTH_ATOMIC_HPP

#include <iostream>
#include <sstream>
#include <limits>
#include "cadmium/modeling/devs/atomic.hpp"

using namespace cadmium;

enum class GrowthStateEn : int {
    passive = 0,
    growing = 1
};

struct growth_atomic_state_st {
    GrowthStateEn state;
    double sigma;

    // Oak parameters
    double height_m;
    double trunk_radius_m;
    double crown_radius_m;
    int age_years;

    // Extra legacy fields
    double base_size;
    double rate;
    double scale;

    std::string type;
    std::string shape;

    growth_atomic_state_st()
        : state(GrowthStateEn::passive)
        , sigma(1.0)
        , height_m(0.0)
        , trunk_radius_m(0.05)
        , crown_radius_m(0.5)
        , age_years(0)
        , base_size(1.0)
        , rate(0.0)
        , scale(1.0)
        , type("oak")
        , shape("tree_mesh")
    {}
};

// Logger-friendly state output
inline std::ostream& operator<<(std::ostream& out, const growth_atomic_state_st& s) {
    out << "{"
        << "\"age_years\":" << s.age_years << ","
        << "\"height_m\":" << s.height_m << ","
        << "\"trunk_radius_m\":" << s.trunk_radius_m << ","
        << "\"crown_radius_m\":" << s.crown_radius_m << ","
        << "\"type\":\"" << s.type << "\""
        << "}";
    return out;
}

class growth_atomic : public Atomic<growth_atomic_state_st> {
public:
    growth_atomic(const std::string& id)
        : Atomic<growth_atomic_state_st>(id, growth_atomic_state_st()) {}

    void internalTransition(growth_atomic_state_st& s) const override {
        s.age_years++;

        // Height growth
        if (s.age_years <= 10)
            s.height_m += 0.914;
        else
            s.height_m += 0.457;

        // Trunk radius growth
        s.trunk_radius_m += 0.00381;

        // Crown radius growth
        double crown_inc =
            (s.age_years <= 10) ? 0.50 :
            (s.age_years <= 40) ? 0.30 : 0.10;

        s.crown_radius_m += crown_inc;

        double max_crown = std::min(12.0, 0.6 * s.height_m);
        if (s.crown_radius_m > max_crown)
            s.crown_radius_m = max_crown;

        s.sigma = 1.0;
        s.state = GrowthStateEn::passive;
    }

    void externalTransition(growth_atomic_state_st& s, double e) const override {
        s.sigma -= e;
        if (s.sigma < 0) s.sigma = 0;
    }

    void output(const growth_atomic_state_st& s) const override {
        // No port outputs
    }

    double timeAdvance(const growth_atomic_state_st& s) const override {
        return s.sigma;
    }
};

#endif
