#ifndef _BLUETOOTH_TEST_H_
#define _BLUETOOTH_TEST_H_

#include "common.h"
#include "Bluetooth.h"
#include "BlockZone.h"

class bluetoothTest{
    public:
        // Bluetoothテスト
        void testFetchData(FILE* bt);
        void testBlockZone(FILE* bt);
    private:
        Bluetooth bluetooth;
        BlockZone blockZone;
};

# endif
