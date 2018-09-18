#include "PerpendicularParking.h"

PerpendicularParking::PerpendicularParking():
    colorSensor( PORT_3 ) {
}

void PerpendicularParking::runParking()
{
    BasicWalker basicWalker;

    basicWalker.reset();
    basicWalker.setPidWithoutTarget( 4.0, 2.0, 0.02 );
    // 最初に進む距離。(350 = 35cm)
    basicWalker.goStraight( 50, 650 );
    basicWalker.reset();

    // 振り向く角度と向き。(90 = 90度)
    basicWalker.spin( basicWalker.SPIN_LEFT, 90 );
    basicWalker.reset();
    tslp_tsk( 100 );

    basicWalker.setPidWithoutTarget( 1.0, 2.0, 0.02 );
    // 駐車場に向かって進む距離。(700 = 70cm)
    basicWalker.goStraight( 50, 700 );
    basicWalker.reset();

    // 3秒待つ。 
    tslp_tsk( 3000 );
}
