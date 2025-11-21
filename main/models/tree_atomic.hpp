#ifndef TREE_ATOMIC_HPP
#define TREE_ATOMIC_HPP

#include <algorithm>
#include <limits>
#include <ostream>
#include <sstream>
#include <string>

struct TreeState {
    int years = 0;
    double height = 0.0;
    double crown = 0.0;
    bool produce = false;
};

inline std::ostream& operator<<(std::ostream& os, const TreeState& s) {
    os << "years=" << s.years
       << ",height=" << s.height
       << ",crown=" << s.crown;
    return os;
}

#include <cadmium/core/modeling/atomic.hpp>
#include <cadmium/core/modeling/port.hpp>

struct TreeDefs {
    static constexpr const char* IN_TICK  = "tick_in";
    static constexpr const char* OUT_PORT = "out";
};

class TreeAtomic : public cadmium::Atomic<TreeState> {
public:
    TreeAtomic(const std::string& id)
        : cadmium::Atomic<TreeState>(id, TreeState{}) {

        this->addInPort<bool>(TreeDefs::IN_TICK);
        this->addOutPort<std::string>(TreeDefs::OUT_PORT);
    }

    void internalTransition(TreeState& s) const override {
        s.produce = false;
    }

    void externalTransition(TreeState& s, double) const override {
        auto inPort = std::dynamic_pointer_cast<cadmium::_Port<bool>>(
            this->getInPort(TreeDefs::IN_TICK));

        for (bool tick : inPort->getBag()) {
            if (tick) {
                s.years++;
                s.height += 3.0;                          // 3 ft/year
                s.crown = std::min(20.0, double(s.years)); // cap at 20 ft
                s.produce = true;
            }
        }
        inPort->clear();
    }

    void output(const TreeState& s) const override {
        if (!s.produce) return;

        auto outPort = std::dynamic_pointer_cast<cadmium::_Port<std::string>>(
            this->getOutPort(TreeDefs::OUT_PORT));

       
        std::string msg = std::to_string(s.height) + "," + std::to_string(s.crown);
        outPort->addMessage(msg);
    }

    double timeAdvance(const TreeState& s) const override {
        return s.produce ? 0.0 : std::numeric_limits<double>::infinity();
    }

    std::string logState() const override {
        std::stringstream ss;
        ss << state;        
        return ss.str();
    }
};

#endif // TREE_ATOMIC_HPP


