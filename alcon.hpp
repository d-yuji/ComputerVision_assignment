#ifndef __ALCON_H_INCLUDED
#define __ALCON_H_INCLUDED

#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <map>
#include <vector>

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// ----------
// One target
// ----------
class CTarget{
private:
  int m_id; // ターゲットID
  string m_img_file_id; // 文書画像のID
  int m_x; // 矩形の左上のX座標
  int m_y; // Y 
  int m_w; // Width
  int m_h; // Height
  string m_dirData; // データセットへのパス
  
public:
  
  // setter
  void SetID( string str ){ m_id = atoi( str.c_str() ); }
  void SetImgFileID( string str ){ m_img_file_id = str; }
  void SetX( string str ){ m_x = atoi( str.c_str() ); }
  void SetY( string str ){ m_y = atoi( str.c_str() ); }
  void SetW( string str ){ m_w = atoi( str.c_str() ); }
  void SetH( string str ){ m_h = atoi( str.c_str() ); }
  void SetDirData( string dirData ){ m_dirData = dirData; }

  // getter
  int GetID(){ return m_id; } 
  string GetImgFileID(){ return m_img_file_id; }
  Rect GetRect(){ return Rect(m_x, m_y, m_w, m_h); } // ターゲットの矩形を取得する

  // Image
  Mat GetTargetImage(); // ターゲット画像を取得する
  Mat GetDocumentImage(); // ターゲットが含まれている文書画像を取得する

  // Print
  void PrintID(){ printf("m_id = %d\n", m_id ); }
  void PrintImgFileID(){ cout << "m_img_file_id = " << m_img_file_id << endl; }
  void PrintX(){ printf("m_x = %d\n", m_x ); }
  void PrintY(){ printf("m_y = %d\n", m_y ); }
  void PrintW(){ printf("m_w = %d\n", m_w ); }
  void PrintH(){ printf("m_h = %d\n", m_h ); }
  void PrintMembers(){
    cout << "The target:" << endl;
    PrintID();
    PrintImgFileID();
    PrintX();
    PrintY();
    PrintW();
    PrintH();
    cout << "m_dirData = " << m_dirData << endl;
  }
  
}; // CTarget


// ---------------
// Alconn Database
// ---------------
class CAlconDatabase{

private:
  string m_dirData; // path to the dataset
  string m_fnTarget; // target filename
  string m_fnGround; // groundtruth filename
  map< int, CTarget > m_targets; // Targets
  map< int, vector<string> > m_grounds; // Groundtruth
  
public:
  // Constructor
  CAlconDatabase( string dirData, string fnTarget, string fnGround ){
    m_dirData = dirData;
    m_fnTarget = fnTarget;
    m_fnGround = fnGround;
    
    SetTargets( m_fnTarget );
    SetGrounds( m_fnGround );
  }
  
  // Setter
  int SetTargets( string fn );
  int SetGrounds( string fn );

  // Getter
  CTarget GetTarget( int id ){ return m_targets.at( id ); } // One target
  vector<string> GetUnicodesForOneTarget( int id ){ return m_grounds.at( id ); }
  vector<int> GetIDs(); // Get IDs for all targets

  // Print
  void PrintGround( int id );

  // Image and Others
  Mat GetTargetImage( int id );
  Mat GetDocumentImage( int id );
  Rect GetTargetRect( int id );

  // Evaluation
  void WriteOutputs( map< int, vector<string> > outs, string fn );
  map< int, vector<string> > ReadOutputs( string fn );
  void Evaluation( map< int, vector<string> > outs );

  // Utilities
  string stripVectorStrings( vector<string> outs );
}; // CAlconDatabase


#endif
