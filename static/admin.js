// Admin Panel JavaScript functionality

document.addEventListener('DOMContentLoaded', function() {
    // Load content list when admin page loads
    loadContentList();
    
    // Form submission handling
    const addContentForm = document.querySelector('form[action="/admin/add-content"]');
    if (addContentForm) {
        addContentForm.addEventListener('submit', function(e) {
            e.preventDefault();
            
            const formData = new FormData(this);
            const data = new URLSearchParams();
            
            for (let [key, value] of formData.entries()) {
                data.append(key, value);
            }
            
            fetch('/admin/add-content', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/x-www-form-urlencoded',
                },
                body: data
            })
            .then(response => {
                if (response.ok) {
                    showMessage('Content added successfully!', 'success');
                    this.reset();
                    loadContentList(); // Reload the content list
                } else {
                    showMessage('Failed to add content. Please try again.', 'error');
                }
            })
            .catch(error => {
                console.error('Error:', error);
                showMessage('An error occurred. Please try again.', 'error');
            });
        });
    }
    
    // Load content list from server
    function loadContentList() {
        const contentListDiv = document.getElementById('content-list');
        if (!contentListDiv) return;
        
        contentListDiv.innerHTML = '<div class="loading">Loading content...</div>';
        
        fetch('/api/content')
            .then(response => response.json())
            .then(data => {
                displayContentList(data);
            })
            .catch(error => {
                console.error('Error loading content:', error);
                contentListDiv.innerHTML = '<div class="error">Failed to load content</div>';
            });
    }
    
    // Display content list
    function displayContentList(contentItems) {
        const contentListDiv = document.getElementById('content-list');
        if (!contentListDiv) return;
        
        if (contentItems.length === 0) {
            contentListDiv.innerHTML = '<p>No content available. Add some content using the form above.</p>';
            return;
        }
        
        let html = '';
        contentItems.forEach(item => {
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
        
        contentListDiv.innerHTML = html;
    }
    
    // Delete content function
    window.deleteContent = function(id) {
        if (!confirm('Are you sure you want to delete this content? This action cannot be undone.')) {
            return;
        }
        
        const data = new URLSearchParams();
        data.append('id', id);
        
        fetch('/admin/delete-content', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/x-www-form-urlencoded',
            },
            body: data
        })
        .then(response => {
            if (response.ok) {
                showMessage('Content deleted successfully!', 'success');
                loadContentList(); // Reload the content list
            } else {
                showMessage('Failed to delete content. Please try again.', 'error');
            }
        })
        .catch(error => {
            console.error('Error:', error);
            showMessage('An error occurred. Please try again.', 'error');
        });
    };
    
    // Show message function
    function showMessage(message, type) {
        // Remove existing messages
        const existingMessages = document.querySelectorAll('.message');
        existingMessages.forEach(msg => msg.remove());
        
        // Create new message
        const messageDiv = document.createElement('div');
        messageDiv.className = `message ${type}`;
        messageDiv.textContent = message;
        
        // Insert at the top of the admin content
        const adminContent = document.querySelector('.admin-content');
        if (adminContent) {
            adminContent.insertBefore(messageDiv, adminContent.firstChild);
        }
        
        // Auto-remove after 5 seconds
        setTimeout(() => {
            if (messageDiv.parentNode) {
                messageDiv.remove();
            }
        }, 5000);
    }
    
    // Escape HTML function
    function escapeHtml(text) {
        const div = document.createElement('div');
        div.textContent = text;
        return div.innerHTML;
    }
    
    // Auto-save draft functionality
    let autoSaveTimer;
    const titleInput = document.querySelector('input[name="title"]');
    const contentInput = document.querySelector('textarea[name="content"]');
    const categorySelect = document.querySelector('select[name="category"]');
    
    function autoSaveDraft() {
        const draft = {
            title: titleInput ? titleInput.value : '',
            content: contentInput ? contentInput.value : '',
            category: categorySelect ? categorySelect.value : ''
        };
        
        localStorage.setItem('rb_publication_draft', JSON.stringify(draft));
    }
    
    // Set up auto-save
    if (titleInput) {
        titleInput.addEventListener('input', function() {
            clearTimeout(autoSaveTimer);
            autoSaveTimer = setTimeout(autoSaveDraft, 1000);
        });
    }
    
    if (contentInput) {
        contentInput.addEventListener('input', function() {
            clearTimeout(autoSaveTimer);
            autoSaveTimer = setTimeout(autoSaveDraft, 1000);
        });
    }
    
    if (categorySelect) {
        categorySelect.addEventListener('change', function() {
            clearTimeout(autoSaveTimer);
            autoSaveTimer = setTimeout(autoSaveDraft, 1000);
        });
    }
    
    // Load draft on page load
    function loadDraft() {
        const draft = localStorage.getItem('rb_publication_draft');
        if (draft) {
            try {
                const draftData = JSON.parse(draft);
                
                if (titleInput && draftData.title) {
                    titleInput.value = draftData.title;
                }
                if (contentInput && draftData.content) {
                    contentInput.value = draftData.content;
                }
                if (categorySelect && draftData.category) {
                    categorySelect.value = draftData.category;
                }
                
                // Show draft loaded message
                showMessage('Draft loaded from previous session', 'success');
            } catch (error) {
                console.error('Error loading draft:', error);
            }
        }
    }
    
    // Load draft when page loads
    loadDraft();
    
    // Clear draft after successful form submission
    if (addContentForm) {
        addContentForm.addEventListener('submit', function() {
            localStorage.removeItem('rb_publication_draft');
        });
    }
    
    // Character counter for content
    if (contentInput) {
        const charCounter = document.createElement('div');
        charCounter.className = 'char-counter';
        charCounter.style.cssText = `
            font-size: 0.8rem;
            color: #666;
            text-align: right;
            margin-top: 0.5rem;
        `;
        
        contentInput.parentNode.appendChild(charCounter);
        
        function updateCharCounter() {
            const count = contentInput.value.length;
            charCounter.textContent = `${count} characters`;
            
            if (count > 1000) {
                charCounter.style.color = '#ff6b6b';
            } else if (count > 500) {
                charCounter.style.color = '#f57c00';
            } else {
                charCounter.style.color = '#666';
            }
        }
        
        contentInput.addEventListener('input', updateCharCounter);
        updateCharCounter(); // Initial count
    }
    
    // Keyboard shortcuts
    document.addEventListener('keydown', function(e) {
        // Ctrl/Cmd + S to save
        if ((e.ctrlKey || e.metaKey) && e.key === 's') {
            e.preventDefault();
            if (addContentForm) {
                addContentForm.dispatchEvent(new Event('submit'));
            }
        }
        
        // Ctrl/Cmd + Enter to submit
        if ((e.ctrlKey || e.metaKey) && e.key === 'Enter') {
            e.preventDefault();
            if (addContentForm) {
                addContentForm.dispatchEvent(new Event('submit'));
            }
        }
    });
    
    // Initialize admin panel
    console.log('RB Publication Admin Panel loaded successfully!');
});
