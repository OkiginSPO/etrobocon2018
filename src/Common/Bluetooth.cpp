
#include "bluetooth.h"

// コンストラクタ
Bluetooth::Bluetooth()
{
  sendCmd = BLUETOOTH_SEND_CMD;
}

/**
 * Bluetoothの仮想シリアルにリクエストを送り、ブロック並べ計算データを受け取り、数値に変換してdijkstraDataメンバ変数に格納。
 * 
 * @param bt 
 * @return void
 */
void Bluetooth::fetchDijkstraData(FILE* bt)
{

    // msg_f("request", 1);
    fwrite(&sendCmd, 1, 1, bt);

    // tslp_tsk( 100 ); スリープすると表示されなくなる。

    // msg_f("response", 2);
    // 文字列を取得
    fgets(recvData, sizeof(recvData), bt);

    // msg_f("response success", 3);
    // msg_f(recvData, 1);
    // std::array<short, arrayBytes> runData;
    for(int i=0; i<(int)sizeof(recvData); i++){
        dijkstraData[i] = recvData[i]-'0';
    };

}
