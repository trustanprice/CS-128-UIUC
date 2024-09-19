#include <iostream>

#include "functions.hpp"

int main(int argc, char** argv) {
  // Checks if arguments were passed correctly
  if (argc != 2) {
    std::string maui = "humuhumunukunukuapua'a";
    std::string awakea = "Hoaloha";

    maui = Pronunciation(maui);
    awakea = GetPronunciation(awakea);
    std::cout << maui << std::endl;
    std::cout << awakea << std::endl;
  }

  // initialize hawaiian_word with passed in argument
  std::string hawaiian_word = argv[1];

  // prints the result of GetPronunciation to terminal
  // std::cout << GetPronunciation(hawaiian_word) << std::endl;
}
