// 自由に編集してください。

#ifndef __MY_ALGORITHM_H_INCLUDED
#define __MY_ALGORITHM_H_INCLUDED

#include <opencv2/opencv.hpp>
#include "alcon.hpp"
using namespace cv;

class CMyAlgorithm{
public:
  vector<string> exe( CTarget target, CAlconDatabase traindata ); // 一つのターゲットに含まれるUnicodeを返す
  Mat extractFeatures( Mat img );
}; // CMyAlgorithm

#endif
