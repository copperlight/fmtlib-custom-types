from conan import ConanFile


class SpectatorDConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    requires = (
        "fmt/11.0.2",
    )
    tool_requires = ()
    generators = "CMakeDeps", "CMakeToolchain"
