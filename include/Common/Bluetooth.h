#ifndef _BLUETOOTH_H_
#define _BLUETOOTH_H_

// 配列のサイズ(値が切れないように余分に確保)
#define DIJKSTRA_ARRAY_BYTE 200
#define BLUETOOTH_SEND_CMD 'c'

#include "common.h"
#include <vector>

/**
 * @brief 
 * TODO バリデーション(文字数のチェック)処理を書く。
 */
class Bluetooth
{
  public:
    // コンストラクタ
    Bluetooth();
    // Bluetoothから値をdijkstraData可変長配列にセット
    void fetchDijkstraData(FILE* bt);
    // dijkstraData可変長配列
    std::vector<int8_t> dijkstraData;
    // バリデーションの結果確認
    bool isValidDijkstraData = true;

  private:
    char sendCmd;
    char recvData[DIJKSTRA_ARRAY_BYTE+1];
};

#endif
