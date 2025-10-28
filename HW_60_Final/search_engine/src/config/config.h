// src/config/config.h
#pragma once
#include <string>
#include <map>

class Config {
public:
    static Config& getInstance();
    
    bool load(const std::string& filename);
    std::string getString(const std::string& section, const std::string& key) const;
    int getInt(const std::string& section, const std::string& key) const;
    
private:
    Config() = default;
    std::map<std::string, std::map<std::string, std::string>> data_;
};