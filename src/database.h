#pragma once
#include <string>
#include <vector>
#include <sqlite3.h>
#include "content_manager.h"

class Database {
private:
    sqlite3* db;
    std::string dbPath;
    
public:
    Database();
    ~Database();
    
    // Database operations
    bool initialize();
    void close();
    
    // Content operations
    bool insertContent(const std::string& title, const std::string& content, const std::string& category);
    bool deleteContent(int id);
    bool updateContent(int id, const std::string& title, const std::string& content, const std::string& category);
    
    // Content retrieval
    std::vector<ContentItem> getAllContent();
    ContentItem getContentById(int id);
    std::vector<ContentItem> getContentByCategory(const std::string& category);
    
private:
    // Helper methods
    std::string getCurrentTimestamp();
    bool createTables();
    static int callback(void* data, int argc, char** argv, char** azColName);
};
