#pragma once
#include <crow.h>
#include <string>
#include <map>
#include "content_manager.h"

class AdminPanel {
private:
    ContentManager* contentManager;
    std::string adminUsername;
    std::string adminPassword;
    bool isLoggedIn;
    
public:
    AdminPanel(ContentManager* cm);
    
    // Admin page handlers
    crow::response getAdminPage();
    crow::response handleLogin(const crow::request& req);
    crow::response addContent(const crow::request& req);
    crow::response deleteContent(const crow::request& req);
    
    // Helper methods
    bool authenticate(const std::string& username, const std::string& password);
    std::string generateAdminHTML();
    std::string generateLoginHTML();
};
