[![MIT License][license-shield]][license-url]
[![GitHub Project Board][board-shield]][board-url]


<!-- PROJECT LOGO -->
<br />
<p align="center">
  <a href="#">
    <img src="docs/resources/logo_head.png" alt="BeastEngine Logo" width="80" height="80">
  </a>

  <h3 align="center">BeastEngine</h3>
</p>

## About The Project

BeastEngine is a 2D game engine created with Action-RPG games in mind. It's a hobby project developed out of interest in game engines.

### History and Motivation

I have been passionate about games and game engines since I started programming. Early in my programming career, I attempted to create my own game engine, with the hope of producing something great. Quickly, though, I realized I didn't have a clear vision or enough skill to build it, so it eventually became a playground for experimenting with `OpenGL`, rendering, animations, and other aspects of game engine development. I had a lot of fun, and I learnt a lot. Although I haven't worked on it for a couple of years now, the project is still available online and can be found [here](https://bitbucket.org/bearengine/bearenginegraphics/src/master/).

The current incarnation of BeastEngine is a spiritual successor to the previous project - this time with much smaller ambitions. I want to have fun with creating it, without any specific milestones or goals. I jump in and out of the codebase whenever I feel like it.

The features I add are focused on 2D games. I wish to have some demo game(s) eventually, but I don't expect any full-fledged productions running on BeastEngine (at least in the foreseeable future).

### Supported Platforms
Currently, the project only supports Windows with DirectX 11.

## Building The Project

### Prerequisites

In order to build the project locally, you need a couple of things:

 * C++20-compatible MSVC compiler
 * CMake
 * Conan package manager

For making the development easier, I use [BeastSDK](https://github.com/BeastEngine/BeastSDK) - a set of Python scripts that make my life easier.

The SDK is a Python-base application, so the recommended way is to install via a tool like [pipx](https://pipx.pypa.io/stable/installation/) to have it accessible globally.

> NOTE: BeastSDK requires at least Python 3.12.

### Conan and CMake

The project uses the [Conan package manager](https://conan.io/) for managing dependencies, and cmake.
The versions I use are `conan==2.9.2` and `cmake==3.31.0`. I haven't tested with other versions, but I assume higher versions should work. The minimum CMake version is `3.16`. For conan, I wouldn't go below `2.9.2`.

> `BeastSDK` comes with CMake and Conan bundled as part of its virtual environment. If you want to, instead of having a global conan and cmake, you can use `pipenv` to enter BeastSDK's virtual env, and use conan and cmake that come with the scripts.
> For details, see: [BeastSDK docs](https://github.com/BeastEngine/BeastSDK/blob/master/README.md#usage)

Running the `beast install` command from the root of the project installs all C++ dependencies and creates the `CMakeUserPresets.json` file.

The presets file (as of writing this text) contains only a single `conan-default` configuration preset, and `conan-debug`, `conan-release`, `conan-minsizerel`, and `conan-relwithdebinfo` build presets.

To configure and build the project, you need the following:

```bash
# Install all dependencies using BeastSDK
beast install

# Configure the project
cmake --preset conan-default

# Build the project
cmake --build --preset conan-debug
```

> NOTE: For altering the build options, see the root [CMakeLists.txt](CMakeLists.txt) file.

## Work in Progress

The current two biggest features I work on are:
 * [Basic 2D textures support](https://github.com/BeastEngine/BeastEngine/issues/43)
 * [ECS system (vol. 2)](https://github.com/BeastEngine/BeastEngine/issues/47)

For the list of all issues and ideas, see the [project board](https://github.com/orgs/BeastEngine/projects/1).

## License
Distributed under the MIT License. See [`LICENSE`](LICENSE) for more information.

## Acknowledgements
* [README template](https://github.com/othneildrew/Best-README-Template)


[license-shield]: https://img.shields.io/github/license/othneildrew/Best-README-Template.svg?style=for-the-badge
[license-url]: LICENSE
[board-shield]: https://img.shields.io/badge/PROJECT-BOARD-blue?style=for-the-badge&logo=trello
[board-url]: https://github.com/orgs/BeastEngine/projects/1
