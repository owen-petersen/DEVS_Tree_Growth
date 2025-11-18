#include "cadmium/simulation/root_coordinator.hpp"
#include <limits>
#include "models/top.hpp"

#include "cadmium/simulation/logger/json.hpp"

using namespace cadmium;

int main() {

    std::shared_ptr<topSystem> model = std::make_shared<topSystem>("topSystem");


    auto rootCoordinator = cadmium::RootCoordinator(model);


    rootCoordinator.setLogger<cadmium::JSONLogger>("treeLog", 50);

    rootCoordinator.start();
    rootCoordinator.simulate(50.0);
    rootCoordinator.stop();

    return 0;
}
