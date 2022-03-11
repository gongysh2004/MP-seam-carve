// #include "seam_carver.hpp"

// #include <iostream>

// #include "image_ppm.hpp"

// // implement the rest of SeamCarver's functions here

// // given functions below, DO NOT MODIFY

// SeamCarver::SeamCarver(const ImagePPM& image): image_(image) {
//   height_ = image.GetHeight();
//   width_ = image.GetWidth();
// }

// void SeamCarver::SetImage(const ImagePPM& image) {
//   image_ = image;
//   width_ = image.GetWidth();
//   height_ = image.GetHeight();
// }

// const ImagePPM& SeamCarver::GetImage() const { return image_; }

// int SeamCarver::GetHeight() const { return height_; }

// int SeamCarver::GetWidth() const { return width_; }

// // need to fix
// int SeamCarver::GetEnergy(int row, int col) const {
//   // if (row >= height_ || row < 0) {
//   //   throw std::out_of_range("row out of bounds");
//   // }
//   // if (col >= width_ || col < 0) {
//   //   throw std::out_of_range("column out of bounds");
//   // }
//   Pixel top_pixel = image_.GetPixel((row + 1) % height_, col);
//   Pixel bottom_pixel = image_.GetPixel(row <= 0 ? height_ - 1 : row - 1,
//   col); Pixel right_pixel = image_.GetPixel(row, (col + 1) % width_); Pixel
//   left_pixel = image_.GetPixel(row, col <= 0 ? width_ - 1 : col - 1); int
//   vertical_pixel = (top_pixel.GetRed() - bottom_pixel.GetRed()) *
//                            (top_pixel.GetRed() - bottom_pixel.GetRed()) +
//                        (top_pixel.GetGreen() - bottom_pixel.GetGreen()) *
//                            (top_pixel.GetGreen() - bottom_pixel.GetGreen()) +
//                        (top_pixel.GetBlue() - bottom_pixel.GetBlue()) *
//                            (top_pixel.GetBlue() - bottom_pixel.GetBlue());
//   int horizontal_pixel = (left_pixel.GetRed() - right_pixel.GetRed()) *
//                              (left_pixel.GetRed() - right_pixel.GetRed()) +
//                          (left_pixel.GetGreen() - right_pixel.GetGreen()) *
//                              (left_pixel.GetGreen() - right_pixel.GetGreen())
//                              +
//                          (left_pixel.GetBlue() - right_pixel.GetBlue()) *
//                              (left_pixel.GetBlue() - right_pixel.GetBlue());
//   return vertical_pixel + horizontal_pixel;
// }

// int* SeamCarver::GetVerticalSeam() const {
//   // findMinPathDynamicProgramming():
//   //   Values ← 2D array with height rows and width columns
//   int** vertical_values = new int*[height_];
//   for (int row = 0; row < height_; ++row)
//     vertical_values[row] = new int[width_];

//   //   For each cell in the last row
//   //       Values[cell_row][cell_col] ← E(cell_row, cell_col)
//   for (int col = 0; col < width_; ++col) {
//     vertical_values[height_ - 1][col] = GetEnergy(height_ - 1, col);
//   }

//   //   For each row starting from the second to last
//   //       For each cell in the row
//   //           Best ← min(Values[/*down-right*/], Values[/*down*/],
//   //           Values[/*down-left*/]) Values[cell_row][cell_col] ← Best +
//   //           E(cell_row, cell_col)

//   int best = 0;
//   for (size_t row = height_ - 2; row >= 0; --row) {
//     for (int col = 0; col < width_; ++col) {
//       if (col == 0) {
//         best =
//             vertical_values[row + 1][col] <= vertical_values[row + 1][col +
//             1]
//                 ? vertical_values[row + 1][col]
//                 : vertical_values[row + 1][col + 1];
//         vertical_values[row][col] = best + GetEnergy(row, col);
//         continue;
//       }

//       if (col == width_ - 1) {
//         best =
//             vertical_values[row + 1][col] <= vertical_values[row + 1][col -
//             1]
//                 ? vertical_values[row + 1][col]
//                 : vertical_values[row + 1][col - 1];
//         vertical_values[row][col] = best + GetEnergy(row, col);
//         continue;
//       }

//       if (vertical_values[row + 1][col] <= vertical_values[row + 1][col - 1]
//       &&
//           vertical_values[row + 1][col] <= vertical_values[row + 1][col + 1])
//           {
//         best = vertical_values[row + 1][col];
//       } else if (vertical_values[row + 1][col - 1] <
//                      vertical_values[row + 1][col] &&
//                  vertical_values[row + 1][col - 1] <=
//                      vertical_values[row + 1][col + 1]) {
//         best = vertical_values[row + 1][col - 1];
//       } else {
//         best = vertical_values[row + 1][col + 1];
//       }

//       vertical_values[row][col] = best + GetEnergy(row, col);
//     }
//   }

//   return TraceVSeam(vertical_values);

//   //   Return minimum cell of Values[0]
// }

// int* SeamCarver::TraceVSeam(int**& values) const {
//   int* vertical_array = new int[height_];
//   int smallest_col_index = 0;
//   for (int col = 1; col < width_; ++col) {
//     if (values[0][col] < values[0][smallest_col_index]) {
//       smallest_col_index = col;
//     }
//   }
//   vertical_array[0] = smallest_col_index;

//   for (int row = 0; row < height_ - 1; ++row) {
//     if (smallest_col_index == 0) {
//       smallest_col_index = values[row + 1][smallest_col_index] <=
//                                    values[row + 1][smallest_col_index + 1]
//                                ? smallest_col_index
//                                : smallest_col_index + 1;
//       vertical_array[row + 1] = smallest_col_index;
//       continue;
//     }
//     if (smallest_col_index == width_ - 1) {
//       smallest_col_index = values[row + 1][smallest_col_index] <=
//                                    values[row + 1][smallest_col_index - 1]
//                                ? smallest_col_index
//                                : smallest_col_index - 1;
//       vertical_array[row + 1] = smallest_col_index;
//       continue;
//     }

//     if (values[row + 1][smallest_col_index] <=
//             values[row + 1][smallest_col_index - 1] &&
//         values[row + 1][smallest_col_index] <=
//             values[row + 1][smallest_col_index + 1]) {
//       vertical_array[row + 1] = smallest_col_index;
//     } else if (values[row + 1][smallest_col_index - 1] <
//                    values[row + 1][smallest_col_index] &&
//                values[row + 1][smallest_col_index - 1] <=
//                    values[row + 1][smallest_col_index + 1]) {
//       vertical_array[row + 1] = smallest_col_index - 1;
//     } else {
//       vertical_array[row + 1] = smallest_col_index + 1;
//     }
//   }

//   for (int row = 0; row < height_; ++row) delete[] values[row];
//   delete[] values;
//   values = nullptr;

//   return vertical_array;
// }

// void SeamCarver::RemoveVerticalSeam() {
//   int* to_delete_v = GetVerticalSeam();
//   image_.CarveVerticalSeam(to_delete_v);
//   // need to work on releasing the memories, a reference pointer might help

//   delete[] to_delete_v;
//   to_delete_v = nullptr;
//   width_--;
// }

// int* SeamCarver::GetHorizontalSeam() const {
//   // initialize horizontal board of energy
//   int** horizontal_values = new int*[height_];
//   for (int row = 0; row < height_; ++row)
//     horizontal_values[row] = new int[width_];

//   // last row energy
//   for (int row = 0; row < height_; ++row) {
//     horizontal_values[row][width_ - 1] = GetEnergy(row, width_ - 1);
//   }
//   // calculate horizontal seam energy
//   int best = 0;
//   for (int col = width_ - 2; col >= 0; --col) {
//     for (int row = 0; row < height_; ++row) {
//       if (row == 0) {  // need to fix it
//         best = horizontal_values[row][col + 1] <=
//                        horizontal_values[row + 1][col + 1]
//                    ? horizontal_values[row][col + 1]
//                    : horizontal_values[row + 1][col + 1];
//         horizontal_values[row][col] = best + GetEnergy(row, col);
//         continue;
//       }
//       if (row == height_ - 1) {  // need to fix it
//         best = horizontal_values[row][col + 1] <=
//                        horizontal_values[row - 1][col + 1]
//                    ? horizontal_values[row][col + 1]
//                    : horizontal_values[row - 1][col + 1];
//         horizontal_values[row][col] = best + GetEnergy(row, col);
//         continue;
//       }
//       if (horizontal_values[row][col + 1] <=
//               horizontal_values[row + 1][col + 1] &&
//           horizontal_values[row][col + 1] <=
//               horizontal_values[row - 1][col + 1]) {
//         best = horizontal_values[row][col + 1];
//       } else if (horizontal_values[row - 1][col + 1] <
//                      horizontal_values[row][col + 1] &&
//                  horizontal_values[row - 1][col + 1] <=
//                      horizontal_values[row + 1][col + 1]) {
//         best = horizontal_values[row - 1][col + 1];
//       } else {
//         best = horizontal_values[row + 1][col + 1];
//       }
//       horizontal_values[row][col] = best + GetEnergy(row, col);
//     }
//   }
//   return TraceHSeam(horizontal_values);
// }

// // trace horizontal seam
// int* SeamCarver::TraceHSeam(int**& values) const {
//   int* horizontal_array = new int[width_];
//   int smallest_row_index = 0;
//   // smallest at column 0
//   for (int row = 1; row < height_; ++row) {
//     if (values[row][0] < values[smallest_row_index][0]) {
//       smallest_row_index = row;
//     }
//   }
//   horizontal_array[0] = smallest_row_index;

//   for (int col = 0; col < width_ - 1; ++col) {
//     if (smallest_row_index == 0) {
//       smallest_row_index = values[smallest_row_index][col + 1] <=
//                                    values[smallest_row_index + 1][col + 1]
//                                ? smallest_row_index
//                                : smallest_row_index + 1;
//       horizontal_array[col + 1] = smallest_row_index;
//       continue;
//     }
//     if (smallest_row_index == height_ - 1) {
//       smallest_row_index = values[smallest_row_index][col + 1] <=
//                                    values[smallest_row_index - 1][col + 1]
//                                ? smallest_row_index
//                                : smallest_row_index - 1;
//       horizontal_array[col + 1] = smallest_row_index;
//       continue;
//     }
//     if (values[smallest_row_index][col + 1] <=
//             values[smallest_row_index - 1][col + 1] &&
//         values[smallest_row_index][col + 1] <=
//             values[smallest_row_index + 1][col + 1]) {
//       horizontal_array[col + 1] = smallest_row_index;
//     } else if (values[smallest_row_index - 1][col + 1] <
//                    values[smallest_row_index][col + 1] &&
//                values[smallest_row_index - 1][col + 1] <=
//                    values[smallest_row_index + 1][col + 1]) {
//       horizontal_array[col + 1] = smallest_row_index - 1;
//     } else {
//       horizontal_array[col + 1] = smallest_row_index + 1;
//     }
//   }
//   for (int row = 0; row < height_; ++row) delete[] values[row];

//   delete[] values;
//   values = nullptr;
//   return horizontal_array;
// }

// void SeamCarver::RemoveHorizontalSeam() {
//   int* to_delete_h = GetHorizontalSeam();
//   image_.CarveHorizontalSeam(to_delete_h);

//   delete[] to_delete_h;
//   to_delete_h = nullptr;
//   height_--;
//   // there might be dangling pointers
// }

// // junk

#include "seam_carver.hpp"

#include <iostream>

#include "image_ppm.hpp"

// implement the rest of SeamCarver's functions here

// given functions below, DO NOT MODIFY

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

// need to fix
int SeamCarver::GetEnergy(int row, int col) const {
  // if (row >= height_ || row < 0) {
  //   throw std::out_of_range("row out of bounds");
  // }
  // if (col >= width_ || col < 0) {
  //   throw std::out_of_range("column out of bounds");
  // }
  Pixel top_pixel = image_.GetPixel(row == 0 ? height_ - 1 : row - 1, col);
  Pixel bottom_pixel = image_.GetPixel((row + 1) % height_, col);
  Pixel right_pixel = image_.GetPixel(row, (col + 1) % width_);
  Pixel left_pixel = image_.GetPixel(row, col <= 0 ? width_ - 1 : col - 1);
  int vertical_pixel = (top_pixel.GetRed() - bottom_pixel.GetRed()) *
                           (top_pixel.GetRed() - bottom_pixel.GetRed()) +
                       (top_pixel.GetGreen() - bottom_pixel.GetGreen()) *
                           (top_pixel.GetGreen() - bottom_pixel.GetGreen()) +
                       (top_pixel.GetBlue() - bottom_pixel.GetBlue()) *
                           (top_pixel.GetBlue() - bottom_pixel.GetBlue());
  int horizontal_pixel = (left_pixel.GetRed() - right_pixel.GetRed()) *
                             (left_pixel.GetRed() - right_pixel.GetRed()) +
                         (left_pixel.GetGreen() - right_pixel.GetGreen()) *
                             (left_pixel.GetGreen() - right_pixel.GetGreen()) +
                         (left_pixel.GetBlue() - right_pixel.GetBlue()) *
                             (left_pixel.GetBlue() - right_pixel.GetBlue());
  return vertical_pixel + horizontal_pixel;
}
int SeamCarver::Min(int a, int b) const { return a <= b ? a : b; }
int SeamCarver::Min(int a, int b, int c) const {
  int min = this->Min(a, b);
  return this->Min(min, c);
}
int* SeamCarver::GetVerticalSeam() const {
  int** vertical_values =
      new int*[height_];  //(int**)malloc(sizeof(int*) * height_);
  for (int row = 0; row < height_; ++row)
    vertical_values[row] =
        new int[width_];  //(int*)malloc(sizeof(int) * width_);
  for (int col = 0; col < width_; ++col) {
    vertical_values[height_ - 1][col] = GetEnergy(height_ - 1, col);
  }
  int best = 0;
  for (int row = height_ - 2; row >= 0; --row) {
    for (int col = 0; col < width_; ++col) {
      if (col == 0) {
        best = this->Min(vertical_values[row + 1][col],
                         vertical_values[row + 1][col + 1]);
        vertical_values[row][col] = best + GetEnergy(row, col);
        continue;
      }
      if (col == width_ - 1) {
        best = this->Min(vertical_values[row + 1][col],
                         vertical_values[row + 1][col - 1]);
        vertical_values[row][col] = best + GetEnergy(row, col);
        continue;
      }
      best = this->Min(vertical_values[row + 1][col + 1],
                       vertical_values[row + 1][col],
                       vertical_values[row + 1][col - 1]);

      vertical_values[row][col] = best + GetEnergy(row, col);
    }
  }

  int* va = TraceVSeam(vertical_values);
  for (int row = 0; row < height_; ++row) delete[] vertical_values[row];
  delete[] vertical_values;
  vertical_values = nullptr;
  return va;
  //   Return minimum cell of Values[0]
}

int* SeamCarver::TraceVSeam(int** values) const {
  int* vertical_array = new int[height_];
  int smallest = 0;
  for (int col = 1; col < width_; ++col) {
    if (values[0][col] < values[0][smallest]) {
      smallest = col;
    }
  }
  vertical_array[0] = smallest;
  for (int row = 0; row < height_ - 1; ++row) {
    if (smallest == 0) {
      smallest = values[row + 1][smallest] <= values[row + 1][smallest + 1]
                     ? smallest
                     : smallest + 1;
      vertical_array[row + 1] = smallest;
      continue;
    }
    if (smallest == width_ - 1) {
      smallest = values[row + 1][smallest] <= values[row + 1][smallest - 1]
                     ? smallest
                     : smallest - 1;
      vertical_array[row + 1] = smallest;
      continue;
    }
    if (values[row + 1][smallest] <= values[row + 1][smallest - 1] &&
        values[row + 1][smallest] <= values[row + 1][smallest + 1]) {
      vertical_array[row + 1] = smallest;
    } else if (values[row + 1][smallest - 1] < values[row + 1][smallest] &&
               values[row + 1][smallest - 1] <= values[row + 1][smallest + 1]) {
      vertical_array[row + 1] = smallest - 1;
    } else {
      vertical_array[row + 1] = smallest + 1;
    }
  }
  return vertical_array;
}

void SeamCarver::RemoveVerticalSeam() {
  int* to_delete_v = GetVerticalSeam();
  image_.CarveVerticalSeam(to_delete_v);

  delete[] to_delete_v;
  to_delete_v = nullptr;
  width_--;
}

int* SeamCarver::GetHorizontalSeam() const {
  int** horizontal_values = new int*[height_];
  for (int row = 0; row < height_; ++row)
    horizontal_values[row] = new int[width_];
  for (int row = 0; row < height_; ++row) {
    horizontal_values[row][width_ - 1] = GetEnergy(row, width_ - 1);
  }
  int best = 0;
  for (int col = width_ - 2; col >= 0; --col) {
    for (int row = 0; row < height_; ++row) {
      if (row == 0) {
        best = this->Min(horizontal_values[row][col + 1],
                         horizontal_values[row + 1][col + 1]);
        horizontal_values[row][col] = best + GetEnergy(row, col);
        continue;
      }
      if (row == height_ - 1) {
        best = this->Min(horizontal_values[row - 1][col + 1],
                         horizontal_values[row][col + 1]);
        horizontal_values[row][col] = best + GetEnergy(row, col);
        continue;
      }
      best = this->Min(horizontal_values[row + 1][col + 1],
                       horizontal_values[row][col + 1],
                       horizontal_values[row - 1][col + 1]);
      horizontal_values[row][col] = best + GetEnergy(row, col);
    }
  }
  int* hs = TraceHSeam(horizontal_values);
  for (int row = 0; row < height_; ++row) delete[] horizontal_values[row];
  delete[] horizontal_values;
  horizontal_values = nullptr;
  return hs;
}

// trace horizontal seam
int* SeamCarver::TraceHSeam(int** values) const {
  int* horizontal_array = new int[width_];
  int smallest_r = 0;
  for (int row = 1; row < height_; ++row) {
    if (values[row][0] < values[smallest_r][0]) {
      smallest_r = row;
    }
  }
  horizontal_array[0] = smallest_r;
  for (int col = 0; col < width_ - 1; ++col) {
    if (smallest_r == 0) {
      smallest_r =
          values[smallest_r][col + 1] <= values[smallest_r + 1][col + 1]
              ? smallest_r
              : smallest_r + 1;
      horizontal_array[col + 1] = smallest_r;
      continue;
    }
    if (smallest_r == height_ - 1) {
      smallest_r =
          values[smallest_r][col + 1] <= values[smallest_r - 1][col + 1]
              ? smallest_r
              : smallest_r - 1;
      horizontal_array[col + 1] = smallest_r;
      continue;
    }
    if (values[smallest_r][col + 1] <= values[smallest_r - 1][col + 1] &&
        values[smallest_r][col + 1] <= values[smallest_r + 1][col + 1]) {
      horizontal_array[col + 1] = smallest_r;
    } else if (values[smallest_r - 1][col + 1] < values[smallest_r][col + 1] &&
               values[smallest_r - 1][col + 1] <=
                   values[smallest_r + 1][col + 1]) {
      horizontal_array[col + 1] = smallest_r - 1;
    } else {
      horizontal_array[col + 1] = smallest_r + 1;
    }
  }
  return horizontal_array;
}

void SeamCarver::RemoveHorizontalSeam() {
  int* to_delete_h = GetHorizontalSeam();
  image_.CarveHorizontalSeam(to_delete_h);

  delete[] to_delete_h;
  to_delete_h = nullptr;
  height_--;
  // there might be dangling pointers
}

// junk