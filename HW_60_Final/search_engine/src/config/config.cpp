// src/config/config.cpp
#include "config.h"
#include <fstream>
#include <sstream>
#include <algorithm>

Config& Config::getInstance() {
    static Config instance;
    return instance;
}

bool Config::load(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    std::string line;
    std::string current_section;
    
    while (std::getline(file, line)) {
        // Remove comments
        size_t comment_pos = line.find(';');
        if (comment_pos != std::string::npos) {
            line = line.substr(0, comment_pos);
        }
        
        // Trim whitespace
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);
        
        if (line.empty()) continue;
        
        // Section
        if (line[0] == '[' && line[line.size()-1] == ']') {
            current_section = line.substr(1, line.size()-2);
        }
        // Key-value pair
        else if (current_section != "") {
            size_t equals_pos = line.find('=');
            if (equals_pos != std::string::npos) {
                std::string key = line.substr(0, equals_pos);
                std::string value = line.substr(equals_pos + 1);
                
                // Trim
                key.erase(0, key.find_first_not_of(" \t"));
                key.erase(key.find_last_not_of(" \t") + 1);
                value.erase(0, value.find_first_not_of(" \t"));
                value.erase(value.find_last_not_of(" \t") + 1);
                
                data_[current_section][key] = value;
            }
        }
    }
    
    return true;
}

std::string Config::getString(const std::string& section, const std::string& key) const {
    auto sect_it = data_.find(section);
    if (sect_it == data_.end()) return "";
    
    auto key_it = sect_it->second.find(key);
    if (key_it == sect_it->second.end()) return "";
    
    return key_it->second;
}

int Config::getInt(const std::string& section, const std::string& key) const {
    std::string value = getString(section, key);
    if (value.empty()) return 0;
    
    try {
        return std::stoi(value);
    } catch (...) {
        return 0;
    }
}