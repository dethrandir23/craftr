https://img.shields.io/badge/License-MIT-yellow.svg
https://img.shields.io/badge/C%252B%252B-17-blue.svg
https://img.shields.io/badge/CMake-3.15%252B-064F8C.svg
https://img.shields.io/badge/Platform-Linux%2520%257C%2520Windows%2520%257C%2520macOS-blue.svg
https://img.shields.io/github/issues/dethrandir23/craftr.svg
https://img.shields.io/github/forks/dethrandir23/craftr.svg
https://img.shields.io/github/stars/dethrandir23/craftr.svg
https://img.shields.io/github/contributors/dethrandir23/craftr.svg

# Craftr 🛠️

[](https://www.google.com/search?q=https://github.com/dethrandir23/craftr)
[](https://opensource.org/licenses/MIT)
[](https://isocpp.org/std/the-standard)

**Craftr** is a powerful, template-driven command-line tool that eliminates the hassle of starting projects from scratch. Tired of manually creating directories, copying boilerplate, setting up build systems, and adding license files every time you start a new project? Craftr is a flexible scaffolding engine that automates this entire process for you.

While it was born as a C++ project creator, its **YAML-based template engine** now allows you to create instant starter templates for **any language or project structure**.

## ✨ Features

  - **Template-Driven:** Define your own project structures with YAML. Create templates for C++, Python, Web, or anything you can imagine.
  - **Interactive Setup:** `craftr` prompts you for variables from your template (like Project Name, Author, etc.), so you don't have to memorize long commands.
  - **Fully Customizable:** Make `craftr` your personal project launcher by adding your own templates to the `~/.config/craftr` directory.
  - **Dynamic Content:** Placeholders like `{{PLACEHOLDER}}` in your template files are automatically populated with your inputs.
  - **Cross-Platform:** Designed to work seamlessly on Linux, macOS, and Windows.
  - **Post-Creation Hooks**: Automatically run shell commands (like git init or npm install) after your project is created.

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

## 🚀 Quick Start

Creating a new project has never been easier. Let's start a project using the built-in `cpp` template:

```bash
craftr --create --template cpp
```

Craftr will then ask you for the project details:

```
Enter the PROJECT_NAME : Mordor
Enter the AUTHOR_NAME : Sauron
Select your license type:
- apache
- bsd
- gpl
- mit
Selected license (if you don't want one, press enter): mit
Project 'Mordor' created successfully!
```

And voilà\! A complete project named `Mordor` is generated, filled with the details you provided:

```
Mordor/
├── CMakeLists.txt
├── include/
├── LICENSE
├── metadata/
│   └── metadata.json
├── README.md
└── src/
    └── main.cpp
```

## 🎨 How The Template System Works

The heart of Craftr is its template system, defined by YAML. A template defines the project structure, folders, files, commands, and the variables to be requested from the user. Here is the `template.yaml` for the built-in `cpp` template:

```yaml
name: "cpp"
version: "1.0.0"
author: "system"
description: "Default C++ project template"

# These variables will be prompted to the user during creation
variables:
  PROJECT_NAME: "{{PROJECT_NAME}}"
  AUTHOR_NAME: "{{AUTHOR_NAME}}"
  DATE: "{{DATE}}" # Some variables like DATE can be auto-filled by the system

# Subfolders to be created
subfolders:
  - include
  - src
  - metadata

# Files to be created and their corresponding templates
files:
  - target: "CMakeLists.txt"
    template: "cmake/CMake_Template.txt"
  - target: "src/main.cpp"
    template: "cpp/main.txt"
  - target: "README.md"
    template: "readme/Readme.txt"
  - target: "LICENSE"
    template: "license" # 'license' is a special keyword for license generation
  - target: "metadata/metadata.json"
    template: "metadata/metadata.txt"

command_mode: cautious # cautious mode stops executing commands if one of them fails, but execute_all mode executes all of them at any cost

# Commands to be executed after project creation
commands:
  - "cd {{PROJECT_NAME}} && git init"
  - "cd {{PROJECT_NAME}} && git add ."
  - "cd {{PROJECT_NAME}} && git commit -m 'Initial commit'"
```

Craftr parses this file and replaces placeholders like `{{PROJECT_NAME}}` in the template source files (e.g., `CMake_Template.txt`) with the values provided by the user.

## 🧑‍🎨 Creating Your Own Templates

Unleash the full power of Craftr by creating your own templates\!

1.  Create the main directory for your templates:

    ```bash
    mkdir -p ~/.config/craftr/templates
    ```

2.  Create a new folder for your template. For example, `python-api` for a Python project:

    ```bash
    mkdir ~/.config/craftr/templates/python-api
    ```

3.  Inside this folder, place your `template.yaml` file and your template source files, such as `main.py.txt`, `requirements.txt.txt`, etc.

Now you can start a new project using your custom template:

```bash
craftr --create --template python-api
```

## 🤝 Contributing

We welcome contributions\! Please feel free to submit issues, feature requests, or pull requests.

1.  Fork this repository.
2.  Create a new feature branch (`git checkout -b feature/amazing-feature`).
3.  Commit your changes (`git commit -m 'Add some amazing feature'`).
4.  Push to the branch (`git push origin feature/amazing-feature`).
5.  Open a Pull Request.

## 📄 License

This project is licensed under the MIT License. See the [LICENSE](https://www.google.com/search?q=LICENSE) file for details.

## 🙏 Acknowledgments

  - Inspired by great tools like `cargo new` and `cookiecutter`.
  - Built with [Cliopatra](https://github.com/dethrandir23/Cliopatra) for command-line parsing.
  - Thanks to all contributors who help improve Craftr\!

-----

## 🌐 Other Languages

- [Türkçe](docs/tr/README.md)


**Craftr** is made with ❤️ by [dethrandir23](https://github.com/dethrandir23).
