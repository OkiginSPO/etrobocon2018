
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
    // char ms[30];
    // sprintf(ms, "raw: %d %d %d %d", recvData[0], recvData[1], recvData[2], recvData[3]);
    // msg_f(ms, 2);

    // 例外処理
    if(sizeof(recvData) <= 1){
        isValidDijkstraData = false;
        return;
    }
    // msg_f("exeption pass", 1);

    // 先頭4文字はバリデーションデータの為、初期値を4にして5番目から取得する。
    for(int i=4; recvData[i]!='\n'; i++){
        dijkstraData.push_back(recvData[i]-'0');
    };
    // msg_f("convert pass", 1);

    // 例外処理
    size_t size = (recvData[1]-'0')*100 + (recvData[2]-'0')*10 + (recvData[3]-'0');
    if(dijkstraData.size() != size){
        isValidDijkstraData = false;
        return;
    }

}
