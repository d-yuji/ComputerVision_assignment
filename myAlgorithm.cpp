// * Unicodeを出力してください。
// * 出力結果は、vector<string>に格納してください。
// * レベルごとに出力個数が異なるので注意してください。
//   レベル1は出力1つ、レベル2では3つ、レベル3は3つ以上です。

#include "myAlgorithm.hpp"
#include "float.h"

// 一つのターゲットに含まれるUnicodeを返す
vector<string> CMyAlgorithm::exe( CTarget target, CAlconDatabase traindata ){

	// ターゲットの画像を取得する
	Mat docImg = target.GetDocumentImage(); // 文書画像
	Rect rect = target.GetRect(); // 矩形
	Mat imgTest = Mat( docImg, rect ).clone(); // ターゲット画像

	// ターゲットの特徴抽出
	Mat fTest = extractFeatures( imgTest );

// ここら辺のアルゴリズムを変えて考える
	
	// NN法で認識する
	vector<int> ids = traindata.GetIDs();
	double dmin = DBL_MAX;
	int idMin;

	for( vector<int>::iterator it=ids.begin(); it!=ids.end(); it++ ){
		// 学習データとの距離を計算する
		CTarget tr = traindata.GetTarget( *it ); // 学習データのターゲット
		Mat imgTrain = tr.GetTargetImage();
		Mat fTrain = extractFeatures( imgTrain ); // 学習データの特徴
		double d = norm( fTest - fTrain ); // L2ノルム
		
		// 距離を比較してその最小値を、文字の種類とする
		if( dmin > d ){
			dmin = d;
			idMin = tr.GetID();
		}
	}

	// 認識結果をベクトルとして返す
	vector<string> output = traindata.GetUnicodesForOneTarget( idMin );
	return output;
}


// 特徴抽出を行うメソッド
Mat CMyAlgorithm::extractFeatures( Mat img ){
	cvtColor( img, img, CV_RGB2GRAY ); // グレースケール変換
	resize( img, img, Size(32,32), 0, 0 ); // Resize
	img = img.reshape( 0, 1  );
	img.convertTo( img, CV_64FC1 ); // double
	return img;
}
