#include "bluetoothTest.h"


void bluetoothTest::testFetchData(FILE* bt)
{
    char title[] = "bluetooth test";
    init_f(title);

    tslp_tsk( 2000 );

    bluetooth.fetchDijkstraData(bt);

    char msg[30];
    sprintf(msg, "valid: %s", (bluetooth.isValidDijkstraData ? "true" : "false"));
    msg_f(msg, 1);

    sprintf(msg, "size: %d", bluetooth.dijkstraData.size());
    msg_f(msg,3);

    sprintf(msg
    , "first: %d %d %d %d"
    , bluetooth.dijkstraData[0]
    , bluetooth.dijkstraData[1]
    , bluetooth.dijkstraData[2]
    , bluetooth.dijkstraData[3]);
    msg_f(msg,5);

    sprintf(msg
    , "last: %d %d %d %d"
    , bluetooth.dijkstraData[bluetooth.dijkstraData.size()-4]
    , bluetooth.dijkstraData[bluetooth.dijkstraData.size()-3]
    , bluetooth.dijkstraData[bluetooth.dijkstraData.size()-2]
    , bluetooth.dijkstraData[bluetooth.dijkstraData.size()-1]);
    msg_f(msg,7);

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

void bluetoothTest::testBlockZone(FILE* bt)
{
  msg_f("prepareBluetooth",1);
  
  struct GRID_XY* grid_xy;
  blockZone.prepareMoveData(bt, grid_xy);

  msg_f("start",2);
  tslp_tsk( 2000 ); // 表示されているか確認する為、一時停止
  blockZone.start();
}
