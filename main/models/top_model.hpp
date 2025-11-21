#ifndef TOP_MODEL_HPP
#define TOP_MODEL_HPP

#include <cadmium/core/modeling/coupled.hpp>
#include "tick_atomic.hpp"
#include "tree_atomic.hpp"

class TopModel : public cadmium::Coupled {
public:
    TopModel() : cadmium::Coupled("top") {

        auto tick = addComponent<TickAtomic>("tick", 50);  // 50 years
        auto tree = addComponent<TreeAtomic>("tree");

        // tick.out -> tree.tick_in
        addCoupling(
            tick->getOutPort(TickDefs::OUT_PORT),
            tree->getInPort(TreeDefs::IN_TICK)
        );

        // tree.out -> top.tree_out
        auto topOut = this->addOutPort<std::string>("tree_out");

        addCoupling(
            tree->getOutPort(TreeDefs::OUT_PORT),
            topOut
        );
    }
};

#endif // TOP_MODEL_HPP
