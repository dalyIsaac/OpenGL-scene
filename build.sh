mkdir -p build_sh

g++ -c -o build_sh/cannon.o src/cannon.cpp 
g++ -c -o build_sh/castle.o src/castle.cpp 
g++ -c -o build_sh/donut.o src/donut.cpp 
g++ -c -o build_sh/main.o src/main.cpp 
g++ -c -o build_sh/robot.o src/robot.cpp 
g++ -c -o build_sh/spaceship.o src/spaceship.cpp 
g++ -c -o build_sh/sweepSurfaces.o src/sweepSurfaces.cpp 
g++ -c -o build_sh/trampoline.o src/trampoline.cpp 

g++ -o program.out build_sh/cannon.o build_sh/castle.o build_sh/donut.o build_sh/main.o build_sh/robot.o build_sh/spaceship.o build_sh/sweepSurfaces.o build_sh/trampoline.o -lm -lGL -lGLU -lglut

program.out
