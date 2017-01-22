PACKAGE_NAME        := ModernFLAC
FILE                := $(CURDIR)/libModernFLAC/include/libModernFLAC.h
VERSION             := $(shell cat ${FILE} | grep -e "@version")
CC                  := cc
DESTINATION         := /usr/local/Packages/$(PACKAGE_NAME)
BUILD_DIR           := $(CURDIR)/BUILD
CFLAGS              := -std=c11 -march=native `pkg-config --libs libBitIO` `pkg-config --libs libPCM`
LDFLAGS             := -flto=thin
DEB_ERROR_OPTIONS   := -Wno-unused-parameter -Wno-unused-variable -Wno-int-conversion
REL_ERROR_OPTIONS   := -Weverything -Wunreachable-code -Wno-conversion
DEB_FLAGS           := $(CFLAGS) -openmp=libomp -g -o0 $(DEB_ERROR_OPTIONS) $(LDFLAGS)
SANITIZER           := -fsanitize=undefined -fsanitize=address
REL_FLAGS           := $(CFLAGS) -openmp=libomp -ofast $(REL_ERROR_OPTIONS) $(LDFLAGS)

.PHONY: all distclean CheckVer release debug utility install clean uninstall

all: release
	$(release)
	$(utility)
distclean: clean
	$(clean)
modernflac:
	$(release)
	$(utility)
CheckVer:
	$(shell echo ${VERSION})
release:
	mkdir -p   $(BUILD_DIR)/libModernFLAC
	$(CC)      $(REL_FLAGS) -c $(CURDIR)/libModernFLAC/src/ParseFLAC.c -o $(BUILD_DIR)/libModernFLAC/ParseFLAC.o
	$(CC)      $(REL_FLAGS) -c $(CURDIR)/libModernFLAC/src/DecodeFLAC.c -o $(BUILD_DIR)/libModernFLAC/DecodeFLAC.o
	$(CC)      $(REL_FLAGS) -c $(CURDIR)/libModernFLAC/src/EncodeFLAC.c -o $(BUILD_DIR)/libModernFLAC/EncodeFLAC.o
	ar -crsu   $(BUILD_DIR)/libModernFLAC/libModernFLAC.a $(BUILD_DIR)/libModernFLAC/*.o
	ranlib -sf $(BUILD_DIR)/libModernFLAC/libModernFLAC.a
debug: 
	mkdir -p   $(BUILD_DIR)/libModernFLAC
	$(CC)      $(DEB_FLAGS) -c $(CURDIR)/libModernFLAC/src/ParseFLAC.c -o $(BUILD_DIR)/libModernFLAC/ParseFLAC.o
	$(CC)      $(DEB_FLAGS) -c $(CURDIR)/libModernFLAC/src/DecodeFLAC.c -o $(BUILD_DIR)/libModernFLAC/DecodeFLAC.o
	$(CC)      $(DEB_FLAGS) -c $(CURDIR)/libModernFLAC/src/EncodeFLAC.c -o $(BUILD_DIR)/libModernFLAC/EncodeFLAC.o
	ar -crsu   $(BUILD_DIR)/libModernFLAC/libModernFLAC.a $(BUILD_DIR)/libModernFLAC/*.o
	ranlib -sf $(BUILD_DIR)/libModernFLAC/libModernFLAC.a
utility: $(CURDIR)/ModernFLAC/ModernFLAC.c
	mkdir -p   $(BUILD_DIR)/ModernFLAC
	$(CC)      $(REL_FLAGS) -c $(CURDIR)/ModernFLAC/ModernFLAC.c -o $(BUILD_DIR)/ModernFLAC/ModernFLAC.o
	$(CC)      $(BUILD_DIR)/ModernFLAC/ModernFLAC.o -o $(BUILD_DIR)/ModernFLAC/ModernFLAC $(BUILD_DIR)/libModernFLAC/libModernFLAC.a `pkg-config --libs libBitIO libPCM`
	strip	   $(BUILD_DIR)/ModernFLAC/ModernFLAC
install:
	install -d -m 777 $(DESTINATION)/lib
	install -d -m 777 $(DESTINATION)/bin
	install -d -m 777 $(DESTINATION)/include
	install -C -v -m 444 $(BUILD_DIR)/libModernFLAC/libModernFLAC.a $(DESTINATION)/lib/libModernFLAC.a
	install -C -v -m 444 $(CURDIR)/libModernFLAC/include/libModernFLAC.h $(DESTINATION)/include/libModernFLAC.h
	install -C -v -m 444 $(BUILD_DIR)/ModernFLAC/ModernFLAC $(DESTINATION)/bin/ModernFLAC
	chmod +x $(DESTINATION)/bin/ModernFLAC
	ln -i $(DESTINATION)/bin/ModernFLAC /usr/bin/ModernFLAC
	chmod +x /usr/bin/ModernFLAC
	install -C -v -m 444 $(CURDIR)/ModernFLAC.pc /usr/share/pkgconfig/ModernFLAC.pc
clean:
	rm -d -i -R $(BUILD_DIR)
uninstall:
	rm -d -i $(DESTINATION)
