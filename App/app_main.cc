//
// Created by 35691 on 6/30/2023.
//
#include "app_main.h"
#include "TurtleFlight.h"




void app_main() {
#ifdef LAUNCH_TEST
    turtle::app().testAll();
#else
    turtle::app().run();
#endif
}

void launch_app() {
    app_main();
}
