#include "Logger.h"

#include "usart.h"
#include "cmsis_os.h"
#include <cstring>
#include <iostream>

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *uart) {
    turtle::Logger logger = turtle::Logger::getInstance();
    if (uart == logger.getUart()) {
        HAL_UART_DMAStop(logger.getUart());
        osThreadFlagsSet(logger.getThread(), 0x01);
    }
}

namespace turtle {
    /**
     * @brief Task For Log Framwok
     *
     * @param arg
     * @version 0.1
     * @author Monoliths (monoliths-uni@outlook.com)
     * @date 2023-03-15
     * @copyright Copyright (c) 2023
     */
    _Noreturn static void LogTask(void *arg) {
        turtle::Logger logger = turtle::Logger::getInstance();
        LogMessage message{};

        for (;;) {
            osMessageQueueGet(logger.getMessageQueue(), &message, nullptr, osWaitForever);

            osThreadFlagsWait(0x01, osFlagsWaitAll, osWaitForever);

            logger.releaseTempMessage();

            // Assert
            assert_param(message.m_pMessage);

            char* heapData = logger.makeTempMessage(message.m_pMessage);
            HAL_UART_Transmit_DMA(logger.getUart(), (unsigned char *) heapData,
                                  LogMessage::MessageLength);

            osDelay(10);

            if (osThreadFlagsWait(0x0F, osFlagsWaitAll, 1) == osOK) {
                osThreadExit();
            }
        }
    }

    char* Logger::makeTempMessage(const char *message) {
        tempMessage = reinterpret_cast<char *>(pvPortMalloc(LogMessage::MessageLength));
        memmove(tempMessage, message, LogMessage::MessageLength);
        return tempMessage;
    }

    void Logger::releaseTempMessage() {
        if (tempMessage != nullptr) {
            vPortFree(tempMessage);
        }
    }


    Logger::Logger() {
#ifdef LOG_USE_UART1
        uart = &huart1;
#else
        // You Have No Define LOG Serial Port
        assert_param(false);
#endif
        const osMessageQueueAttr_t logQueueHandle_attributes = {.name = "logQueue"};
        messageQueue = osMessageQueueNew(16, sizeof(LogMessage), &logQueueHandle_attributes);

        const osThreadAttr_t logTask_attributes = {
                .name       = "logTask",
                .stack_size = 256,
                .priority   = (osPriority_t) osPriorityNormal,
        };

        thread = osThreadNew(LogTask, nullptr, &logTask_attributes);

        osThreadFlagsSet(thread, 0x01);
    }

    Logger::~Logger() {
        osMessageQueueDelete(messageQueue);
        // Exit Flag
        osThreadFlagsSet(thread, 0x0F);

        releaseTempMessage();
    }

    UART_HandleTypeDef *Logger::getUart() const {
        return uart;
    }

    void Logger::setUart(UART_HandleTypeDef *uart) {
        Logger::uart = uart;
    }

    void *Logger::getMessageQueue() const {
        return messageQueue;
    }

    void Logger::setMessageQueue(void *messageQueue) {
        Logger::messageQueue = messageQueue;
    }

    void *Logger::getThread() const {
        return thread;
    }

    void Logger::setThread(void *thread) {
        Logger::thread = thread;
    }

    Logger &Logger::getInstance() {
        static Logger logger;
        return logger;
    }

    void Logger::operator<<(LogMessage &l) {
        osMessageQueuePut(messageQueue, &l, 0, osWaitForever);

    }


//    template<class T>
//    std::ostream &operator<<(std::ostream &os, turtle::LogLevel& level) {
//        switch (level) {
//            case turtle::LogLevel::L_INFO:
//                os << "I";
//                break;
//            case turtle::LogLevel::L_DEBUG:
//                os << "D";
//                break;
//            case turtle::LogLevel::L_WARNING:
//                os << "W";
//                break;
//            case turtle::LogLevel::L_ERROR:
//                os << "E";
//                break;
//            default:
//                os << "K";
//                break;
//        }
//
//        os << "[" << typeid(T).name() << "]: ";
//        return os;
//    }

//    template<class T, enum LogLevel L>
//    void Logger::operator<<(const Logger::Stream<T, L> &s) {
//        osMessageQueuePut(messageQueue, &s.getMsg(), 0, osWaitForever);
//    }

//    void operator<<(Logger &logger, std::string& data) {
//
//        LogMessage logData{
//                .m_pMessage = data.c_str()
//        };
//
//        osMessageQueuePut(logger.messageQueue, &logData, 0, osWaitForever);
//    }


//    template<class T, enum LogLevel L>
//    std::ostream& operator<<(std::ostream& os, Logger::Stream<T, L> &s) {
//        os << s;
//        return os;
//    }

//    template<class T>
//    void operator<<(Logger &logger, LogLevel &level) {
//
//        std::ostream &os = std::cout;
//
//        switch (level) {
//            case turtle::LogLevel::L_INFO:
//                os << "I";
//                break;
//            case turtle::LogLevel::L_DEBUG:
//                os << "D";
//                break;
//            case turtle::LogLevel::L_WARNING:
//                os << "W";
//                break;
//            case turtle::LogLevel::L_ERROR:
//                os << "E";
//                break;
//            default:
//                os << "K";
//                break;
//        }
//
//        os << "[" << typeid(T).name() << "]: ";
//        return os;
//    }

};
