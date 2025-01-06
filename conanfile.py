import os

from conan import ConanFile
from conan.tools.cmake import cmake_layout
from conan.tools.files import copy


class SandboxConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        self.requires("imgui/1.91.5-docking")
        self.requires("glfw/3.3.8")
        self.requires("glad/0.1.36")
        self.requires("glm/1.0.1")
        self.requires("assimp/5.4.3")

    def generate(self):
        copy(self, "*glfw*", os.path.join(self.dependencies["imgui"].package_folder,
             "res", "bindings"), os.path.join(self.source_folder, "bindings"))
        copy(self, "*opengl3*", os.path.join(self.dependencies["imgui"].package_folder,
             "res", "bindings"), os.path.join(self.source_folder, "bindings"))

    def layout(self):
        cmake_layout(self)
