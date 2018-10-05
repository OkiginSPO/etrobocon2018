/**
 * ファイル名: app.cpp
 *
 * 概要: 
 *
 * 注記:
 */

#include "ev3api.h"
#include "app.h"
#include "Walker_.h"
#include "ScenarioManager.h"
#include "include/Common/Bluetooth.h"
#include "include/Right/BlockZone.h"
#include "include/Right/PerpendicularParking.h"

#define DEVELOP
//#define PRODUCT
#define R_COURSE
//#define L_COURSE

#define DEBUG
#ifdef DEBUG
#define _debug(x) (x)
#else
#define _debug(x)
#endif

static Bluetooth *bluetooth;
static Walker_ *walker;
static ScenarioManager *scenarioManager;
static BlockZone *blockZone;
static PerpendicularParking *perpendicularParking;

void tracer_cyc(intptr_t unused) {
    if (walker->Terminated()) {
        wup_tsk(MAIN_TASK);
    } else {
        walker->LineTrace();
    }
    act_tsk(TRACER_TASK);
}

void tracer_task(intptr_t unused) {
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

void sendlog_cyc(intptr_t unused) {
    act_tsk(SENDLOG_TASK);
}

void sendlog_task(intptr_t exinf) {
    bluetooth->SendLog();
    ext_tsk();
}

void receivecmd_cyc(intptr_t unused) {
    act_tsk(RECEIVECMD_TASK);
}

void receivecmd_task(intptr_t exinf) {
    int receiveCmd = bluetooth->ReceiveCmd();
    
    switch(receiveCmd) {
        case 'q':
            // 強制停止
            break;
        default:
            break;
    }
    
    ext_tsk();
}

/* メインタスク */
void main_task(intptr_t unused) {
    
    bluetooth = new Bluetooth();
    bluetooth->Open();
    
    tslp_tsk(100);
    
#ifdef DEVELOP
    ev3_sta_cyc(SENDLOG_CYC);
    ev3_sta_cyc(RECEIVECMD_CYC);
#endif
    
#ifdef R_COURSE
    scenarioManager = new ScenarioManager(12);
    scenarioManager->CreateRcourse();
#endif
    
#ifdef L_COURSE
    scenarioManager = new ScenarioManager(12);
    scenarioManager->CreateLcourse();
#endif
    
    walker = new Walker_(scenarioManager);
    walker->Initialize();
    
    tslp_tsk(100);
    
    walker->WaitForStart();
    
    ev3_sta_cyc(TRACER_CYC);
    
    slp_tsk();
    
    ev3_stp_cyc(TRACER_CYC);

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
    
    Bluetooth->Close();
    
#ifdef DEVELOP
    ev3_stp_cyc(SENDLOG_CYC);
    ev3_stp_cyc(RECEIVECMD_CYC);
#endif

    ext_tsk();
}
