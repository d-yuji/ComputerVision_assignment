# ComputerVision_assignment
2017 spring term Computer Vision assginment

## Language
C++

## コンパイル
* datachange.cpp
```
g++ -O2 -o dc datachange.cpp `pkg-config --cflags opencv` `pkg-config --libs opencv`
```

* テストプログラム
```
make
```

## 実行
./run 'テストデータへのパス' 1
