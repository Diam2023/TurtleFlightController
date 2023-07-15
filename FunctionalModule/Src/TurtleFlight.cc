/**
 * @author Monoliths
 */

#include "TurtleFlight.h"

namespace turtle {
    namespace tasks {

        _Noreturn void TestTask(void *tes) {

            Logger::getInstance() << (Logger::Stream<TurtleFlight, LogLevel::L_DEBUG>{} << "Test LogOutput");

            while (true) {
                osDelay(1000);

                osDelay(10);
            }

            osThreadExit();
        }
    };

    TurtleFlight &TurtleFlight::instance() {
        static TurtleFlight instance;
        return instance;
    }

    TurtleFlight::TurtleFlight() {

    }

    void TurtleFlight::run() {

    }

    void TurtleFlight::testAll() {

        const osThreadAttr_t testTask_attributes = {
                .name       = "testTask",
                .stack_size = 128 * 4,
                .priority   = (osPriority_t) osPriorityLow,
        };
        osThreadId_t pTestTaskId = osThreadNew(tasks::TestTask, nullptr, &testTask_attributes);
    }

};
