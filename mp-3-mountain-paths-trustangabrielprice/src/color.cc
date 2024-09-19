#include "color.hpp"

#include <stdexcept>

Color::Color(int grayscale):
    red_(grayscale), green_(grayscale), blue_(grayscale) {
  if (red_ < 0 || red_ > kColorValueMax || green_ < 0 ||
      green_ > kColorValueMax || blue_ < 0 || blue_ > kColorValueMax) {
    throw std::out_of_range("out of range");
  }
}

Color::Color(int r, int g, int b): red_(r), green_(g), blue_(b) {
  if (r < 0 || r > kColorValueMax || g < 0 || g > kColorValueMax || b < 0 ||
      b > kColorValueMax) {
    throw std::out_of_range("out of range");
  }
}

int Color::Red() const { return red_; }
int Color::Green() const { return green_; }
int Color::Blue() const { return blue_; }

bool operator==(const Color& rhs, const Color& lhs) {
  return (rhs.Red() == lhs.Red() && rhs.Green() == lhs.Green() &&
          rhs.Blue() == lhs.Blue());
}