TEMPLATE = app
CONFIG += console c++20
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        http_conn.cpp \
        log.cpp \
        main.cpp \
        noactive/nonactive_conn.cpp \
        sql_connection_pool.cpp

DISTFILES += \
    web_time.pro.user

HEADERS += \
    blockqueue.h \
    http_conn.h \
    locker.h \
    log.h \
    lst_timer.h \
    noactive/lst_timer.h \
    sql_connection_pool.h \
    threadpool.h

RESOURCES += \
    noactive.qrc
