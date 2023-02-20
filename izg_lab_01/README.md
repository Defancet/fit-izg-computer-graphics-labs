# Navod ke kompilaci
- Ke kompilaci je pozadovano Visual Studio 2019 nebo GCC 7.0.0+ a CMake 3.12.0+

## Linux
- mkdir build
- cd build/
- cmake ..
- make -j 4

V případě problémů se spuštěním zkontrolujte, zda máte požadované závislosti pro renderování pomocí SDL, zejména ověřte aktuálnost grafických ovladačů. SDL není potřeba instalovat, je přibaleno ke kostře programu. Kompletní závislosti pro Ubuntu 20.04 lze nalézt zde:

https://packages.ubuntu.com/en/source/focal/libsdl2

Většina balíčků je již součástí základního systému, překladače či grafického ovladače, avšak na některých distribucích mohou některé chybět a způsobovat pády.
## Windows
- Spuste CMake GUI
- Stistknete Browse Source a vyberte slozku s rozbalenym cvicenim
- Stistknete Browse Build a vyberte stejnou adresu s priponou build/, tzn. napr. cestaKeSlozce/izg_lab_01/build/
- Kliknete na Configure
- Nasledne kliknete na Generate
- Ve slozce build/ nyni bude soubor izg_lab_01.sln, pres ten spustte Visual Studio
- Nasledne lze jiz primo sestavit aplikaci a spustit
