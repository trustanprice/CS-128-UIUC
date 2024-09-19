// clang-format off
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
//  Written By : Student Name                    Environment : ubuntu:bionic               //
//  Date ......: 2021/02/10                      Compiler ...: clang-10                    //
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
// clang-format on
/////////////////////////////////////////////////////////////////////////////////////////////
//                             Framework Set-up //
/////////////////////////////////////////////////////////////////////////////////////////////
#ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif
#include "catch.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////
//                                 Includes //
/////////////////////////////////////////////////////////////////////////////////////////////

#include <stdexcept>

#include "image_ppm.hpp"
#include "pixel.hpp"
#include "seam_carver.hpp"
/////////////////////////////////////////////////////////////////////////////////////////////
//                             Helpers/Constants //
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
//                                Test Cases //
/////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("True == True", "") { REQUIRE(true == true); }

TEST_CASE("Print Vertical Energies", "") {
  std::string image_path =
      "/workspaces/mp-4-seam-carving-trustangabrielprice/images/TGP_created "
      "copy.ppm";
  ImagePPM image(image_path);
  SeamCarver creation(image);
  for (int i = 0; i < creation.GetHeight(); ++i) {
    for (int j = 0; j < creation.GetWidth(); ++j) {
      std::cout << creation.GetEnergy(i, j) << " ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
  int* new_arr = creation.GetVerticalSeam();
  for (int i = 0; i < creation.GetHeight(); i++) {
    std::cout << new_arr[i] << std::endl;
  }
}

TEST_CASE("Print Horizontal Energies", "") {
  std::string image_path =
      "/workspaces/mp-4-seam-carving-trustangabrielprice/images/TGP_created "
      "copy.ppm";
  ImagePPM image(image_path);
  SeamCarver creation(image);
  for (int i = 0; i < creation.GetHeight(); ++i) {
    for (int j = 0; j < creation.GetWidth(); ++j) {
      std::cout << creation.GetEnergy(i, j) << " ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
  int* new_arr = creation.GetHorizontalSeam();
  for (int i = creation.GetWidth() - 1; i >= 0; i--) {
    std::cout << new_arr[i] << std::endl;
  }
}

TEST_CASE("Remove Horizontal Energies", "") {
  std::string image_path =
      "/workspaces/mp-4-seam-carving-trustangabrielprice/images/TGP_created "
      "copy.ppm";
  ImagePPM image(image_path);
  SeamCarver creation(image);

  // Store energies before seam removal
  std::vector<std::vector<double>> energies_before;
  for (int i = 0; i < creation.GetHeight(); ++i) {
    std::vector<double> row;
    for (int j = 0; j < creation.GetWidth(); ++j) {
      row.push_back(creation.GetEnergy(i, j));
    }
    energies_before.push_back(row);
  }

  // Print energies after seam removal
  std::cout << "Energies after removing horizontal seam:" << std::endl;
  int* removed_seam = creation.RemoveHorizontalSeam();
  for (int i = 0; i < creation.GetHeight(); ++i) {
    for (int j = 0; j < creation.GetWidth(); ++j) {
      // Skip the pixel that was removed by the seam removal
      if (removed_seam[j] == i) continue;
      std::cout << energies_before[i][j] << " ";
    }
    std::cout << std::endl;
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////