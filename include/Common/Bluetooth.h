#ifndef _BLUETOOTH_H_
#define _BLUETOOTH_H_

// 配列のサイズ(値が切れないように余分に確保)
#define DIJKSTRA_ARRAY_BYTE 200
#define BLUETOOTH_SEND_CMD 'c'

#include "common.h"

class Bluetooth
{
  public:
    // コンストラクタ
    Bluetooth();
    // Bluetoothから値をdijkstraData配列にセット
    void fetchDijkstraData(FILE* bt);
    int8_t dijkstraData[DIJKSTRA_ARRAY_BYTE];

  private:
    char sendCmd;
    char recvData[DIJKSTRA_ARRAY_BYTE+1];
};

#endif
