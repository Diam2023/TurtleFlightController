//
// Created by 35691 on 6/30/2023.
//
#pragma once

#include "stm32f1xx.h"
#include "cmsis_os2.h"
#include <ostream>
#include <sstream>

namespace turtle {


    struct LogMessage {
        static const size_t MessageLength = 64;
        const char *m_pMessage;
    };

    /**
     * @brief Log level
     *
     * @version 0.1
     * @author Monoliths (monoliths-uni@outlook.com)
     * @date 2023-03-15
     * @copyright Copyright (c) 2023
     */
    enum class LogLevel {
        L_INFO,
        L_DEBUG,
        L_WARNING,
        L_ERROR
    };


    class Logger {
    private:
        // pointer for log output device
        UART_HandleTypeDef *uart = nullptr;

        osMessageQueueId_t messageQueue = nullptr;

        osThreadId_t thread = nullptr;

        // Temp
        char *tempMessage = nullptr;
    public:
        template<class T, enum LogLevel L>
        class Stream
        {
        protected:
            LogMessage msg;

            std::string tempString;
        public:
            Stream() = default;

            friend std::ostream &operator<<(std::ostream& os, const Stream& stream)
            {
                switch (L) {
                    case LogLevel::L_INFO:
                        os << "I";
                        break;
                    case LogLevel::L_DEBUG:
                        os << "D";
                        break;
                    case LogLevel::L_WARNING:
                        os << "W";
                        break;
                    case LogLevel::L_ERROR:
                        os << "E";
                        break;
                    default:
                        os << "K";
                        break;
                }

                os << "[" << typeid(T).name() << "]: ";
                return os;
            }

            LogMessage& operator<<(const std::string& string)
            {
                tempString = string;

                std::stringstream ss;
                ss << this << tempString;
                std::string str(ss.str());
                msg.m_pMessage = str.c_str();

                return msg;
            }

            LogMessage& getMsg()
            {
                std::stringstream ss;
                ss << this << tempString;
                std::string str(ss.str());
                msg.m_pMessage = str.c_str();
                return msg;
            }
        };

        char *makeTempMessage(const char *message);

        void releaseTempMessage();

//        friend std::ostream &operator<<(std::ostream &os, LogLevel &level);

//        template<class T, enum LogLevel L>
//        void operator<<(const Stream<T, L>& s);

        void operator<<(LogMessage& l);

//        friend void operator<<(Logger &logger, std::string &data);

        // Instance
        static Logger &getInstance();

        // TODO Add Argument Fix
        Logger();

        ~Logger();


        [[nodiscard]] UART_HandleTypeDef *getUart() const;

        void setUart(UART_HandleTypeDef *uart);

        [[nodiscard]] void *getMessageQueue() const;

        void setMessageQueue(void *messageQueue);

        [[nodiscard]] void *getThread() const;

        void setThread(void *thread);
    };


};






