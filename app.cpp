/**
 * ファイル名: app.cpp
 *
 * 概要: 
 *
 * 注記:
 */

#include "ev3api.h"
#include "app.h"
#include "TestLineTrace.h"
#include "include/Right/BlockZone.h"
#include "include/Right/PerpendicularParking.h"

#define DEBUG
#ifdef DEBUG
#define _debug(x) (x)
#else
#define _debug(x)
#endif

/* Bluetooth */
static int32_t bt_cmd = 0; // Bluetooth コマンド 1:リモートスタート
static FILE *bt = NULL; // Bluetooth ファイルハンドル
static TestLineTrace *lineTrace;
static BlockZone *blockZone;
static PerpendicularParking *perpendicularParking;

void tracer_cyc(intptr_t unused) {
    act_tsk(TRACER_TASK);
}

void tracer_task(intptr_t unused) {
    if (lineTrace->Terminated()) {
        wup_tsk(MAIN_TASK);
    } else {
        lineTrace->TestRun();
    }
    ext_tsk();
}

void block_cyc(intptr_t unused) {
    act_tsk(BLOCK_TASK);
}

void block_task(intptr_t unused) {

    switch (blockZone->GetBlockZoneState()) {
        case BEFORE:
            blockZone->MovingStartPosition();
            break;
        case INITIALIZE:
            blockZone->MoveBlockZoneInit();
            break;
        case BLOCKZONE:
            blockZone->MoveBlockZone();
            break;
        case AFTER:
            wup_tsk(MAIN_TASK);
            break;
    }
}

/* メインタスク */
void main_task(intptr_t unused) {
//    lineTrace = new TestLineTrace();
//    lineTrace->Initialize();
//
//    tslp_tsk(100);
//
//    lineTrace->WaitForStart();
//
//    ev3_sta_cyc(TRACER_CYC);
//
//    slp_tsk();
//
//    ev3_stp_cyc(TRACER_CYC);

    // ブロック並べ
    blockZone = new BlockZone();
    //        blockZone->start();
    tslp_tsk(100);
    ev3_sta_cyc(BLOCK_CYC);
    slp_tsk();
    ev3_stp_cyc(BLOCK_CYC);

    //    // 直角駐車場
    perpendicularParking = new PerpendicularParking();
    perpendicularParking->runParking();
    slp_tsk();

    /* Open Bluetooth file */
    //  bt = ev3_serial_open_file(EV3_SERIAL_BT);
    //  assert(bt != NULL);

    /* Bluetooth 通信タスクの軌道 */
    //  act_tsk(BT_TASK);

    //  ter_tsk(BT_TASK);
    //  fclose(bt);

    ext_tsk();
}

/*
 * 関数名: bt_task
 * 引数: unused
 * 返り値: なし
 * 概要: Bluetooth 通信によるリモートスタート。ASCIIコードで 1 を受信するとリモートスタートする。
 */
void bt_task(intptr_t unused) {
    while (1) {
        uint8_t c = fgetc(bt); // 受信
        switch (c) {
            case '1':
                bt_cmd = 1;
                break;
            default:
                break;
        }
        fputc(c, bt); // エコーバック
    }
}

