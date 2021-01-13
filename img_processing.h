// Chris Wilhelm cwilhel8
// Udochukwu Nwosu unwosu6

#ifndef IMG_PROCESSING_H
#define IMG_PROCESSING_H
#include <stdio.h>
#include <math.h>
#include "ppm_io.h"

/** Function to create a new image that is the edited
 *  version of an input image.
 *  @param original the pointer to the original image
 *  @return the pointer to the image edit
 */
Image* newImage(Image* original);

/** Function to calculate the gradient for a given 
 *  pixel of an image using its boundary
 *  @param img the original image
 *  @param index the index of pixel to be calculated
 *  @return the magnitude of the grad
 */
Pixel gradCalc(Image* img, int index);

/** Function to implement grayscale edit to an image
 *  @param img the pointer to the image
 *  @return the pointer to the newly edited image
 */
Image* grayscale(Image* img);

/** Function to implement the binarize edit to an image
 *  Binarizing converts the input image to black and white 
 *  at a certain threshold
 *  @param img the pointer to the image
 *  @param threshold is the threshold for binarization
 *  @return the pointer to the newly edited image
 */
Image* binarize(Image* img, int threshold);

/** Function to implement a crop edit to an image
 *  @param img the pointer to the image
 *  @param ulr the upper left row number of the crop
 *  @param ulc the upper left column number of the crop
 *  @param lrr the lower right row number of the crop
 *  @param lrc the lower right column number of the crop
 *  @return the pointer to the newly edited image
 */
Image* crop(Image* img, int ulr, int ulc, int lrr, int lrc);

/** Function to transpose an image
 *  @param img the pointer to the image
 *  @return the pointer to the newly edited image
 */
Image* transpose(Image* img);

/** Function to produce the gradient of an image
 *  @param img the pointer to the image
 *  @return the pointer to the newly edited image
 */
Image* gradient(Image* img);

/** Function to find the next index in a verticle seam
 * @param index the current index in a verticle seam
 * @param img the gradient of the image being seam carved
 * @return next index in verticle seam
 */
int min(int index, Image* img);

/** Function to remove verticle seams of minimum gradient
 * energy in a given image
 * @param the number of seams to be removed
 * @param the pointer to the image
 * @return the pointer to the newly edited image
 */
Image* vertSeam(int rem_Col, Image* img);

/** Function to implement a simplified seam carving
 *  edit to an image
 *  @param img the pointer to the image
 *  @param col for percent of columns to be removed
 *  @param row for percent of rows to be removed
 *  @return the pointer to the newly edited image
 */
Image* seam(Image* img, double col, double row);

#endif
