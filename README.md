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
| `T` | Trigger takeoff |
| `W` | Move jet forward |
| `A` | Move jet left |
| `S` | Move jet backward |
| `D` | Move jet right |
| `Space` | Shoot missile |
| `L` | Trigger landing sequence |
| `ESC` | Exit simulation |

---

## Requirements

- g++ compiler
- OpenGL libraries: GLEW, GL, GLU, GLUT

To install the necessary libraries on Ubuntu/Debian:

```bash
sudo apt-get update
sudo apt-get install build-essential freeglut3-dev libglew-dev libglm-dev
```

---

## How to Compile and Run

```bash
g++ f2.c++ -o f2 -lGLEW -lGL -lGLU -lglut
./f2
```

---

## Project Structure

| File/Folder | Description |
|:------------|:------------|
| `f2.c++` | Main source code file |
| `models/` | (Optional) Folder for 3D jet models |
| `textures/` | (Optional) Folder for texture images |

---

## Future Improvements

- Add skybox and dynamic clouds
- Add smoke trails during missile firing
- Sound effects for engine and shooting
- Multiple enemy jets
- Scoring system and HUD

---

## Screenshots

*(Add screenshots of your simulation here!)*

---

## License

This project is created for educational purposes.  
Feel free to modify, use, and improve it!
