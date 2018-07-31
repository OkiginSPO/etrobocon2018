/**
 * @file Parking.cpp
 * @brief 駐車クラス
 * @author Keisuke MORI
 */

#include "Parking.h"

Parking::Parking():
    colorSensor( PORT_3 ) {
}


void Parking::runParallel()
{
    BasicWalker basicWalker;

	msg_f( "Do ParallelParking...", 0 );

    // 左に36°回転
    basicWalker.spin( basicWalker.SPIN_LEFT, 36 );
    basicWalker.reset();
    tslp_tsk( 100 );

    basicWalker.setPidWithoutTarget( 4.0, 2.0, 0.02 );
    // 60cm直進。
    basicWalker.goStraight( 50, 600 );
    basicWalker.reset();

    // 右に81°回転
    basicWalker.spin( basicWalker.SPIN_RIGHT, 81 );
    
    waitThreeTimes();
}


void Parking::runParpendicular()
{
    BasicWalker basicWalker;

	msg_f( "Do ParpendicularParking...", 0 );

    basicWalker.reset();
    basicWalker.setPidWithoutTarget( 4.0, 2.0, 0.02 );
    // 最初に進む距離。(550 = 55cm)
    basicWalker.goStraight( 50, 550 );
    basicWalker.reset();

    // 振り向く角度と向き。(90 = 90度)
    basicWalker.spin( basicWalker.SPIN_LEFT, 90 );
    basicWalker.reset();
    tslp_tsk( 100 );

    basicWalker.setPidWithoutTarget( 1.0, 2.0, 0.02 );
    // 駐車場に向かって進む距離。(700 = 70cm)
    basicWalker.goStraight( 50, 700 );
    basicWalker.reset();

    waitThreeTimes();
}

void Parking::waitThreeTimes() {
	ev3_speaker_play_tone( NOTE_FS4, 1 );
    tslp_tsk( 1000 );
	ev3_speaker_play_tone( NOTE_FS4, 1 );
    tslp_tsk( 1000 );
	ev3_speaker_play_tone( NOTE_FS4, 1 );
    tslp_tsk( 1000 );
	ev3_speaker_play_tone( NOTE_FS5, 1 );
}
