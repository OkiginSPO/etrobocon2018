#include "ParallelParking.h"

ParallelParking::ParallelParking():
    colorSensor( PORT_3 ) {
}

void ParallelParking::runParking()
{
    BasicWalker basicWalker;

    // 左に36°回転
    basicWalker.spin( basicWalker.SPIN_LEFT, 50 );
    basicWalker.reset();
    tslp_tsk( 100 );

    basicWalker.setPidWithoutTarget( 4.0, 2.0, 0.02 );
    // 550mm直進。
    basicWalker.goStraight( 50, 550 );
    basicWalker.reset();

    // 右に81°回転
    basicWalker.spin( basicWalker.SPIN_RIGHT, 70 );
    
    // 3秒待つ。 
    tslp_tsk( 3000 );
}
