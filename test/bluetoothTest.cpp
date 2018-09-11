#include "bluetoothTest.h"


void bluetoothTest::test(FILE* bt)
{
    char title[] = "bluetooth test";
    init_f(title);

    tslp_tsk( 2000 );

    bluetooth.fetchDijkstraData(bt);

    char msg[22];
    sprintf(msg,"ds: %d %d %d %d"
    , bluetooth.dijkstraData[0], bluetooth.dijkstraData[1], bluetooth.dijkstraData[2], bluetooth.dijkstraData[3]);
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
