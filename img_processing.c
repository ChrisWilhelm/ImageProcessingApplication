//Chris Wilhelm cwilhel8
//Udochukwu Nwosu unwosu6
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include "img_processing.h"
#include "ppm_io.h"
Image* newImage(Image* original) {
	Image* newImg = malloc(sizeof(Image));
	int rows = original->rows;
	int cols = original->cols;
	int size = rows * cols;
	newImg->rows = rows;
	newImg->cols = cols;
	newImg->data = malloc(sizeof(Pixel) * (size));
	return newImg;
}

Pixel gradCalc(Image* img, int index) { //since r,g, and b for a given pixel are all the same, since it it grayscale, we will only reference the r value.
  int cols = img->cols; 
  int x = ((img->data[index + 1].r - img->data[index-1].r))/2; //calculates the change in the horizontal direction for a pixel 
  int y = ((img->data[index + cols].r - img->data[index-cols].r))/2; //calculates the change in the vertical direction for a pixel
  unsigned char pix = abs(x) + abs(y); //sums the absolute values of x and y to get the gradient at that pixel
  Pixel grad;
  grad.r = pix; //sets each pixel equal to the sum
  grad.b = pix;
  grad.g = pix;
  return grad; //returns that pixel
}

Image* grayscale(Image* img) {
  int rows = img->rows;
  int cols = img->cols;
  int size = rows * cols;
  for(int i = 0; i < size; i++) { //loops throug each pixel in the array
    unsigned char r = img->data[i].r;//multiplies the r by .3
    unsigned char g = img->data[i].g;//the g by .59
    unsigned char b = img->data[i].b;//the b by .11
    img->data[i].r = r * .3 + g * .59 + b * .11;//then sums and sets r, g, and b equal to the sum
    img->data[i].g = r * .3 + g * .59 + b * .11;
    img->data[i].b = r * .3 + g * .59 + b * .11;
  }
  return img; //returns image of grayscale
}  

Image* binarize(Image* img, int threshold) {
  int size = (img->rows)*(img->cols);
  for(int i = 0; i < size; i++) { //loops through all pixels in image
    unsigned char r = img->data[i].r;
    unsigned char g = img->data[i].g;
    unsigned char b = img->data[i].b;
    unsigned char val = .3 * r + .59 * g + .11 * b; //computes the grayscale for a pixel
    if(val < threshold) { //if value is less than threshold sets to black
      img->data[i].r = 0;
      img->data[i].g = 0;
      img->data[i].b = 0;
    }
    else { //if not less than threshold makes white
      img->data[i].r = 255;
      img->data[i].g = 255;
      img->data[i].b = 255;
    }
  }
  return img; //returns binarized image
}

Image* crop(Image* img, int ulr, int ulc, int lrr, int lrc) {
        int oldWidth = img->cols; //width of the input image
	int numRows = lrr - ulr;
	int numCols = lrc - ulc;
	int size = numRows * numCols;
	//update rows and cols for the crop  and allocate memory for the pixels of the crop
	img->rows = numRows;
	img->cols = numCols;
	Pixel* cropImg  = malloc(sizeof(Pixel) * size);
	int newWidth = img->cols;
		for (int row = 0; row < numRows; row++) {
			for (int col = 0; col < numCols; col++) {
			        cropImg[newWidth * row + col] = img->data[oldWidth * (row + ulr) + (col + ulc)];
			}
		}
		// free memory allocated for pixels of original image and make the pointer point to the cropped pixels
		free(img->data);
		img->data = cropImg;
	return img;
}

Image* transpose(Image* img) {
  //allocate space for an array of Pixels and copy the image pixels into it
  Pixel* img_Copy = malloc(sizeof(Pixel)*(img->rows)*(img->cols));
  int oldWidth = img->cols;
  int newWidth = img->rows;
  for(int i = 0; i < oldWidth*newWidth; i++) {
    img_Copy[i] = img->data[i];
  }
  //switch cols and rows
  img->rows = oldWidth;
  img->cols = newWidth;
  for (int row = 0; row < img->rows; row++) {
    for (int col = 0; col < img->cols; col++) {
      img->data[newWidth * row + col] = img_Copy[oldWidth * col + row];
    }
  }
  free(img_Copy);
  return img;
}

Image* gradient(Image* img) {
  int rows = img->rows;
  int cols = img->cols;
  Image* grayImg = malloc(sizeof(Image)*rows*cols);//allocates for the grayscale image
  grayImg->rows = rows;
  grayImg->cols = cols;
  grayImg->data = malloc(sizeof(Image)*rows*cols);
  for(int i = 0; i < rows*cols; i++) { //initializes grayImg->data
    grayImg->data[i] = img->data[i];
  }
  grayImg = grayscale(grayImg);//makes grayscale
  for(int i = 0; i < rows*cols; i++) { //loops throug all pixels
    if(i > (cols - 1) && i % cols != 0 && (i + 1) % cols  != 0 && i < (rows * cols - cols)) { //checks if it is a border pixel
      Pixel grad = gradCalc(grayImg, i); //not a border pixel gets the gradient calculated in gradCalc
      img->data[i] = grad;
    }
    else {//if it is a border pixel, it is set to black
      unsigned char n = 0;
      img->data[i].r = n;
      img->data[i].g = n;
      img->data[i].b = n;
    }
  }
  free(grayImg->data);
  free(grayImg);
  return img;//returns gradient of the original image
}


int min(int index, Image* img) {
  int cols = img->cols;
  unsigned char ltVal;
  unsigned char midVal;
  unsigned char rtVal;

  if (index % cols == 0) { //if its a left most edge
	rtVal = img->data[index + cols + 1].r;
	return index + cols + 1;
  } else if((index - 1) % cols == 0) { //if this is the second column ie. index 1, don't check for left neighbor
    midVal = img->data[index + cols].r;
    rtVal = img->data[index + cols + 1].r;
    if(midVal <= rtVal) {
      return index + cols;
    }
    else {
      return index + cols + 1;
    }
  } else if ((index + 1) % cols == 0) { //checks for right most edge case
         ltVal = img->data[index + cols - 1].r;
	 return index + cols - 1;
  } else if ((index + 2) % cols == 0) { //second to last col., ignore right neighbor
    ltVal = img->data[index + cols - 1].r;
    midVal = img->data[index + cols].r;
    if(midVal <= ltVal) {
      return index + cols;
    }
    else {
      return index + cols - 1;
    }
  } else {
  	ltVal = img->data[index + cols - 1].r;
	midVal = img->data[index + cols].r;
	rtVal = img->data[index + cols + 1].r;
  }

  //find the minimum row neighbor for pixels with 3 valid row neighbors
  if(midVal <= ltVal && midVal <= rtVal) {
    return index + cols;
  }
  else if(ltVal <= rtVal) {
    return index + cols - 1;
  }
  else {
    return index + cols + 1;
  }
}


Image* seam(Image* img, double col, double row) {
  int new_Col = col * (img->cols);//calculates number of Columns in seamed image
  int rem_Col = img->cols - new_Col;//calculates number of columns to be removed
  int new_Row = row * (img->rows);//calculates number of rows in seamed image
  int rem_Row = img->rows - new_Row;//calculates number of rows to be removed
  
  img = vertSeam(rem_Col, img);//removes rem_Col number of vertical seams
  img = transpose(img);
  img = vertSeam(rem_Row, img);//removes rem_Row number of vertical seams(it has been tranposed so now columns)
  img = transpose(img);

  return img;
}

Image* vertSeam(int rem_Col, Image* img) {
  for(int newC = 0; newC < rem_Col; newC++) {
    Image* grad_Img = newImage(img);
    
    // create gradient here
    for(int i = 0; i < (img->rows)*(img->cols); i++) {
      grad_Img->data[i] = img->data[i];
    }
    
    grad_Img = gradient(grad_Img);
    int cols = grad_Img->cols;
    int rows = grad_Img->rows;
    //create array to store seams in and variable to store seam sums
    int seam_Arr[cols][rows];  
    unsigned long rem_Seam = ULONG_MAX;
    int rem_Index = -1;
  
    for(int c = 0; c < cols; c++) {
      //begin each verticle seam at the 0 row and the cth column
      int index = c;
      unsigned long seam_Sum = 0;
      
      for(int r = 0; r < rows; r++) {
	seam_Arr[c][r] = index;
	seam_Sum += grad_Img->data[index].r;
	if (r < rows - 2) { 
	  index = min(index, grad_Img);
	  }
	if (r == rows - 2) {
	  index = index + cols;
	} 
      }

      if(seam_Sum < rem_Seam) {
	rem_Seam = seam_Sum;
	rem_Index = c;
      }

    }

    //deallocate the memory for the current image
    free(grad_Img->data);
    free(grad_Img);

    //make a new array that holds all the pixels of the pervious image
    Pixel* pixs = malloc(sizeof(Pixel)*rows*cols);
    for(int i = 0; i < rows*cols; i++) {
      pixs[i] = img->data[i];
    }

    //resize the image
    int origSize = rows*cols;
    int size = rows * (cols - 1);
    img->cols = cols - 1;
    free(img->data);
    img->data = malloc(sizeof(Pixel)*(size));
      
    // remove the seam index by index. If the current index i is not in
    // the seam array then copy the pixel into the smaller image but
    // if the index is in the seam array do not copy and move to the 
    // next bad index in the seam array
    int r = 0;
    int new_Index = 0;
    for(int i = 0; i < origSize; i++) {
      if(i != seam_Arr[rem_Index][r] && new_Index < size) {
	img->data[new_Index] = pixs[i];
	new_Index++;
      }
      else {
	r++;
      }
    }

    free(pixs);
  }

  return img;
} 
    
					     
    
  
      
