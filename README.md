# CG-Project

This project simulates a **3D fighter jet takeoff and landing** using **OpenGL**, featuring smooth animations, dynamic camera control, and realistic airstrip and terrain rendering. It showcases jet movement, rotation, and interaction with the environment.

---

## Features

- 3D textured fighter jet model
- Smooth takeoff and landing animations
- Dynamic camera following the jet
- WASD flight controls
- Target enemy jet
- Shooting missiles
- Green grass ground and detailed airstrip with white stripes

---

## Controls

| Key | Action |
|:----|:-------|
| `T` | Trigger takeoff (automated) |
| `L` | Trigger landing (automated) |
| `W` | Move jet forward |
| `A` | Move jet left |
| `S` | Move jet backward |
| `D` | Move jet right / Emit smoke trail |
| `Q` | Pitch jet upward |
| `E` | Pitch jet downward |
| `Space` | Shoot missile |
| `ESC` | Exit simulation |

---
## Requirements

- g++ compiler
- OpenGL libraries: GLEW, GL, GLU, GLUT
- OpenAL for sound effects

To install the necessary libraries on Ubuntu/Debian:

```bash
sudo apt-get update
sudo apt-get install build-essential freeglut3-dev libglew-dev libglm-dev libopenal-dev
```

---

## How to Compile and Run [Linux]

```bash
g++ f2.c++ -o f2 -lGLEW -lGL -lGLU -lglut -lopenal -ldl ; ./f2
```

---

## Project Structure

| File/Folder | Description |
|:------------|:------------|
| `f2.c++` | Main source code file |
| `model/` | (Optional) Folder for 3D jet models |
| `textures/` | (Optional) Folder for texture images |

---

## Future Improvements

- Add skybox and dynamic clouds
- AI enabled Jets
- Multiple enemy jets
- Scoring system and HUD

---

## Project Report

- Project Report PDF is avialable in this repository. 

---

## License

This project is created for educational purposes.  
Feel free to modify, use, and improve it!
