#include <iostream>

int main(int, char**) {
    std::cout << "Hello, world!\n";
}


#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;
fs::current_path("/home/black_wraith/Documents//AIR/S8/RealTime/packt/Beginning-Cpp-Game-Programming-Second-Edition/Chapter07");
std::cout << "Current path is " << fs::current_path() << '\n';