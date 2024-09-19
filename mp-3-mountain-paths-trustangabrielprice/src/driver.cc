#include <iostream>
#include <string>

#include "elevation_dataset.hpp"
#include "grayscale_image.hpp"
#include "path_image.hpp"

int main() {
  const std::string kTestInputElevation =
      "/workspaces/mp-3-mountain-paths-trustangabrielprice/example-data/"
      "ex_input_data/map-input-w51-h55.dat";
  constexpr size_t kTestInputFileWidth = 51;
  constexpr size_t kTestInputFileHeight = 55;

  const std::string kTestFileGrayscale =
      "/workspaces/mp-3-mountain-paths-trustangabrielprice/example-data/"
      "ex_output_grayscale/map-input-w51-h55.ppm";

  ElevationDataset image(
      kTestInputElevation, kTestInputFileWidth, kTestInputFileHeight);
  GrayscaleImage file(image);
  GrayscaleImage ifs(
      kTestInputElevation, kTestInputFileWidth, kTestInputFileHeight);
  PathImage path_image(ifs, image);
  PathImage make_image(file, image);

  std::cout << "Testing PathImage::MaxColorValue(): "
            << path_image.MaxColorValue() << std::endl;
  std::cout << "Testing PathImage::MaxColorValue(): "
            << make_image.MaxColorValue() << std::endl;

  return 0;
}
