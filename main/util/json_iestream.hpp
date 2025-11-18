#ifndef JSON_IESTREAM_HPP
#define JSON_IESTREAM_HPP

#include <fstream>
#include <iostream>
#include <optional>
#include "cadmium/modeling/devs/atomic.hpp"
#include "cadmium/exception.hpp"

#include "cadmium/../../json/single_include/nlohmann/json.hpp"

namespace cadmium::lib {
    /**
     * Input event parser.
     * @tparam MSG data type of the event to be parsed
     */
    template<typename MSG>
    class JsonEventParser {
     private:
        std::ifstream file; //!< input file stream of the file with all the events to be injected.
     public:
        /**
         * Constructor function.
         * @param file_path path to the file containing the events.
         * The format of this file is the following:
         * - Each line corresponds to one event.
         * - Each line is a JSON string containing the time of the event and value of input (with those names)
         * - Lines should be in order of times (i.e. input at t=5 should not come after input at t=6)
         */
        explicit JsonEventParser(const char* file_path) {
            file.open(file_path);
            if(!file.is_open()) {
                throw CadmiumModelException("input event file could not be opened");
            }
        };

        /**
         * It reads the next line of the input file and parses the time when the event happens and its value.
         * If the parser has reached the end of the file, it returns infinity and an empty event.
         * @return tuple <next time, next event>.
         */
        std::pair<double, std::optional<MSG>> nextTimedInput() {
            // Default return values: infinity and none
            double nextTime = std::numeric_limits<double>::infinity();
            std::optional<MSG> contents = std::optional<MSG>();
            if (file.is_open() && !file.eof()) {
                try {
                    std::string json_input;
                    file >> json_input; // each line in the file is a json string with an input
                    
                    // deserialize the json string
                    nlohmann::json json_obj = nlohmann::json::parse(json_input);

                    // .at() throws exception if name of field is not found in json
                    nextTime = json_obj.at("time"); // read time of next message
                    MSG value = json_obj.at("value"); // value to feed to input port
                    contents.template emplace(value);

                } catch (std::exception) {
                    // reset to default values if error
                    nextTime = std::numeric_limits<double>::infinity();
                    contents = std::optional<MSG>();
                }
            }
            return std::make_pair(nextTime,contents);
        }
    };

    /**
     * Class for representing the Input Event Stream DEVS model state.
     * @tparam MSG data type of the event to be parsed
     */
    template<typename MSG>
    struct JsonIEStreamState {
        JsonEventParser<MSG> parser;  //!< Input events parser.
        std::optional<MSG> lastInputRead;  //!< las input messages read from the file.
        double clock;  //!< Current simulation time.
        double sigma;  //!< Time to wait before outputting the next event.

        /**
         * Processor state constructor. By default, the processor is idling.
         * @param filePath path to the file containing the events.
         */
        explicit JsonIEStreamState(const char* filePath): parser(JsonEventParser<MSG>(filePath)), lastInputRead(), clock(), sigma() {
            auto [nextTime, nextEvent] = parser.nextTimedInput();
            sigma = nextTime;
            lastInputRead = nextEvent;
        }
    };

    /**
     * Insertion operator for ProcessorState objects. It only displays the value of sigma.
     * @param out output stream.
     * @param s state to be represented in the output stream.
     * @return output stream with sigma already inserted.
     */
    template<typename MSG>
    std::ostream& operator<<(std::ostream &out, const JsonIEStreamState<MSG>& state) {
        out << state.sigma;
        return out;
    }

    /**
     * Atomic DEVS model for injecting input events from a file.
     * @tparam MSG message type of the events to be injected.
     */
    template<typename MSG>
    class JsonIEStream : public Atomic<JsonIEStreamState<MSG>> {
     public:
        Port<MSG> out;

        /**
         * Constructor function.
         * @param id ID of the new input event stream model.
         * @param filePath path to the file with the events to be injected
         */
        JsonIEStream(const std::string& id, const char* filePath): Atomic<JsonIEStreamState<MSG>>(id, JsonIEStreamState<MSG>(filePath)) {
            out = Atomic<JsonIEStreamState<MSG>>::template addOutPort<MSG>("out");
        }

        /**
         * Read the next line of file and when that message should be sent
         * if the time to be sent is in the past, then passivate model
         * @param state reference to the current state of the model.
         */
        void internalTransition(JsonIEStreamState<MSG>& state) const override {
            state.clock += state.sigma;
            while(true) {  // loop to ignore outdated events
                auto [nextTime, nextEvent] = state.parser.nextTimedInput();
                if (nextTime < state.clock) {
                    std::cerr << "Outdated event in input file. Scheduled time: " << nextTime << " Current time: " << state.clock << ". " << nextEvent.value() << std::endl;
                } else {
                    state.sigma = nextTime - state.clock;
                    state.lastInputRead = nextEvent;
                    break;
                }
            };
        }

        /**
         * External transitions should never occur (no input ports).
         * @param state reference to the current model state.
         * @param e time elapsed since the last state transition function was triggered.
         */
        void externalTransition(JsonIEStreamState<MSG>& state, double e) const override {
            // External Events should not occur for this model
            state.clock += e;
            state.sigma -= e;
        }

        /**
         * It outputs the next message
         * @param state reference to the current model state.
         */
        void output(const JsonIEStreamState<MSG>& state) const override {
            if(state.lastInputRead.has_value()){
                out->addMessage(state.lastInputRead.value());
            }
        }

        /**
         * It returns the value of State::sigma.
         * @param state reference to the current model state.
         * @return the sigma value.
         */
        [[nodiscard]] double timeAdvance(const JsonIEStreamState<MSG>& state) const override {
            return state.sigma;
        }
    };
}  //namespace cadmium::lib

#endif //JSON_IESTREAM_HPP
