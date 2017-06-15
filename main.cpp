#include <iostream>
#include <string>
#include "alcon.hpp"
#include "myAlgorithm.hpp"

using namespace std;
using namespace cv;


int main(int argc, char *argv[]) {

	// Check arguments
	if( argc != 3 ){
		cout << "Usage: ./run /path/to/the/dataset/ lv" << endl;
		exit(1);
	}

	// データセットのパスを設定する
	const string dirData = argv[1]; // Path to the dataset
	const string lv = argv[2]; //レベルの指定
	
	// アノテーションファイルを指定します
	const string fn_train_target = "target_lv" + lv + "_train_1.csv";
	const string fn_test_target = "target_lv" + lv + "_test_1.csv";
	const string fn_train_ground = "groundtruth_lv" + lv + "_train_1.csv";
	const string fn_test_ground = "groundtruth_lv" + lv + "_test_1.csv"; 
	
	// 学習データを読み込む
	string fnTrainTar = dirData + "annotations/train_lv1/" + fn_train_target;
	string fnTrainGrn = dirData + "annotations/train_lv1/" + fn_train_ground;
	CAlconDatabase traindata( dirData, fnTrainTar, fnTrainGrn );

	// テストデータを読み込む
	string fnTestTar = dirData + "annotations/test_lv1/" + fn_test_target;
	string fnTestGrn = dirData + "annotations/test_lv1/" + fn_test_ground;
	CAlconDatabase testdata( dirData, fnTestTar, fnTestGrn );

	// ターゲットごとに認識します
	CMyAlgorithm myAlg;//自分でアルゴリズムを作るクラス

	vector<int> ids = testdata.GetIDs(); // テストデータのID
	map< int, vector<string> > outs; // すべての出力結果. map[ key ] = vector<string>

	// テストデータごとにアルゴリズムを適用
	for( vector<int>::iterator it = ids.begin(); it!=ids.end(); it++ ){
		
		// 認識対象のデータを取得する
		CTarget targetData = testdata.GetTarget( *it );
		//t.PrintMembers();

		// 自分のアルゴリズムを起動する
		vector<string> outsResult = myAlg.exe( targetData, traindata );

		// 結果を表示する
		cout << "Output: target[" << *it << "] = ";
		
		for(vector<string>::iterator it2 = outsResult.begin(); it2 != outsResult.end(); it2++){
			cout << *it2 << ",";
		}
		cout << endl;
		
		// 出力結果を格納する
		outs[ targetData.GetID() ] = outsResult;

	}

	// Evaluation
	string fnOuts = "myOutputs" + lv + ".csv";
	testdata.WriteOutputs( outs, fnOuts );
	testdata.Evaluation( outs );
	
	return 0;
}

