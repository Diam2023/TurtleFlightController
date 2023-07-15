/**
 * @author Mono
 */
#pragma once

#include "Logger.h"

namespace turtle {
    class TurtleFlight
    {
    public:
        /**
         * Def Flight
         */
        TurtleFlight();

        void run();

        void testAll();
        // void testMpu();



        void stop();

        static TurtleFlight &instance();

        // 禁止拷贝
        TurtleFlight(const TurtleFlight&) = delete;
        TurtleFlight& operator&(const TurtleFlight&) = delete;
    };



    inline TurtleFlight &app()
    {
        return TurtleFlight::instance();
    };



};

