Add to you project's CMakeLists.txt (as an example, let's add the **quasyncserialport** library):

```cmake
cmake_path(SET QUDEVICE_DIR "") # path to builded library folder
```
- for linking shared library
```cmake
add_library(quasyncserialport SHARED IMPORTED)
set_property(TARGET quasyncserialport PROPERTY
    IMPORTED_LOCATION   ${QUDEVICE_DIR}/bin/libquasyncserialport.dll
    IMPORTED_IMPLIB     ${QUDEVICE_DIR}/lib/libquasyncserialport.dll.a
)
target_compile_definitions(${PROJECT} PRIVATE
    QUDEVICE_SHARED
)
target_include_directories(${PROJECT} PRIVATE
    ${QUDEVICE_DIR}/include
    ${QUDEVICE_DIR}/include/quasyncserialport
)
target_link_directories(${PROJECT} PRIVATE
    ${QUDEVICE_DIR}/bin
)
target_link_libraries(${PROJECT} PRIVATE
    libquasyncserialport.dll
)
```

- for linking static library
```cmake
add_library(quasyncserialport STATIC IMPORTED)
target_include_directories(${PROJECT} PRIVATE
    ${QUDEVICE_DIR}/include
    ${QUDEVICE_DIR}/include/quasyncserialport
)
target_link_directories(${PROJECT} PRIVATE
    ${QUDEVICE_DIR}/lib
)
target_link_libraries(${PROJECT} PRIVATE
    libquasyncserialport.a
    # static libraries must come before shared
)
```

In main.cpp
```cpp
#include "quasyncserialport.h"
#include "quasyncserialportconfiguration.h"
#include "quasyncserialportwidget.h"

QSharedPointer<QUAsyncSerialPort> device = QSharedPointer<QUAsyncSerialPort>::create();
QSharedPointer<QUAsyncSerialPortConfiguration> configuration = QSharedPointer<QUAsyncSerialPortConfiguration>::create();
QSharedPointer<QUAsyncSerialPortWidget> widget = QSharedPointer<QUAsyncSerialPortWidget>::create();

device->setConfiguration(configuration);
widget->setConfiguration(configuration);

connect(device.get(), &QUAsyncSerialPort::outputBytes, this, 
    [](QByteArray data) { qDebug() << data; });

widget->show();

device->acquire();
device->write("Hello!");
device->release();
```