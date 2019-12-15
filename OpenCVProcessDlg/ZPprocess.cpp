#include "stdafx.h"
#include "ZPprocess.h"



void dfs(const std::vector<T_RUN_LENGTH_ENCODING>* input, int* labels, int index, int label)
{
	labels[index] = label;
	//printf("%d\n", v);
	int c_row = (*input)[index].row;
	int c_start_column = (*input)[index].start_column;
	int c_end_column = (*input)[index].end_column;
	int i = 0;
	//#pragma omp parallel for private(i) schedule(guided)
	for (i = index + 1; i < (*input).size(); i++)
	{
		int row = (*input)[i].row;
		int start_column = (*input)[i].start_column;
		int end_column = (*input)[i].end_column;
		if (row - c_row > 1)
		{
			break;
		}
		//else if (row - c_row == 1 && labels[i] == -1)
		else if (labels[i] == -1)
		{
			/*int e_colum = (end_column > c_end_column) ? c_end_column : end_column;
			int s_colum = (start_column > c_start_column) ? start_column : c_start_column;*/
			if (std::min(end_column, c_end_column) - std::max(start_column, c_start_column) >= -1)
				//if (e_colum - s_colum >= 0)
			{
				dfs(input, labels, i, label);
			}

		}
		//dfs_count++;
	}
	for (i = index - 1; i >= 0; i--)
	{
		int row = (*input)[i].row;
		int start_column = (*input)[i].start_column;
		int end_column = (*input)[i].end_column;

		if (c_row - row > 1)
		{
			break;
		}
		//else if (c_row - row == 1 && labels[i] == -1)
		else if (labels[i] == -1)
		{
			if (std::min(end_column, c_end_column) - std::max(start_column, c_start_column) >= 0)
			{
				dfs(input, labels, i, label);
			}

		}
		//dfs_count++;
	}
}
void CV_Threshold(cv::Mat grayImage, T_RUN_LENGTH_ENCODING* output, int minGray, int maxGray)
{
	int nl = grayImage.rows;                   // 行数
	int nc = grayImage.cols * grayImage.channels();
	int count = 0;
	T_RUN_LENGTH_ENCODING t;

	for (int j = 0; j < nl; j++)
	{
		bool flag = false;
		uchar* data = grayImage.ptr<uchar>(j);
		for (int i = 0; i < nc; i++)
		{
			if ((data[i] > minGray) && !flag)
			{
				flag = true;
				t.row = j;
				t.start_column = i;
			}
			else if (data[i] <= minGray && flag == 1)
			{
				flag = false;
				t.end_column = i - 1;
				//(*output).push_back(t);
			}
			else if (i == nc - 1 && flag == 1)
			{
				flag = false;
				t.end_column = i;
				//(*output).push_back(t);
			}
		}
	}

}

void CV_Threshold(cv::Mat grayImage, std::vector<T_RUN_LENGTH_ENCODING>* output, int minGray, int maxGray)
{
	(*output).clear();
	int nl = grayImage.rows;                   // 行数
	int nc = grayImage.cols * grayImage.channels();

	T_RUN_LENGTH_ENCODING t;
	for (int j = 0; j < nl; j++)
	{
		bool flag = false;
		uchar* data = grayImage.ptr<uchar>(j);
		for (int i = 0; i < nc; i++)
		{
			if ((data[i] >= minGray) && (data[i] <= maxGray) && !flag)
			{
				flag = true;
				t.row = j;
				t.start_column = i;
			}
			else if ((data[i] < minGray || data[i] > maxGray) && flag)
			{
				flag = false;
				t.end_column = i - 1;
				(*output).push_back(t);
			}
			else if (i == nc - 1 && flag)
			{
				flag = false;
				t.end_column = i;
				(*output).push_back(t);
			}
		}
	}

}

void CV_Connection(const std::vector<T_RUN_LENGTH_ENCODING>* input, std::vector<std::vector<T_RUN_LENGTH_ENCODING>>* output)
{
	size_t rle_size = (*input).size();
	int label = -1;
	int* labels;
	labels = new int[rle_size];

	memset(labels, -1, sizeof(int)*rle_size);

	for (int i = 0; i < rle_size; i++)
	{
		if (labels[i] == -1)
		{
			label++;
			dfs(input, labels, i, label);
			//printf("----------------------\n");
		}
	}
	//std::vector <std::vector<T_RUN_LENGTH_ENCODING>> vts;
	(*output) = std::vector <std::vector<T_RUN_LENGTH_ENCODING>>(label + 1);
	for (int i = 0; i < rle_size; i++)
	{
		(*output)[labels[i]].push_back((*input)[i]);
	}
	delete labels;
}

void CV_AreaCenter(const std::vector<T_RUN_LENGTH_ENCODING>* input_region, double* output_area, double* output_row, double* output_column)
{
	const std::vector<T_RUN_LENGTH_ENCODING>* p = input_region;

	size_t sum_r = 0;
	size_t sum_c = 0;
	double area = 0.0;
	for (int j = 0; j < p->size(); j++)
	{

		int length = ((*p)[j].end_column - (*p)[j].start_column + 1);
		area += length;
		sum_c += length * ((*p)[j].end_column + (*p)[j].start_column);//需要除2
		sum_r += length * (*p)[j].row;
	}
	(*output_area) = area;
	(*output_row) = sum_r / area;
	(*output_column) = sum_c / 2.0 / area;
}

void CV_MomentsRegion2nd(const std::vector<T_RUN_LENGTH_ENCODING>* input, double* m11, double* m02, double* m20, double* ia, double* ib)
{
	const std::vector<T_RUN_LENGTH_ENCODING>* p = input;

	size_t sum_r = 0;
	size_t sum_c = 0;
	size_t sum_r_2 = 0;
	size_t sum_c_2 = 0;
	size_t sum_r_c = 0;

	double area = 0.0;
	for (int j = 0; j < input->size(); j++)
	{

		int length = ((*p)[j].end_column - (*p)[j].start_column + 1);
		area += length;
		sum_c += length * ((*p)[j].end_column + (*p)[j].start_column);//需要除2
		sum_r += length * (*p)[j].row;

		//1^2+2^2+3^2+...+n^2=n(n+1)(2n+1)/6
		sum_c_2 += (((*p)[j].end_column)*((*p)[j].end_column + 1)*((*p)[j].end_column * 2 + 1) -
			(((*p)[j].start_column - 1))*(((*p)[j].start_column - 1) + 1)*(((*p)[j].start_column - 1) * 2 + 1));//需要除6
		sum_r_2 += length * ((*p)[j].row*(*p)[j].row);

		sum_r_c += length * (*p)[j].row * ((*p)[j].end_column + (*p)[j].start_column);//需要除2

	}
	double sum_rd = (double)sum_r;
	double sum_cd = (double)sum_c / 2.0;
	double sum_r_2d = (double)sum_r_2;
	double sum_c_2d = (double)sum_c_2 / 6.0;
	double sum_r_cd = (double)sum_r_c / 2.0;


	/*double mu11 = sum_r_cd - sum_rd * sum_cd / area;
	double mu20 = sum_r_2d - sum_rd * sum_rd / area;
	double mu02 = sum_c_2d - sum_cd * sum_cd / area;*/
	(*m11) = sum_r_cd - sum_rd * sum_cd / area;
	(*m20) = sum_r_2d - sum_rd * sum_rd / area;
	(*m02) = sum_c_2d - sum_cd * sum_cd / area;

	double h = (*m20 + *m02) / 2;
	double temp = sqrt(h*h - (*m20)*(*m02) + (*m11)*(*m11));

	(*ia) = h + temp;
	(*ib) = h - temp;
}

void CV_EllipticAxis(const std::vector<T_RUN_LENGTH_ENCODING>* input, double* ra, double* rb, double* phi)
{
	const std::vector<T_RUN_LENGTH_ENCODING>* p = input;

	size_t sum_r = 0;
	size_t sum_c = 0;
	size_t sum_r_2 = 0;
	size_t sum_c_2 = 0;
	size_t sum_r_c = 0;

	double area = 0.0;
	for (int j = 0; j < input->size(); j++)
	{

		int length = ((*p)[j].end_column - (*p)[j].start_column + 1);
		area += length;
		sum_c += length * ((*p)[j].end_column + (*p)[j].start_column);//需要除2
		sum_r += length * (*p)[j].row;

		//1^2+2^2+3^2+...+n^2=n(n+1)(2n+1)/6
		sum_c_2 += (((*p)[j].end_column)*((*p)[j].end_column + 1)*((*p)[j].end_column * 2 + 1) -
			(((*p)[j].start_column - 1))*(((*p)[j].start_column - 1) + 1)*(((*p)[j].start_column - 1) * 2 + 1));//需要除6
		sum_r_2 += length * ((*p)[j].row*(*p)[j].row);

		sum_r_c += length * (*p)[j].row * ((*p)[j].end_column + (*p)[j].start_column);//需要除2

	}
	double sum_rd = (double)sum_r;
	double sum_cd = (double)sum_c / 2.0;
	double sum_r_2d = (double)sum_r_2;
	double sum_c_2d = (double)sum_c_2 / 6.0;
	double sum_r_cd = (double)sum_r_c / 2.0;


	double m11 = sum_r_cd - sum_rd * sum_cd / area;
	double m20 = sum_r_2d - sum_rd * sum_rd / area;
	double m02 = sum_c_2d - sum_cd * sum_cd / area;

	double mu11 = m11 / area;
	double mu20 = m20 / area;
	double mu02 = m02 / area;

	double temp = sqrt((mu20 - mu02)*(mu20 - mu02) + 4 * mu11*mu11);
	(*ra) = sqrt(2 * (mu20 + mu02 + temp));
	(*rb) = sqrt(2 * (mu20 + mu02 - temp));
	(*phi) = -0.5*atan2(2 * mu11, mu02 - mu20);
}

void CV_Contlength(const std::vector<T_RUN_LENGTH_ENCODING>* input, double* length)
{
	std::vector<std::vector<T_RUN_LENGTH_ENCODING>> output;
	CV_Connection(input, &output);
	(*length) = 0.0;
	for (int i = 0; i < output.size(); i++)
	{
		const std::vector<T_RUN_LENGTH_ENCODING>* p = &output[i];

		if (p->size() == 1)
		{
			(*length) += 1;
			continue;
		}

		//(-1,-1)(5) ( 0,-1)(4) ( 1,-1)(3)
		//(-1, 0)(6)            ( 1, 0)(2)
		//(-1, 1)(7) ( 0, 1)(0) ( 1, 1)(1)
		int directions[8][2] = { {0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1} };
		int start_direction = 5;
		int start_index = 0;
		int start_row = (*p)[start_index].row;
		int start_column = (*p)[start_index].start_column;
		(*length) += 1;
		//printf("row,colunm,dir - %d,%d,%d\n", start_row, start_column, start_direction);

		while (1)
		{
			int row = start_row + directions[start_direction][1];
			int column = start_column + directions[start_direction][0];

			if (row == (*p)[0].row && column == (*p)[0].start_column)
			{
				break;
			}

			if (row == 5 && column == 37)
			{
				int m = 0;;
			}
			bool is_find = false;
			int index = start_index;
			if (start_row < row)//++
			{
				index = start_index;
				while (1)
				{
					index++;
					if (index > (*p).size() - 1 || (*p)[index].row > row)
					{
						break;
					}
					else if ((*p)[index].row == row)
					{
						if (column <= (*p)[index].end_column && column >= (*p)[index].start_column)
						{
							is_find = true;
							break;
						}

					}

				}
			}
			else if (start_row > row)//--
			{
				index = start_index;
				while (1)
				{
					index--;
					if (index < 0 || (*p)[index].row < row)
					{
						break;
					}
					else if ((*p)[index].row == row)
					{
						if (column <= (*p)[index].end_column && column >= (*p)[index].start_column)
						{
							is_find = true;

							break;
						}

					}

				}
			}
			else
			{
				index = start_index;
				if (column <= (*p)[index].end_column && column >= (*p)[index].start_column)
				{
					is_find = true;
				}

			}
			if (is_find)
			{
				start_index = index;
				start_row = row;
				start_column = column;
				if (start_direction % 2 == 0)
				{
					(*length) += 1;
				}
				else
				{
					(*length) += 1.4142135623731;
				}


				//printf("row,colunm,dir - %d,%d,%d\n", start_row, start_column, start_direction);

				start_direction -= 2;
				if (start_direction < 0)
				{
					start_direction += 8;
				}
			}
			else
			{
				start_direction++;
				start_direction = start_direction % 8;
			}
		}

	}
}

void CV_SmallestRectangle1(const std::vector<T_RUN_LENGTH_ENCODING>* input, int* row1, int* column1, int* row2, int* column2)
{
	const std::vector<T_RUN_LENGTH_ENCODING>* p = input;
	size_t size = p->size();
	int min_row = (*p)[0].row;
	int max_row = (*p)[size - 1].row;
	int min_column = (*p)[0].start_column;
	int max_column = (*p)[0].end_column;
	for (int i = 1; i < size; i++)
	{
		min_column = std::min(min_column, (*p)[i].start_column);
		max_column = std::max(max_column, (*p)[i].end_column);
	}
	(*row1) = min_row;
	(*column1) = min_column;
	(*row2) = max_row;
	(*column2) = max_column;

}

void CV_HeightWidthRatio(const std::vector<T_RUN_LENGTH_ENCODING>* input, int* width, int* height, double* ratio)
{
	const std::vector<T_RUN_LENGTH_ENCODING>* p = input;
	int row1 = 0;
	int column1 = 0;
	int row2 = 0;
	int column2 = 0;
	CV_SmallestRectangle1(p, &row1, &column1, &row2, &column2);

	(*width) = column2 - column1 + 1;
	(*height) = row2 - row1 + 1;

	(*ratio) = 1.0*(*height) / (*width);
}

void CV_GetRegionConvex(const std::vector<T_RUN_LENGTH_ENCODING>* input, std::vector<cv::Point>* output)
{
	const std::vector<T_RUN_LENGTH_ENCODING>* p = input;
	size_t size = p->size();
	if (size <= 2)
	{
		return;
	}

	std::vector<cv::Point> vP;

	vP.push_back(cv::Point((*p)[0].row, (*p)[0].start_column));
	vP.push_back(cv::Point((*p)[0].row, (*p)[0].end_column));
	for (int i = 1; i < size; i++)
	{
		int index = vP.size() - 1;
		if (vP[index].x == (*p)[i].row)
		{
			vP[index].y = (*p)[i].end_column;
		}
		else
		{
			vP.push_back(cv::Point((*p)[i].row, (*p)[i].start_column));
			vP.push_back(cv::Point((*p)[i].row, (*p)[i].end_column));
		}
	}


	//(x2 - x1)(y0 - y1)-(y2 - y1)(x0 - x1) > 0   左侧
	//(x2 - x1)(y0 - y1)-(y2 - y1)(x0 - x1) < 0   右侧
	//(x2 - x1)(y0 - y1)-(y2 - y1)(x0 - x1) = 0   线段上

	size = vP.size();

	std::vector<cv::Point> vR;
	vR.push_back(vP[0]);
	vR.push_back(vP[1]);
	for (int i = 2; i < size; i++)
	{
		vR.push_back(vP[i]);
		while (1)
		{
			int index = vR.size() - 1;
			int x1 = vR[index - 2].x;
			int y1 = vR[index - 2].y;
			int x2 = vR[index - 1].x;
			int y2 = vR[index - 1].y;
			int x0 = vR[index].x;
			int y0 = vR[index].y;
			int temp = (x2 - x1)*(y0 - y1) - (y2 - y1)*(x0 - x1);

			if (temp < 0)
			{
				break;
			}
			else
			{
				/*vL[index].x = vP[i].x;
				vL[index].y = vP[i].y;*/
				vR.erase(vR.begin() + index - 1);
				if (vR.size() < 3)
				{
					break;
				}

			}
		}

	}
	//删除vR的首个
	//vR.erase(vR.begin() + 0);

	std::vector<cv::Point> vL;
	vL.push_back(vP[size - 1]);
	vL.push_back(vP[size - 2]);
	for (int i = size - 3; i >= 0; i--)
	{
		vL.push_back(vP[i]);
		while (1)
		{
			int index = vL.size() - 1;
			int x1 = vL[index - 2].x;
			int y1 = vL[index - 2].y;
			int x2 = vL[index - 1].x;
			int y2 = vL[index - 1].y;
			int x0 = vL[index].x;
			int y0 = vL[index].y;
			int temp = (x2 - x1)*(y0 - y1) - (y2 - y1)*(x0 - x1);
			if (temp < 0)
			{
				break;
			}
			else
			{
				vL.erase(vL.begin() + index - 1);
				if (vL.size() < 3)
				{
					break;
				}
			}
		}
	}


	//删除vL的首个
	vL.erase(vL.begin() + 0);
	//HALCON中首尾元素一致,此处跟HALCON一致

	(*output).clear();
	output->insert((*output).end(), vR.begin(), vR.end());
	output->insert((*output).end(), vL.begin(), vL.end());
	


	int m = 0;
}

bool ComparePointY(cv::Point a, cv::Point b)
{
	return a.y < b.y; //升序排列
}
bool ComparePointX(cv::Point a, cv::Point b)
{
	return a.x < b.x; //升序排列
}
void CV_GenRegionPolygonFilled(const std::vector<cv::Point>* input, std::vector<T_RUN_LENGTH_ENCODING>* output)
{
	const std::vector<cv::Point>* p = input;
	size_t size = p->size();
	std::vector<cv::Point> vP;
	cv::Point pt = (*p)[0];
	vP.push_back(pt);
	int min_row = pt.x;
	int max_row = pt.x;
	for (int i = 1; i < size; i++)
	{
		int length = (*p)[i].x - pt.x;
		if ((*p)[i].x - pt.x >= -1 && (*p)[i].x - pt.x <= 1)
		{
			pt = (*p)[i];
			vP.push_back(pt);
		}
		else if(length > 1)
		{
			int index = abs(length);
			double a = 1.0*((*p)[i].y - pt.y) / ((*p)[i].x - pt.x);
			double b = pt.y - a * pt.x + 0.5;
			while (index--)
			{
				pt.x = pt.x + 1;
				//double c = a * pt.x + b;
				pt.y = a * pt.x + b;
				vP.push_back(pt);
			}
			pt = (*p)[i];
			vP.push_back(pt);
		}
		else
		{
			int index = abs(length) - 1;
			double a = 1.0*((*p)[i].y - pt.y) / ((*p)[i].x - pt.x);
			double b = pt.y - a * pt.x + 0.5;
			while (index--)
			{
				pt.x = pt.x - 1;
				pt.y = a * pt.x + b;
				vP.push_back(pt);
			}
			pt = (*p)[i];
			vP.push_back(pt);
		}

		min_row = std::min(min_row, pt.x);
		max_row = std::max(max_row, pt.x);
	}

	

	size = vP.size();
	if (vP[0] == vP[size - 1])
	{
		vP.erase(vP.begin() + size - 1);
	}
	size = vP.size();
	std::vector <std::vector<cv::Point>> vPs(max_row - min_row + 1);
	for (int i = 0; i < size; i++)
	{
		vPs[vP[i].x - min_row].push_back(vP[i]);
	
	}

	size = vPs.size();
	for (int i = 0; i < size; i++)
	{
		std::vector<cv::Point>* q = &vPs[i];
		std::sort(q->begin(), q->end(), ComparePointY);
		int length = q->size() / 2;
		for (int j=0;j< length;j++)
		{
			int index = j << 1;
			output->push_back(T_RUN_LENGTH_ENCODING{ (*q)[index].x,(*q)[index].y,(*q)[index+1].y });
		
		}

	}


	int m = 0;
}
void CV_GenRegionPolygonFilled_V1(const std::vector<cv::Point>* input, std::vector<T_RUN_LENGTH_ENCODING>* output)
{
	std::vector<cv::Point> _temp(*input);
	std::sort(_temp.begin(), _temp.end(), ComparePointY);
	int mat_height = _temp[_temp.size() - 1].y;
	std::sort(_temp.begin(), _temp.end(), ComparePointX);
	int mat_width = _temp[_temp.size() - 1].x;
	cv::Mat _mat(mat_height + 1, mat_width + 1, CV_8UC1);

	std::vector<cv::Point> _inputcopy(*input);
	_inputcopy.push_back(_inputcopy[0]);

	const std::vector<cv::Point>* p = &_inputcopy;
	size_t size = p->size();

	std::vector<cv::Point> vP;
	cv::Point pt = (*p)[0];
	//vP.push_back(pt);
	int min_row = pt.y;
	int max_row = pt.y;
	for (int i = 0; i < size - 1; i++)
	{
		cv::LineIterator linefit(_mat, (*p)[i], (*p)[i + 1], 8);

		for (size_t i = 0; i < linefit.count - 1; i++) //不要最后一位  -1
		{
			vP.push_back(linefit.pos());
			linefit++;
		}

		min_row = (min_row - (*p)[i].y>0) ? (*p)[i].y : min_row;
		max_row = (max_row - (*p)[i].y<0) ? (*p)[i].y : max_row;

	}

	size = vP.size();
	std::vector <std::vector<cv::Point>> vPs(max_row - min_row + 1);
	for (int i = 0; i < size; i++)
	{
		vPs[vP[i].y - min_row].push_back(vP[i]);

	}

	size = vPs.size();

	for (int i = 0; i < size; i++)
	{
		std::vector<cv::Point>* q = &vPs[i];
		std::sort(q->begin(), q->end(), ComparePointX);
		output->push_back(T_RUN_LENGTH_ENCODING{ (*q)[0].y,(*q)[0].x,(*q)[q->size() - 1].x });
	}

	int a = 0;

}
void CV_Convexity(const std::vector<T_RUN_LENGTH_ENCODING>* input, double* convexity)
{
	std::vector<cv::Point> output;
	CV_GetRegionConvex(input, &output);

	std::vector<T_RUN_LENGTH_ENCODING> vR;
	CV_GenRegionPolygonFilled(&output, &vR);
	double c_area = 0.0;
	c_area=cv::contourArea(output);
	double r_area1, r_area2;
	double row;
	double column;
	CV_AreaCenter(&vR, &r_area1, &row, &column);
	CV_AreaCenter(input, &r_area2, &row, &column);
	(*convexity) = r_area2 / (r_area1);
	int i = 0;
}


double CV_Distance(const cv::Point2d* pt1, const  cv::Point2d* pt2)
{
	return sqrt((pt1->x - pt2->x)*(pt1->x - pt2->x) + (pt1->y - pt2->y)*(pt1->y - pt2->y));
}
void CV_DistancePr(const cv::Point2d* input_pt, const std::vector<T_RUN_LENGTH_ENCODING>* input_region, double* output_mindistance, double* output_maxdistance)
{
	const std::vector<T_RUN_LENGTH_ENCODING>* p = input_region;
	const cv::Point2d* pt = input_pt;
	size_t size = p->size();

	double min_distance = CV_Distance(pt, &cv::Point2d((*p)[0].row, (*p)[0].start_column));
	double max_distance = CV_Distance(pt, &cv::Point2d((*p)[0].row, (*p)[0].start_column));
	double distance = 0.0;
	for (int i = 0; i < size; i++)
	{
		distance = CV_Distance(pt, &cv::Point2d((*p)[i].row, (*p)[i].start_column));
		if (distance > max_distance)
		{
			max_distance = distance;
		}
		if (distance < min_distance)
		{
			min_distance = distance;
		}
		distance = CV_Distance(pt, &cv::Point2d((*p)[i].row, (*p)[i].end_column));
		if (distance > max_distance)
		{
			max_distance = distance;
		}
		if (distance < min_distance)
		{
			min_distance = distance;
		}
	}

	(*output_mindistance) = min_distance;
	(*output_maxdistance) = max_distance;
}


void CV_Circularity(const std::vector<T_RUN_LENGTH_ENCODING>* input_region, double* output_circularity)
{
	/*std::vector<cv::Point> contour;
	CV_GetRegionConvex(input, &contour);
	int size = (int)contour.size();
	cv::Point* pt1 = &contour[0];
	cv::Point* pt2 = &contour[1];
	double max_distance = CV_Distance(pt1, pt2);
	for (int i = 1; i < size - 1; i++)
	{
		for (int j = i + 1; j < size; j++)
		{
			double distance = CV_Distance(&contour[i], &contour[j]);
			if (distance > max_distance)
			{
				max_distance = distance;
				pt1 = &contour[i];
				pt2 = &contour[j];
			}

		}
	}
	double radius = max_distance / 2;
	double temp = CV_PI * radius*radius;

	double area;
	double row;
	double column;
	CV_AreaCenter(input, &area, &row, &column);*/


	const std::vector<T_RUN_LENGTH_ENCODING>* p = input_region;
	size_t size = p->size();

	double area;
	double row;
	double column;
	CV_AreaCenter(input_region, &area, &row, &column);
	


	double min_distance;
	double max_distance;
	CV_DistancePr(&cv::Point2d(row, column), input_region, &min_distance, &max_distance);


	//double radius = max_distance / 2;
	double temp = CV_PI * max_distance*max_distance;

	(*output_circularity) = area / temp;

}

void CV_DiameterRegion(const std::vector<T_RUN_LENGTH_ENCODING>* input, int* row1, int* column1, int* row2, int* column2, double* diameter)
{
	/*std::vector<cv::Point> contour;
	CV_GetRegionConvex(input, &contour);
	int size = (int)contour.size();
	cv::Point* pt1 = &contour[0];
	cv::Point* pt2 = &contour[1];
	double max_distance = CV_Distance(pt1, pt2);
	for (int i = 1; i < size - 1; i++)
	{
		for (int j = i + 1; j < size; j++)
		{
			double distance = CV_Distance(&contour[i], &contour[j]);
			if (distance > max_distance)
			{
				max_distance = distance;
				pt1 = &contour[i];
				pt2 = &contour[j];
			}

		}
	}
	double radius = max_distance / 2;
	double temp = CV_PI * radius*radius;

	double area;
	double row;
	double column;
	CV_AreaCenter(input, &area, &row, &column);*/

}


void CV_SmallestCircle(const std::vector<T_RUN_LENGTH_ENCODING>* input, double* row, double* column, double* radius)
{


}
