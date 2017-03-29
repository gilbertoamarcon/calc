#!/bin/sh

# Executable
cp ./bin/calc /bin

# Icon 
mkdir -p /usr/share/icons/calc/
cp calc.svg /usr/share/icons/calc/calc.svg

# Launcher
echo [Desktop Entry]'\n'\
Type=Application'\n'\
Comment=A terminal-based RPN calculator.'\n'\
Name=Calc'\n'\
Icon=/usr/share/icons/calc/calc.svg'\n'\
Exec=xterm -cm -uc -fa \'Liberation Mono\' -fs 18 -geometry 18x10+0+0 -T Calc -e \'/bin/calc\''\n'\
 > /usr/share/applications/calc.desktop
