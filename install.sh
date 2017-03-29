#!/bin/sh

# Executable
cp ./bin/calc /bin

# Icon 
mkdir -p /usr/share/icons/calc/
cp calc.svg /usr/share/icons/calc/calc.svg

# Launcher
echo [Desktop Entry]'\n'\
Type=Application'\n'\
Terminal=true'\n'\
Name=calc'\n'\
Icon=/usr/share/icons/calc/calc.svg'\n'\
Exec=/bin/calc > /usr/share/applications/calc.desktop