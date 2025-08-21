#include <crow.h>
#include <iostream>
#include <fstream>
#include <string>
#include "admin_panel.h"
#include "content_manager.h"
#include "database.h"

int main() {
    crow::SimpleApp app;
    
    // Initialize database
    Database db;
    db.initialize();
    
    // Initialize content manager
    ContentManager contentManager(&db);
    
    // Initialize admin panel
    AdminPanel adminPanel(&contentManager);
    
    // Serve static files (CSS, JS, images)
    CROW_ROUTE(app, "/static/<path>")
    ([](std::string path) {
        std::string file_path = "static/" + path;
        std::ifstream file(file_path);
        if (file.is_open()) {
            std::string content((std::istreambuf_iterator<char>(file)),
                               std::istreambuf_iterator<char>());
            file.close();
            
            // Determine content type based on file extension
            std::string content_type = "text/plain";
            if (path.find(".css") != std::string::npos) content_type = "text/css";
            else if (path.find(".js") != std::string::npos) content_type = "application/javascript";
            else if (path.find(".html") != std::string::npos) content_type = "text/html";
            else if (path.find(".png") != std::string::npos) content_type = "image/png";
            else if (path.find(".jpg") != std::string::npos) content_type = "image/jpeg";
            
            crow::response res(content);
            res.add_header("Content-Type", content_type);
            return res;
        }
        return crow::response(404, "File not found");
    });
    
    // Main page route
    CROW_ROUTE(app, "/")
    ([&contentManager]() {
        auto page = contentManager.getMainPage();
        return crow::response(page);
    });
    
    // Admin panel routes
    CROW_ROUTE(app, "/admin")
    ([&adminPanel]() {
        return adminPanel.getAdminPage();
    });
    
    CROW_ROUTE(app, "/admin/login")
    .methods("POST"_method)
    ([&adminPanel](const crow::request& req) {
        return adminPanel.handleLogin(req);
    });
    
    CROW_ROUTE(app, "/admin/add-content")
    .methods("POST"_method)
    ([&adminPanel](const crow::request& req) {
        return adminPanel.addContent(req);
    });
    
    CROW_ROUTE(app, "/admin/delete-content")
    .methods("POST"_method)
    ([&adminPanel](const crow::request& req) {
        return adminPanel.deleteContent(req);
    });
    
    // API routes for content
    CROW_ROUTE(app, "/api/content")
    ([&contentManager]() {
        return contentManager.getAllContent();
    });
    
    CROW_ROUTE(app, "/api/content/<int>")
    ([&contentManager](int id) {
        return contentManager.getContentById(id);
    });
    
    std::cout << "RB Publication Server starting on http://localhost:8080" << std::endl;
    std::cout << "Admin Panel: http://localhost:8080/admin" << std::endl;
    
    app.port(8080).multithreaded().run();
    
    return 0;
}
