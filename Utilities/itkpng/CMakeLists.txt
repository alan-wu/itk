PROJECT(ITKPNG)
INCLUDE_REGULAR_EXPRESSION("^(png|itk|z).*$")

INCLUDE_DIRECTORIES(${ITKPNG_SOURCE_DIR})

# source files for png
SET(PNG_SRCS
pngget.c    pngrio.c    pngwrite.c
png.c       pngmem.c    pngrtran.c  pngtrans.c  pngwtran.c
pngerror.c  pngpread.c  pngrutil.c  pngwutil.c
pngread.c   pngset.c   pngwio.c
)

IF(WIN32)
  IF(BUILD_SHARED_LIBS)
    ADD_DEFINITIONS(-DPNG_BUILD_DLL)
    SET(PNG_NO_MODULEDEF 1)
  ELSE(BUILD_SHARED_LIBS)
    SET(PNG_STATIC 1)
  ENDIF(BUILD_SHARED_LIBS)
ENDIF(WIN32)

CONFIGURE_FILE(${ITKPNG_SOURCE_DIR}/.NoDartCoverage
               ${ITKPNG_BINARY_DIR}/.NoDartCoverage)
CONFIGURE_FILE(${ITKPNG_SOURCE_DIR}/pngDllConfig.h.in
               ${ITKPNG_BINARY_DIR}/pngDllConfig.h)

ADD_LIBRARY(itkpng ${PNG_SRCS})
TARGET_LINK_LIBRARIES(itkpng ${ITK_ZLIB_LIBRARIES})

# Apply user-defined properties to the library target.
IF(ITK_LIBRARY_PROPERTIES)
  SET_TARGET_PROPERTIES(itkpng PROPERTIES ${ITK_LIBRARY_PROPERTIES})
ENDIF(ITK_LIBRARY_PROPERTIES)

IF(NOT ITK_INSTALL_NO_LIBRARIES)
  INSTALL(TARGETS itkpng
    RUNTIME DESTINATION ${ITK_INSTALL_BIN_DIR_CM24} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${ITK_INSTALL_LIB_DIR_CM24} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${ITK_INSTALL_LIB_DIR_CM24} COMPONENT Development)
ENDIF(NOT ITK_INSTALL_NO_LIBRARIES)

IF(ITK_USE_FLAT_DIRECTORY_INSTALL)
  SET( ITK_INSTALL_INCLUDE_DIR_CM24_UTILITIES_ITKPNG ${ITK_INSTALL_INCLUDE_DIR_CM24})
ELSE(ITK_USE_FLAT_DIRECTORY_INSTALL)
  SET( ITK_INSTALL_INCLUDE_DIR_CM24_UTILITIES_ITKPNG ${ITK_INSTALL_INCLUDE_DIR_CM24}/Utilities/itkpng)
ENDIF(ITK_USE_FLAT_DIRECTORY_INSTALL)

IF(NOT ITK_INSTALL_NO_DEVELOPMENT)
  INSTALL(FILES
    ${ITKPNG_BINARY_DIR}/pngDllConfig.h
    ${ITKPNG_SOURCE_DIR}/png.h
    ${ITKPNG_SOURCE_DIR}/pngconf.h
    ${ITKPNG_SOURCE_DIR}/itk_png_mangle.h
    DESTINATION ${ITK_INSTALL_INCLUDE_DIR_CM24_UTILITIES_ITKPNG}
    COMPONENT Development)
ENDIF(NOT ITK_INSTALL_NO_DEVELOPMENT)

IF(CMAKE_COMPILER_IS_GNUCC)
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-cast-qual")
ENDIF(CMAKE_COMPILER_IS_GNUCC)
