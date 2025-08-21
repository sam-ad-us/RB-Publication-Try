// RB Publication - Main JavaScript functionality

// Sample content data (in a real app, this would come from a database)
let contentData = JSON.parse(localStorage.getItem('rb_publication_content')) || [
    {
        id: 1,
        title: "Welcome to RB Publication",
        content: "This is your first publication. You can add more content through the admin panel. Click on the Admin link in the navigation to get started.",
        category: "news",
        created_at: new Date().toLocaleDateString()
    }
];

// Admin credentials
const ADMIN_USERNAME = "admin";
const ADMIN_PASSWORD = "rbpublication123";

document.addEventListener('DOMContentLoaded', function() {
    // Initialize the application
    loadContent();
    setupEventListeners();
    setupAdminPanel();
    
    console.log('RB Publication loaded successfully!');
});

function loadContent() {
    // Load content for each category
    const categories = ['news', 'article', 'story', 'poem'];
    
    categories.forEach(category => {
        const gridId = category === 'article' ? 'articles-grid' : category + '-grid';
        const grid = document.getElementById(gridId);
        if (grid) {
            const categoryContent = contentData.filter(item => item.category === category);
            grid.innerHTML = generateContentHTML(categoryContent);
        }
    });
}

function generateContentHTML(content) {
    if (content.length === 0) {
        return '<p class="no-content">No content available yet. Add some content through the admin panel!</p>';
    }
    
    return content.map(item => `
        <article class="content-card">
            <div class="content-header">
                <h3>${escapeHtml(item.title)}</h3>
                <span class="category-badge ${item.category}-badge">${escapeHtml(item.category)}</span>
            </div>
            <div class="content-body">
                <p>${escapeHtml(item.content.length > 200 ? item.content.substring(0, 200) + '...' : item.content)}</p>
            </div>
            <div class="content-footer">
                <span class="date">${escapeHtml(item.created_at)}</span>
                <button class="read-more-btn" onclick="showFullContent(${item.id})">Read More</button>
            </div>
        </article>
    `).join('');
}

function showFullContent(id) {
    const item = contentData.find(content => content.id === id);
    if (item) {
        alert(`Full Content: ${item.title}\n\n${item.content}`);
    }
}

function setupEventListeners() {
    // Smooth scrolling for navigation links
    const navLinks = document.querySelectorAll('.nav-link[href^="#"]');
    navLinks.forEach(link => {
        link.addEventListener('click', function(e) {
            e.preventDefault();
            const targetId = this.getAttribute('href').substring(1);
            const targetSection = document.getElementById(targetId);
            if (targetSection) {
                targetSection.scrollIntoView({
                    behavior: 'smooth',
                    block: 'start'
                });
            }
        });
    });
    
    // Admin link click
    const adminLink = document.querySelector('.admin-link');
    if (adminLink) {
        adminLink.addEventListener('click', function(e) {
            e.preventDefault();
            showAdminModal();
        });
    }
    
    // Modal close functionality
    const modal = document.getElementById('admin-modal');
    const closeBtn = document.querySelector('.close');
    
    if (closeBtn) {
        closeBtn.addEventListener('click', function() {
            modal.style.display = 'none';
        });
    }
    
    if (modal) {
        modal.addEventListener('click', function(e) {
            if (e.target === modal) {
                modal.style.display = 'none';
            }
        });
    }
    
    // Back to top button
    setupBackToTop();
}

function setupAdminPanel() {
    // Login form
    const loginForm = document.getElementById('login-form');
    if (loginForm) {
        loginForm.addEventListener('submit', function(e) {
            e.preventDefault();
            handleLogin();
        });
    }
    
    // Add content form
    const addContentForm = document.getElementById('add-content-form');
    if (addContentForm) {
        addContentForm.addEventListener('submit', function(e) {
            e.preventDefault();
            handleAddContent();
        });
    }
}

function showAdminModal() {
    const modal = document.getElementById('admin-modal');
    const loginDiv = document.getElementById('admin-login');
    const panelDiv = document.getElementById('admin-panel');
    
    // Check if user is logged in
    if (localStorage.getItem('rb_admin_logged_in') === 'true') {
        loginDiv.style.display = 'none';
        panelDiv.style.display = 'block';
        loadContentList();
    } else {
        loginDiv.style.display = 'block';
        panelDiv.style.display = 'none';
    }
    
    modal.style.display = 'block';
}

function handleLogin() {
    const username = document.getElementById('username').value;
    const password = document.getElementById('password').value;
    
    if (username === ADMIN_USERNAME && password === ADMIN_PASSWORD) {
        localStorage.setItem('rb_admin_logged_in', 'true');
        showAdminPanel();
        showMessage('Login successful!', 'success');
    } else {
        showMessage('Invalid credentials!', 'error');
    }
}

function showAdminPanel() {
    const loginDiv = document.getElementById('admin-login');
    const panelDiv = document.getElementById('admin-panel');
    
    loginDiv.style.display = 'none';
    panelDiv.style.display = 'block';
    loadContentList();
}

function handleAddContent() {
    const title = document.getElementById('title').value;
    const category = document.getElementById('category').value;
    const content = document.getElementById('content').value;
    
    if (title && category && content) {
        const newContent = {
            id: Date.now(), // Simple ID generation
            title: title,
            content: content,
            category: category,
            created_at: new Date().toLocaleDateString()
        };
        
        contentData.push(newContent);
        localStorage.setItem('rb_publication_content', JSON.stringify(contentData));
        
        // Reload content
        loadContent();
        loadContentList();
        
        // Reset form
        document.getElementById('add-content-form').reset();
        
        showMessage('Content added successfully!', 'success');
    } else {
        showMessage('Please fill all fields!', 'error');
    }
}

function loadContentList() {
    const contentList = document.getElementById('content-list');
    if (!contentList) return;
    
    if (contentData.length === 0) {
        contentList.innerHTML = '<p>No content available. Add some content using the form above.</p>';
        return;
    }
    
    let html = '';
    contentData.forEach(item => {
        const preview = item.content.length > 100 ? 
            item.content.substring(0, 100) + '...' : 
            item.content;
        
        html += `
            <div class="content-item">
                <h3>${escapeHtml(item.title)}</h3>
                <span class="category">${escapeHtml(item.category)}</span>
                <div class="date">${escapeHtml(item.created_at)}</div>
                <div class="preview">${escapeHtml(preview)}</div>
                <div class="actions">
                    <button class="delete-btn" onclick="deleteContent(${item.id})">Delete</button>
                </div>
            </div>
        `;
    });
    
    contentList.innerHTML = html;
}

function deleteContent(id) {
    if (confirm('Are you sure you want to delete this content? This action cannot be undone.')) {
        contentData = contentData.filter(item => item.id !== id);
        localStorage.setItem('rb_publication_content', JSON.stringify(contentData));
        
        loadContent();
        loadContentList();
        showMessage('Content deleted successfully!', 'success');
    }
}

function showMessage(message, type) {
    // Remove existing messages
    const existingMessages = document.querySelectorAll('.message');
    existingMessages.forEach(msg => msg.remove());
    
    // Create new message
    const messageDiv = document.createElement('div');
    messageDiv.className = `message ${type}`;
    messageDiv.textContent = message;
    
    // Insert at the top of the modal content
    const modalContent = document.querySelector('.modal-content');
    if (modalContent) {
        modalContent.insertBefore(messageDiv, modalContent.firstChild);
    }
    
    // Auto-remove after 5 seconds
    setTimeout(() => {
        if (messageDiv.parentNode) {
            messageDiv.remove();
        }
    }, 5000);
}

function setupBackToTop() {
    const backToTopBtn = document.createElement('button');
    backToTopBtn.innerHTML = '↑';
    backToTopBtn.className = 'back-to-top';
    backToTopBtn.style.cssText = `
        position: fixed;
        bottom: 20px;
        right: 20px;
        width: 50px;
        height: 50px;
        border-radius: 50%;
        background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
        color: white;
        border: none;
        cursor: pointer;
        font-size: 20px;
        box-shadow: 0 5px 15px rgba(0,0,0,0.2);
        transition: all 0.3s ease;
        opacity: 0;
        visibility: hidden;
        z-index: 1000;
    `;
    
    document.body.appendChild(backToTopBtn);
    
    // Show/hide back to top button
    window.addEventListener('scroll', function() {
        if (window.pageYOffset > 300) {
            backToTopBtn.style.opacity = '1';
            backToTopBtn.style.visibility = 'visible';
        } else {
            backToTopBtn.style.opacity = '0';
            backToTopBtn.style.visibility = 'hidden';
        }
    });
    
    // Back to top functionality
    backToTopBtn.addEventListener('click', function() {
        window.scrollTo({
            top: 0,
            behavior: 'smooth'
        });
    });
    
    // Hover effects
    backToTopBtn.addEventListener('mouseenter', function() {
        this.style.transform = 'translateY(-3px)';
        this.style.boxShadow = '0 8px 20px rgba(0,0,0,0.3)';
    });
    
    backToTopBtn.addEventListener('mouseleave', function() {
        this.style.transform = 'translateY(0)';
        this.style.boxShadow = '0 5px 15px rgba(0,0,0,0.2)';
    });
}

function escapeHtml(text) {
    const div = document.createElement('div');
    div.textContent = text;
    return div.innerHTML;
}

// Global functions for onclick handlers
window.showFullContent = showFullContent;
window.deleteContent = deleteContent;
