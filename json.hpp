#ifndef CADMIUM_SIMULATION_LOGGER_JSON_HPP_
#define CADMIUM_SIMULATION_LOGGER_JSON_HPP_

#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cstddef>
#include <algorithm>

#include "logger.hpp"

namespace cadmium {

class JSONLogger : public Logger {
private:
    struct Entry {
        double time;
        long modelId;
        std::string modelName;
        std::string state;
    };

    std::string prefix;
    std::size_t packetSize;
    std::size_t packetIndex;

    std::vector<Entry> buffer;

    static std::string trim(const std::string& s) {
        auto b = s.begin();
        auto e = s.end();

        while (b != e && std::isspace(*b)) b++;
        do { --e; } while (e >= b && std::isspace(*e));

        return (b <= e) ? std::string(b, e + 1) : "";
    }

    void flush() {
        if (buffer.empty()) return;

        std::ostringstream fname;
        fname << prefix << "_packet_" << packetIndex++ << ".jsonl";

        std::ofstream out(fname.str());

        for (auto& e : buffer) {
            out << "{"
                << "\"time\":" << e.time << ","
                << "\"modelId\":" << e.modelId << ","
                << "\"modelName\":\"" << e.modelName << "\"";

            std::string st = trim(e.state);

            if (!st.empty() && st.front() == '{' && st.back() == '}') {
                std::string inner = st.substr(1, st.size() - 2);
                if (!inner.empty())
                    out << "," << inner;
            }

            out << "}\n";
        }

        out.close();
        buffer.clear();
    }

public:
    JSONLogger(std::string prefix, std::size_t packetSize = 100)
        : prefix(std::move(prefix)), packetSize(packetSize), packetIndex(0) {}

    void start() override {}
    void stop() override { flush(); }

    void logOutput(double, long, const std::string&, const std::string&, const std::string&) override {}

    void logState(double t, long id, const std::string& name, const std::string& state) override {
        buffer.push_back({t, id, name, state});
        if (buffer.size() >= packetSize) flush();
    }
};

} // namespace cadmium

#endif
