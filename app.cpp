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

#define DEBUG
#ifdef DEBUG
#define _debug(x) (x)
#else
#define _debug(x)
#endif

/* Bluetooth */
static int32_t bt_cmd = 0;  // Bluetooth コマンド 1:リモートスタート
static FILE *bt = NULL; // Bluetooth ファイルハンドル
static TestLineTrace *lineTrace;

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

/* メインタスク */
void main_task(intptr_t unused)
{
    lineTrace = new TestLineTrace();
    lineTrace->Initialize();
    
    tslp_tsk(100);
    
    lineTrace->WaitForStart();
    
    ev3_sta_cyc(TRACER_CYC);
    
    slp_tsk();
    
    ev3_stp_cyc(TRACER_CYC);

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
void bt_task(intptr_t unused)
{
  while(1)
  {
    uint8_t c = fgetc(bt);  // 受信
    switch(c)
    {
      case '1':
        bt_cmd = 1;
        break;
      default:
        break;
    }
    fputc(c, bt); // エコーバック
  }
}

