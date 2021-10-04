dir=$(pwd)

echo "Compilando el programa de consola para Linux."

# Copiamos los archivos
mkdir /tmp/Volcanes
cp -r VolcanesGUI /tmp/Volcanes

# Compilamos el programa como tal
cd /tmp/Volcanes/VolcanesGUI/Simulación
echo "Compilando fundamentos.cpp"
g++ -c fundamentos.cpp
echo "Compilando Koyaguchi.cpp"
g++ -c Koyaguchi.cpp
echo "Compilando progreso.cpp"
g++ -c progreso.cpp
echo "Compilando solveDiff.cpp"
g++ -c solveDiff.cpp
echo "Compilando volcanes.cpp"
g++ -c volcanes.cpp
echo "Compilando analyticalKoyaguchi.cpp"
g++ -c analyticalKoyaguchi.cpp
echo "Compilando útil.cpp"
g++ -c útil.cpp
cd ..
cd Consola
echo "Compilando main.cpp"
g++ -pthread main.cpp -o Volcanes "../Simulación/fundamentos.o" "../Simulación/Koyaguchi.o" "../Simulación/progreso.o" "../Simulación/solveDiff.o" "../Simulación/volcanes.o" "../Simulación/analyticalKoyaguchi.cpp" "../Simulación/útil.o"
mv -f Volcanes "$dir/Ejecutables"

# Borramos los archivos
rm -r /tmp/Volcanes
cd "$dir"


echo "Compilando el programa de consola para Windows 32-bit."

# Copiamos los archivos
mkdir /tmp/Volcanes
cp -r VolcanesGUI /tmp/Volcanes

# Compilamos el programa como tal
cd /tmp/Volcanes/VolcanesGUI/Simulación
echo "Compilando fundamentos.cpp"
i686-w64-mingw32.static-g++ -c -std=gnu++11 fundamentos.cpp
echo "Compilando Koyaguchi.cpp"
i686-w64-mingw32.static-g++ -c -std=gnu++11 -lm Koyaguchi.cpp
echo "Compilando progreso.cpp"
i686-w64-mingw32.static-g++ -c -std=gnu++11 progreso.cpp
echo "Compilando solveDiff.cpp"
i686-w64-mingw32.static-g++ -c -std=gnu++11 solveDiff.cpp
echo "Compilando volcanes.cpp"
i686-w64-mingw32.static-g++ -c -std=gnu++11 volcanes.cpp
echo "Compilando analyticalKoyaguchi.cpp"
i686-w64-mingw32.static-g++ -c -std=gnu++11 -lm analyticalKoyaguchi.cpp
echo "Compilando útil.cpp"
i686-w64-mingw32.static-g++ -c -std=gnu++11 útil.cpp
cd ..
cd Consola
echo "Compilando main.cpp"
i686-w64-mingw32.static-g++ -pthread -std=gnu++11 main.cpp -o "Volcanes (32-bit).exe" "../Simulación/fundamentos.o" "../Simulación/Koyaguchi.o" "../Simulación/progreso.o" "../Simulación/solveDiff.o" "../Simulación/volcanes.o" "../Simulación/analyticalKoyaguchi.cpp" "../Simulación/útil.o"
mv -f "Volcanes (32-bit).exe" "$dir/Ejecutables"

# Borramos los archivos
rm -r /tmp/Volcanes
cd "$dir"



echo "Compilando el programa de consola para Windows 64-bit."

# Copiamos los archivos
mkdir /tmp/Volcanes
cp -r VolcanesGUI /tmp/Volcanes

# Compilamos el programa como tal
cd /tmp/Volcanes/VolcanesGUI/Simulación
echo "Compilando fundamentos.cpp"
x86_64-w64-mingw32.static-g++ -c -std=gnu++11 fundamentos.cpp
echo "Compilando Koyaguchi.cpp"
x86_64-w64-mingw32.static-g++ -c -std=gnu++11 -lm Koyaguchi.cpp
echo "Compilando progreso.cpp"
x86_64-w64-mingw32.static-g++ -c -std=gnu++11 progreso.cpp
echo "Compilando solveDiff.cpp"
x86_64-w64-mingw32.static-g++ -c -std=gnu++11 solveDiff.cpp
echo "Compilando volcanes.cpp"
x86_64-w64-mingw32.static-g++ -c -std=gnu++11 volcanes.cpp
echo "Compilando analyticalKoyaguchi.cpp"
x86_64-w64-mingw32.static-g++ -c -std=gnu++11 -lm analyticalKoyaguchi.cpp
echo "Compilando útil.cpp"
x86_64-w64-mingw32.static-g++ -c -std=gnu++11 útil.cpp
cd ..
cd Consola
echo "Compilando main.cpp"
x86_64-w64-mingw32.static-g++ -pthread -std=gnu++11 main.cpp -o "Volcanes (64-bit).exe" "../Simulación/fundamentos.o" "../Simulación/Koyaguchi.o" "../Simulación/progreso.o" "../Simulación/solveDiff.o" "../Simulación/volcanes.o" "../Simulación/analyticalKoyaguchi.cpp" "../Simulación/útil.o"
mv -f "Volcanes (64-bit).exe" "$dir/Ejecutables"

# Borramos los archivos
rm -r /tmp/Volcanes
cd "$dir"



echo "Compilando el programa de interfaz gráfica para Linux."

# Copiamos los archivos
mkdir /tmp/Volcanes
cp -r VolcanesGUI /tmp/Volcanes

cd /tmp/Volcanes/VolcanesGUI
qmake VolcanesGUI.pro
make -j8
mv -f "VolcanesGUI" "$dir/Ejecutables"

# Borramos los archivos
rm -r /tmp/Volcanes
cd "$dir"



echo "Compilando el programa de interfaz gráfica para Windows 32-bit."

# Copiamos los archivos
mkdir /tmp/Volcanes
cp -r VolcanesGUI /tmp/Volcanes

cd /tmp/Volcanes/VolcanesGUI
/home/andres/AUR/MXE/mxe/usr/i686-w64-mingw32.static/qt5/bin/qmake VolcanesGUI.pro
make -j8
cd release
mv "VolcanesGUI.exe" "VolcanesGUI (32-bit).exe"
mv -f "VolcanesGUI (32-bit).exe" "$dir/Ejecutables"

# Borramos los archivos
rm -r /tmp/Volcanes
cd "$dir"



echo "Compilando el programa de interfaz gráfica para Windows 64-bit."

# Copiamos los archivos
mkdir /tmp/Volcanes
cp -r VolcanesGUI /tmp/Volcanes

cd /tmp/Volcanes/VolcanesGUI
/home/andres/AUR/MXE/mxe/usr/x86_64-w64-mingw32.static/qt5/bin/qmake VolcanesGUI.pro
make -j8
cd release
mv "VolcanesGUI.exe" "VolcanesGUI (64-bit).exe"
mv -f "VolcanesGUI (64-bit).exe" "$dir/Ejecutables"

# Borramos los archivos
rm -r /tmp/Volcanes
cd "$dir"
