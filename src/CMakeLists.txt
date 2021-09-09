set(GDAL_HSS_SOURCES
    entrypoint.cpp
    hss/ogrhssdriver.cpp
    hss/ogrhssdatasource.cpp
    hss/ogrhsslayer.cpp
)

add_library(gdal_hss SHARED ${GDAL_HSS_SOURCES})

set_target_properties(gdal_hss PROPERTIES PREFIX "")
set_target_properties(gdal_hss PROPERTIES OUTPUT_NAME "gdal_hss")

target_include_directories(gdal_hss PRIVATE ${CMAKE_SOURCE_DIR}/src ${GDAL_INCLUDE_DIR})
target_link_libraries(gdal_hss PUBLIC ${GDAL_LIBRARY})

install(TARGETS gdal_hss DESTINATION lib/gdalplugins)
