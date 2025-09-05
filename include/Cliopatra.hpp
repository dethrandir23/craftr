/*
 * Cliopatra.hpp - Header for the Cliopatra command-line argument parser
 *
 * Cliopatra is a lightweight and customizable C++ command-line argument parser.
 * - Supports booleans, strings, integers, doubles, and chars
 * - Handles both short (-v) and long (--verbose) flags
 * - Accepts --flag value and --flag=value syntaxes
 * - Multilingual true/false keyword support (yes, no, evet, hayır, 1, 0, ...)
 * - Results are returned as a std::map<std::string, std::variant<...>>
 *
 * Example usage:
 *   Cliopatra cliopatra;
 *   cliopatra.addOption("v", "verbose", Cliopatra::Option::bool_o);
 *   auto results = cliopatra.parse(argc, argv);
 *
 * See examples/basic_example.cpp for a full example.
 */
#pragma once
#include <string>
#include <vector>
#include <variant>
#include <map>
#include <algorithm>
#include <cctype>

class Cliopatra {
public:
    // Supported option types
    enum class Option {
        bool_o,    // Boolean flag
        string_o,  // String value
        int_o,     // Integer value
        double_o,  // Double/float value
        char_o,    // Single character value
        multi_bool_o, // Multiple boolean values
        multi_string_o, // Multiple string values
        multi_int_o, // Multiple integer values (not implemented yet)
        multi_double_o, // Multiple double values (not implemented yet)
        multi_char_o, // Multiple character values (not implemented yet)
        // Add more types as needed
    };

    // Structure to hold option metadata
    struct OptionData {
        std::string shortName; // Short flag, e.g. "v"
        std::string name;      // Long flag, e.g. "verbose"
        Option type;           // Option type
    };

    // Constructor
    inline Cliopatra() {
        // Constructor implementation can be added here
    }

    // Add a new option definition
    inline void addOption(std::string shortName, std::string name, Option opt) {
        OptionData data;
        data.shortName = shortName;
        data.name = name;
        data.type = opt;
        options.push_back(data);
    }

    // Variant type for parsed values
    using parsedValue = std::variant<bool, std::string, int, double, char, 
                                    std::vector<std::string>, std::vector<bool>, 
                                    std::vector<int>, std::vector<double>, std::vector<char>>;
    
    // Map type for parsed results: option name -> value
    using ParsedMap = std::map<std::string, parsedValue>;

    // Parse command-line arguments
    inline ParsedMap parse(int argc, char** argv) {
        ParsedMap results;
        for (int i = 0; i < argc; i++){
            std::string arg = argv[i];
            std::string value;
            bool hasEquals = false;
            size_t eqPos = arg.find('=');
            
            // Support --flag=value and -f=value syntax
            if ((arg.rfind("--", 0) == 0 || arg.rfind("-", 0) == 0) && eqPos != std::string::npos) {
                value = arg.substr(eqPos + 1);
                arg = arg.substr(0, eqPos);
                hasEquals = true;
            }
            
            // Check if the next argument is likely a flag (not a value)
            bool isNextLikelyAFlag = false;
            if (i + 1 < argc) {
                std::string next = argv[i + 1];
                for (const auto& opt : options) {
                    if (next == opt.name || next == opt.shortName)
                        isNextLikelyAFlag = true;
                }
            }
            
            // Match the argument to a defined option and parse its value
            for (const auto& option : options) {
                auto possibleFlags = getPossibleFlags(option);
                if (std::find(possibleFlags.begin(), possibleFlags.end(), arg) != possibleFlags.end()) {
                    std::string key = option.name;
                    switch (option.type) {
                        case Option::bool_o:
                            if (hasEquals) {
                                results[key] = parseBool(value, true);
                            } else if (i + 1 < argc && argv[i + 1][0] != '-' && !isNextLikelyAFlag) {
                                results[key] = parseBool(argv[i + 1], true);
                                i++;
                            } else {
                                results[key] = true;
                            }
                            break;
                        case Option::string_o:
                            if (hasEquals) {
                                results[key] = value;
                            }else if (i + 1 < argc && argv[i + 1][0] != '-' && !isNextLikelyAFlag) {
                                results[key] = std::string(argv[i + 1]);
                                i++;
                            }
                            break;
                        case Option::int_o:
                            if (hasEquals) {
                                results[key] = std::stoi(value);
                            } else if (i + 1 < argc && argv[i + 1][0] != '-' && !isNextLikelyAFlag) {
                                results[key] = std::stoi(argv[i + 1]);
                                i++;
                            }
                            break;
                        case Option::double_o:
                            if (hasEquals) {
                                results[key] = std::stod(value);
                            } else if (i + 1 < argc && argv[i + 1][0] != '-' && !isNextLikelyAFlag) {
                                results[key] = std::stod(argv[i + 1]);
                                i++;
                            }
                            break;
                        case Option::char_o:
                            if (hasEquals && !value.empty()) {
                                results[key] = value[0];
                            } else if (i + 1 < argc && argv[i + 1][0] != '-' && !isNextLikelyAFlag) {
                                results[key] = argv[i + 1][0];
                                i++;
                            }
                            break;
                        case Option::multi_string_o: {
                            std::vector<std::string> values;
                            if (hasEquals) {
                                values.push_back(value);
                            }
                            int j = i + 1;
                            while (j < argc) {
                                std::string nextArg = argv[j];
                                // stop if the next argument is a flag or another option
                                if (nextArg.rfind("-", 0) == 0) break;
                                bool isKnownFlag = false;
                                for (const auto& opt : options) {
                                    auto flags = getPossibleFlags(opt);
                                    if (std::find(flags.begin(), flags.end(), nextArg) != flags.end()) {
                                        isKnownFlag = true;
                                        break;
                                    }
                                }
                                if (isKnownFlag) break;
                                values.push_back(nextArg);
                                j++;
                            }
                            if (!values.empty()) {
                                results[key] = values;
                                i = j - 1;
                            } else {
                                results[key] = std::vector<std::string>{};
                            }
                            break;
                        }
                        case Option::multi_bool_o: {
                            std::vector<bool> boolValues;
                            if (hasEquals) {
                                boolValues.push_back(parseBool(value, true));
                            }
                            int j = i + 1;
                            while (j < argc) {
                                std::string nextArg = argv[j];
                                // stop if the next argument starts with a dash or is a known flag
                                if (nextArg.rfind("-", 0) == 0) break;
                                bool isKnownFlag = false;
                                for (const auto& opt : options) {
                                    auto flags = getPossibleFlags(opt);
                                    if (std::find(flags.begin(), flags.end(), nextArg) != flags.end()) {
                                        isKnownFlag = true;
                                        break;
                                    }
                                }
                                if (isKnownFlag) break;
                                boolValues.push_back(parseBool(nextArg, true));
                                j++;
                            }
                            if (!boolValues.empty()) {
                                results[key] = boolValues;
                                i = j - 1;
                            } else {
                                results[key] = std::vector<bool>{};
                            }
                            break;
                        }
                        case Option::multi_int_o: {
                            std::vector<int> intValues;
                            if (hasEquals) {
                                intValues.push_back(std::stoi(value));
                            }
                            int j = i + 1;
                            while (j < argc) {
                                std::string nextArg = argv[j];
                                // stop if the next argument starts with a dash or is a known flag
                                if (nextArg.rfind("-", 0) == 0) break;
                                bool isKnownFlag = false;
                                for (const auto& opt : options) {
                                    auto flags = getPossibleFlags(opt);
                                    if (std::find(flags.begin(), flags.end(), nextArg) != flags.end()) {
                                        isKnownFlag = true;
                                        break;
                                    }
                                }
                                if (isKnownFlag) break;
                                intValues.push_back(std::stoi(nextArg));
                                j++;
                            }
                            if (!intValues.empty()) {
                                results[key] = intValues;
                                i = j - 1;
                            } else {
                                results[key] = std::vector<int>{};
                            }
                            break;
                        }
                        case Option::multi_double_o: {
                            std::vector<double> doubleValues;
                            if (hasEquals) {
                                doubleValues.push_back(std::stod(value));
                            }
                            int j = i + 1;
                            while (j < argc) {
                                std::string nextArg = argv[j];
                                // stop if the next argument starts with a dash or is a known flag
                                if (nextArg.rfind("-", 0) == 0) break;
                                bool isKnownFlag = false;
                                for (const auto& opt : options) {
                                    auto flags = getPossibleFlags(opt);
                                    if (std::find(flags.begin(), flags.end(), nextArg) != flags.end()) {
                                        isKnownFlag = true;
                                        break;
                                    }
                                }
                                if (isKnownFlag) break;
                                doubleValues.push_back(std::stod(nextArg));
                                j++;
                            }
                            if (!doubleValues.empty()) {
                                results[key] = doubleValues;
                                i = j - 1;
                            } else {
                                results[key] = std::vector<double>{};
                            }
                            break;
                        }
                        case Option::multi_char_o: {
                            std::vector<char> charValues;
                            if (hasEquals && !value.empty()) {
                                charValues.push_back(value[0]);
                            }
                            int j = i + 1;
                            while (j < argc) {
                                std::string nextArg = argv[j];
                                // stop if the next argument starts with a dash or is a known flag
                                if (nextArg.rfind("-", 0) == 0) break;
                                bool isKnownFlag = false;
                                for (const auto& opt : options) {
                                    auto flags = getPossibleFlags(opt);
                                    if (std::find(flags.begin(), flags.end(), nextArg) != flags.end()) {
                                        isKnownFlag = true;
                                        break;
                                    }
                                }
                                if (isKnownFlag) break;
                                if (!nextArg.empty())
                                    charValues.push_back(nextArg[0]);
                                j++;
                            }
                            if (!charValues.empty()) {
                                results[key] = charValues;
                                i = j - 1;
                            } else {
                                results[key] = std::vector<char>{};
                            }
                            break;
                        }
                    }
                }
            }
        }
        return results;
    }

private:
    // Internal struct for true/false keyword sets
    struct BoolStrings {
        static inline const std::vector<std::string> TRUE_VALUES = {
            "true", "1", "yes", "y", "on", "evet", "t", "+", "enable", "doğru", "var", "ok", "sure", "yeah", "aye", "sim", "si", "oui", "jawohl"
        };
        static inline const std::vector<std::string> FALSE_VALUES = {
            "false", "0", "no", "n", "off", "hayır", "f", "-", "disable", "yanlış", "yok", "nope", "nah", "non", "nein", "não", "niet"
        };
    };

    // List of all defined options
    std::vector<OptionData> options;

    // Generate all possible flag representations for an option
    inline std::vector<std::string> getPossibleFlags(const OptionData& option) const {
        return {
            option.shortName,
            option.name,
            "--" + option.name,
            "-" + option.name,
            "--" + option.shortName,
            "-" + option.shortName
        };
    }

    // Parse a string as a boolean value
    inline bool parseBool(std::string input, bool strict) {
        std::string lowerInput = input;
        std::transform(lowerInput.begin(), lowerInput.end(), lowerInput.begin(),
            [](unsigned char c){ return std::tolower(c); });
        lowerInput.erase(lowerInput.begin(), std::find_if(lowerInput.begin(), lowerInput.end(), 
            [](int ch) { return !std::isspace(ch); }));
        lowerInput.erase(std::find_if(lowerInput.rbegin(), lowerInput.rend(), 
            [](int ch) { return !std::isspace(ch); }).base(), lowerInput.end());
        
        if (std::find(BoolStrings::TRUE_VALUES.begin(), BoolStrings::TRUE_VALUES.end(), lowerInput) != BoolStrings::TRUE_VALUES.end()) {
            return true;
        }
        if (std::find(BoolStrings::FALSE_VALUES.begin(), BoolStrings::FALSE_VALUES.end(), lowerInput) != BoolStrings::FALSE_VALUES.end()) {
            return false;
        }
        if (!strict) {
            try {
                int num = std::stoi(lowerInput);
                return num != 0;
            } catch (...) {
            }
        }
        return false;
    }
};