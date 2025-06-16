# GRICS - Graphics & Physics Engine

Hey there! 👋  
This is my attempt on making a full fledged Graphics + Physics engine. The major focus will heavily be on simulating physics so support for custom shaders , lightning etc. is not planned yet. Please feel free to take a look and use this for your own projects 

## What’s Done uptil now 

- **Cube Renderer**: Spawn, move, and delete cubes in real-time.
- **ImGui UI**: Add cubes using sliders, manage them in a table, and tweak settings live.
- **Camera System**: First-person navigation using mouse and keyboard (WASD + mouse).
- **Grid Floor**: A visual reference plane so your cubes don’t float in limbo.
- **Basic Structure**: Organizing rendering, UI, and interaction cleanly for future physics simulations.

## Controls

| Action        | Key / Input                    |
|---------------|--------------------------------|
| Move camera   | `W A S D Space Shift`          |
| Look around   | Mouse                          |
| Toggle mouse  | `Tab`                          |
| Exit          | `Esc`                          |

## Tech Stack

- **C++**
- **OpenGL 3.3 Core**
- **GLAD** (loader)
- **GLFW** (window/input)
- **ImGui** (docking UI)
- **GLM** (math)

## How to Build
Right now You can only build using Visual studio Community 2022 (x86) but please feel free to build it yourself as all the external dependencies is included in the project files.

## Future Ideas
- Physics simulation for particles and rigid bodies
- Collision detection
- Lighting & shadows
- Texture support

## Contributing
Right now it’s mostly for learning and fun, but feel free to open an issue or drop a suggestion!
