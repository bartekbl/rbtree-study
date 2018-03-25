TEMPLATE = app
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    rbtree.c \
    rbtree-dump.c \
    rbtree-verify.c \
    rbtree-remove.c \
    rbtree-insert.c

HEADERS += \
    rbtree.h \
    rbtree-dump.h \
    rbtree-verify.h \
    rbtree-internal.h
