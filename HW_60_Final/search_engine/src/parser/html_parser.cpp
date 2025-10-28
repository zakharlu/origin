// src/parser/html_parser.cpp
#include "html_parser.h"
#include <algorithm>
#include <cctype>
#include <sstream>

ParsedPage HTMLParser::parse(const std::string& html, const std::string& base_url) {
    ParsedPage result;
    
    result.title = extractTitle(html);
    result.clean_text = cleanHTML(html);
    result.links = extractLinks(html, base_url);
    result.word_frequencies = countWords(result.clean_text);
    
    return result;
}

std::string HTMLParser::extractTitle(const std::string& html) {
    std::regex title_regex("<title>(.*?)</title>", std::regex::icase);
    std::smatch match;
    
    if (std::regex_search(html, match, title_regex) && match.size() > 1) {
        return match[1].str();
    }
    
    return "No Title";
}

std::string HTMLParser::cleanHTML(const std::string& html) {
    // Remove script and style tags
    std::string text = std::regex_replace(html, std::regex("<script.*?</script>", std::regex::icase), "");
    text = std::regex_replace(text, std::regex("<style.*?</style>", std::regex::icase), "");
    
    // Remove HTML tags
    text = std::regex_replace(text, std::regex("<[^>]*>"), " ");
    
    // Replace multiple spaces with single space
    text = std::regex_replace(text, std::regex("\\s+"), " ");
    
    // Remove punctuation and convert to lowercase
    std::string clean_text;
    for (char c : text) {
        if (std::isalnum(c) || std::isspace(c)) {
            clean_text += std::tolower(c);
        } else if (std::ispunct(c)) {
            clean_text += ' ';
        }
    }
    
    return clean_text;
}

std::vector<std::string> HTMLParser::extractLinks(const std::string& html, const std::string& base_url) {
    std::vector<std::string> links;
    std::regex link_regex("<a\\s+[^>]*href=\"([^\"]*)\"[^>]*>", std::regex::icase);
    
    auto begin = std::sregex_iterator(html.begin(), html.end(), link_regex);
    auto end = std::sregex_iterator();
    
    for (std::sregex_iterator i = begin; i != end; ++i) {
        std::smatch match = *i;
        std::string url = normalizeURL(match[1].str(), base_url);
        if (!url.empty()) {
            links.push_back(url);
        }
    }
    
    return links;
}

std::map<std::string, int> HTMLParser::countWords(const std::string& text) {
    std::map<std::string, int> frequencies;
    std::istringstream iss(text);
    std::string word;
    
    while (iss >> word) {
        if (isValidWord(word)) {
            frequencies[word]++;
        }
    }
    
    return frequencies;
}

std::string HTMLParser::normalizeURL(const std::string& url, const std::string& base_url) {
    if (url.empty()) return "";
    
    // Skip javascript, mailto, etc.
    if (url.find("javascript:") == 0 || url.find("mailto:") == 0 || 
        url.find("#") == 0 || url.find("tel:") == 0) {
        return "";
    }
    
    std::string normalized = url;
    
    // Handle relative URLs
    if (url.find("http") != 0) {
        if (url[0] == '/') {
            // Absolute path
            size_t protocol_end = base_url.find("://");
            if (protocol_end != std::string::npos) {
                size_t domain_end = base_url.find('/', protocol_end + 3);
                std::string domain = base_url.substr(0, domain_end);
                normalized = domain + url;
            }
        } else {
            // Relative path
            size_t last_slash = base_url.find_last_of('/');
            if (last_slash != std::string::npos) {
                std::string base_path = base_url.substr(0, last_slash + 1);
                normalized = base_path + url;
            }
        }
    }
    
    return normalized;
}

bool HTMLParser::isValidWord(const std::string& word) {
    return word.length() >= 3 && word.length() <= 32;
}