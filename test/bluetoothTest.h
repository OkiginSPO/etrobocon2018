#ifndef _BLUETOOTH_TEST_H_
#define _BLUETOOTH_TEST_H_

#include "common.h"
#include "Bluetooth.h"

class bluetoothTest{
    public:
        // Bluetoothテスト
        void test(FILE* bt);
    private:
        Bluetooth bluetooth;
};

# endif
