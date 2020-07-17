from conans import ConanFile, CMake


class BeastEngine(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    requires = "spdlog/[>=1.4.2]", "gtest/[>=1.8.1]"
    generators = "cmake"
