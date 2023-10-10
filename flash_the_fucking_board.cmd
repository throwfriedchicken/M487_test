Echo "Erase APROM area"
"%ProgramFiles(x86)%\Nuvoton Tools\NuLink Command Tool\NuLink.exe" -e APROM
Echo "Flash your program to APROM"
"%ProgramFiles(x86)%\Nuvoton Tools\NuLink Command Tool\NuLink.exe" -w APROM "D:\M487_test\build\main.hex"
"%ProgramFiles(x86)%\Nuvoton Tools\NuLink Command Tool\NuLink.exe" -reset
