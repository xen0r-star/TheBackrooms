<p align="center">
  <a href="" rel="noopener">
 <img src="./public/image/The_Backrooms_logo.png" alt="Project logo" width="200"></a>
</p>
<h1 align="center">The Backrooms</h1>

<div align="center">

  ![Game name](https://img.shields.io/badge/game-The_Backrooms-orange.svg)
  ![Status](https://img.shields.io/badge/status-active-success.svg)
  [![GitHub Issues](https://img.shields.io/github/issues/xen0r-star/TheBackrooms.svg)](https://github.com/xen0r-star/TheBackrooms/issues)
  [![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/xen0r-star/TheBackrooms/blob/main/LICENSE)
  ![Version](https://img.shields.io/badge/version-beta_0.5.2-blue.svg)

</div>

<p align="center"> 
  Escape the Backrooms is a raycasting game inspired by classics such as Wolfenstein 3D.
  <br> 
</p>


## 📝 Table of Contents

- [🧐 About](#about)
- [🏁 Getting Started](#getting_started)
- [🎈 Usage](#usage)
- [🔧 Running the tests](#tests)
- [🚧 Development Roadmap](#roadmap)
- [⛏️ Built Using](built_using)
- [✍️ Authors](authors)

## 🧐 About <a name = "about"></a>

**The Backrooms** is a raycasting game inspired by classic first-person shooters like Wolfenstein 3D. Developed entirely in C using the SDL2 library, this project demonstrates advanced graphics rendering techniques and real-time event handling to create an immersive 3D experience.

The game challenges players to navigate through procedurally generated mazes filled with obstacles and hazards. Using raycasting algorithms for 3D rendering, players must explore the mysterious backrooms environment, avoid dangers, and find their way to the

## 🏁 Getting Started <a name = "getting_started"></a>

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

> [!WARNING]
> Note: This project has only been tested on Windows.

### Installing

You have two options to get the game running:

#### Option 1: Download Release (Recommended)
1. Go to the [Releases](https://github.com/xen0r-star/TheBackrooms/releases) page
2. Download the latest release
3. Extract and run `TheBackrooms.exe`

#### Option 2: Compile from Source

1. **Prerequisites**
  - C Compiler *(GCC or MinGW for Windows)*
  - Make *(install with `pacman -S make` if using MSYS2)*
  - Git *(for cloning the repository)*

2. **Clone the repository**
```bash
git clone https://github.com/xen0r-star/TheBackrooms.git
cd TheBackrooms
```

3. **Compile using the provided Makefile**
```bash
make -f Makefile.windows
```

4. **Run the game**
```bash
cd build
./TheBackrooms.exe
```

## 🎈 Usage <a name="usage"></a>

### Game Controls
#### Keyboard & Mouse
- **ZQSD** - Move around
- **Mouse** - Look around
- **Mouse Wheel** - Change weapon/tool
- **ESC** - Open pause menu

#### Controller Support
- **Left Stick** - Move around
- **Right Stick** - Look around
- **Shoulder Buttons** - Change weapon/tool
- **Start/Menu Button** - Open pause menu

### Objective
Navigate through the procedurally generated backrooms maze, avoid obstacles, and find the exit to escape.


## 🔧 Running the tests <a name = "tests"></a>

Currently, no automated tests are implemented for this project. Testing is done manually through gameplay.

<!-- TODO List -->
## 🚧 Development Roadmap <a name = "roadmap"></a>

### Upcoming Features
- [ ] **Creatures System** - Add hostile entities roaming the backrooms
- [ ] **Exit/Escape Mechanism** - Implement win condition and escape routes
- [ ] **Achievement System** - Complete the achievements/success tracking
- [ ] **Settings Menu** - Finish implementation of game configuration options
- [ ] **Save/Load System** - Complete save game and load game functionality

### Future Enhancements
- [ ] **Sound Effects** - Add atmospheric audio and creature sounds  
- [ ] **Multiple Levels** - Expand beyond Level 0 of the backrooms
- [ ] **Inventory System** - Add items and tools for survival
- [ ] **Performance Optimization** - Improve raycasting efficiency
- [ ] **Cross-platform Support** - Test and adapt for Linux/macOS

## ⛏️ Built Using <a name = "built_using"></a>

- [C](https://en.wikipedia.org/wiki/C_(programming_language)) - Programming Language
- [SDL2](https://www.libsdl.org/) - Graphics and Event Handling Library

## ✍️ Authors <a name = "authors"></a>

- [@xen0r-star](https://github.com/xen0r-star) - Project Creator & Developer