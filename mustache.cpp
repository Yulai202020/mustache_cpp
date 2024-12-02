#include <iostream>
#include <mustache.hpp>
#include <string>
#include <yaml-cpp/yaml.h>
#include <fstream>
#include <vector>
#include <utility>

using namespace kainjow::mustache;

// Function to extract key-value pairs from a YAML node
std::vector<std::pair<std::string, std::string>> getYamlNode(const YAML::Node& node) {
    std::vector<std::pair<std::string, std::string>> data;
    if (node.IsMap()) {
        for (const auto& entry : node) {
            data.emplace_back(entry.first.as<std::string>(), entry.second.as<std::string>());
        }
    } else {
        std::cerr << "Node is not a map." << std::endl;
    }
    return data;
}

int main(int argc, char** argv) {
    if (argc < 4) {
        std::cout << argv[0] << " <input_filename> <output_filename> <config_file>" << std::endl;
        return 1;
    }

    std::string input_filename = argv[1];
    std::string output_filename = argv[2];
    std::string config_filename = argv[3];
    std::string text;

    // Read config
    YAML::Node config;
    try {
        config = YAML::LoadFile(config_filename);
    } catch (const std::exception& e) {
        std::cerr << "Error loading config file: " << e.what() << std::endl;
        return 1;
    }

    YAML::Node person = config["data"];
    auto data = getYamlNode(person);

    // Get file
    std::ifstream inputfile(input_filename);
    if (!inputfile.is_open()) {
        std::cerr << "Error opening input file." << std::endl;
        return 1;
    }
    text.assign((std::istreambuf_iterator<char>(inputfile)), std::istreambuf_iterator<char>());
    inputfile.close();

    // Make template
    mustache template1{text};
    std::string returned = template1.render({}); // Use extracted data for rendering

    // Write to output file
    std::ofstream outputfile(output_filename);
    if (!outputfile.is_open()) {
        std::cerr << "Error opening output file." << std::endl;
        return 1;
    }
    outputfile << returned;
    outputfile.close();

    return 0;
}
