# sdl-lua

### Screenshot

![Screenshot](https://github.com/LukeGoule/sdl-lua/blob/main/runtime/res/git.png?raw=true)

### Installation
1. Download the following packages (or build if necessary), create this folder structure:
- `./packages`
    - [FFTW3](https://www.fftw.org/install/windows.html)
        - `include`
            - fftw3.h
        - `lib`
            - libfftw3f-3.lib
    - [GLEW](https://glew.sourceforge.net/)
        - `include`
            - GL
                - eglew.h
                - glew.h
                - glxew.h
                - wglew.h
        - `lib`
            - x86
                - glew32.lib
                - glew32s.lib
    - [GLM](https://github.com/g-truc/glm)
        - (dump the full repo contents in here)
    - [ImGui](https://github.com/ocornut/imgui)
        - (paste all of the source files, including the implementations, in here)
    - [ImGuiColorTextEdit](https://github.com/santaclose/ImGuiColorTextEdit)
        - Contents of [this fork](https://github.com/santaclose/ImGuiColorTextEdit) go in here. See [this issue](https://github.com/santaclose/ImGuiColorTextEdit/issues/13#issue-1891214405) for getting rid of Boost.
    -  [Lua](https://www.lua.org/ftp/)
        - (paste all of the Lua C/C++ source in here.)
    - [Portaudio](https://www.portaudio.com/docs/v19-doxydocs/compile_windows.html)
        - `include`
            - portaudio.h
        - `lib`
            - portaudio_x86.lib
    - [SDL](https://www.libsdl.org/)
        - `include`
            - (header files from SDL 2.)
        - `lib`
            - x86
                - (lib files from SDL 2.)
2. Build as Debug x86.
3. Add the `.dll` files from each of the (pre) compiled packages to the `runtime` folder.
4. Run the application.
