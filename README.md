# SeamCarving Using Dynamic Programming Algorithm Paradigm.

In broad terms the seam carving algorithm uses horizontal or vertical seams to iteratively reduce
the size of the image. A seam is a pixel thick cut through the image that stretches either from left to right or from
up to down (depending on the orientation of the seam). Removing the seam reduces the image by one unit. For
example, a horizontal seam will reduce the size of the image by one row, and a vertical seam by one column. 

The main idea is to find the seams that cut through the non-salient content of the image. 
A good seam basically cuts through pixels that are similar with each other. In other words,
a good seam is a seam that has small variations in color along the seam. Finding such optimal seams can be done
using a dynamic programming algorithm.
