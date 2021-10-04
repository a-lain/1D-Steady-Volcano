dir=$(pwd)

echo "Compilando el programa de consola para Linux."

# Copiamos los archivos
mkdir /tmp/Volcanes
cp -r VolcanesGUI /tmp/Volcanes

# Compilamos el programa como tal
cd /tmp/Volcanes/VolcanesGUI/Simulación
echo "Compilando fundamentos.cpp"
g++ -c -Wall -Wextra -pedantic fundamentos.cpp
echo "Compilando Koyaguchi.cpp"
g++ -c -Wall -Wextra -pedantic Koyaguchi.cpp
echo "Compilando progreso.cpp"
g++ -c -Wall -Wextra -pedantic progreso.cpp
echo "Compilando solveDiff.cpp"
g++ -c -Wall -Wextra -pedantic solveDiff.cpp
echo "Compilando volcanes.cpp"
g++ -c -Wall -Wextra -pedantic volcanes.cpp
echo "Compilando analyticalKoyaguchi.cpp"
g++ -c -Wall -Wextra -pedantic analyticalKoyaguchi.cpp
echo "Compilando útil.cpp"
g++ -c -Wall -Wextra -pedantic útil.cpp
cd ..
cd Consola
echo "Compilando main.cpp"
g++ -pthread -Wall -Wextra -pedantic main.cpp -o Volcanes "../Simulación/fundamentos.o" "../Simulación/Koyaguchi.o" "../Simulación/progreso.o" "../Simulación/solveDiff.o" "../Simulación/volcanes.o" "../Simulación/analyticalKoyaguchi.cpp" "../Simulación/útil.o"
mv -f Volcanes "$dir/Ejecutables"

# Borramos los archivos
rm -r /tmp/Volcanes
cd "$dir"
