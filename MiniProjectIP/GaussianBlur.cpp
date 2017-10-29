#include <iostream>
#include <vector>
#include <assert.h>
#include <cmath>

using namespace std;

typedef vector<double> Array;
typedef vector<Array> Matrix;
typedef vector<Matrix> Image;

Matrix getGaussian(int height, int width, double sigma)
{
	Matrix kernel(height, Array(width));
	double sum = 0.0;
	int i, j;

	for (i = 0; i<height; i++) {
		for (j = 0; j<width; j++) {
			kernel[i][j] = exp(-(i*i + j*j) / (2 * sigma*sigma)) / (2 * M_PI*sigma*sigma);
			sum += kernel[i][j];
		}
	}

	for (i = 0; i<height; i++) {
		for (j = 0; j<width; j++) {
			kernel[i][j] /= sum;
		}
	}

	return kernel;
}

Image loadImage(const char *filename)
{
	png::image<png::rgb_pixel> image(filename);
	Image imageMatrix(3, Matrix(image.get_height(), Array(image.get_width())));

	int h, w;
	for (h = 0; h<image.get_height(); h++) {
		for (w = 0; w<image.get_width(); w++) {
			imageMatrix[0][h][w] = image[h][w].red;
			imageMatrix[1][h][w] = image[h][w].green;
			imageMatrix[2][h][w] = image[h][w].blue;
		}
	}

	return imageMatrix;
}


Image applyFilter(Image &image, Matrix &filter){
	assert(image.size() == 3 && filter.size() != 0);

	int height = image[0].size();
	int width = image[0][0].size();
	int filterHeight = filter.size();
	int filterWidth = filter[0].size();
	int newImageHeight = height - filterHeight + 1;
	int newImageWidth = width - filterWidth + 1;
	int d, i, j, h, w;

	Image newImage(3, Matrix(newImageHeight, Array(newImageWidth)));

	for (d = 0; d<3; d++) {
		for (i = 0; i<newImageHeight; i++) {
			for (j = 0; j<newImageWidth; j++) {
				for (h = i; h<i + filterHeight; h++) {
					for (w = j; w<j + filterWidth; w++) {
						newImage[d][i][j] += filter[h - i][w - j] * image[d][h][w];
					}
				}
			}
		}
	}

	return newImage;
}

Image applyFilter(Image &image, Matrix &filter, int times)
{
	Image newImage = image;
	for (int i = 0; i<times; i++) {
		newImage = applyFilter(newImage, filter);
	}
	return newImage;
}