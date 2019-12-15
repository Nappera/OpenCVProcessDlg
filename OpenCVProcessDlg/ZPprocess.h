#pragma once
#include "opencv2/opencv.hpp"
typedef struct RUN_LENGTH_ENCODING
{
	int row;
	int start_column;
	int end_column;


}T_RUN_LENGTH_ENCODING;


void CV_Threshold(cv::Mat grayImage, std::vector<T_RUN_LENGTH_ENCODING>* output, int minGray, int maxGray);

void CV_Connection(const std::vector<T_RUN_LENGTH_ENCODING>* input, std::vector<std::vector<T_RUN_LENGTH_ENCODING>>* output);

void CV_AreaCenter(const std::vector<T_RUN_LENGTH_ENCODING>* input, double* area, double* row, double* column);

void CV_MomentsRegion2nd(const std::vector<T_RUN_LENGTH_ENCODING>* input, double* m11, double* m02, double* m20, double* ia, double* ib);

void CV_EllipticAxis(const std::vector<T_RUN_LENGTH_ENCODING>* input, double* ra, double* rb, double* phi);

void CV_Contlength(const std::vector<T_RUN_LENGTH_ENCODING>* input, double* length);

void CV_HeightWidthRatio(const std::vector<T_RUN_LENGTH_ENCODING>* input, int* width, int* height, double* ratio);

void CV_Convexity(const std::vector<T_RUN_LENGTH_ENCODING>* input, double* convexity);

void CV_Circularity(const std::vector<T_RUN_LENGTH_ENCODING>* input, double* circularity);

void CV_DiameterRegion(const std::vector<T_RUN_LENGTH_ENCODING>* input, int* row1, int* column1, int* row2, int* column2, double* diameter);

//最小外接矩形（不带角度）
void CV_SmallestRectangle1(const std::vector<T_RUN_LENGTH_ENCODING>* input, int* row1, int* column1, int* row2, int* column2);
void CV_SmallestCircle(const std::vector<T_RUN_LENGTH_ENCODING>* input, double* row, double* column, double* radius);

