set(ARDUINO_CORE_API "arduino-core-api")

add_library(${ARDUINO_CORE_API} INTERFACE)
target_sources(${ARDUINO_CORE_API}
        INTERFACE
            api/CanMsg.cpp
            api/CanMsgRingbuffer.cpp
            api/Common.cpp
            api/IPAddress.cpp
            api/PluggableUSB.cpp
            api/Print.cpp
            api/Stream.cpp
            api/String.cpp
        INTERFACE FILE_SET arduino_core_headers TYPE HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
        FILES
            api/ArduinoAPI.h
            api/Binary.h
            api/CanMsg.h
            api/CanMsgRingbuffer.h
            api/Client.h
            api/Common.h
            api/Compat.h
            api/DMAPool.h
            api/HardwareCAN.h
            api/HardwareI2C.h
            api/HardwareSerial.h
            api/HardwareSPI.h
            api/Interrupts.h
            api/IPAddress.h
            api/itoa.h
            api/PluggableUSB.h
            api/Print.h
            api/Printable.h
            api/RingBuffer.h
            api/Server.h
            api/Stream.h
            api/String.h
            api/Udp.h
            api/USBAPI.h
            api/WCharacter.h
)

add_library(ARDUINO::CORE_API ALIAS ${ARDUINO_CORE_API})
