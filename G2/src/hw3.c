/*
 * hw3.c
 *
 *  Created on: Mar 10, 2013
 *      Author: lou
 */

#include "hw3.h"
#include <string.h>

#define NUM_DIMENSIONS 3

/**
 * Loads an image (stored as an IplImage struct) for each
 * filename provided.
 * @param numImages		The number of images to load
 * @param filenames		A list of strings of the filenames to load
 */
IplImage** loadImages(int numImages, char ** fileNames) {
	IplImage** rv; // the return result

    rv = (IplImage**) malloc(numImages * sizeof(IplImage*));

    if (rv == NULL)
    {
        fprintf(stderr, "Could not allocate memory for images\n");
        return NULL;
    }

    for (int i = 0; i < numImages; ++i)
    {
		rv[i] = cvLoadImage(fileNames[i], 1);

		if (rv[i] == NULL)
        {
            fprintf(stderr, "File %s could not be loaded\n", fileNames[i]);
            return NULL;
        }
    }

	return rv;
}

/**
 * Computes the distance between two colors (stored as CvScalars).
 *
 * Given a CvScalar c, you can access the blue, green, and red (BGR) elements
 * via c.val[0], c.val[1], and c.val[2], respectively.
 *
 * This function computes the distance between two colors as the euclidean
 * distance between the two BGR vectors.
 *
 * @see http://en.wikipedia.org/wiki/Euclidean_distance
 *
 * @param c1	The first color
 * @param c2	The second color
 * @returns		The euclidean distance between the two 3-d vectors
 */
double colorDistance(CvScalar c1, CvScalar c2) {
	double d = 0.0; // the result

    for (int i = 0; i < NUM_DIMENSIONS; ++i)
        d += (c1.val[i] - c2.val[i]) * (c1.val[i] - c2.val[i]);

	return d;
}

/**
 * Splits up an image into numColumns by numRows sub-images and returns the results.
 *
 * @param src	The source image to split
 * @param numColumns	The number of columns to split into
 * @param numRows 		The number of rows to split into
 * @returns				A numColumns x numRows array of IplImages
 */
IplImage ** getSubImages(IplImage* src, int numColumns, int numRows) {
	int cellWidth, cellHeight, y, x, i;
	IplImage ** rv;
	CvSize s = cvGetSize(src);

	// Compute the cell width and the cell height
	cellWidth = s.width / numColumns;
	cellHeight = s.height / numRows;

	// Allocate an array of IplImage* s
	rv = (IplImage**) malloc(sizeof(IplImage*) * numColumns * numRows);
	if (rv == NULL) {
		fprintf(stderr, "Could not allocate rv array\n");
	}

	// Iterate over the cells
	i = 0;
	for (y = 0; y < s.height; y += cellHeight)
		for (x = 0; x < s.width; x += cellWidth) {
			// Create a new image of size cellWidth x cellHeight and
			// store it in rv[i]. The depth and number of channels should
			// be the same as src.
			rv[i] = cvCreateImage(cvSize(cellWidth, cellHeight), src->depth,
					src->nChannels);
			if (rv[i] == NULL) {
				fprintf(stderr, "Could not allocate image %d\n", i);
			}

			// set the ROI of the src image
			cvSetImageROI(src, cvRect(x, y, cellWidth, cellHeight));

			// copy src to rv[i] using cvCopy, which obeys ROI
			cvCopy(src, rv[i], NULL);

			// reset the src image roi
			cvResetImageROI(src);

			// increment i
			i++;
		}

	// return the result
	return rv;
}

/**
 * Finds the CvScalar in colors closest to t using the colorDistance function.
 * @param t		 	The color to look for
 * @param scolors	The colors to look through
 * @param numColors	The length of scolors
 * @returns			The index of scolors that t is closest to
 * 					(i.e., colorDistance( t, scolors[result]) <=
 * 					colorDistance( t, scolors[i]) for all i != result)
 */
int findClosest(CvScalar t, CvScalar* scolors, int numColors) {
	int rv = 0;
	double dist;
    double min = sqrt(3 * 255 * 255); // Worst case distance

    for (int i = 0; i < numColors; ++i)
    {
        if ((dist = colorDistance(t, scolors[i])) < min)
        {
            min = dist;
            rv = i;
        }
    }

    return rv;
}

/**
 * For each image provided, computes the average color vector
 * (represented as a CvScalar object).
 *
 * @param images	The images
 * @param numImages	The length of images
 * @returns 		An numImages length array of CvScalars where rv[i] is the average color in images[i]
 */
CvScalar* getAvgColors(IplImage** images, int numImages) {
	CvScalar* rv = (CvScalar*) malloc(numImages * sizeof(CvScalar));

    if (rv == NULL)
    {
        fprintf(stderr, "Could not allocate memory for color averages\n");
        return NULL;
    }

    for (int i = 0; i < numImages; ++i)
    {
        rv[i] = cvAvg(images[i], NULL);
    }

    return rv;
}

/**
 * Given an ordered list of images (iclosest), creates a
 * numColumns x numRows grid in a new image, copies each image in, and returns the result.
 *
 * Thus, if numColumns is 10, numRows is 5, and each iclosest image is 64x64, the resulting image
 * would be 640x320 pixels.
 *
 * @param iclosest		A numColumns x numRows list of images in row-major order to be put into the resulting image.
 * @param numColumns  	Number of horizontal cells
 * @param numRows		Number of vertical cells
 */
IplImage* stitchImages(IplImage** iclosest, int numColumns, int numRows) {
    CvSize first_size = cvGetSize(iclosest[0]);
    int width = first_size.width;
    int height = first_size.height;

    IplImage* final = cvCreateImage(
        cvSize(width * numColumns, height * numRows),
        iclosest[0]->depth,
        iclosest[0]->nChannels);

    if (final == NULL)
    {
        fprintf(stderr, "Could not allocate memory for final image\n");
        return NULL;
    }

    printf("Final image dimension: %dx%d\n", final->width, final->height);

    int i = 0; // The current index in the array of sub-images

    for (int y = 0; y < height * numRows; y += height)
    {
        for (int x = 0; x < width * numColumns; x += width)
        {
            cvSetImageROI(final, cvRect(x, y, width, height));
            cvCopy(iclosest[i], final, NULL);
            cvResetImageROI(final);

            i++;
        }
    }

    return final;
}
