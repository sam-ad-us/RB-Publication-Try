#pragma once
#include <crow.h>
#include <string>
#include <vector>
#include <map>
#include "database.h"

struct ContentItem {
    int id;
    std::string title;
    std::string content;
    std::string category;
    std::string created_at;
};

class ContentManager {
private:
    Database* database;
    
public:
    ContentManager(Database* db);
    
    // Content management
    bool addContent(const std::string& title, const std::string& content, const std::string& category);
    bool deleteContent(int id);
    bool updateContent(int id, const std::string& title, const std::string& content, const std::string& category);
    
    // Content retrieval
    std::vector<ContentItem> getAllContent();
    ContentItem getContentById(int id);
    std::vector<ContentItem> getContentByCategory(const std::string& category);
    
    // Page generation
    std::string getMainPage();
    crow::response getAllContentJSON();
    crow::response getContentByIdJSON(int id);
    
    // Helper methods
    std::string escapeHTML(const std::string& text);
    std::string generateContentHTML(const std::vector<ContentItem>& items);
};
