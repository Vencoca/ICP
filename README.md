# ICP
Řešení obsahuje dva projekty - Tracker a záverečnou práci
## Popis

### Hlavní projekt ICP
Aplikace "chyť si svůj teapod". Po zapnutí je uživatel v aréně která se každých 5 sekund mění. Za úkol má sebrat točící se oranžový teapod. Když ho sebere do konzole se vypíše skóre a změní se poloha teapodu. Uživatel se musí vyhýbat krabícím a snažit se nezaseknout se měnící se aréně, prootže jinak musí 5s čekat než se aréna změní a pustí ho z krabice. Celá aréna je osvětlena levitujícím králíkem.

- Ovládání 
    - WSAD pro pohyb
    - Myš pro rozhlížení
    - Kolečko myši mení FOV
    - F pro změnu z okna na fullscreen
    - ESC pro ukočení programu

### Tracker
Vícevláknová aplikace pro trakování zelené "žárovky". Aplikace otevře webkameru a do konzole vypisuje kde se nachází střed zelené barvy. V okně je zobrazen obraz z webkamery s křížem kde se nachází střed.

## Sestavení
Pro sestavení projektů je potřeba mít vytrovřenou systémovou proměnou OPENCV_DIR která směřuje do složky /build opencv. A do PATH přidat %OPENCV_DIR%\x64\vc15\bin. Zbylé závislosti už jsou řešeny v rámci projektu a jsou součástí repozitáře. (glew, glfw...)