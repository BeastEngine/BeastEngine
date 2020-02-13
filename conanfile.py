from conans import ConanFile, CMake

class BeastEngine(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    requires = "spdlog/[>=1.4.2]"
    generators = "cmake"