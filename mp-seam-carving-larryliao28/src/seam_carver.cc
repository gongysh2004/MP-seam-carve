#include "seam_carver.hpp"

#include <iostream>
#include <string>
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
void SeamCarver::Print(std::string header, int ** values, int width, int height) const{
  char *debug = std::getenv("DEBUG");
  if(debug==nullptr) return;
  std::cout << header << std::endl;
  for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
           std::cout << values[row][col] << (col==width-1? "":"\t");
        }
        std::cout << std::endl;
    }
}
void SeamCarver::Print(std::string header, int * values, int width) const{
  char *debug = std::getenv("DEBUG");
  if(debug==nullptr) return;
  std::cout << header << std::endl;
  for (int col = 0; col < width; col++)
  {
    std::cout << values[col] <<  (col==width-1? "":"\t");
  }
  std::cout << std::endl;

}
void SeamCarver::PrintEnergy(std::string header) const{
  char *debug = std::getenv("DEBUG");
  if(debug==nullptr) return;
  int** energy_v = Allocate2DArray(width_, height_);
  for(int  row=0; row<width_; row++){
     for(int col=0; col<height_; col++){
       energy_v[row][col] = GetEnergy(row,col);
     }
  }
  Print(header, energy_v, width_, height_);
  this->Free2DArray(energy_v, height_);
  energy_v = nullptr;
}

int** SeamCarver::Allocate2DArray(int width,  int  height) const{
  int** values =
      new int*[height];
  for (int row = 0; row < height; ++row)
    values[row] =
        new int[width];
  return values;
}
void SeamCarver::Free2DArray(int** &values, int  height) const{
  for (int row = 0; row < height; ++row) delete[] values[row];
  delete[] values;
}

int* SeamCarver::GetVerticalSeam() const {
  int** vertical_values = Allocate2DArray(width_, height_);
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

  int * va = TraceVSeam(vertical_values);
  this->PrintEnergy("Energy Array");
  this->Print("vetical seam energy", vertical_values, width_, height_);
  this->Print("vetical trace  seam", va, width_);
  Free2DArray(vertical_values, height_);
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
    //middle, left, right
    if (values[row + 1][smallest] <=
            values[row + 1][smallest - 1] &&
        values[row + 1][smallest] <=
            values[row + 1][smallest + 1]) {
      vertical_array[row + 1] = smallest;
    // left
      }else if (values[row + 1][smallest - 1] <
                   values[row + 1][smallest] &&
               values[row + 1][smallest - 1] <=
                   values[row + 1][smallest + 1]) {
      smallest = smallest - 1;
    } else { //right
      smallest = smallest + 1;
    }
    vertical_array[row + 1] = smallest;
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
  int** horizontal_values = Allocate2DArray(width_, height_);
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
  int * hs = TraceHSeam(horizontal_values);
  this->PrintEnergy("Energy Array");
  this->Print("horizontal seam energy", horizontal_values, width_, height_);
  this->Print("horizontal trace  seam", hs, width_);
  this->Free2DArray(horizontal_values, height_);
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
    //middle, top, bottom
    if (values[smallest_r][col + 1] <=
            values[smallest_r - 1][col + 1] &&
        values[smallest_r][col + 1] <=
            values[smallest_r + 1][col + 1]) {
      horizontal_array[col + 1] = smallest_r;
    } else if (values[smallest_r - 1][col + 1] <
                   values[smallest_r][col + 1] &&
               values[smallest_r - 1][col + 1] <=
                   values[smallest_r + 1][col + 1]) {
      smallest_r = smallest_r - 1;
    } else {
      smallest_r = smallest_r + 1;
    }
    horizontal_array[col + 1] = smallest_r;
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