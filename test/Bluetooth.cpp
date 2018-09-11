
#include "bluetooth.h"

// コンストラクタ
Bluetooth::Bluetooth()
{
  sendCmd = BLUETOOTH_SEND_CMD;
}


void Bluetooth::test(FILE* bt)
{
    //XXX 削除しても良い
    char title[] = "bluetooth test";
    init_f(title);

    tslp_tsk( 2000 );

    fetchDijkstraData(bt);

    char msg[22];
    sprintf(msg,"ds: %d %d %d %d"
    , dijkstraData[0], dijkstraData[1], dijkstraData[2], dijkstraData[3]);
    msg_f(msg,5);

    // char subject[] = "msg_f test";
    // init_f(subject);
    // msg_f(msg, 2);

    // msg_f(dijkstraData, 1);

    // msg_f(dijkstraData, 1);

    // char op = (runData[1] + runData[2]) + '0';
    // char sum[] = {op, '\0'};
    // snprintf(buf, 12, "%d", i); フォーマットで変換する。
    // msg_f(sum, 2);
    // msg_f(&recv[0], 1); 0番目の値が表示される。
    // msg_f(recv, 1);
}

/**
 * Bluetoothの仮想シリアルにリクエストを送り、ブロック並べ計算データを受け取り、8ビット配列に変換する。
 * 
 * @param bt 
 * @return const char* 
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
