cmake_minimum_required(VERSION 3.4.1)
include_directories(crypto curve25519 ed25519 playfair llhttp plist/plist
        fdk-aac/libAACdec/include
        fdk-aac/libAACenc/include
        fdk-aac/libFDK/include
        fdk-aac/libMpegTPDec/include
        fdk-aac/libMpegTPEnc/include
        fdk-aac/libPCMutils/include
        fdk-aac/libSBRdec/include
        fdk-aac/libSBRenc/include
        fdk-aac/libSYS/include
        #fdk-aac/libArithCoding/include
        #fdk-aac/libDRCdec/include
        #fdk-aac/libSACdec/include
        #fdk-aac/libSACenc/include
        )
#include_directories(../openssl/arm64-v8a/include)
include_directories(../openssl/${ANDROID_ABI}/include)
include_directories(./plist)

message("enter top level")
aux_source_directory(. play_src)
set(DIR_SRCS ${play_src})
set(openssl_lib_path ${CMAKE_CURRENT_SOURCE_DIR}/../openssl/${ANDROID_ABI}/lib)
add_library( crypto2 SHARED IMPORTED )
set_target_properties( crypto2 PROPERTIES IMPORTED_LOCATION ${openssl_lib_path}/libcrypto.so )

add_library( ssl
        SHARED
        IMPORTED )

set_target_properties( ssl
        PROPERTIES IMPORTED_LOCATION
        ${openssl_lib_path}/libssl.so)
add_library( play-lib
        SHARED
        ${DIR_SRCS}
        )
find_library( log-lib
        log )
target_link_libraries( play-lib
        crypto
        curve25519
        ed25519
        playfair
        plist
        fdk-aac
        llhttp
        crypto2
        ssl
        ${log-lib})