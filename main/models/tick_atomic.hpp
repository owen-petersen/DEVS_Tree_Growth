#ifndef TICK_ATOMIC_HPP
#define TICK_ATOMIC_HPP

#include <limits>
#include <ostream>
#include <sstream>

struct TickState {
    int ticks = 0;
    int max_ticks = 50;
};

// Make TickState printable BEFORE Atomic<TickState> is instantiated
inline std::ostream& operator<<(std::ostream& os, const TickState& s) {
    os << "ticks=" << s.ticks << "/" << s.max_ticks;
    return os;
}

#include <cadmium/core/modeling/atomic.hpp>
#include <cadmium/core/modeling/port.hpp>

struct TickDefs {
    static constexpr const char* OUT_PORT = "out";
};

class TickAtomic : public cadmium::Atomic<TickState> {
public:
    TickAtomic(const std::string& id, int max)
        : cadmium::Atomic<TickState>(id, TickState{0, max}) {

        // Create output port
        this->addOutPort<bool>(TickDefs::OUT_PORT);
    }

    // internal transition: advance tick count
    void internalTransition(TickState& s) const override {
        s.ticks++;
    }

    // no external inputs
    void externalTransition(TickState&, double) const override {}

    // output: send a boolean tick while we still have ticks left
    void output(const TickState& s) const override {
        if (s.ticks < s.max_ticks) {
            auto port = std::dynamic_pointer_cast<cadmium::_Port<bool>>(
                this->getOutPort(TickDefs::OUT_PORT));
            port->addMessage(true);
        }
    }

    double timeAdvance(const TickState& s) const override {
        if (s.ticks >= s.max_ticks) {
            return std::numeric_limits<double>::infinity();
        }
        return 1.0; // 1 year per tick
    }

    std::string logState() const override {
        std::stringstream ss;
        ss << state;        // uses our operator<< above
        return ss.str();
    }
};

#endif // TICK_ATOMIC_HPP
