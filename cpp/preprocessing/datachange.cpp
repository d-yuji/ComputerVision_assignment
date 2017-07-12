//画像の前処理するプログラム
//g++ -O2 -o dc datachange.cpp `pkg-config --cflags opencv` `pkg-config --libs opencv`
#include <iostream>
#include <string>
#include <vector>
#include <cv.h>
#include <highgui.h>
#include <dirent.h>
#include <sys/stat.h>

using namespace std;
using namespace cv;

int main(int argc, char const *argv[]){
    vector<string> dirNameList;
    const char* dataset_path = "";//元のデータセットへのパス
    const char* writedata_path = "../dataset/";//保存先のパス
    string dirName = dataset_path;

    DIR *dp;       // ディレクトリへのポインタ
    DIR *fp;
    dirent* entry; // readdir() で返されるエントリーポイント
    vector<string>::iterator itr;

    dp = opendir(dataset_path);
    if (dp==NULL) exit(1);
    do {
        entry = readdir(dp);
        if (entry != NULL && (entry->d_type == DT_DIR) && (strlen(entry->d_name)>2)){
            dirNameList.push_back(entry->d_name);
            cout << entry->d_name;
            cout << "\n";
        }
    } while (entry != NULL);

    for(itr = dirNameList.begin();itr != dirNameList.end(); ++itr){
        vector<string> fileName;
        string path = dataset_path+(*itr);
        const char* filepath = path.c_str();

        string tmppath = writedata_path+(*itr);
        const char* newdir = tmppath.c_str();
        mkdir(newdir, 0777);
        //cout << "\n";
        fp = opendir(filepath);
        if (fp==NULL) exit(1);
        do {
            //cout << "debug\n";
            entry = readdir(fp);
            if (entry != NULL && (entry->d_type != DT_DIR)){
                fileName.push_back(entry->d_name);
                cout << entry->d_name;
                cout << "\n";
            }
        } while (entry != NULL);

        Mat loadimage;
        Mat bininv_img;
        Mat grayscale;
        vector<string>::iterator itr2;

        for(itr2 = fileName.begin(); itr2 != fileName.end(); ++itr2) {
            loadimage = imread(dataset_path+(*itr)+"/"+(*itr2));
            grayscale = loadimage.clone();
            cvtColor( grayscale, grayscale, CV_RGB2GRAY );
            threshold(grayscale, bininv_img, 0, 255, THRESH_BINARY_INV|THRESH_OTSU);
            resize(bininv_img,bininv_img,Size(256,256),0,0);
            GaussianBlur(bininv_img, bininv_img, cv::Size(11,11), 10, 10);
            resize( bininv_img, bininv_img, Size(32,32), 0, 0 );
            bininv_img = ~bininv_img;
            imwrite( writedata_path+(*itr)+"/"+(*itr2), bininv_img);
        }
    }
    return 0;
}