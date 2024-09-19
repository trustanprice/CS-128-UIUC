#include "seam_carver.hpp"

SeamCarver::SeamCarver(const ImagePPM& image): image_(image) {
  height_ = image.GetHeight();
  width_ = image.GetWidth();
}

void SeamCarver::SetImage(const ImagePPM& image) {
  image_ = image;
  width_ = image.GetWidth();
  height_ = image.GetHeight();
}
const ImagePPM& SeamCarver::GetImage() const { return image_; }

int SeamCarver::GetHeight() const { return height_; }

int SeamCarver::GetWidth() const { return width_; }

int SeamCarver::RedEnergy(int row, int col) const {
  int r_energy = 0;
  int red_right = 0, red_left = 0, red_top = 0, red_bottom = 0;
  if (col == 0) {
    red_left = GetImage().GetPixel(row, width_ - 1).GetRed();
  } else {
    red_left = GetImage().GetPixel(row, col - 1).GetRed();
  }

  if (col < width_ - 1) {
    red_right = GetImage().GetPixel(row, col + 1).GetRed();
  } else {
    red_right = GetImage().GetPixel(row, 0).GetRed();
  }

  if (row < height_ - 1) {
    red_bottom = GetImage().GetPixel(row + 1, col).GetRed();
  } else {
    red_bottom = GetImage().GetPixel(0, col).GetRed();
  }

  if (row == 0) {
    red_top = GetImage().GetPixel(height_ - 1, col).GetRed();
  } else {
    red_top = GetImage().GetPixel(row - 1, col).GetRed();
  }

  int r_diff_sides = red_left - red_right;
  int r_diff_tb = red_top - red_bottom;
  r_energy = r_diff_sides * r_diff_sides + r_diff_tb * r_diff_tb;
  return r_energy;
}

int SeamCarver::GreenEnergy(int row, int col) const {
  int g_energy = 0;
  int green_right = 0, green_left = 0, green_top = 0, green_bottom = 0;
  if (col == 0) {
    green_left = GetImage().GetPixel(row, width_ - 1).GetGreen();
  } else {
    green_left = GetImage().GetPixel(row, col - 1).GetGreen();
  }

  if (col < width_ - 1) {
    green_right = GetImage().GetPixel(row, col + 1).GetGreen();
  } else {
    green_right = GetImage().GetPixel(row, 0).GetGreen();
  }

  if (row < height_ - 1) {
    green_bottom = GetImage().GetPixel(row + 1, col).GetGreen();
  } else {
    green_bottom = GetImage().GetPixel(0, col).GetGreen();
  }

  if (row == 0) {
    green_top = GetImage().GetPixel(height_ - 1, col).GetGreen();
  } else {
    green_top = GetImage().GetPixel(row - 1, col).GetGreen();
  }

  int g_diff_sides = green_left - green_right;
  int g_diff_tb = green_top - green_bottom;
  g_energy = g_diff_sides * g_diff_sides + g_diff_tb * g_diff_tb;
  return g_energy;
}

int SeamCarver::BlueEnergy(int row, int col) const {
  int b_energy = 0;
  int blue_right = 0, blue_left = 0, blue_top = 0, blue_bottom = 0;
  if (col == 0) {
    blue_left = GetImage().GetPixel(row, width_ - 1).GetBlue();
  } else {
    blue_left = GetImage().GetPixel(row, col - 1).GetBlue();
  }

  if (col < width_ - 1) {
    blue_right = GetImage().GetPixel(row, col + 1).GetBlue();
  } else {
    blue_right = GetImage().GetPixel(row, 0).GetBlue();
  }

  if (row < height_ - 1) {
    blue_bottom = GetImage().GetPixel(row + 1, col).GetBlue();
  } else {
    blue_bottom = GetImage().GetPixel(0, col).GetBlue();
  }

  if (row == 0) {
    blue_top = GetImage().GetPixel(height_ - 1, col).GetBlue();
  } else {
    blue_top = GetImage().GetPixel(row - 1, col).GetBlue();
  }
  int b_diff_sides = blue_left - blue_right;
  int b_diff_tb = blue_top - blue_bottom;
  b_energy = b_diff_sides * b_diff_sides + b_diff_tb * b_diff_tb;
  return b_energy;
}

int SeamCarver::GetEnergy(int row, int col) const {
  int energy = 0;
  energy = RedEnergy(row, col) + GreenEnergy(row, col) + BlueEnergy(row, col);
  return energy;
}

int SeamCarver::BreakTieVertical(int** seam,
                                 int row,
                                 int col,
                                 int width) const {
  if (col == 0) {
    return std::min(seam[row + 1][col], seam[row + 1][col + 1]);
  }
  if (col == width - 1) {
    return std::min(seam[row + 1][col - 1], seam[row + 1][col]);
  }
  return Min(
      seam[row + 1][col - 1], seam[row + 1][col], seam[row + 1][col + 1]);
}

int SeamCarver::BreakTieHorizontal(int** seam,
                                   int row,
                                   int col,
                                   int height) const {
  if (row == 0) {
    return std::min(seam[row][col + 1], seam[row + 1][col + 1]);
  }
  if (row == height - 1) {
    return std::min(seam[row - 1][col + 1], seam[row][col + 1]);
  }
  return Min(
      seam[row - 1][col + 1], seam[row][col + 1], seam[row + 1][col + 1]);
}

int SeamCarver::FindSmallestVertAdjacent(int row, int col, int** seam) const {
  if (col == 0) {
    return std::min(seam[row + 1][col], seam[row + 1][col + 1]);
  }
  if (col == width_ - 1) {
    return std::min(seam[row + 1][col - 1], seam[row + 1][col]);
  }
  return Min(
      seam[row + 1][col - 1], seam[row + 1][col], seam[row + 1][col + 1]);
}

int SeamCarver::FindSmallestHorizAdjacent(int row, int col, int** seam) const {
  if (row == 0) {
    return std::min(seam[row][col + 1], seam[row + 1][col + 1]);
  }
  if (row == height_ - 1) {
    return std::min(seam[row][col + 1], seam[row - 1][col + 1]);
  }
  return Min(
      seam[row - 1][col + 1], seam[row][col + 1], seam[row + 1][col + 1]);
}

int** SeamCarver::VerticalSeamEnergy(int row, int col) const {
  int** seam = new int*[height_];
  for (int i = 0; i < height_; ++i) {
    seam[i] = new int[width_];
  }
  // Initialize energies for the bottom row
  for (col = 0; col < width_; ++col) {
    seam[height_ - 1][col] = GetEnergy(height_ - 1, col);
  }
  // Calculate energies for each spot in the seam
  for (row = height_ - 2; row >= 0; --row) {
    for (col = 0; col < width_; ++col) {
      int smallest_adj = FindSmallestVertAdjacent(row, col, seam);
      seam[row][col] = GetEnergy(row, col) +
                       smallest_adj;  // Update the energy of the current spot
    }
  }
  for (int i = 0; i < height_; ++i) {
    for (int j = 0; j < width_; ++j) {
      std::cout << seam[i][j] << " ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
  return seam;
}

int* SeamCarver::GetVerticalSeam() const {
  int** seam = VerticalSeamEnergy(height_, width_);
  int* arr = new int[height_];
  int spot_energy = seam[0][0];
  arr[0] = 0;
  for (int col = 0; col < width_; ++col) {
    if (seam[0][col] < spot_energy) {
      arr[0] = col;
      spot_energy = seam[0][col];
    }
  }
  for (int row = 0; row < height_ - 1; ++row) {
    int curr_col = arr[row];
    BreakTieVertical(seam, row, curr_col, width_);
    if (curr_col == 0) {
      if (seam[row + 1][curr_col] <= seam[row + 1][curr_col + 1]) {
        arr[row + 1] = curr_col;
      } else {
        arr[row + 1] = curr_col + 1;
      }
    } else if (curr_col == width_ - 1) {
      arr[row + 1] = seam[row + 1][curr_col - 1] < seam[row + 1][curr_col]
                         ? curr_col - 1
                         : curr_col;
    } else {
      int prev = seam[row + 1][curr_col - 1], curr = seam[row + 1][curr_col],
          next = seam[row + 1][curr_col + 1];
      if (curr <= prev && curr <= next) {
        arr[row + 1] = curr_col;
      } else if (prev < curr && prev <= next) {
        arr[row + 1] = curr_col - 1;
      } else if (next < curr && next < prev) {
        arr[row + 1] = curr_col + 1;
      }
    }
  }
  DeleteSeam(seam, height_);
  return arr;
}

int* SeamCarver::GetHorizontalSeam() const {
  int** seam = HorizontalSeamEnergy(height_, width_);
  int* arr = new int[width_];
  int spot_energy = seam[0][0];
  arr[0] = 0;
  for (int row = 0; row < height_; ++row) {
    if (seam[row][0] < spot_energy) {
      arr[0] = row;
      spot_energy = seam[row][0];
    }
  }
  for (int col = 0; col < width_ - 1; ++col) {
    int curr_row = arr[col];
    BreakTieHorizontal(seam, curr_row, col, height_);
    if (curr_row == 0) {
      if (seam[curr_row][col + 1] <= seam[curr_row + 1][col + 1]) {
        arr[col + 1] = curr_row;
      } else {
        arr[col + 1] = curr_row + 1;
      }
    } else if (curr_row == height_ - 1) {
      if (seam[curr_row - 1][col + 1] < seam[curr_row][col + 1]) {
        arr[col + 1] = curr_row - 1;
      } else {
        arr[col + 1] = curr_row;
      }
    } else {
      int prev = seam[curr_row - 1][col + 1], curr = seam[curr_row][col + 1],
          next = seam[curr_row + 1][col + 1];
      if (curr <= prev && curr <= next) {
        arr[col + 1] = curr_row;
      } else if (prev < curr && prev <= next) {
        arr[col + 1] = curr_row - 1;
      } else if (next < curr && next < prev) {
        arr[col + 1] = curr_row + 1;
      }
    }
  }
  DeleteSeam(seam, height_);
  return arr;
}

int** SeamCarver::HorizontalSeamEnergy(int row, int col) const {
  int** seam = new int*[height_];
  for (int i = 0; i < height_; ++i) {
    seam[i] = new int[width_];
  }

  // Initialize energies for the rightmost column
  for (row = 0; row < height_; ++row) {
    seam[row][width_ - 1] = GetEnergy(row, width_ - 1);
  }

  // Calculate energies for each spot in the seam
  for (col = width_ - 2; col >= 0; --col) {
    for (row = 0; row < height_; ++row) {
      int smallest_adj = FindSmallestHorizAdjacent(row, col, seam);
      seam[row][col] = GetEnergy(row, col) + smallest_adj;
    }
  }

  return seam;
}

void SeamCarver::RemoveHorizontalSeam() {
  int* seam = GetHorizontalSeam();  // Calculate the horizontal seam

  image_.RemovingHorizontalSeam(seam);
  height_--;
  delete[] seam;  // Free the memory allocated for the seam
}

void SeamCarver::RemoveVerticalSeam() {
  int* seam = GetVerticalSeam();  // Calculate the vertical seam
  image_.RemovingVerticalSeam(seam);
  width_--;
  delete[] seam;  // Free the memory allocated for the seam
}

// implement the rest of SeamCarver's functions here

// given functions below, DO NOT MODIFY