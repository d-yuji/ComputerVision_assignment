#include "alcon.hpp"

// 出力結果を評価する
void CAlconDatabase::Evaluation( map<int, vector<string> > outputs ){

  int count = 0;
  int total = 0;
  
  // All IDs
  vector<int> ids = GetIDs();

  // 各IDごとに評価する
  for( vector<int>::iterator it=ids.begin(); it!=ids.end(); it++ ){
    // Output and Groundtruth
    vector<string> o = outputs[*it]; // My output
    vector<string> g = GetUnicodesForOneTarget( *it ); // Groundtruth
    //cout << "id=" << *it << ", g=" << stripVectorStrings(g)
    //<< ", o=" << stripVectorStrings(o) << endl;

    // Compare unicodes
    int nO = o.size();
    int nG = g.size();
    total += nG;
    //cout << "nG=" << nG << ", nO=" << nO << endl;
    
    // nG == nO, compare using nG
    // nG < nO, compare using nG
    // nG > nO, compare using nO
    if( nG <= nO ){
      for( int i=0; i<nG; i++ ){ if( g[i] == o[i] ){ count++; } }
    }
    else{ 
      for( int i=0; i<nO; i++ ){ if( g[i] == o[i] ){ count++; } }
    }
  }

  cout << "result = " << (float)count / (float)total << endl;

  
}


// 出力結果のCSVを読み込む
map<int, vector<string> > CAlconDatabase::ReadOutputs( string fn ){
  // Oepn annnotation file
  ifstream ifs( fn );
  if( !ifs ){ cerr << "Error: cannot open " << fn << endl; exit(1); }

  map<int, vector<string> > outputs;
  
  // Read a line
  string str;
  getline( ifs, str ); // Skip the first line
  while( getline( ifs, str ) ){
    vector< string > unicodes;
    string str2;
    int c = 0;
    int id;
    // Divide the line into ID and unicodes
    for( string::iterator it=str.begin(); it!=str.end(); it++ ){
      if( *it != ','  ){ str2 += *it; } // Add to str if not ','
      else{ // Put str2 if ',' appears
	if( c==0 ){ id = atoi( str2.c_str() ); 	c++; }
	else{ unicodes.push_back( str2 ); }
	str2.erase(); // reset
      }
    }
    if( c==1 ){ unicodes.push_back( str2 ); }

    // Set the groundtruth into array
    outputs.insert( make_pair( id, unicodes ) );
  }
  return outputs;
}

// 出力結果をCSVに書き出す
void CAlconDatabase::WriteOutputs( map< int, vector<string> > outs, string fn ){
  // IDs for all targets
  vector<int> idsTars = GetIDs(); // IDs for m_targets

  // Get IDs for outputs
  vector<int> idsOuts;
  for( map<int,vector<string> >::iterator it=outs.begin(); it!=outs.end(); it++ ){
    idsOuts.push_back( it->first );
  }

  // Oepn file
  ofstream ofs( fn );
  if( !ofs ){ cerr << "Error: cannot open " << fn << endl; exit(1); }
  
  // Write file
  ofs << "ID,Unicode" << endl;
  for( int i=0; i<idsTars.size(); i++ ){
    if( idsTars[i] != idsOuts[i] ){
      cerr << "Error in WriteOutputs @ " << __LINE__ << ":" << endl;
      printf("idsTars[%d]=%d, idsOuts[%d]=%d\n",i,idsTars[i],i,idsOuts[i]);
      exit(1);
    }
    int id = idsTars[i];
    cout << stripVectorStrings( outs[id] ) << endl;
    string str = to_string( idsTars[i] ) + "," + stripVectorStrings( outs[id] );
    ofs << str << endl;
  }
}


string CAlconDatabase::stripVectorStrings( vector<string> strs ){
  string strOut;
  for( vector<string>::iterator it=strs.begin(); it!=strs.end(); it++ ){
    strOut += *it;
    strOut += ",";
  }
  strOut.pop_back();
  return strOut;
}
  

Rect CAlconDatabase::GetTargetRect( int id ){
  CTarget t = GetTarget( id );
  Rect rect = t.GetRect();
  return rect;
}

Mat CAlconDatabase::GetDocumentImage( int id ){
  CTarget t = GetTarget( id ); 
  string fnDoc = m_dirData + "images/" + t.GetImgFileID() + ".jpg";
  Mat img = imread( fnDoc  );
  if( img.empty() ){ cerr << "Cannot open " << fnDoc << endl; exit(1); }
  return img;
}


Mat CAlconDatabase::GetTargetImage( int id ){
  Rect rect = GetTargetRect( id );
  Mat docImg = GetDocumentImage( id );
  Mat imgTar = Mat( docImg, rect ).clone();
  return imgTar;
}


void CAlconDatabase::PrintGround( int id ){
  
  cout << "Ground: id=" << id;
  vector<string> codes = GetUnicodesForOneTarget( id );
  for( vector<string>::iterator it=codes.begin();
       it != codes.end(); it++ ){
    cout << ", " << *it;
  }
  cout << endl;
  
}


vector<int> CAlconDatabase::GetIDs(){

  vector<int> keys;
  map<int,CTarget>::iterator it = m_targets.begin();
  map<int,CTarget>::iterator itE = m_targets.end();
  while( it != itE ){
    keys.push_back( it->first );
    it++;
  }

  return keys;
}


// Read groundtruth from an annotation file
int CAlconDatabase::SetGrounds( string fn ){

  // Oepn annnotation file
  ifstream ifs( fn );
  if( !ifs ){ cerr << "Error: cannot open " << fn << endl; exit(1); }

  // Read a line
  string str;
  getline( ifs, str ); // Skip the first line
  while( getline( ifs, str ) ){
    vector< string > unicodes;
    string str2;
    int c = 0;
    int id;
    // Divide the line into ID and unicodes
    for( string::iterator it=str.begin(); it!=str.end(); it++ ){
      if( *it != ','  ){ str2 += *it; } // Add to str if not ','
      else{ // Put str2 if ',' appears
	if( c==0 ){ id = atoi( str2.c_str() ); 	c++; }
	else{ unicodes.push_back( str2 ); }
	str2.erase(); // reset
      }
    }
    if( c==1 ){ unicodes.push_back( str2 ); }

    // Set the groundtruth into array
    m_grounds.insert( make_pair( id, unicodes ) );
  }
  return 0;
}


// Read targets from an annotation file
// Output is stored in CAlconDatabase::m_target
int CAlconDatabase::SetTargets( string fn ){

  // Oepn annnotation file
  ifstream ifs( fn);
  if( !ifs ){ cerr << "Error: cannot open " << fn << endl; exit(1); }

  // Read a line
  string str;
  getline( ifs, str ); // Skip the first line
  while( getline( ifs, str ) ){

    CTarget t; // A target
    string str2;
    int c = 0;
    // Divide the line into elements
    for( int i=0; i<(int)str.size(); i++ ){
      if( str[i] != ','  ){ str2 += str[i]; } // Add to str if not ','
      else{ // Put str2 if ',' appears
	if( c==0 ){ t.SetID( str2 ); }
	else if( c==1 ){ t.SetImgFileID( str2 ); }
	else if( c==2 ){ t.SetX( str2 ); }
	else if( c==3 ){ t.SetY( str2 ); }
	else if( c==4 ){ t.SetW( str2 ); }
	
	str2.erase(); // reset
	c++;
      }
    }
    if( c==5 ){ t.SetH( str2 ); }
    t.SetDirData( m_dirData ); // ターゲットにも格納する

    // Set the target into targets
    m_targets.insert( make_pair( t.GetID(), t ) );
  }

  return 0;
}


Mat CTarget::GetDocumentImage(){
  string fnDoc = m_dirData + "images/" + GetImgFileID() + ".jpg";
  Mat img = imread( fnDoc  );
  if( img.empty() ){ cerr << "Cannot open " << fnDoc << endl; exit(1); }
  return img;
}

Mat CTarget::GetTargetImage(){
  Rect rect = GetRect();
  Mat docImg = GetDocumentImage();
  Mat imgTar = Mat( docImg, rect ).clone();
  return imgTar;
}


