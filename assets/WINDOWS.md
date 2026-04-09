# Windows Build Tutorial (CLion + Visual Studio + vcpkg) — LibGraphics

Deze handleiding legt uit hoe je LibGraphics correct bouwt op Windows met:

- CLion
- Visual Studio 2022 Community
- MSVC v143
- Windows SDK (automatisch geïnstalleerd)
- vcpkg (x64-windows)
- Visual Studio generator (GEEN Ninja)

Dit is de enige configuratie die 100% stabiel werkt met OpenCV4, Tesseract en Leptonica.

============================================================
1. Installeer Visual Studio 2022
   ============================================================

Open de Visual Studio Installer en vink slechts één workload aan:

    ✔ Desktop development with C++

Dit installeert automatisch:

- MSVC v143 compiler
- Windows SDK (10 of 11)
- CMake tools
- NMake Makefiles
- Visual Studio generator

Je hoeft geen individuele SDK’s of extra componenten te selecteren.

============================================================
2. Installeer vcpkg
   ============================================================

   git clone https://github.com/microsoft/vcpkg
   cd vcpkg
   bootstrap-vcpkg.bat

Installeer dependencies:

    vcpkg install opencv4:x64-windows
    vcpkg install tesseract:x64-windows
    vcpkg install leptonica:x64-windows
    vcpkg install tesseract-data

Belangrijk:
- Gebruik **opencv4**, niet `opencv`.
- Windows gebruikt libs zonder “4”-suffix, dit is normaal.

============================================================
3. Configureer CLion Toolchain
   ============================================================

Ga naar:

    File → Settings → Build, Execution, Deployment → Toolchains

Stel in:

- Toolchain: Visual Studio
- C Compiler: cl.exe
- C++ Compiler: cl.exe
- Debugger: bundled
- Environment: leeg laten

CLion detecteert automatisch:

- Windows SDK
- MSVC toolchain

⚠ BELANGRIJK:
Gebruik GEEN Ninja generator — Ninja veroorzaakt timestamp- en include‑path‑bugs op Windows.

============================================================
4. Configureer CMake in CLion
   ============================================================

Ga naar:

    Settings → Build, Execution, Deployment → CMake

Stel in:

- Generator: **Visual Studio 17 2022**
- Toolchain: Visual Studio
- CMake options:

      -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake

============================================================
5. Verwijder oude build-mappen
   ============================================================

Verwijder:

    cmake-build-debug/
    cmake-build-release/
    cmake-build-debug-visual-studio/
    cmake-build-release-visual-studio/
    build/

============================================================
6. Builden
   ============================================================

Klik:

- Reload CMake Project
- Build

Je krijgt:

- ✔ OpenCV4 gevonden
- ✔ Leptonica gevonden
- ✔ Tesseract gevonden
- ✔ MSVC toolchain gevonden
- ✔ Windows SDK gevonden
- ✔ LibGraphics.dll gebouwd

============================================================
7. Installeren (Administrator)
   ============================================================

Ga naar de CLion build-map:

    cd Graphics/cmake-build-debug-visual-studio

Voer uit:

    cmake --install . --config Release --prefix "C:/Program Files/LibGraphics"


Dit installeert:

- C:/Program Files/LibGraphics/bin/LibGraphics.dll
- C:/Program Files/LibGraphics/lib/LibGraphics.lib
- C:/Program Files/LibGraphics/include/LibGraphics/...
- C:/Program Files/LibGraphics/lib/cmake/LibGraphics/LibGraphicsConfig.cmake

============================================================
Klaar
============================================================

Je hoeft dus geen Windows 10 SDK handmatig te selecteren.
Je hoeft geen Desktop SDK te zoeken.
Je hoeft geen Ninja te gebruiken (sterker nog: niet doen).
Je hoeft geen individuele componenten aan te vinken.

Alleen “Desktop development with C++” is genoeg.
