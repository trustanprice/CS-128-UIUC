#include "path_image.hpp"

std::pair<int, int> PathImage::CurrentRow(
    std::vector<std::pair<int, int>> next_moves) {
  if (next_moves[1].first == next_moves[0].first &&
      next_moves[1].first < next_moves[2].first) {
    return next_moves[1];
  }
  if (next_moves[1].first == next_moves[2].first &&
      next_moves[1].first < next_moves[0].first) {
    return next_moves[1];
  }
  if (next_moves[1].first == next_moves[2].first &&
      next_moves[1].first == next_moves[0].first) {
    return next_moves[1];
  }
  if (next_moves[0].first == next_moves[2].first &&
      next_moves[0].first < next_moves[1].first) {
    return next_moves[2];
  }
  if (next_moves[1].first < next_moves[2].first &&
      next_moves[1].first < next_moves[0].first) {
    return next_moves[1];
  }
  if (next_moves[0].first < next_moves[1].first &&
      next_moves[0].first < next_moves[2].first) {
    return next_moves[0];
  }
  if (next_moves[2].first < next_moves[1].first &&
      next_moves[2].first < next_moves[0].first) {
    return next_moves[2];
  }
  return next_moves[0];
}

std::pair<int, int> PathImage::FirstRow(
    std::vector<std::pair<int, int>> next_moves) {
  // If there's a tie, prioritize the left path
  if (next_moves[0].first == next_moves[1].first) {
    return next_moves[0];
  }
  // If there's no tie, prioritize the left path if it has the least elevation
  // difference, otherwise prioritize the right path
  if (next_moves[0].first < next_moves[1].first) {
    return next_moves[0];
  }
  return next_moves[1];
}

std::pair<int, int> PathImage::LastRow(
    std::vector<std::pair<int, int>> next_moves) {
  // If there's a tie, prioritize the right path
  if (next_moves[0].first == next_moves[1].first) {
    return next_moves[1];
  }
  if (next_moves[0].first < next_moves[1].first) {
    return next_moves[0];
  }
  return next_moves[1];
}

int PathImage::MinEle() {
  // Use std::min_element to find the minimum element in paths_
  auto min_element = std::min_element(
      paths_.begin(), paths_.end(), [](const Path& p1, const Path& p2) {
        return p1.EleChange() < p2.EleChange();
      });
  // Return the index of the minimum element
  return static_cast<int>(std::distance(paths_.begin(), min_element));
}

void PathImage::GeneratePaths(const ElevationDataset& dataset) {
  for (int row = 0; row < static_cast<int>(dataset.Height()); ++row) {
    int temp_row = row;
    Path p = {dataset.Width(), static_cast<size_t>(temp_row)};

    for (int col = 0; col < static_cast<int>(dataset.Width()) - 1; ++col) {
      if (col == 0) {
        p.SetLoc(static_cast<size_t>(col), static_cast<size_t>(temp_row));
      }
      std::vector<std::pair<int, int>> next_move;

      if (temp_row == 0) {
        Uno(dataset, next_move, temp_row, col);
        std::pair<int, int> best_move = FirstRow(next_move);
        p.SetLoc(col + 1, static_cast<size_t>(best_move.second));
        p.IncEleChange(best_move.first);
        temp_row = best_move.second;
      } else if (temp_row == static_cast<int>(dataset.Height()) - 1) {
        Dos(dataset, next_move, temp_row, col);
        std::pair<int, int> best_move = LastRow(next_move);
        p.SetLoc(col + 1, static_cast<size_t>(best_move.second));
        p.IncEleChange(best_move.first);
        temp_row = best_move.second;
      } else {
        Tres(dataset, next_move, temp_row, col);
        std::pair<int, int> best_move = CurrentRow(next_move);
        p.SetLoc(col + 1, static_cast<size_t>(best_move.second));
        p.IncEleChange(best_move.first);
        temp_row = best_move.second;
      }
    }
    paths_.push_back(p);
  }
}
// Helper function to calculate paths for a given starting column
PathImage::PathImage(const GrayscaleImage& image,
                     const ElevationDataset& dataset) {
  GeneratePaths(dataset);  // Call GeneratePaths with the dataset parameter

  path_image_ = image.GetImage();
  int min_ele = MinEle();
  const int kRr = 252, kGr = 25, kBr = 63, kRg = 31, kGg = 253, kBg = 13;
  Color red = {kRr, kGr, kBr};
  Color green = {kRg, kGg, kBg};

  for (int row = 0; row < static_cast<int>(paths_.size()); ++row) {
    Path& p = paths_[row];
    for (int col = 0; col < static_cast<int>(p.Length()); ++col) {
      path_image_[p.GetPath()[col]][col] = red;
    }
  }
  for (int row = 0; row < static_cast<int>(paths_.size()); ++row) {
    Path& p = paths_[row];
    for (int col = 0; col < static_cast<int>(p.Length()); ++col) {
      if (min_ele == row) {
        path_image_[p.GetPath()[col]][col] = green;
      }
    }
  }

  width_ = dataset.Width();
  height_ = dataset.Height();
}

size_t PathImage::Width() const { return width_; }
size_t PathImage::Height() const { return height_; }
unsigned int PathImage::MaxColorValue() const { return kMaxColorValue; }

const std::vector<Path>& PathImage::Paths() const { return paths_; }
const std::vector<std::vector<Color>>& PathImage::GetPathImage() const {
  return path_image_;
}

void PathImage::ToPpm(const std::string& name) const {
  std::ofstream ppm_file(name);

  if (!ppm_file.is_open()) {
    throw std::runtime_error("Failed to open ppm file: " + name);
  }

  // Write PPM header
  ppm_file << "P3\n";
  ppm_file << width_ << " " << height_ << "\n";
  ppm_file << "255\n";  // Maximum color value (0 to 255)

  // Write image data
  for (size_t row = 0; row < height_; ++row) {
    for (size_t col = 0; col < width_; ++col) {
      const Color& color = path_image_[row][col];
      ppm_file << color.Red() << " " << color.Green() << " " << color.Blue()
               << "\n";
    }
  }

  ppm_file.close();
}
