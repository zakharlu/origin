// src/parser/html_parser.h
#pragma once
#include <string>
#include <vector>
#include <map>
#include <regex>

struct ParsedPage {
    std::string title;
    std::string clean_text;
    std::vector<std::string> links;
    std::map<std::string, int> word_frequencies;
};

class HTMLParser {
public:
    static ParsedPage parse(const std::string& html, const std::string& base_url);
    
private:
    static std::string extractTitle(const std::string& html);
    static std::string cleanHTML(const std::string& html);
    static std::vector<std::string> extractLinks(const std::string& html, const std::string& base_url);
    static std::map<std::string, int> countWords(const std::string& text);
    static std::string normalizeURL(const std::string& url, const std::string& base_url);
    static bool isValidWord(const std::string& word);
};