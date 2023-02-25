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
find_package_handle_standard_args(
  Marble
  DEFAULT_MSG Marble_INCLUDE_DIR Marble_LIBRARIES)
