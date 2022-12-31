#ifndef WEBSERIAL_HPP
#define WEBSERIAL_HPP

#define USE_WEBSERIAL 0
#if USE_WEBSERIAL
#include <WebSerial.h>
#define MySerial WebSerial
#else
#define MySerial Serial
#endif

#endif // !WEBSERIAL_HPP