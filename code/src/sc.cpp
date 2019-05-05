
#include "sc.h"

using namespace cv;
using namespace std;

bool seam_carving(Mat& in_image, int new_width, int new_height, Mat& out_image) {

	// some sanity checks
	// Check 1 -> new_width <= in_image.cols
	if (new_width>in_image.cols) {
		cout << "Invalid request!!! new_width has to be smaller than the current size!" << endl;
		return false;
	}
	if (new_height>in_image.rows) {
		cout << "Invalid request!!! ne_height has to be smaller than the current size!" << endl;
		return false;
	}

	if (new_width <= 0) {
		cout << "Invalid request!!! new_width has to be positive!" << endl;
		return false;

	}

	if (new_height <= 0) {
		cout << "Invalid request!!! new_height has to be positive!" << endl;
		return false;

	}


	return seam_carving_trivial(in_image, new_width, new_height, out_image);
}


// seam carves by removing trivial seams
bool seam_carving_trivial(Mat& in_image, int new_width, int new_height, Mat& out_image) {

	Mat iimage = in_image.clone();
	Mat oimage = in_image.clone();
	while (iimage.rows != new_height || iimage.cols != new_width) {
		// horizontal seam if needed
		if (iimage.rows>new_height) {
			reduce_horizontal_seam_trivial(iimage, oimage);
			iimage = oimage.clone();
		}

		if (iimage.cols>new_width) {
			reduce_vertical_seam_trivial(iimage, oimage);
			iimage = oimage.clone();
		}
	}

	out_image = oimage.clone();
	return true;
}

bool reduce_horizontal_seam_trivial(Mat& in_image, Mat& out_image) {

	int** EnergyFunc1;
	int** EnergySum1;
	int*  horizontalSeam;
	Vec3b up;
	Vec3b down;
	Vec3b left;
	Vec3b right;
	double xGrad;
	double yGrad;

	EnergyFunc1 = new int*[in_image.rows];
	for (int i = 0; i <in_image.rows; i++)
		EnergyFunc1[i] = new int[in_image.cols];

	EnergySum1 = new int*[in_image.rows];

	for (int i = 0; i <in_image.rows; i++)
		EnergySum1[i] = new int[in_image.cols];

	horizontalSeam = new int[in_image.cols];

	for (int i = 0; i < in_image.rows; i++)
	{
		for (int j = 0; j < in_image.cols; j++)
		{

			if (i == 0)
			{
				if (j == 0)
				{
					left = in_image.at<Vec3b>(i, in_image.cols - 1);
					right = in_image.at<Vec3b>(i, j + 1);
					up = in_image.at<Vec3b>(in_image.rows - 1, j);
					down = in_image.at<Vec3b>(i + 1, j);

				}
				else if (j == in_image.cols - 1)
				{
					left = in_image.at<Vec3b>(i, j - 1);
					right = in_image.at<Vec3b>(i, 0);
					up = in_image.at<Vec3b>(in_image.rows - 1, j);
					down = in_image.at<Vec3b>(i + 1, j);

				}
				else
				{
					left = in_image.at<Vec3b>(i, j - 1);
					right = in_image.at<Vec3b>(i, j + 1);
					up = in_image.at<Vec3b>(in_image.rows - 1, j);
					down = in_image.at<Vec3b>(i + 1, j);

				}

			}
			else if (i == in_image.rows - 1)
			{
				if (j == 0)

				{
					//(3,0)
					left = in_image.at<Vec3b>(i, in_image.cols - 1);// (3,3)
					right = in_image.at<Vec3b>(i, j + 1);// (3,1)
					up = in_image.at<Vec3b>(i - 1, j);
					down = in_image.at<Vec3b>(0, j);

				}
				else if (j == in_image.cols - 1)
				{
					//(3,3)
					left = in_image.at<Vec3b>(i, j - 1);// (3,2)
					right = in_image.at<Vec3b>(i, 0);// (3,0)
					up = in_image.at<Vec3b>(i - 1, j);//(2,3)
					down = in_image.at<Vec3b>(0, j);//(0,3)

				}
				else
				{
					//(3,1)
					left = in_image.at<Vec3b>(i, j - 1);// (3,0)
					right = in_image.at<Vec3b>(i, j + 1);// (3,2)
					up = in_image.at<Vec3b>(i - 1, j);//(2,1)
					down = in_image.at<Vec3b>(0, j);//(0,1)

				}

			}
			else
			{
				if (j == 0) {

					//(1,0)
					left = in_image.at<Vec3b>(i, in_image.cols - 1);// (1,3)
					right = in_image.at<Vec3b>(i, j + 1);// (1,1)
					up = in_image.at<Vec3b>(i - 1, j);//(0,0)
					down = in_image.at<Vec3b>(i + 1, j);//(2,0)

				}


				else if (j == (in_image.cols - 1)) {

					//(1,3)

					left = in_image.at<Vec3b>(i, j - 1);// (1,2)
					right = in_image.at<Vec3b>(i, 0);// (1,0)
					up = in_image.at<Vec3b>(i - 1, j);//(0,3)
					down = in_image.at<Vec3b>(i + 1, j);//(2,3)

				}
				else {
					//(2,2)
					left = in_image.at<Vec3b>(i, j - 1);// (2,1)
					right = in_image.at<Vec3b>(i, j + 1);// (2,3)
					up = in_image.at<Vec3b>(i - 1, j);//(1,2)
					down = in_image.at<Vec3b>(i + 1, j);//(3,2)
				}
			}

			xGrad = ((left[0] - right[0])*(left[0] - right[0])) + ((left[1] - right[1])*(left[1] - right[1])) + ((left[2] - right[2])*(left[2] - right[2]));
			yGrad = ((up[0] - down[0])*(up[0] - down[0])) + ((up[1] - down[1])*(up[1] - down[1])) + ((up[2] - down[2])*(up[2] - down[2]));
			EnergyFunc1[i][j] = xGrad + yGrad;
		}


	}


	//Filling the Average energy by Dynamic programming using bottom-up approach.
	for (int j = 0; j < in_image.cols; j++)
	{
		for (int i = 0; i < in_image.rows; i++)
		{
			if (j == 0)
			{
				EnergySum1[i][j] = EnergyFunc1[i][j];
			}
			else if (i == 0)
			{
				int temp = min(EnergySum1[i][j - 1], EnergySum1[i + 1][j - 1]);
				EnergySum1[i][j] = EnergyFunc1[i][j] + temp;
			}
			else if (i == in_image.rows - 1)
			{
				int temp = min(EnergySum1[i - 1][j - 1], EnergySum1[i][j - 1]);
				EnergySum1[i][j] = EnergyFunc1[i][j] + temp;
			}

			else
			{
				int temp = min(min(EnergySum1[i - 1][j - 1], EnergySum1[i][j - 1]), EnergySum1[i + 1][j - 1]);
				EnergySum1[i][j] = EnergyFunc1[i][j] + temp;
			}
		}
	}


	int minStart = EnergySum1[0][in_image.cols - 1];

	//cout << endl;
	//cout << EnergySum1[0][in_image.rows - 1] << " // " << endl;


	int horizontalSeamTracker = 0;
	for (int i = 1; i < in_image.rows; i++)
	{
		if (EnergySum1[i][in_image.cols - 1] < minStart)
		{
			minStart = EnergySum1[i][in_image.cols - 1];
			horizontalSeamTracker = i;
		}
	}

	horizontalSeam[in_image.cols - 1] = horizontalSeamTracker;

	//backtracking to fill the Vertical Seam.

	for (int i = in_image.cols - 1; i > 0; i--)
	{
		if (horizontalSeamTracker == 0) //handles if the position of the verticalSeamTracker is at the start of the row
		{
			if (EnergySum1[horizontalSeamTracker][i - 1] > EnergySum1[horizontalSeamTracker + 1][i - 1])
			{
				horizontalSeamTracker++;
			}
		}
		else if (horizontalSeamTracker == in_image.rows - 1) //handles if the position of the verticalSeamTracker is at the end of the row
		{
			if (EnergySum1[horizontalSeamTracker][i - 1]>EnergySum1[horizontalSeamTracker - 1][i - 1])
			{
				horizontalSeamTracker--;
			}

		}
		else
		{
			if (EnergySum1[horizontalSeamTracker - 1][i - 1] < EnergySum1[horizontalSeamTracker][i - 1] && EnergySum1[horizontalSeamTracker - 1][i - 1]<EnergySum1[horizontalSeamTracker + 1][i - 1])
			{
				horizontalSeamTracker--;
			}
			if (EnergySum1[horizontalSeamTracker + 1][i - 1] < EnergySum1[horizontalSeamTracker][i - 1] && EnergySum1[horizontalSeamTracker + 1][i - 1]<EnergySum1[horizontalSeamTracker - 1][i - 1])
			{
				horizontalSeamTracker++;
			}

		}

		horizontalSeam[i - 1] = horizontalSeamTracker;

	}




	// retrieve the dimensions of the new image
	int rows = in_image.rows;
	int cols = in_image.cols;

	// create an image slighly smaller
	out_image = Mat(rows - 1, cols, CV_8UC3);

	for (int i = 0; i <cols; ++i)
	{
		for (int j = 0; j < horizontalSeam[i]; ++j) {
			Vec3b pixel = in_image.at<Vec3b>(j, i);


			out_image.at<Vec3b>(j, i) = pixel;
		}


		for (int j = horizontalSeam[i]; j < rows - 1; ++j) {
			Vec3b pixel = in_image.at<Vec3b>(j + 1, i);


			out_image.at<Vec3b>(j, i) = pixel;
		}
	}






	for (int i = 0; i <in_image.rows; i++)
		delete EnergyFunc1[i];
	delete EnergyFunc1;
	for (int i = 0; i <in_image.rows; i++)
		delete EnergySum1[i];
	delete EnergySum1;
	delete horizontalSeam;


	return true;
}
// vertical trivial seam is a seam through the center of the image
bool reduce_vertical_seam_trivial(Mat& in_image, Mat& out_image) {

	int** EnergyFunc;
	int** EnergySum;
	int*  verticalSeam;
	Vec3b up;
	Vec3b down;
	Vec3b left;
	Vec3b right;
	double xGrad;
	double yGrad;

	EnergyFunc = new int*[in_image.rows];
	for (int i = 0; i <in_image.rows; i++)
		EnergyFunc[i] = new int[in_image.cols];

	EnergySum = new int*[in_image.rows];
	for (int i = 0; i <in_image.rows; i++)
		EnergySum[i] = new int[in_image.cols];

	verticalSeam = new int[in_image.rows];
	for (int i = 0; i < in_image.rows; i++)
	{
		for (int j = 0; j < in_image.cols; j++)
		{

			if (i == 0)
			{
				if (j == 0)
				{
					left = in_image.at<Vec3b>(i, in_image.cols - 1);
					right = in_image.at<Vec3b>(i, j + 1);
					up = in_image.at<Vec3b>(in_image.rows - 1, j);
					down = in_image.at<Vec3b>(i + 1, j);

				}
				else if (j == in_image.cols - 1)
				{
					left = in_image.at<Vec3b>(i, j - 1);
					right = in_image.at<Vec3b>(i, 0);
					up = in_image.at<Vec3b>(in_image.rows - 1, j);
					down = in_image.at<Vec3b>(i + 1, j);

				}
				else
				{
					left = in_image.at<Vec3b>(i, j - 1);
					right = in_image.at<Vec3b>(i, j + 1);
					up = in_image.at<Vec3b>(in_image.rows - 1, j);
					down = in_image.at<Vec3b>(i + 1, j);

				}

			}
			else if (i == in_image.rows - 1)
			{
				if (j == 0)

				{
					//(3,0)
					left = in_image.at<Vec3b>(i, in_image.cols - 1);// (3,3)
					right = in_image.at<Vec3b>(i, j + 1);// (3,1)
					up = in_image.at<Vec3b>(i - 1, j);
					down = in_image.at<Vec3b>(0, j);

				}
				else if (j == in_image.cols - 1)
				{
					//(3,3)
					left = in_image.at<Vec3b>(i, j - 1);// (3,2)
					right = in_image.at<Vec3b>(i, 0);// (3,0)
					up = in_image.at<Vec3b>(i - 1, j);//(2,3)
					down = in_image.at<Vec3b>(0, j);//(0,3)

				}
				else
				{
					//(3,1)
					left = in_image.at<Vec3b>(i, j - 1);// (3,0)
					right = in_image.at<Vec3b>(i, j + 1);// (3,2)
					up = in_image.at<Vec3b>(i - 1, j);//(2,1)
					down = in_image.at<Vec3b>(0, j);//(0,1)

				}

			}
			else
			{
				if (j == 0) {

					//(1,0)
					left = in_image.at<Vec3b>(i, in_image.cols - 1);// (1,3)
					right = in_image.at<Vec3b>(i, j + 1);// (1,1)
					up = in_image.at<Vec3b>(i - 1, j);//(0,0)
					down = in_image.at<Vec3b>(i + 1, j);//(2,0)

				}


				else if (j == (in_image.cols - 1)) {

					//(1,3)

					left = in_image.at<Vec3b>(i, j - 1);// (1,2)
					right = in_image.at<Vec3b>(i, 0);// (1,0)
					up = in_image.at<Vec3b>(i - 1, j);//(0,3)
					down = in_image.at<Vec3b>(i + 1, j);//(2,3)

				}
				else {
					//(2,2)
					left = in_image.at<Vec3b>(i, j - 1);// (2,1)
					right = in_image.at<Vec3b>(i, j + 1);// (2,3)
					up = in_image.at<Vec3b>(i - 1, j);//(1,2)
					down = in_image.at<Vec3b>(i + 1, j);//(3,2)


				}
			}

			xGrad = ((left[0] - right[0])*(left[0] - right[0])) + ((left[1] - right[1])*(left[1] - right[1])) + ((left[2] - right[2])*(left[2] - right[2]));
			yGrad = ((up[0] - down[0])*(up[0] - down[0])) + ((up[1] - down[1])*(up[1] - down[1])) + ((up[2] - down[2])*(up[2] - down[2]));
			EnergyFunc[i][j] = xGrad + yGrad;
		}


	}

	//Filling the Average energy by Dynamic programming using bottom-up approach.
	for (int i = 0; i < in_image.rows; i++)
	{
		for (int j = 0; j < in_image.cols; j++)
		{
			if (i == 0)
			{
				EnergySum[i][j] = EnergyFunc[i][j];
			}
			else if (j == 0)
			{
				int temp = min(EnergySum[i - 1][j], EnergySum[i - 1][j + 1]);
				EnergySum[i][j] = EnergyFunc[i][j] + temp;
			}
			else if (j == in_image.cols - 1)
			{
				int temp = min(EnergySum[i - 1][j - 1], EnergySum[i - 1][j]);
				EnergySum[i][j] = EnergyFunc[i][j] + temp;
			}

			else
			{
				int temp = min(min(EnergySum[i - 1][j - 1], EnergySum[i - 1][j]), EnergySum[i - 1][j + 1]);
				EnergySum[i][j] = EnergyFunc[i][j] + temp;
			}
		}
	}

	//finding the minimum energy sum in the last row.
	int minStart = EnergySum[in_image.rows - 1][0];
	int verticalSeamTracker = 0;
	for (int i = 1; i < in_image.cols - 1; i++)
	{
		if (EnergySum[in_image.rows - 1][i] < minStart)
		{
			minStart = EnergySum[in_image.rows - 1][i];
			verticalSeamTracker = i;
		}
	}

	verticalSeam[in_image.rows - 1] = verticalSeamTracker;

	//backtracking to fill the Vertical Seam.

	for (int i = in_image.rows - 1; i > 0; i--)
	{
		if (verticalSeamTracker == 0) //handles if the position of the verticalSeamTracker is at the start of the row
		{
			if (EnergySum[i - 1][verticalSeamTracker] > EnergySum[i - 1][verticalSeamTracker + 1])
			{
				verticalSeamTracker++;
			}
		}
		else if (verticalSeamTracker == in_image.cols - 1) //handles if the position of the verticalSeamTracker is at the end of the row
		{
			if (EnergySum[i - 1][verticalSeamTracker] > EnergySum[i - 1][verticalSeamTracker - 1])
			{
				verticalSeamTracker--;
			}

		}
		else
		{
			if (EnergySum[i - 1][verticalSeamTracker - 1] < EnergySum[i - 1][verticalSeamTracker] && EnergySum[i - 1][verticalSeamTracker - 1]<EnergySum[i - 1][verticalSeamTracker + 1])
			{
				verticalSeamTracker--;
			}
			if (EnergySum[i - 1][verticalSeamTracker + 1] < EnergySum[i - 1][verticalSeamTracker] && EnergySum[i - 1][verticalSeamTracker + 1]<EnergySum[i - 1][verticalSeamTracker - 1])
			{
				verticalSeamTracker++;
			}

		}

		verticalSeam[i - 1] = verticalSeamTracker;

	}




	// retrieve the dimensions of the new image
	int rows = in_image.rows;
	int cols = in_image.cols - 1;

	// create an image slighly smaller
	out_image = Mat(rows, cols, CV_8UC3);

	//populate the image


	for (int i = 0; i < rows; ++i)

	{
		for (int j = 0; j < verticalSeam[i]; ++j) {
			Vec3b pixel = in_image.at<Vec3b>(i, j);

			/* at operator is r/w
			pixel[0] --> red
			pixel[1] --> green
			pixel[2] --> blue
			*/
			out_image.at<Vec3b>(i, j) = pixel;
		}


		for (int j = verticalSeam[i]; j < cols; ++j) {
			Vec3b pixel = in_image.at<Vec3b>(i, j + 1);

			/* at operator is r/w
			pixel[0] --> red
			pixel[1] --> green
			pixel[2] --> blue
			*/


			out_image.at<Vec3b>(i, j) = pixel;
		}

	}
	for (int i = 0; i <in_image.rows; i++)
		delete EnergyFunc[i];
	delete EnergyFunc;
	for (int i = 0; i <in_image.rows; i++)
		delete EnergySum[i];
	delete EnergySum;
	delete verticalSeam;
	return true;
}
