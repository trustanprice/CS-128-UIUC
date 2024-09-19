#include <iostream>

#include "cord.hpp"
#include "cord_utilities.hpp"

int main() {
  // Create several Cord objects
  SharedPointer<Cord> cord1 = new Cord("he");   // 0-1
  SharedPointer<Cord> cord2 = new Cord("llo");  // 2-4
  SharedPointer<Cord> cord3 = new Cord("wor");  // 5-6
  SharedPointer<Cord> cord4 = new Cord("ld");   // 7-9

  // Concatenate the cords
  SharedPointer<Cord> concatenated = ConcatCords(
      ConcatCords(cord1, cord2),
      ConcatCords(cord3, cord4));  // tree that represents hello world

  // Output the concatenated string
  auto concatenated_str = concatenated->ToString();

  // Perform substring operation
  SharedPointer<Cord> sub_cord = SubString(concatenated, 6, 9);
  auto substring_str = sub_cord->ToString();
  std::cout << substring_str.Get() << std::endl;

  /* Output the substring
  auto sub_cord_str = sub_cord->ToString();
  std::cout << sub_cord_str.Get() << std::endl;

  // auto res = SubString(c, 0, 3);
  // auto res_str = res->ToString();
  // std::cout << res_str.Get() << std::endl;*/
}