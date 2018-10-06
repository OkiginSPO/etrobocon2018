#ifndef _BLUETOOTH_H_
#define _BLUETOOTH_H_

// 配列のサイズ(値が切れないように余分に確保)
#define DIJKSTRA_ARRAY_BYTE 200
#define BLUETOOTH_SEND_CMD 'c'

#include "common.h"
#include "BlockZone.h"
// #include <vector> はコンパイル環境によっては失敗するため、使わない。

/**
 * @brief 
 * TODO バリデーション(文字数のチェック)処理を書く。
 */
class Bluetooth
{
  private:
    // BlockZone blockZone;
    char sendCmd;
    bool isConnect;
    FILE *btFile;

  public:
    // コンストラクタ
    Bluetooth();
    ~Bluetooth();
    void Open(void);
    void Close(void);
    void SendLog(void);
    void SendCmd(char cmd);
    int ReceiveCmd(void);
    // Bluetoothから値をdijkstraData可変長配列にセット
    void fetchDijkstraData(struct GRID_XY* grid_xy);
    // dijkstraData可変長配列
    // std::vector<int8_t> dijkstraData;
    char recvData[DIJKSTRA_ARRAY_BYTE+1];
    // バリデーションの結果確認
    bool isValidDijkstraData = true;
};

#endif
