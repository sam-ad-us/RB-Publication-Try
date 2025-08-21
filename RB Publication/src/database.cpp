#include "database.h"
#include <iostream>
#include <sstream>
#include <ctime>

Database::Database() : db(nullptr) {
    dbPath = "rb_publication.db";
}

Database::~Database() {
    close();
}

bool Database::initialize() {
    int rc = sqlite3_open(dbPath.c_str(), &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    
    return createTables();
}

void Database::close() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}

bool Database::createTables() {
    const char* sql = R"(
        CREATE TABLE IF NOT EXISTS content (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            title TEXT NOT NULL,
            content TEXT NOT NULL,
            category TEXT NOT NULL,
            created_at TEXT NOT NULL
        );
    )";
    
    char* errMsg = 0;
    int rc = sqlite3_exec(db, sql, callback, 0, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    
    return true;
}

bool Database::insertContent(const std::string& title, const std::string& content, const std::string& category) {
    std::string timestamp = getCurrentTimestamp();
    
    std::stringstream ss;
    ss << "INSERT INTO content (title, content, category, created_at) VALUES (?, ?, ?, ?);";
    
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, ss.str().c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    
    sqlite3_bind_text(stmt, 1, title.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, content.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, category.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, timestamp.c_str(), -1, SQLITE_STATIC);
    
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    return rc == SQLITE_DONE;
}

bool Database::deleteContent(int id) {
    std::stringstream ss;
    ss << "DELETE FROM content WHERE id = ?;";
    
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, ss.str().c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    
    sqlite3_bind_int(stmt, 1, id);
    
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    return rc == SQLITE_DONE;
}

bool Database::updateContent(int id, const std::string& title, const std::string& content, const std::string& category) {
    std::stringstream ss;
    ss << "UPDATE content SET title = ?, content = ?, category = ? WHERE id = ?;";
    
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, ss.str().c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    
    sqlite3_bind_text(stmt, 1, title.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, content.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, category.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, id);
    
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    return rc == SQLITE_DONE;
}

std::vector<ContentItem> Database::getAllContent() {
    std::vector<ContentItem> items;
    
    const char* sql = "SELECT id, title, content, category, created_at FROM content ORDER BY created_at DESC;";
    
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return items;
    }
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        ContentItem item;
        item.id = sqlite3_column_int(stmt, 0);
        item.title = (const char*)sqlite3_column_text(stmt, 1);
        item.content = (const char*)sqlite3_column_text(stmt, 2);
        item.category = (const char*)sqlite3_column_text(stmt, 3);
        item.created_at = (const char*)sqlite3_column_text(stmt, 4);
        items.push_back(item);
    }
    
    sqlite3_finalize(stmt);
    return items;
}

ContentItem Database::getContentById(int id) {
    ContentItem item = {0, "", "", "", ""};
    
    const char* sql = "SELECT id, title, content, category, created_at FROM content WHERE id = ?;";
    
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return item;
    }
    
    sqlite3_bind_int(stmt, 1, id);
    
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        item.id = sqlite3_column_int(stmt, 0);
        item.title = (const char*)sqlite3_column_text(stmt, 1);
        item.content = (const char*)sqlite3_column_text(stmt, 2);
        item.category = (const char*)sqlite3_column_text(stmt, 3);
        item.created_at = (const char*)sqlite3_column_text(stmt, 4);
    }
    
    sqlite3_finalize(stmt);
    return item;
}

std::vector<ContentItem> Database::getContentByCategory(const std::string& category) {
    std::vector<ContentItem> items;
    
    const char* sql = "SELECT id, title, content, category, created_at FROM content WHERE category = ? ORDER BY created_at DESC;";
    
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return items;
    }
    
    sqlite3_bind_text(stmt, 1, category.c_str(), -1, SQLITE_STATIC);
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        ContentItem item;
        item.id = sqlite3_column_int(stmt, 0);
        item.title = (const char*)sqlite3_column_text(stmt, 1);
        item.content = (const char*)sqlite3_column_text(stmt, 2);
        item.category = (const char*)sqlite3_column_text(stmt, 3);
        item.created_at = (const char*)sqlite3_column_text(stmt, 4);
        items.push_back(item);
    }
    
    sqlite3_finalize(stmt);
    return items;
}

std::string Database::getCurrentTimestamp() {
    time_t now = time(0);
    struct tm* ltm = localtime(&now);
    
    std::stringstream ss;
    ss << (1900 + ltm->tm_year) << "-"
       << std::setfill('0') << std::setw(2) << (1 + ltm->tm_mon) << "-"
       << std::setfill('0') << std::setw(2) << ltm->tm_mday << " "
       << std::setfill('0') << std::setw(2) << ltm->tm_hour << ":"
       << std::setfill('0') << std::setw(2) << ltm->tm_min << ":"
       << std::setfill('0') << std::setw(2) << ltm->tm_sec;
    
    return ss.str();
}

int Database::callback(void* data, int argc, char** argv, char** azColName) {
    // This callback is used for CREATE TABLE operations
    return 0;
}
