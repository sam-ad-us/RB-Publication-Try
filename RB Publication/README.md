# RB Publication - Web Application

एक modern web application जो दो versions में उपलब्ध है:
1. **C++ Version** - Crow framework के साथ (Advanced)
2. **HTML/CSS/JS Version** - Standalone (Simple, GitHub Pages पर deploy करने योग्य)

## 🌟 Features

### Main Features
- **Modern Web Interface**: Beautiful, responsive design
- **Admin Panel**: Secure login system with content management
- **Content Categories**: News, Articles, Stories, Poems
- **Real-time Updates**: Content updates immediately reflect on the website
- **Mobile Responsive**: Works perfectly on all devices

### Admin Panel Features
- **Secure Login**: Username/Password authentication
- **Add Content**: Easy form to add new content
- **Delete Content**: Remove unwanted content
- **Local Storage**: Content persists in browser
- **Character Counter**: Track content length

### 📱 Responsive Design
- Mobile-friendly interface
- Modern CSS with animations
- Smooth scrolling navigation
- Back-to-top button

## 🚀 Quick Start (HTML Version)

### Option 1: Direct Use (Recommended for beginners)
1. **Download Files**: `index.html`, `style.css`, `script.js`
2. **Open**: Double-click `index.html` in your browser
3. **Admin Access**: Click "Admin" in navigation
4. **Login**: Username: `admin`, Password: `rbpublication123`

### Option 2: GitHub Pages Deployment
1. **Create Repository**: Create a new GitHub repository
2. **Upload Files**: Upload `index.html`, `style.css`, `script.js`
3. **Enable Pages**: Go to Settings → Pages → Source: Deploy from branch
4. **Access**: Your site will be live at `https://yourusername.github.io/repositoryname`

## 🔧 C++ Version Setup

### Prerequisites
- C++17 compatible compiler
- CMake 3.10 or higher
- SQLite3 development libraries
- Crow framework

### Installation

#### Windows:
```bash
# Install vcpkg
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.bat

# Install dependencies
vcpkg install crow sqlite3

# Build project
./build.bat
```

#### Linux/macOS:
```bash
# Install dependencies
sudo apt install build-essential cmake libsqlite3-dev  # Ubuntu
brew install cmake sqlite3                             # macOS

# Build project
./build.sh
```

### Run C++ Version
```bash
cd build
./rb_publication
# Visit http://localhost:8080
```

## 📁 Project Structure

### HTML Version (Simple)
```
RB Publication/
├── index.html          # Main website
├── style.css           # Styling
├── script.js           # Functionality
└── README.md           # This file
```

### C++ Version (Advanced)
```
RB Publication/
├── src/
│   ├── main.cpp              # Main application
│   ├── admin_panel.h/cpp     # Admin functionality
│   ├── content_manager.h/cpp # Content management
│   └── database.h/cpp        # Database operations
├── static/
│   ├── style.css             # Website styles
│   ├── admin.css             # Admin panel styles
│   ├── script.js             # Website JavaScript
│   └── admin.js              # Admin panel JavaScript
├── CMakeLists.txt            # Build configuration
├── build.bat                 # Windows build script
├── build.sh                  # Linux/macOS build script
└── README.md                 # This file
```

## 🔐 Default Admin Credentials

- **Username**: `admin`
- **Password**: `rbpublication123`

⚠️ **Important**: Change these credentials in production!

## 📖 Usage

### For Visitors
1. Open the website
2. Browse content by categories (News, Articles, Stories, Poems)
3. Use navigation to jump to specific sections
4. Click "Read More" to view full content

### For Administrators
1. Click "Admin" in the navigation
2. Login with admin credentials
3. Add new content using the form
4. Manage existing content
5. Delete unwanted content

## 🌐 Deployment Options

### HTML Version (Recommended for beginners)
- **GitHub Pages**: Free hosting
- **Netlify**: Drag & drop deployment
- **Vercel**: Easy deployment
- **Any web server**: Upload files directly

### C++ Version (For advanced users)
- **Traditional Server**: VPS with nginx
- **Docker**: Containerized deployment
- **Cloud Platforms**: Heroku, Railway, AWS

## 🎨 Customization

### Changing Admin Credentials
**HTML Version**: Edit `script.js`:
```javascript
const ADMIN_USERNAME = "your_username";
const ADMIN_PASSWORD = "your_password";
```

**C++ Version**: Edit `src/admin_panel.cpp`:
```cpp
adminUsername = "your_username";
adminPassword = "your_password";
```

### Adding New Categories
1. Update category options in forms
2. Add CSS styles for new category badges
3. Update JavaScript filtering logic

### Styling Changes
- Edit `style.css` for visual changes
- All colors and layouts are easily customizable

## 🔒 Security Features

1. **Input Validation**: All user inputs are validated
2. **XSS Protection**: HTML escaping implemented
3. **Local Storage**: Secure content storage
4. **Admin Authentication**: Login required for admin functions

## 🛠️ Troubleshooting

### HTML Version Issues
```bash
# Clear browser cache
Ctrl + F5 (Windows/Linux)
Cmd + Shift + R (macOS)

# Clear local storage
F12 → Application → Local Storage → Clear
```

### C++ Version Issues
```bash
# Build errors
sudo apt install build-essential cmake libsqlite3-dev

# Runtime errors
chmod +x ./rb_publication
```

## 📱 Mobile Support

Both versions are fully responsive and work on:
- 📱 Smartphones
- 📱 Tablets
- 💻 Desktop computers
- 🖥️ Large screens

## 🚀 Performance

### HTML Version
- ⚡ Instant loading
- 💾 Local storage for content
- 🎯 No server required
- 📊 Lightweight (under 100KB)

### C++ Version
- ⚡ High performance
- 🗄️ SQLite database
- 🔄 Real-time updates
- 📊 Scalable architecture

## 🔮 Future Enhancements

- [ ] Rich text editor
- [ ] Image upload functionality
- [ ] Search functionality
- [ ] User comments system
- [ ] Newsletter subscription
- [ ] Social media integration
- [ ] Multi-language support
- [ ] RSS feeds
- [ ] Analytics dashboard

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request

## 📄 License

This project is licensed under the MIT License.

## 💬 Support

For support and questions:
- Create an issue on GitHub
- Check the troubleshooting section
- Review the code comments

## 🎯 Which Version Should You Choose?

### Choose HTML Version if:
- ✅ You're a beginner
- ✅ You want quick setup
- ✅ You want to deploy on GitHub Pages
- ✅ You don't need a database
- ✅ You want simple maintenance

### Choose C++ Version if:
- ✅ You're experienced with C++
- ✅ You need database storage
- ✅ You want high performance
- ✅ You plan to scale the application
- ✅ You want full control over the backend

---

**RB Publication** - Built with ❤️ using modern web technologies

*Choose the version that best fits your needs!*
