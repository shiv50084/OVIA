PACKAGE_NAME        := ModernFLAC
FILE                := $(CURDIR)/libModernFLAC/include/libModernFLAC.h
VERSION             := $(shell cat ${FILE} | grep -e "@version")
CC                  := cc
DESTINATION         := /usr/local/Packages/$(PACKAGE_NAME)
BUILD_DIR           := $(CURDIR)/BUILD
CFLAGS              := -std=c11 -march=native `pkg-config --libs BitIO`
LDFLAGS             := -flto=thin
DEB_ERROR_OPTIONS   := -Wno-unused-parameter -Wno-unused-variable -Wno-int-conversion
REL_ERROR_OPTIONS   := -Weverything -Wunreachable-code -Wno-conversion
DEB_FLAGS           := $(CFLAGS) -g -o0 $(DEB_ERROR_OPTIONS) $(LDFLAGS)
SANITIZER           := -fsanitize=undefined -fsanitize=address
REL_FLAGS           := $(CFLAGS) -ofast $(REL_ERROR_OPTIONS) $(LDFLAGS)
BUILD_LIB           := $(BUILD_DIR)/libModernFLAC
BUILD_EXE	    := $(BUILD_DIR)/ModernFLAC

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
	mkdir -p   $(BUILD_DIR)
	mkdir -p   $(BUILD_LIB)
	$(CC)      $(REL_FLAGS) -c $(CURDIR)/libModernFLAC/src/DecodeFLAC.c -o $(BUILD_LIB)/DecodeFLAC.o
	$(CC)      $(REL_FLAGS) -c $(CURDIR)/libModernFLAC/src/EncodeFLAC.c -o $(BUILD_LIB)/EncodeFLAC.o
	ar -crsu   $(BUILD_LIB)/libModernFLAC.a $(BUILD_LIB)/DecodeFLAC.o $(BUILD_LIB)/EncodeFLAC.o
	ranlib -sf $(BUILD_LIB)/libModernFLAC.a
debug: 
	mkdir -p   $(BUILD_DIR)
	mkdir -p   $(BUILD_LIB)
	$(CC)      $(DEB_FLAGS) -c $(CURDIR)/libModernFLAC/src/DecodeFLAC.c -o $(BUILD_LIB)/DecodeFLAC.o
	$(CC)      $(DEB_FLAGS) -c $(CURDIR)/libModernFLAC/src/EncodeFLAC.c -o $(BUILD_LIB)/EncodeFLAC.o
	ar -crsu   $(BUILD_LIB)/libModernFLAC.a $(BUILD_LIB)/DecodeFLAC.o $(BUILD_LIB)/EncodeFLAC.o
	ranlib -sf $(BUILD_LIB)/libModernFLAC.a
utility: 
	mkdir -p   $(BUILD_EXE)
	$(CC)      $(REL_FLAGS) -c $(CURDIR)/ModernFLAC/ModernFLAC.c -o $(BUILD_EXE)/ModernFLAC/ModernFLAC.o
	$(CC)      $(BUILD_DIR)/ModernFLAC/ModernFLAC.o $(BUILD_LIB)/libModernFLAC.a -o $(BUILD_DIR)/ModernFLAC/ModernFLAC
install:
	install -d -m 777 $(DESTINATION)/lib
	install -d -m 777 $(DESTINATION)/bin
	install -d -m 777 $(DESTINATION)/include
	install -C -v -m 444 $(BUILD_LIB)/libModernFLAC.a $(DESTINATION)/lib/libModernFLAC.a
	install -C -v -m 444 $(BUILD_DIR)/../libModernFLAC/include/libModernFLAC.h $(DESTINATION)/include/libModernFLAC.h
	install -C -v -m 444 $(BUILD_DIR)/../libModernFLAC/include/DecodeFLAC.h $(DESTINATION)/include/DecodeFLAC.h
	install -C -v -m 444 $(BUILD_DIR)/../libModernFLAC/include/EncodeFLAC.h $(DESTINATION)/include/EncodeFLAC.h
	install -C -v -m 444 $(BUILD_DIR)/ModernFLAC/ModernFLAC $(DESTINATION)/bin/ModernFLAC
	chmod +x $(DESTINATION)/bin/ModernFLAC
	ln -i $(DESTINATION)/bin/ModernFLAC /usr/bin/ModernFLAC
	chmod +x /usr/bin/ModernFLAC
	install -C -v -m 444 $(BUILD_DIR)/../ModernFLAC.pc /usr/share/pkgconfig/ModernFLAC.pc
clean:
	cd $(BUILD_LIB)/
	rm -f -v -r DecodeFLAC.o
	rm -f -v -r EncodeFLAC.o
	rm -f -v -r libModernFLAC.a
	rm -f -v -r .DS_Store
	rm -f -v -r Thumbs.db
	rm -f -v -r desktop.ini
	cd ../ModernFLAC/
	rm -f -v -r ModernFLAC.o
	rm -f -v -r ModernFLAC
	rm -f -v -r .DS_Store
	rm -f -v -r Thumbs.db
	rm -f -v -r desktop.ini
	rmdir $(BUILD_DIR)
uninstall:
	rm -d -i $(DESTINATION)
