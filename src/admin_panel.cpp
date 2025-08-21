#include "admin_panel.h"
#include <sstream>
#include <iostream>

AdminPanel::AdminPanel(ContentManager* cm) : contentManager(cm), isLoggedIn(false) {
    // Default admin credentials (in production, use environment variables)
    adminUsername = "admin";
    adminPassword = "rbpublication123";
}

crow::response AdminPanel::getAdminPage() {
    if (!isLoggedIn) {
        return crow::response(generateLoginHTML());
    }
    return crow::response(generateAdminHTML());
}

crow::response AdminPanel::handleLogin(const crow::request& req) {
    auto body = req.body;
    
    // Parse form data
    std::string username, password;
    size_t pos = body.find("username=");
    if (pos != std::string::npos) {
        size_t end = body.find("&", pos);
        if (end != std::string::npos) {
            username = body.substr(pos + 9, end - pos - 9);
        } else {
            username = body.substr(pos + 9);
        }
    }
    
    pos = body.find("password=");
    if (pos != std::string::npos) {
        password = body.substr(pos + 9);
    }
    
    if (authenticate(username, password)) {
        isLoggedIn = true;
        return crow::response(302, "").add_header("Location", "/admin");
    } else {
        return crow::response(401, "Invalid credentials");
    }
}

crow::response AdminPanel::addContent(const crow::request& req) {
    if (!isLoggedIn) {
        return crow::response(401, "Not authenticated");
    }
    
    auto body = req.body;
    
    // Parse form data for new content
    std::string title, content, category;
    
    // Extract title
    size_t pos = body.find("title=");
    if (pos != std::string::npos) {
        size_t end = body.find("&", pos);
        if (end != std::string::npos) {
            title = body.substr(pos + 6, end - pos - 6);
        } else {
            title = body.substr(pos + 6);
        }
    }
    
    // Extract content
    pos = body.find("content=");
    if (pos != std::string::npos) {
        size_t end = body.find("&", pos);
        if (end != std::string::npos) {
            content = body.substr(pos + 8, end - pos - 8);
        } else {
            content = body.substr(pos + 8);
        }
    }
    
    // Extract category
    pos = body.find("category=");
    if (pos != std::string::npos) {
        category = body.substr(pos + 9);
    }
    
    // Add content to database
    bool success = contentManager->addContent(title, content, category);
    
    if (success) {
        return crow::response(302, "").add_header("Location", "/admin");
    } else {
        return crow::response(500, "Failed to add content");
    }
}

crow::response AdminPanel::deleteContent(const crow::request& req) {
    if (!isLoggedIn) {
        return crow::response(401, "Not authenticated");
    }
    
    auto body = req.body;
    
    // Parse content ID
    std::string contentId;
    size_t pos = body.find("id=");
    if (pos != std::string::npos) {
        contentId = body.substr(pos + 3);
    }
    
    int id = std::stoi(contentId);
    bool success = contentManager->deleteContent(id);
    
    if (success) {
        return crow::response(302, "").add_header("Location", "/admin");
    } else {
        return crow::response(500, "Failed to delete content");
    }
}

bool AdminPanel::authenticate(const std::string& username, const std::string& password) {
    return username == adminUsername && password == adminPassword;
}

std::string AdminPanel::generateLoginHTML() {
    return R"(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>RB Publication - Admin Login</title>
    <link rel="stylesheet" href="/static/admin.css">
</head>
<body>
    <div class="login-container">
        <div class="login-box">
            <h1>RB Publication</h1>
            <h2>Admin Login</h2>
            <form action="/admin/login" method="POST">
                <div class="form-group">
                    <label for="username">Username:</label>
                    <input type="text" id="username" name="username" required>
                </div>
                <div class="form-group">
                    <label for="password">Password:</label>
                    <input type="password" id="password" name="password" required>
                </div>
                <button type="submit">Login</button>
            </form>
        </div>
    </div>
</body>
</html>
    )";
}

std::string AdminPanel::generateAdminHTML() {
    std::stringstream ss;
    ss << R"(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>RB Publication - Admin Panel</title>
    <link rel="stylesheet" href="/static/admin.css">
</head>
<body>
    <div class="admin-container">
        <header class="admin-header">
            <h1>RB Publication - Admin Panel</h1>
            <a href="/" class="view-site-btn">View Site</a>
        </header>
        
        <div class="admin-content">
            <div class="add-content-section">
                <h2>Add New Content</h2>
                <form action="/admin/add-content" method="POST">
                    <div class="form-group">
                        <label for="title">Title:</label>
                        <input type="text" id="title" name="title" required>
                    </div>
                    <div class="form-group">
                        <label for="category">Category:</label>
                        <select id="category" name="category" required>
                            <option value="news">News</option>
                            <option value="article">Article</option>
                            <option value="story">Story</option>
                            <option value="poem">Poem</option>
                        </select>
                    </div>
                    <div class="form-group">
                        <label for="content">Content:</label>
                        <textarea id="content" name="content" rows="10" required></textarea>
                    </div>
                    <button type="submit">Add Content</button>
                </form>
            </div>
            
            <div class="manage-content-section">
                <h2>Manage Existing Content</h2>
                <div id="content-list">
                    <!-- Content will be loaded here via JavaScript -->
                </div>
            </div>
        </div>
    </div>
    
    <script src="/static/admin.js"></script>
</body>
</html>
    )";
    
    return ss.str();
}
