# Pentane

A (work-in-progress) user-configurable plugin loader and modding framework for the following games:
- Rhythm Racing Engine
	- Cars: The Video Game (PC) 🟨 *Experimental!*
	- Cars: Mater-National Championship (PC) 🟨 *Experimental!*
- Octane Engine
	- Cars 2: The Video Game (PC) ✅ *Production-Ready!*
	- Cars 2: Arcade (PC) ✅ *Production-Ready!*
	- Cars 3: Driven to Win (Xbox One) ❌ *Non-Functional!*

For documentation and usage information, head on over to [our website](https://high-octane-dev.github.io/)!

# Downloads
Head to the [Releases](https://github.com/high-octane-dev/pentane/releases/latest) page to get the latest build!

## Installation and Usage

Head to the [Installation and Setup](https://high-octane-dev.github.io/installation-and-setup/) guide on the website!

# Building
Pentane uses Visual Studio 2022 (MSBuild) as its build system, as it plays nicely with vcpkg; which is what it uses for pulling dependencies.
Pentane uses [vcpkg](https://vcpkg.io) in manifest mode, and depends on the following libraries:
- tomlplusplus (Used for `toml` parsing)
- detours (Required by sunset)
- zydis (Required by sunset)

Additionally, Pentane depends on [sunset](https://github.com/itsmeft24/sunset), an inline/replacement hooking library. As of writing, sunset is directly included in the repository, but will likely be *converted into a git submodule* in the future.

# Credits
- flaynirite (Russian Localization Help)
- [Code](https://github.com/Daavel) (German/Polish Localization Help)
- [maximilian](https://github.com/DJmax0955) (Polish Localization Help)
- [OwenTheProgrammer](https://github.com/OwenTheProgrammer) (French Localization Help)
- Timix91 (Russian Localization Help)
- [MythicalBry](https://github.com/MythicalBry) (Spanish Localization Help)
- [Raytwo](https://github.com/Raytwo) (French Localization Help)

# Inspirations
- [Raytwo/ARCropolis](https://github.com/Raytwo/ARCropolis)
- [skyline-dev/skyline](https://github.com/skyline-dev/skyline)
