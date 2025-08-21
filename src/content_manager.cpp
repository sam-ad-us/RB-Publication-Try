#include "content_manager.h"
#include <sstream>
#include <ctime>
#include <algorithm>

ContentManager::ContentManager(Database* db) : database(db) {}

bool ContentManager::addContent(const std::string& title, const std::string& content, const std::string& category) {
    return database->insertContent(title, content, category);
}

bool ContentManager::deleteContent(int id) {
    return database->deleteContent(id);
}

bool ContentManager::updateContent(int id, const std::string& title, const std::string& content, const std::string& category) {
    return database->updateContent(id, title, content, category);
}

std::vector<ContentItem> ContentManager::getAllContent() {
    return database->getAllContent();
}

ContentItem ContentManager::getContentById(int id) {
    return database->getContentById(id);
}

std::vector<ContentItem> ContentManager::getContentByCategory(const std::string& category) {
    return database->getContentByCategory(category);
}

std::string ContentManager::getMainPage() {
    auto allContent = getAllContent();
    
    std::stringstream ss;
    ss << R"(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>RB Publication</title>
    <link rel="stylesheet" href="/static/style.css">
</head>
<body>
    <header class="main-header">
        <div class="container">
            <h1>RB Publication</h1>
            <nav class="main-nav">
                <a href="/" class="nav-link">Home</a>
                <a href="/#news" class="nav-link">News</a>
                <a href="/#articles" class="nav-link">Articles</a>
                <a href="/#stories" class="nav-link">Stories</a>
                <a href="/#poems" class="nav-link">Poems</a>
            </nav>
        </div>
    </header>
    
    <main class="main-content">
        <div class="container">
            <section class="hero-section">
                <h2>Welcome to RB Publication</h2>
                <p>Discover amazing stories, articles, news, and poems</p>
            </section>
            
            <section id="news" class="content-section">
                <h2>Latest News</h2>
                <div class="content-grid">
)";
    
    // Add news content
    for (const auto& item : allContent) {
        if (item.category == "news") {
            ss << generateContentHTML({item});
        }
    }
    
    ss << R"(
                </div>
            </section>
            
            <section id="articles" class="content-section">
                <h2>Articles</h2>
                <div class="content-grid">
)";
    
    // Add articles content
    for (const auto& item : allContent) {
        if (item.category == "article") {
            ss << generateContentHTML({item});
        }
    }
    
    ss << R"(
                </div>
            </section>
            
            <section id="stories" class="content-section">
                <h2>Stories</h2>
                <div class="content-grid">
)";
    
    // Add stories content
    for (const auto& item : allContent) {
        if (item.category == "story") {
            ss << generateContentHTML({item});
        }
    }
    
    ss << R"(
                </div>
            </section>
            
            <section id="poems" class="content-section">
                <h2>Poems</h2>
                <div class="content-grid">
)";
    
    // Add poems content
    for (const auto& item : allContent) {
        if (item.category == "poem") {
            ss << generateContentHTML({item});
        }
    }
    
    ss << R"(
                </div>
            </section>
        </div>
    </main>
    
    <footer class="main-footer">
        <div class="container">
            <p>&copy; 2024 RB Publication. All rights reserved.</p>
        </div>
    </footer>
    
    <script src="/static/script.js"></script>
</body>
</html>
    )";
    
    return ss.str();
}

crow::response ContentManager::getAllContentJSON() {
    auto content = getAllContent();
    
    std::stringstream ss;
    ss << "[";
    for (size_t i = 0; i < content.size(); ++i) {
        if (i > 0) ss << ",";
        ss << "{";
        ss << "\"id\":" << content[i].id << ",";
        ss << "\"title\":\"" << escapeHTML(content[i].title) << "\",";
        ss << "\"content\":\"" << escapeHTML(content[i].content) << "\",";
        ss << "\"category\":\"" << escapeHTML(content[i].category) << "\",";
        ss << "\"created_at\":\"" << escapeHTML(content[i].created_at) << "\"";
        ss << "}";
    }
    ss << "]";
    
    crow::response res(ss.str());
    res.add_header("Content-Type", "application/json");
    return res;
}

crow::response ContentManager::getContentByIdJSON(int id) {
    auto item = getContentById(id);
    
    if (item.id == 0) {
        return crow::response(404, "Content not found");
    }
    
    std::stringstream ss;
    ss << "{";
    ss << "\"id\":" << item.id << ",";
    ss << "\"title\":\"" << escapeHTML(item.title) << "\",";
    ss << "\"content\":\"" << escapeHTML(item.content) << "\",";
    ss << "\"category\":\"" << escapeHTML(item.category) << "\",";
    ss << "\"created_at\":\"" << escapeHTML(item.created_at) << "\"";
    ss << "}";
    
    crow::response res(ss.str());
    res.add_header("Content-Type", "application/json");
    return res;
}

std::string ContentManager::escapeHTML(const std::string& text) {
    std::string result = text;
    
    // Replace special characters
    size_t pos = 0;
    while ((pos = result.find("&", pos)) != std::string::npos) {
        result.replace(pos, 1, "&amp;");
        pos += 5;
    }
    
    pos = 0;
    while ((pos = result.find("<", pos)) != std::string::npos) {
        result.replace(pos, 1, "&lt;");
        pos += 4;
    }
    
    pos = 0;
    while ((pos = result.find(">", pos)) != std::string::npos) {
        result.replace(pos, 1, "&gt;");
        pos += 4;
    }
    
    pos = 0;
    while ((pos = result.find("\"", pos)) != std::string::npos) {
        result.replace(pos, 1, "&quot;");
        pos += 6;
    }
    
    pos = 0;
    while ((pos = result.find("'", pos)) != std::string::npos) {
        result.replace(pos, 1, "&#39;");
        pos += 5;
    }
    
    return result;
}

std::string ContentManager::generateContentHTML(const std::vector<ContentItem>& items) {
    std::stringstream ss;
    
    for (const auto& item : items) {
        ss << R"(
                    <article class="content-card">
                        <div class="content-header">
                            <h3>)" << escapeHTML(item.title) << R"(</h3>
                            <span class="category-badge )" << item.category << R"(-badge">)" << escapeHTML(item.category) << R"(</span>
                        </div>
                        <div class="content-body">
                            <p>)" << escapeHTML(item.content.substr(0, 200)) << (item.content.length() > 200 ? "..." : "") << R"(</p>
                        </div>
                        <div class="content-footer">
                            <span class="date">)" << escapeHTML(item.created_at) << R"(</span>
                            <button class="read-more-btn" onclick="readMore()">Read More</button>
                        </div>
                    </article>
        )";
    }
    
    return ss.str();
}
