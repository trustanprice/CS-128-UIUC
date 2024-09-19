#include <fstream>
#include <iostream>
#include <string>

#include "image_ppm.hpp"
#include "seam_carver.hpp"

int main() {
  std::string image_path =
      "/workspaces/mp-4-seam-carving-trustangabrielprice/images/TGP_created "
      "copy.ppm";
  ImagePPM image(image_path);
  SeamCarver creation(image);

  return 0;
}
