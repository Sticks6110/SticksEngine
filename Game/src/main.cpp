#include <iostream>
#include <SticksEngine/Engine.h>

int main() {
    std::cout << "Hello, World!" << std::endl;

    Engine engine;
    engine.Initialize();

    Renderer render;
    render.CreateWindow("Stick's Engine Demo");

    engine.Run();

    return 0;
}