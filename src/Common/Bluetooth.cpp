#include "Bluetooth.h"
#include "Logger.h"
#include "stdlib.h"
#include "LCDController.h"

// コンストラクタ
Bluetooth::Bluetooth()
{
    btFile = NULL;
    sendCmd = BLUETOOTH_SEND_CMD;
}

Bluetooth::~Bluetooth()
{

}

void Bluetooth::Open()
{
    btFile = ev3_serial_open_file(EV3_SERIAL_BT);
}

void Bluetooth::Close()
{
    fclose(btFile);
    btFile = NULL;
}

void Bluetooth::SendLog(void)
{
    LOGPARAM *log = Logger::GetLog();
    fwrite(log, sizeof(*log), 1, btFile);
}

void Bluetooth::SendCmd(char cmd)
{
    fwrite(&cmd, sizeof(cmd), 1, btFile);
}

int Bluetooth::ReceiveCmd(void)
{
    int receiveCmd = fgetc(btFile);
    
    return receiveCmd;
}


/**
 * Bluetoothの仮想シリアルにリクエストを送り、ブロック並べ計算データを受け取り、grid_xyに格納。
 * 
 * @param bt 
 * @return void
 */
void Bluetooth::fetchDijkstraData(struct GRID_XY* grid_xy)
{

    // char ms[30]; //test用の配列

    // msg_f("request", 1);
//    fwrite(&sendCmd, 1, 1, bt);
    SendCmd('c');

    // tslp_tsk( 100 ); スリープすると表示されなくなる。

    // 文字列を取得
    fgets(recvData, sizeof(recvData), btFile);
    // msg_f("recieve success", 1);

    // sprintf(ms, "recvData: %s %s %s", recvData[6], recvData[7], recvData[8]);
    // msg_f(ms, 2);

    // 例外処理
    if(sizeof(recvData) <= 1){
        isValidDijkstraData = false;
        return;
    }
    // msg_f("exception 1 pass", 1);

    for (int i = 5, j = 0; recvData[i] != '\n'; i = i + 4){
    // atoiにcharを渡すと上手く変換されない。
    //    int prefix1 = atoi(recvData[i]);
    //    int prefix2 = atoi((recvData[i+1]));

       int prefix1 = recvData[i] - '0';
       int prefix2 = recvData[i+1] - '0';

       int gridY = recvData[i+2] - '0';
       int gridX = recvData[i+3] - '0';
       int order = (prefix1 * 10) + prefix2; // 10の位と1の位を足し合わせる

    //    grid_xy[j++] = {gridY, gridX, order};
        grid_xy[j].gridX = gridX;
        grid_xy[j].gridY = gridY;
        grid_xy[j].order = order;
        j++;

    }
    // msg_f("convert pass", 1);

    
    // sprintf(ms, "grid_xy: %d %d %d", grid_xy[0].gridX, grid_xy[0].gridY, grid_xy[0].order);
    // msg_f(ms, 1);

    // 例外処理
    // size_t size = (recvData[1]-'0')*100 + (recvData[2]-'0')*10 + (recvData[3]-'0');
    // if(strlen(recvData) != size){

    //     isValidDijkstraData = false;
    //     return;
    // }
    // msg_f("exeption pass", 1);

    return;



// for(size_t i = 0; i < count; i+3)
// {

//     grid_xy[0]
//     i,i+1,i+2
// }

//    // 命令文字列を配列型(vector)形式に組み立てる
//    for (int i = 0; i < rootInputByBlueTooth.length(); i = i + 5) {
//        // 頭の２桁を一時格納
//        int prefix1 = atoi(rootInputByBlueTooth.substr(i - 1, 1).c_str());
//        int prefix2 = atoi(rootInputByBlueTooth.substr(i, 1).c_str());
//
//        int gridY = atoi(rootInputByBlueTooth.substr(i + 1, 1).c_str());
//        int gridX = atoi(rootInputByBlueTooth.substr(i + 2, 1).c_str());
//        int order = (prefix1 * 10) + prefix2; // 10の位と1の位を足し合わせる
//
//        target_grid[i] = {gridY, gridX, order};
//    }


    // 先頭4文字はバリデーションデータの為、初期値を4にして5番目から取得する。
    // for(int i=4; recvData[i]!='\n'; i++){
    //     dijkstraData.push_back(recvData[i]-'0');
    // };
    // msg_f("convert pass", 1);


}
