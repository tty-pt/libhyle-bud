FOLDER := hyle-bud

all := libhyle-bud

EXTRA_CFLAGS += -I../hyle/include -I../hyle-source/include
EXTRA_CFLAGS += $(shell if [ -d ../bud/include ]; then echo "-I../bud/include"; else pkg-config --cflags bud 2>/dev/null; fi)
EXTRA_CFLAGS += $(shell if [ -d ../libqmap/include ]; then echo "-I../libqmap/include"; else pkg-config --cflags qmap 2>/dev/null; fi)

LDLIBS-libhyle-bud := -lhyle -lbud -lqmap -ljson-c -Wl,--no-as-needed -lhyle-source -Wl,--as-needed

libhyle-bud-obj-y := src/filter.o src/table.o src/picker.o src/form.o

BUD_LIB_DIR := $(shell if [ -d ../bud/lib ]; then cd ../bud/lib && pwd; fi)
HYLE_SOURCE_LIB_DIR := $(shell if [ -d ../hyle-source/lib ]; then cd ../hyle-source/lib && pwd; fi)

LDFLAGS-libhyle-bud := -L../hyle/lib
ifneq ($(HYLE_SOURCE_LIB_DIR),)
LDFLAGS-libhyle-bud += -L$(HYLE_SOURCE_LIB_DIR) -Wl,-rpath,$(HYLE_SOURCE_LIB_DIR)
endif
ifneq ($(BUD_LIB_DIR),)
LDFLAGS-libhyle-bud += -L$(BUD_LIB_DIR) -Wl,-rpath,$(BUD_LIB_DIR)
endif
LDFLAGS-libhyle-bud += $(shell if [ -d ../libqmap/lib ]; then echo "-L../libqmap/lib"; else pkg-config --libs-only-L qmap 2>/dev/null; fi)

include ../mk/include.mk

${DESTDIR}${PREFIX}/lib/pkgconfig/hyle-bud.pc: hyle-bud.pc
	install -d ${DESTDIR}${PREFIX}/lib/pkgconfig
	install -m 644 hyle-bud.pc $@

install: ${DESTDIR}${PREFIX}/lib/pkgconfig/hyle-bud.pc
