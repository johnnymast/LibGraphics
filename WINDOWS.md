# Windows Build Tutorial (CLion + Visual Studio + vcpkg)

Deze handleiding legt uit hoe je LibGraphics (of elke C++ library) correct bouwt op Windows met:

- CLion
- Visual Studio 2022 Community
- MSVC v143
- Windows SDK (automatisch geïnstalleerd)
- vcpkg (x64-windows)
- Visual Studio generator (GEEN Ninja)

Dit is de enige configuratie die 100% stabiel werkt met OpenCV, Tesseract en Leptonica.

------------------------------------------------------------
1. Installeer Visual Studio 2022 Community
------------------------------------------------------------

Open de Visual Studio Installer en vink slechts één workload aan:

    ✔ Desktop development with C++

Dit installeert automatisch:

- MSVC v143 compiler
- Windows SDK (10 of 11)
- CMake tools
- NMake Makefiles
- Visual Studio generator
- Alle benodigde headers en libs

Je hoeft geen individuele SDK’s meer te selecteren.

------------------------------------------------------------
2. Installeer vcpkg
------------------------------------------------------------

    git clone https://github.com/microsoft/vcpkg
    cd vcpkg
    bootstrap-vcpkg.bat

Installeer dependencies:

    vcpkg install opencv4:x64-windows
    vcpkg install tesseract:x64-windows
    vcpkg install leptonica:x64-windows
    vcpkg install tesseract-data

------------------------------------------------------------
3. Configureer CLion Toolchain
------------------------------------------------------------

Ga naar:

    File → Settings → Build, Execution, Deployment → Toolchains

Selecteer:

- Toolchain: Visual Studio
- C Compiler: cl.exe
- C++ Compiler: cl.exe
- Debugger: bundled
- Environment: leeg laten

CLion detecteert automatisch:

- Windows SDK
- MSVC toolchain

⚠ BELANGRIJK:
CLion gebruikt standaard Ninja, maar Ninja veroorzaakt op Windows timestamp-bugs.
Daarom gebruiken we de Visual Studio generator.

------------------------------------------------------------
4. Configureer CMake in CLion
------------------------------------------------------------

Ga naar:

    Settings → Build, Execution, Deployment → CMake

Stel in:

- Generator: **Visual Studio 17 2022**
- Toolchain: Visual Studio
- CMake options:

      -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake

------------------------------------------------------------
5. Verwijder oude build-mappen
------------------------------------------------------------

Verwijder:

    cmake-build-debug/
    cmake-build-release/
    cmake-build-debug-visual-studio/
    build/   (als je die handmatig hebt gemaakt)

------------------------------------------------------------
6. Builden
------------------------------------------------------------

Klik:

- Reload CMake Project
- Build

Je krijgt:

- ✔ OpenCV gevonden
- ✔ Leptonica gevonden
- ✔ Tesseract gevonden
- ✔ LibGraphics.dll gebouwd

------------------------------------------------------------
Install (Als Administrator)
------------------------------------------------------------

Ga naar de CLion build-map:

    cd Graphics/cmake-build-debug-visual-studio

Voer uit:

    cmake --install . --prefix "C:/Program Files/LibGraphics"

Dit installeert:

- C:/Program Files/LibGraphics/bin/LibGraphics.dll
- C:/Program Files/LibGraphics/lib/LibGraphics.lib
- C:/Program Files/LibGraphics/include/LibGraphics/...
- C:/Program Files/LibGraphics/lib/cmake/LibGraphics/LibGraphicsConfig.cmake

------------------------------------------------------------
Klaar
------------------------------------------------------------

Je hoeft dus geen Windows 10 SDK meer te selecteren.
Je hoeft geen Desktop SDK meer te zoeken.
Je hoeft geen Ninja te gebruiken (sterker nog: niet doen).
Je hoeft geen individuele componenten meer aan te vinken.

Alleen “Desktop development with C++” is genoeg.
