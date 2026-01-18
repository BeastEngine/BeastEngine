from conan import ConanFile
from conan.tools.cmake import cmake_layout


class BeastEngine(ConanFile):
    settings = ["os", "compiler", "build_type", "arch"]
    requires = [
        "spdlog/1.14.1",
        "gtest/1.12.1",
        "glm/1.0.1",
        "stduuid/1.2.2",
        "xxhash/0.8.1",
    ]
    generators = ["CMakeDeps", "CMakeToolchain"]

    def configure(self):
        self.options["stduuid"].with_cxx20_span = True

    def layout(self):
        cmake_layout(self)
        self.folders.generators = self.folders.build
