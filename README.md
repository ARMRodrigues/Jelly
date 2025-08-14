# Jelly Engine 🐙

Jelly Engine is a C++ engine focused on low-poly, stylized graphics, ideal for simple games.  
The engine is designed to support multiple graphics APIs, although currently only Vulkan is implemented (one step at a time, right? 😅).

## Goals 🎯

- Low-poly and stylized graphics rendering 
- Support for simple games
- Multi-API architecture (currently Vulkan, planned for DirectX, OpenGL, etc.)
- Flexible shader system using [Slang Shader Language](https://github.com/shader-slang/slang)

## Status 🚧

⚡Jelly Engine is in the very early stages of development ⚡  

Currently, only basic Vulkan rendering is implemented. Many core features are still planned, including the full rendering system, multi-API support, and editor tools.

## Technologies and Libraries 🛠️

- **C++20** – main programming language
- **Vulkan** – primary graphics API
- **Slang** – for shader compilation and management
- **GLFW** – window management
- **SDL** – input handling (keyboard, mouse, gamepads)
- **glm** – 3D math library (vectors, matrices, transformations)
- **entt** – ECS (Entity Component System) for entity and component management

## How to Build 🏗️

> Currently only Windows and Linux are supported (MacOS has not been tested yet)

```bash
mkdir Build
cd Build
cmake ..
cmake --build .
````
## License 📄

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.