[![License: GPL](https://img.shields.io/badge/License-GPL-yellow.svg)](https://www.gnu.org/licenses/gpl-3.0.en.html)
[![C++ Standard](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B#Standardization)
[![CMake Version](https://img.shields.io/badge/CMake-3.15%2B-064F8C.svg)](https://cmake.org/)
[![Platform Support](https://img.shields.io/badge/Platform-Linux%20%7C%20Windows%20%7C%20macOS-blue.svg)](https://github.com/dethrandir23/craftr)
[![CLI Tool](https://img.shields.io/badge/CLI-Tool-green.svg)](https://github.com/dethrandir23/craftr)
[![Template Engine](https://img.shields.io/badge/Template-YAML-red.svg)](https://github.com/dethrandir23/craftr)

-----

# Craftr 🛠️

**Craftr** is a powerful, template-driven command-line tool that eliminates the hassle of starting projects from scratch. Tired of manually creating directories, copying boilerplate, setting up build systems, and adding license files every time you start a new project? Craftr is a flexible scaffolding engine that automates this entire process for you.

While it was born as a C++ project creator, its **YAML-based template engine** now allows you to create instant starter templates for **any language or project structure**.

## ✨ Features

- **🔄 Template-Driven:** Define your own project structures with YAML
- **🔍 Interactive Discovery:** Search and browse templates with `--find` and `--list`
- **🌐 Remote Templates:** Pull templates from Git repos or community hub
- **🛠️ Template Extraction:** Convert existing projects to templates with `--extract`
- **✅ Template Validation:** Ensure template quality with `--validate`
- **🌍 Multi-language UI:** **Multi-language UI:** 15 languages supported - English, Turkish, Spanish, French, German, Italian, Portuguese, Russian, Chinese, Japanese, Korean, Arabic, Hindi, Dutch, Polish
- **⚡ Post-Creation Hooks:** Automate setup commands (git init, npm install, etc.)
- **🔨 Smart Build System:** Generate and execute project-specific build commands with a single flag
- **⏱️ Build Timing:** Track exactly how long your build process takes
- **📦 Cross-Platform:** Works on Linux, macOS, and Windows
- **🎨 Colorful CLI:** Beautiful, color-coded output for better UX

## 📦 Installation

### Prerequisites

  - A C++17 compatible compiler (GCC, Clang, MSVC)
  - CMake 3.15 or higher

### Build from Source

```bash
# Clone the repository
git clone https://github.com/dethrandir23/craftr.git
cd craftr

# Create build directory and compile
mkdir build && cd build
cmake ..
make

# Install system-wide (optional)
sudo make install
```

## 🚀 Usage

### Quick Start: Creating a Project

Let's start a project using the built-in `cpp` template:

```bash
craftr --create --template cpp
```

Craftr will guide you through the setup process:

```
Enter PROJECT_NAME: Mordor
Enter AUTHOR_NAME: Sauron
Select your license type:
- apache
- bsd
- gpl
- mit
Selected license (if you don't want one, press enter): mit
[INFO] Initializing Git repository for Mordor...
[OK] cd Mordor && git init
[INFO] Staging files for first commit...
[OK] cd Mordor && git add .
[INFO] Creating first commit...
[OK] cd Mordor && git commit -m 'Initial commit'
[SUCCESS] Executed all commands with success.
[SUCCESS] Project 'Mordor' created successfully!
```

### Managing Templates

#### Validating a Template (`--validate`)

Check if your template is correctly formatted and all its files exist.

```bash
# Example of a failing validation
$ craftr --validate --template noob
[FAIL] Validation errors for template 'noob':
 - Required field missing: version
 - Required field missing: author
 - Invalid value for command_mode. Must be "cautious" or "execute_all".

# Example of a successful validation
$ craftr --validate --template cpp
[OK] Template 'cpp' is valid ✅
```

#### Using Remote Templates

Craftr allows you to use templates directly from Git repositories.

```bash
# 1. Pull all official community templates
craftr --pull=template center

# 2. Pull a specific template from any Git repository
craftr --pull=template https://github.com/user/my-awesome-template

# 3. Use a remote template directly without saving it locally
craftr --create --remote https://github.com/user/my-awesome-template
```

## 🧑‍🎨 Creating Your Own Templates

### The Easy Way: Extracting from a Directory (`--extract`)

The fastest way to create a template is to convert an existing project. Just point Craftr to a directory:

```bash
craftr --extract /path/to/your/existing-project
```

Craftr will ask you a few questions and automatically generate a complete template structure for you.

```
Creating template from directory: /path/to/your/existing-project/
Enter a name for the new template: my-first-template
Enter template version (e.g., 1.0): 1.0.0
...
[SUCCESS] Template 'my-first-template' created successfully!
```

### The Manual Way

1.  Create the main directory for your user templates: `mkdir -p ~/.config/craftr/templates`
2.  Create a new folder for your template: `mkdir ~/.config/craftr/templates/python-api`
3.  Inside, place your `template.yaml` file and source files (e.g., `main.py.txt`).

## 🎨 The Template System Explained

The heart of Craftr is its `template.yaml` file. It defines the structure, variables, and post-creation commands for your project.

```yaml
name: "cpp"
version: "1.0.0"
author: "system"
description: "Default C++ project template"

# These variables will be prompted to the user during creation
variables:
  PROJECT_NAME: "{{PROJECT_NAME}}"
  AUTHOR_NAME: "{{AUTHOR_NAME}}"
  DATE: "{{DATE}}" # Some variables like DATE can be auto-filled

# Subfolders to be created
subfolders:
  - include
  - src

# Files to be created from templates
files:
  - target: "CMakeLists.txt"
    template: "cmake/CMake_Template.txt"
  - target: "src/main.cpp"
    template: "cpp/main.txt"
  - target: "LICENSE"
    template: "license" # 'license' is a special keyword

# --- Post-Creation Hooks ---

# Options: 'cautious' (stops on fail) or 'execute_all'
command_mode: cautious

# Suppress command output, showing only Craftr's status messages
silent_mode: true

# Commands to execute after project files are created
commands:
  - command: "cd {{PROJECT_NAME}} && git init"
    description: "Initializing Git repository for {{PROJECT_NAME}}..." # Optional description for clarity
  - command: "cd {{PROJECT_NAME}} && git add ."
    description: "Staging files for first commit..."
  - command: "cd {{PROJECT_NAME}} && git commit -m 'Initial commit'"
    description: "Creating first commit..."

# Build commands are saved to .craftr/build.yaml in the project folder
build_commands:
  - command: "cd {{PROJECT_NAME}} && mkdir build && cd build && cmake .. && make"
    description: "Building {{PROJECT_NAME}}..."

run_commands:
  - command: "./build/{{PROJECT_NAME}}"
    description: "Running {{PROJECT_NAME}}..."
```

## 🎯 Command Reference

### Quick Help Output

Run craftr --help to see all available commands:
bash

### Basic Commands

| Command | Description |
|---------|-------------|
| `-h, --help` | Show help and exit |
| `-v, --version` | Show version and exit |
| `-l, --language <locale>` | Change UI language |

### Project Creation

| Command | Description |
|---------|-------------|
| `-c, --create` | Create new project |
| `-t, --template <name>` | Use specific template |
| `-f, --find` | Search templates interactively |
| `-n, --name <name>` | Specify project name for find |

### Template Management

| Command | Description |
|---------|-------------|
| `-li, --list [category]` | List templates (all, user, center, remote, system) |
| `-p, --pull <type> <source>` | Download templates or licenses |
| `-e, --extract <directory>` | Create template from existing project |
| `-va, --validate` | Validate a template |
| `-r, --remote <url>` | Use remote template from Git |

### Advanced Commands

| Command | Description |
|---------|-------------|
| `-a, --add <module>` | Add module/component to project |
| `-b, --build` | Execute build commands defined in `.craftr/build.yaml`
| `-co, --config <key=value>` | Set configuration options |

### Examples

```bash
craftr --create --template cpp
craftr --find --name cpp
craftr --list
craftr --pull template center
craftr --language en
```
└──────────────────────────────────────────

Detailed Command Examples
🔍 Finding and Creating Projects
bash

### Interactive template search and creation
craftr --find --name cpp

### Create with specific template
craftr --create --template cpp

### Use remote template directly
craftr --create --remote https://github.com/user/template-repo

📁 Template Management
bash

### List all templates
craftr --list

### List specific categories
craftr --list user
craftr --list center  
craftr --list remote

### Download templates
craftr --pull template center
craftr --pull template https://github.com/user/template-repo

### Validate template
craftr --validate --template cpp

### Create template from existing project
craftr --extract ./my-project

⚙️ Configuration
bash

### Change language
craftr --language tr
craftr --language en
craftr --language system

### Show version
craftr --version

## 🔨 Building Your Project

Once a project is created, Craftr saves the build instructions in `.craftr/build.yaml` within your project directory. You can execute these commands anytime without remembering complex flags:

```bash
craftr --build
>> Building Mordor...
[OK] cd Mordor && mkdir build && cd build && cmake .. && make
[INFO] Build finished in 4.25s
```

## 🤝 Contributing

We welcome contributions\! Please feel free to submit issues, feature requests, or pull requests.

1.  Fork this repository.
2.  Create a new feature branch (`git checkout -b feature/amazing-feature`).
3.  Commit your changes (`git commit -m 'Add some amazing feature'`).
4.  Push to the branch (`git push origin feature/amazing-feature`).
5.  Open a Pull Request.

## 📄 License

This project is licensed under the **GPL License**. See the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

  - Inspired by great tools like `cargo` and `cookiecutter`.
  - Built with [Cliopatra](https://github.com/dethrandir23/Cliopatra) for command-line parsing.
  - Used [yaml-cpp](https://github.com/jbeder/yaml-cpp) for templating system
  - Thanks to all contributors who help improve Craftr\!

-----

## 🌐 Other Languages

  - [Türkçe](docs/tr/README.md)

**Craftr** is made with ❤️ by [dethrandir23](https://github.com/dethrandir23).

## Star History

[![Star History Chart](https://api.star-history.com/image?repos=dethrandir23/craftr&type=date&legend=top-left)](https://www.star-history.com/?repos=dethrandir23%2Fcraftr&type=date&legend=top-left)
