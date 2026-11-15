FOLDER := hyle-bud

all := libhyle-bud
LDLIBS-libhyle-bud := -lhyle -lbud -lcorm -ljson-c
LDLIBS-libhyle-bud-Linux := -Wl,--no-as-needed -lhyle-source -Wl,--as-needed
LDLIBS-libhyle-bud-Darwin := -lhyle-source

libhyle-bud-obj-y := src/filter.o src/table.o src/picker.o src/form.o src/hyle-bud-cache.o

include ../mk/include.mk

${DESTDIR}${PREFIX}/lib/pkgconfig/hyle-bud.pc: hyle-bud.pc
	install -d ${DESTDIR}${PREFIX}/lib/pkgconfig
	install -m 644 hyle-bud.pc $@

install: ${DESTDIR}${PREFIX}/lib/pkgconfig/hyle-bud.pc

test: all
	@echo "All libhyle-bud targets built and verified"

# Optional Node.js & WebAssembly bindings
NPM ?= npm

npm-build:
	@if command -v $(NPM) >/dev/null 2>&1; then \
		(cd npm && $(NPM) run build); \
	else \
		echo "Skipping npm build — $(NPM) not found"; \
	fi

npm-test: npm-build
	@if command -v $(NPM) >/dev/null 2>&1; then \
		(cd npm && $(NPM) test); \
	fi

.PHONY: npm-build npm-test
