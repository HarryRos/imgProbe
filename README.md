# imgProbe
Interface that queries an image sensor and writes the returned data to disk as tiff images.

# Description
This project implements an interface that facilitates the use of a hardware sensor that reads ultrasound or computed tomography images. It provides a wrapper class that calls the hardware asynchronously and allows the main routine to continue without blocking. It also handles errors and hardware freeze.

For the intended use, refer to `imgProbeMain.cpp` (see below for compilation and execution instructions).

For the testing suite, refer to `imgProbeTesting.cpp` (see below for compilation and execution instructions).

# Compile and execute imgProbeMain

    g++ -I. -g .\src\**.c* .\imgProbeMain.cpp -ltiff -lgdi32 -static-libstdc++ -std=c++2a -o .\imgProbe.exe
    .\imgProbe.exe

# Compile and execute imgTesting

    g++ -I. -g .\src\**.c* .\Testing\imgProbeTesting.cpp -ltiff -lgdi32 -static-libstdc++ -std=c++2a -o .\imgProbeTesting.exe
    .\imgProbeTesting.exe   

# Author
Charalambos (Harry) Rossides Ph.D.
rossides.ac@gmail.com