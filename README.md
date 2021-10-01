[![MIT License][license-shield]][license-url]
[![Trello Project Board][board-shield]][board-url]


<!-- PROJECT LOGO -->
<br />
<p align="center">
  <a href="#">
    <img src="docs/resources/logo_head.png" alt="BeastEngine Logo" width="80" height="80">
  </a>

  <h3 align="center">BeastEngine</h3>
</p>



<!-- TABLE OF CONTENTS -->
<details open="open">
	<summary>Table of Contents</summary>
	<ol>
	    <li>
	        <a href="#about-the-project">About The Project</a>
	        <ul>
	            <li><a href="#motivation">Motivation</a></li>
	        </ul>
	        <ul>
	            <li><a href="#supported-platforms">Supported Platforms</a></li>
	        </ul>
	    </li>
	    <li>
	        <a href="#building-the-project">Building The Project</a>
	        <ul>
	            <li><a href="#additional-info">Additional Info</a></li>
	        </ul>
	    </li>
	    <li><a href="#roadmap">Roadmap</a></li>
	    <li><a href="#license">License</a></li>
	    <li><a href="#acknowledgements">Acknowledgements</a></li>
	</ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

BeastEngine is a C++17 game engine created with 2D Action-RPG games in mind.

### Motivation
I've always been passionate about game engines, and that's why I am creating my own from scratch. I have already been working on a game engine before, but I didn't have a clear vision on how to build it and eventually it became a playground for discovering `OpenGL` aspects, and rendering in general. I no longer work on it, but you can find this project [here](https://bitbucket.org/bearengine/bearenginegraphics/src/master/).

### Supported Platforms
Currently, the project has only been built and tested on:

 - Windows 10 + MSVC

<!-- GETTING STARTED -->
## Building The Project
This project is based on the [BasicCppSetup](https://github.com/ComaszTyrulik/BasicCppSetup). Please refer to the [`Getting Started`](https://github.com/ComaszTyrulik/BasicCppSetup#getting-started) section of the `BasicCppSetup` project to find out how to build the `BeastEngine`.

### Additional Info
`BeastEngine` uses all `CMake` options provided by the `BasicCppSetup`, but also defines additional options that can alter the build process.

#### CMake Options:
| Name | Default|Description|
|------|--------|-----------|
|`BE_ENABLE_DEBUG_INFO`|ON|If ON, `BeastEngine` will contain definitions of debugging utilities, such as debug logging. It will also include additional debug information in exceptions. See [`Debugging.h`](https://github.com/BeastEngine/BeastEngine/blob/develop/include/BeastEngine/Core/Debugging.h) for example.|
|`BE_ENABLE_ASSERTIONS`|ON|If ON, `BeastEngine` will contain definitions of debugging assertions. See [`Assertions.h`](https://github.com/BeastEngine/BeastEngine/blob/develop/include/BeastEngine/Core/Assertions.h) for example.|
|`BUILD_WIN32_TESTS`|Platform dependent|BEASTIntegration target contains integration tests for BeastEngine. Among those tests there are ones which test [`Win32Window`](https://github.com/BeastEngine/BeastEngine/blob/develop/include/BeastEngine/Core/Windows/Win32/Win32Window.h) class. Those tests can be time consuming so you can exclude them from build. This value is by default set to `ON` on `Windows` and `OFF` on other platforms.|

## Roadmap
I haven't created a specific roadmap for the project yet, but I am using [Trello](https://trello.com/) for keeping track of tasks and TODOs. You can find the board [here](https://trello.com/b/Rj73uWjS/beastengine).

## License
Distributed under the MIT License. See [`LICENSE`](LICENSE) for more information.

## Acknowledgements
* [README template](https://github.com/othneildrew/Best-README-Template)


[license-shield]: https://img.shields.io/github/license/othneildrew/Best-README-Template.svg?style=for-the-badge
[license-url]: LICENSE
[board-shield]: https://img.shields.io/badge/PROJECT-BOARD-blue?style=for-the-badge&logo=trello
[board-url]: https://trello.com/b/Rj73uWjS/beastengine
