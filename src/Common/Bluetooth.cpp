
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

    // 文字列を取得
    fgets(recvData, sizeof(recvData), bt);
    // msg_f("recieve success", 1);

    // 例外処理
    if(sizeof(recvData) <= 1){
        isValidDijkstraData = false;
        return;
    }
    // msg_f("exeption pass", 1);

    for(int i=0; recvData[i]!='\n'; i++){
        dijkstraData.push_back(recvData[i]-'0');
    };
    // msg_f("convert pass", 1);

}
