from conans import ConanFile
from conan.tools.cmake import cmake_layout


class BeastEngine(ConanFile):
    settings = ["os", "compiler", "build_type", "arch"]
    requires = [
        "spdlog/1.11.0",
        "gtest/1.12.1",
        "glm/0.9.9.8",
        "stduuid/1.2.2",
        "xxhash/0.8.1",
        "fmt/9.1.0",
        "entt/3.11.0",
    ]
    generators = ["CMakeDeps", "CMakeToolchain"]

    def configure(self):
        self.options["stduuid"].with_cxx20_span = True

    # TODO: Uncomment once Visual Studio adds support for CMakePresets schema version 4
    # Otherwise, the "include" key won't work. For now, let the SDK automatically copy the generated presets file
    # def layout(self):
    #     cmake_layout(self)
    #     self.folders.generators = self.folders.build
