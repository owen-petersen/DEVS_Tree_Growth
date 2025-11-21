#ifndef JSON_LOGGER_V2_HPP
#define JSON_LOGGER_V2_HPP

#include <cadmium/core/logger/logger.hpp>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cstdio>

class JSONLogger : public cadmium::Logger {
public:

    struct Entry {
        double time;
        double height;
        double crown;
    };

    std::vector<Entry> buffer;
    std::ofstream out;
    int packet_id = 1;

    JSONLogger() : out("tree_output.json") {}

    //-----------------------------------------
    // LOGGER LIFECYCLE
    //-----------------------------------------

    void start() override {
        buffer.clear();
        packet_id = 1;
    }

    void stop() override {
        flush();
        out.close();
    }

    //-----------------------------------------
    // TIME LOGGING (unused but required)
    //-----------------------------------------

    void logTime(double) override {
        // We don't need to record time-only events
    }

    //-----------------------------------------
    // OUTPUT LOGGING (MAIN FUNCTION)
    //-----------------------------------------

    void logOutput(double t,
                   long /*modelId*/,
                   const std::string& /*modelName*/,
                   const std::string& /*portName*/,
                   const std::string& output) override
    {
        // Expected format: "height,crown"
        double h = 0, c = 0;

        // sscanf works because output looks like:  "9.0,3.0"
        sscanf(output.c_str(), "%lf,%lf", &h, &c);

        buffer.push_back({t, h, c});

        if (buffer.size() == 20) {
            flush();
        }
    }

    //-----------------------------------------
    // STATE LOGGING (unused)
    //-----------------------------------------

    void logState(double,
                  long,
                  const std::string&,
                  const std::string&) override
    {
        // We don't need state logging for tree model
    }

private:

    //-----------------------------------------
    // WRITE BUFFERED ENTRIES TO FILE
    //-----------------------------------------

    void flush() {
        if (buffer.empty()) return;

        out << "{\n";
        out << "  \"packet\": " << packet_id++ << ",\n";
        out << "  \"entries\": [\n";

        for (size_t i = 0; i < buffer.size(); i++) {
            out << "    { \"t\": " << buffer[i].time
                << ", \"height\": " << buffer[i].height
                << ", \"crown\": " << buffer[i].crown << " }";

            if (i + 1 < buffer.size())
                out << ",";

            out << "\n";
        }

        out << "  ]\n";
        out << "}\n";

        buffer.clear();
    }
};

#endif
