#ifndef SEAM_CARVER_HPP
#define SEAM_CARVER_HPP

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "cassert"
#include "image_ppm.hpp"
#include "pixel.hpp"
class SeamCarver {
public:
  // default constructor
  //
  // Not tested, but you are welcome to intialize (or not) member variables
  // however you like in this function
  SeamCarver() = default;
  SeamCarver(const ImagePPM& image);
  void SetImage(const ImagePPM& image);
  const ImagePPM& GetImage() const;
  int GetHeight() const;
  int GetWidth() const;
  int GetEnergy(int row, int col) const;
  void DeleteSeam(int** seam, int height) const {
    for (int row = 0; row < height; ++row) {
      delete[] seam[row];
    }
    delete[] seam;
  }
  int BreakTieHorizontal(int** seam, int row, int col, int height) const;
  int BreakTieVertical(int** seam, int row, int col, int width) const;
  int Min(int a, int b, int c) const { return std::min(a, std::min(b, c)); }

  int RedEnergy(int row, int col) const;
  int GreenEnergy(int row, int col) const;
  int BlueEnergy(int row, int col) const;
  int* GetHorizontalSeam() const;
  int** VerticalSeamEnergy(int row, int col) const;
  int* GetVerticalSeam() const;
  int FindSmallestVertAdjacent(int row, int col, int** seam) const;
  int FindSmallestHorizAdjacent(int row, int col, int** seam) const;
  int** HorizontalSeamEnergy(int rows, int cols) const;
  void RemoveHorizontalSeam();
  void RemoveVerticalSeam();
  /**
   * Add any helper methods you may need
   */

private:
  ImagePPM image_;
  int height_ = 0;
  int width_ = 0;

  /**
   * Add any helper methods you may need
   */
};

#endif