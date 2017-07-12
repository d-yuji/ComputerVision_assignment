// * Unicodeを出力してください。
// * 出力結果は、vector<string>に格納してください。
// * レベルごとに出力個数が異なるので注意してください。
//   レベル1は出力1つ、レベル2では3つ、レベル3は4つ以上16以下です。

#include "myAlgorithm.hpp"
#include "float.h"
#include <string>
#include <vector>
#include <cv.h>
#include <highgui.h>
#include <dirent.h>
#include <sys/stat.h>
#include <iterator>

using namespace std;
using namespace cv;
Mat extractForMeans(Mat img);
// 一つのターゲットに含まれるUnicodeを返す
vector<string> CMyAlgorithm::exe( CTarget target, CAlconDatabase traindata ){

	// ターゲットの画像を取得する
	Mat docImg = target.GetDocumentImage(); // 文書画像
	Rect rect = target.GetRect(); // 矩形
	Mat imgTest = Mat( docImg, rect ).clone(); // ターゲット画像
	Mat fTest;
	Mat fTrain;
    vector<string> dirNameList;

    const char* dataset_path = "../extractData/means/";// 平均画像画像へのパス
    string dirName = dataset_path;

    DIR *dp;       // ディレクトリへのポインタ
    DIR *fp;
    dirent* entry; // readdir() で返されるエントリーポイント

    dp = opendir(dataset_path);
	double dmin = DBL_MAX;

    if (dp==NULL) exit(1);
    do {
        entry = readdir(dp);
        if (entry != NULL && (entry->d_type == DT_DIR) && (strlen(entry->d_name)>2)){
            dirNameList.push_back(entry->d_name);
            //cout << entry->d_name;
            //cout << "\n";
        }
    } while (entry != NULL);

	// NN法で認識する
    string unicode;
    string id;
    for(vector<string>::iterator itr = dirNameList.begin(); itr != dirNameList.end(); itr++){
        vector<string> fileName;
        string path = "../extractData/means/"+(*itr);
        const char* filepath = path.c_str();
        //cout << "\n" << endl;
        fp = opendir(filepath);
        if (fp==NULL) exit(1);
        do {
            entry = readdir(fp);
            if (entry != NULL && (entry->d_type != DT_DIR)){
                fileName.push_back(entry->d_name);
               // cout << entry->d_name << endl;
            }
        } while (entry != NULL);
		
		for(vector<string>::iterator itr2 = fileName.begin(); itr2 != fileName.end(); itr2++){
			// 学習データとの距離を計算する
			// 学習データのターゲット
			fTrain = imread(path+"/"+(*itr2));
			// TODO U+307B mean5.jpg 実行途中で画像読み込みができなくなるエラー

			// ターゲットの特徴抽出
			if(!fTrain.empty()){
				fTest = extractFeatures( imgTest );
				fTrain = extractFeatures(fTrain);
				double d = norm( fTest - fTrain ); // L2ノルム
				// 最小値
				if( dmin > d ){
					dmin = d;
					unicode = (*itr);
					id = (*itr2);
				}
			}else{
				cout <<path+"/"+(*itr2)<< endl;
				cout << "load error" << endl;
			}
		}
	}
	docImg.release();
	imgTest.release();
	fTest.release();
	fTrain.release();

	// 認識結果
	vector<string> output{unicode};
	return output;
}


Mat CMyAlgorithm::extractFeatures( Mat img ){
	cvtColor( img, img, CV_RGB2GRAY ); // グレースケール変換
	threshold(img, img, 0, 255, THRESH_BINARY_INV|THRESH_OTSU);
	resize(img,img,Size(256,256),0,0);
	GaussianBlur(img, img, cv::Size(11,11), 10, 10);
	resize( img, img, Size(32,32), 0, 0 );
	img = ~img;
	img = img.reshape( 0, 1  );
	img.convertTo( img, CV_64FC1 ); // double
	return img;
}
