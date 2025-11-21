#include <memory>
#include <cadmium/core/modeling/coupled.hpp>
#include <cadmium/core/simulation/root_coordinator.hpp>

#include "models/top_model.hpp"
#include "cadmium/core/logger/json.hpp"   

int main() {
    
    auto top = std::make_shared<TopModel>();

    cadmium::RootCoordinator root(top);

    // Attach JSON logger
    auto logger = std::make_shared<JSONLogger>();
    root.setLogger(logger);

    // Start simulation
    root.start();

    // Run for 50 time units (50 years)
    root.simulate(50.0);

    // Stop simulation
    root.stop();

    return 0;
}


