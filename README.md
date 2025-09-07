# Craftr 🛠️

**Craftr** is a powerful and intuitive command-line tool designed to streamline the creation and management of C++ projects. Tired of manually setting up project structures, licenses, and build systems? Craftr automates these tasks with customizable templates, allowing you to focus on what really matters: writing code.

## Features ✨

- **Quick Project Setup**: Generate a complete C++ project structure with a single command
- **Customizable Templates**: Predefined templates for CMake, source files, licenses, and more
- **License Management**: Automatically inject your chosen license (MIT, BSD, GPL, Apache) into your project
- **Metadata Support**: Track project details with a built-in JSON metadata system
- **Modular Design**: Easily add new modules or components to existing projects
- **Cross-Platform**: Designed to work on Linux, macOS, and Windows

## Installation 📦

### Prerequisites
- C++17 compatible compiler
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

# Install (optional)
sudo make install
```

## Usage 🚀

### Create a New Project
```bash
craftr --create MyAwesomeApp --author "Your Name" --license MIT
```

This command generates:
```
MyAwesomeApp/
├── CMakeLists.txt
├── include/
├── LICENSE
├── metadata/
│   └── metadata.json
├── README.md
└── src/
    └── main.cpp
```

### Add a Module
```bash
craftr --add utils
```

### Build the Project
```bash
craftr --build all
```

## Project Structure 📁

```
craftr/
├── include/          # Header files
├── src/             # Source files
├── templates/        # Project templates
│   ├── cpp/
│   ├── cmake/
│   ├── licenses/
│   └── metadata/
└── metadata/         # Metadata storage
```

## Template System 🎨

Craftr uses a flexible template system with placeholder substitution. Available templates include:

- **CMake**: Configure your build system
- **Main.cpp**: Basic program structure
- **Licenses**: MIT, BSD, GPL, Apache
- **README.md**: Project documentation
- **metadata.json**: Project metadata

## Contributing 🤝

We welcome contributions! Please feel free to submit issues, feature requests, or pull requests.

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## License 📄

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments 🙏

- Inspired by tools like `cargo new` and `flet create`
- Built with [Cliopatra](https://github.com/dethrandir23/Cliopatra) for command-line parsing
- Thanks to all contributors who help improve Craftr!

---

**Craftr** is maintained with ❤️ by [dethrandir23](https://github.com/dethrandir23)

```