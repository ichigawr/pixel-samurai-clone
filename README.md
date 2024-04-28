## Appendix
- [Horizon's Fall](#horizons-fall)
  - [Overview](#overview)
  - [Features](#features)
  - [Credits](#credits)
  - [Project Structure](#project-structure)

- [Installation](#installation)
  - [Play Game](#play-game)
  - [Development](#development)
  - [How To Play](#how-to-play)

- [Contributors](#contributors)

- [License](#license)


# Horizon's Fall

## Overview
Horizon's Fall is a Souls-like game developed as a part of the Advanced Programming course at the University of Engineering and Technology, Vietnam National University, Hanoi. It serves as a major assignment for the course, focusing on game development using SDL2.

[Watch introduction video in Vietnamese.](https://youtu.be/xC5n-PCGXZM)

## Features
- **Intense and challenging gameplay** inspired by the Souls-like genre.
- **Unlimited gameplay** with an auto-restart feature.
- **Immersive sound effects and music** to enhance the atmosphere.

## Credits

### Resources Used:
- **Fonts**: [Terminal font](https://www.cufonfonts.com/font/terminal) - Fonts used for text in the game.
- **Graphics**: Graphics used for game's logo, character sprites and environments were sourced from [Pixel Samurai](https://artofkings.itch.io/pixel-samurai-old), a game developed by [ArtofKings](https://itch.io/profile/artofkings).
- **Audio**: 
  - Audio files used for in-game music were obtained from [Japanese Taiko Drums Vol 1](https://youtu.be/jHnuytrr6Ew?si=899zXtztQ4khlSzX).
  - Sound effects used for in-game actions were also sourced from [Pixel Samurai](https://artofkings.itch.io/pixel-samurai-old).

### Code References:
- The game development techniques and code examples were learned from the tutorial series [How To Make A Game In C++ & SDL2 From Scratch!](https://youtube.com/playlist?list=PLhfAbcv9cehhkG7ZQK0nfIGJC_C-wSLrx&si=X97uRO9hj2bBhFcF) by the YouTube channel [Let's Make Games](https://youtube.com/@CarlBirch).
- Code snippets and implementation ideas were also referenced from the repository [BirchEngine](https://github.com/carlbirch/BirchEngine) by [carlbirch](https://github.com/carlbirch).


## Project Structure
```bash
[horizon_s_fall]
│   README.md
│
├── src
│   ├── include
│   │       ...
│   └── lib
│           ...
│
├── assets
│   ├── animations
│   │       ...
│   ├── fonts
│   │       ...
│   ├── maps
│   │       ...
│   ├── sounds
│   │       ...
│   │   ...
│
├── ECS
│   │   Animation.hpp
│   │   ColliderComponent.hpp
│   │   Components.hpp
│   │   ECS.cpp
│   │   ECS.hpp
│   │   KeyboardController.hpp
│   │   SpriteComponent.hpp
│   │   TileComponent.hpp
│   │   TransformComponent.hpp
│   │   UILabel.hpp
│
│   AssetManager.cpp
│   AssetManager.hpp
│   Enemies.cpp
│   Enemies.hpp
│   Game.cpp
│   Game.hpp
│   Map.cpp
│   Map.hpp
│   Menu.cpp
│   Menu.hpp
│   SoundManager.cpp
│   SoundManager.hpp
│   TextureManager.cpp
│   TextureManager.hpp
│   Vector2D.cpp
│   Vector2D.hpp
│   main.cpp
│
│   horizon_s_fall.exe
│   Makefile
│
│   ...
```


# Installation

## Play Game
Download the [Compiled version](https://github.com/ichigoih/horizon_s_fall/releases/tag/v1.0.0) of the game to start playing immediately.

## Development
These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites
What things you need to install the game:
- MinGW
- GNU make

### Installing
1. Clone this repository to your device or [Download Source code (.zip or .tar.gz)](https://github.com/ichigoih/horizon_s_fall/releases/tag/v1.0.0).
2. Navigate to the project directory.
3. Compile the project using Makefile.

## How To Play
- Use **A** and **D** keys to move **left** and **right** respectively.
- Press **J** to **attack** enemies and **K** to **block** incoming attacks.
- Use the **Spacebar** to perform a **dash** maneuver.
- Press **Esc** to **exit** the game.


# Contributors
- NguyenXuanVinh


# License
This project is licensed under the Apache License 2.0. See the [LICENSE](LICENSE) for details.
