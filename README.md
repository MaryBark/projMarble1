# projMarble1

Инструкция по сборке marble от Александра Басаргина (за что ему огромнейшее спасибо))))) )
# Сборка Marble
Выполнялась в Manjaro Linux (Arch-Linux) Linux rd06 5.15.85-1-MANJARO (rolling).
Сборка выполнялась из master-а репозитория marble с github в домашний каталог пользователя, чтобы не использовать sudo и не устанавливать библиотеку и toolset в системе. Выход собранного продукта направлялся в одну из домашних папок (см. install prefix).
bash
# качаем репозиторий
git clone https://github.com/KDE/marble.git
cd marble

# создаём директорию, в которой будут копиться временные и build-файлы
mkdir build && cd build

# запускаем в ней генератор, который проверяет, что все нужные пакеты в системе присутствуют
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$HOME/3rd_party/marble-product ..

#-- The following REQUIRED packages have been found:
# * Qt5Core
# * Qt5Xml
# * Qt5Network
# * Qt5Test
# * Qt5Gui (required version >= 5.15.7)
# * Qt5Widgets
# * Qt5Svg
# * Qt5Sql
# * Qt5Concurrent
# * Qt5Qml (required version >= 5.15.7)
# * Qt5QmlModels (required version >= 5.15.7)
# * Qt5Quick
# * Qt5PrintSupport
# * Qt5 (required version >= 5.14.0)
#
#-- The following OPTIONAL packages have not been found:
# * Qt5WebChannel (required version >= 5.14.0)
# * Qt5WebEngine (required version >= 5.14.0)
# * Qt5WebEngineWidgets (required version >= 5.14.0)
# * Phonon4Qt5, Qt-based audio library, <https://phonon.kde.org/>
#   Support for playback of soundcue elements
#   Voice navigation (sound or human speakers)
# * Qt5SerialPort, Provides access to hardware and virtual serial ports.
#   Reading from serial port in APRS plugin
# * libgps, communicating with the GPS daemon, <https://gpsd.gitlab.io/gpsd/index.html>
#   position information via gpsd
# * Qt5Positioning, a collection of APIs and frameworks, <https://www.qt.io/developers/>
#   position information via Qt5Positioning
# * libwlocate, WLAN-based geolocation, <https://sourceforge.net/p/libwlocate/wiki/Home/>
#   Position information based on neighboring WLAN networks
# * libshp, reading and writing of ESRI Shapefiles (.shp), <http://shapelib.maptools.org/>
#   reading and displaying .shp files
# * ECM (required version >= 1.7.0)

# сборка выполнялась комиплятором /usr/bin/cc --version
# cc (GCC) 12.2.0

# сборка с использованием всех ядрер CPU, т.к. это оч.долгий процесс
cmake --build . --parallel $(nproc)
В результате будет собраны библиотеки и toolset с программами (какие именно и зачем они нужны не разбирался). Чтобы они оказались в директории, которая была указана в самом начале при запуске генератора, используем:
bash
# устанавливаем результаты сборки
make install

# Install the project...
# -- Install configuration: "Release"
# -- Installing: /home/user/3rd_party/marble-product/...

# смотрим что получилось:
cd ~/3rd_party/parble-product
ls -1

# bin
# include
# lib
# mkspecs
# share

ls -1 lib/*.so

# lib/libastro.so
# lib/libmarbledeclarative.so
# lib/libmarblewidget-qt5.so
Обращаю внимание на то, что выполняется именно make install, а не sudo make install, поскольку собранные файлы будут скопированы в пользовательскую папку.
# Собираем свой проект
С этого момента в системе marble есть, но никакой find_package(marble) его не найдёт, потому что он лежит не в стандартных /usr/lib и /usr/include директориях, а посторонней пользовательской папке. Для того, чтобы все нужные нам для библиотеки были найдены, в системе (cmake) должен быть соответствующий FindMarble.cmake файл. То есть, проверяем его наличие:
bash
ls /usr/share/cmake/Modules/FindM*.cmake -1
# /usr/share/cmake/Modules/FindMatlab.cmake
# /usr/share/cmake/Modules/FindMFC.cmake
# /usr/share/cmake/Modules/FindMotif.cmake
# /usr/share/cmake/Modules/FindMPEG2.cmake
# /usr/share/cmake/Modules/FindMPEG.cmake
# /usr/share/cmake/Modules/FindMPI.cmake
# /usr/share/cmake/Modules/FindMsys.cmake
И видим только список cmake-файлов на букву M среди которых marble нет. Поэтому придётся сделать свой собственный FindXXX.cmake файл, чтобы его смог использовать cmake. Это типовая практика для тех библиотек, о которых cmake ничего не знает, т.е. в штатную поставку нужны find-модуль не вошёл. Это означает, что пользоваться find_package() функций в cmake не получится.
Поэтому пишем свой Find Module файл для marble (см. его листинг ниже). Обращаю внимание, что FindMarble.cmake пишется лишь в тех случаях, когда нужного одноимённого нет, либо версии ОС/библиотеки/cmake сильно расходятся, к примеру вы пытаетесь собрать последнюю версию postgresql в старой версии astra linux, в которую установлен старый cmake, который в свою очередь ничего не знает о том, что какие-то библиотеки из состава нового postgresql были перемещены в другие директории, или переименованы.
cmake
# standard FindMarble.cmake

if (MARBLE_INCLUDE_DIR AND MARBLE_LIBRARIES)
   # уже в кеше, отключаем выод отладки
   SET( MARBLE_FIND_QUIETLY TRUE )
endif()

# подключаем стандартный cmake-модуль, который штатно ищет файлы библиотеки
# для его использования необходимо при запуске cmake указать xxx_ROOT, чтобы
# он знал где искать
include( FindPackageHandleStandardArgs )

# указываем, где следует искать файлы библиотеки
set(Marble_PATHS
  ${Marble_ROOT}/
  ${MARBLE_ROOT}/
  $ENV{HOME}/marble
)
# проверяем, что в директории marble есть MarbleModel.h
find_path( Marble_INCLUDE_DIR marble/MarbleModel.h PATHS ${Marble_PATHS} PATH_SUFFIXES "include")
# указываем библиотеку, котораю должна быть найдена среди директорий, куда был собран и инсталлирован продукт
find_library(Marble_LIBRARIES NAMES marblewidget-qt5 PATHS ${Marble_PATHS} PATH_SUFFIXES "lib")

# вызываем макрос из FindPackageHandleStandardArgs
find_package_handle_standard_args( Marble DEFAULT_MSG Marble_INCLUDE_DIR Marble_LIBRARIES )
Наш проект исключительно простой, и состоит из main.cpp, CMakeLists.txt и FindMarble.cmake:
Приведу листинг CMakeLists.txt:
cmake
# стандартная шапка
cmake_minimum_required ( VERSION 3.10.2 )
project(my_project)

# указываем в нашем проекте где лежат дополнительные .cmake файлы
LIST(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" "${PROJECT_SOURCE_DIR}/import/cmake")

# выполняем поиск marble-библиотеки, см. выше файл FindMarble.cmake
find_package(Marble)
if (Marble_FOUND)
  message( STATUS "Marble includes: ${Marble_INCLUDE_DIR}")
  message( STATUS "Marble libraries: ${Marble_LIBRARIES}")
else()
  message(WARNING "Not found Marble!")
endif()

# настраиваем includes
include_directories("${Marble_INCLUDE_DIR}")  # заголовочные файлы marble-библиотеки
# создаём приложение
add_executable(my_application main.cpp)
# подключаем к приложению библиотеку marble
target_link_libraries(my_application ${Marble_LIBRARIES})
Приведу листринг main.cpp (просто файл заглушка, на котором упадёт компилятор, что будет свидетельствовать, что все нуджные зависимости найдены):
cpp
#include <iostream>
#include <marble/MarbleModel.h> // упадёт на #include <QObject> <--- значит includes библиотеки найдены

int main()
{
    return 0;
}
Теперь чтобы всё собралось, надо при сборке проекта указать где лежит marble (он внешний по отношении ку нему):
bash
cmake -DCMAKE_BUILD_TYPE=Release -DMarble_ROOT=$HOME/3rd_party/marble-product ..
# -- Marble includes: /home/user/3rd_party/marble-product/include
# -- Marble libraries: /home/user/3rd_party/marble-product/lib/libmarblewidget-qt5.so
# -- Configuring done
# -- Generating done

cmake --build .

# чтобы собрать нужно из папки cmake в командной строке запустить
cmake -DCMAKE_BUILD_TYPE=Release -DMarble_ROOT=$HOME/3rd_party/marble-product ..

cmake --build .
