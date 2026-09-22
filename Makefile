FOLDER := hyle-bud

all := libhyle-bud
LDLIBS-libhyle-bud := -lhyle -lbud -lqmap -ljson-c
LDLIBS-libhyle-bud-Linux := -Wl,--no-as-needed -lhyle-source -Wl,--as-needed
LDLIBS-libhyle-bud-Darwin := -lhyle-source

libhyle-bud-obj-y := src/filter.o src/table.o src/picker.o src/form.o

include ../mk/include.mk

${DESTDIR}${PREFIX}/lib/pkgconfig/hyle-bud.pc: hyle-bud.pc
	install -d ${DESTDIR}${PREFIX}/lib/pkgconfig
	install -m 644 hyle-bud.pc $@

install: ${DESTDIR}${PREFIX}/lib/pkgconfig/hyle-bud.pc
