PACKAGE_NAME        := NewFLAC
FILE                := $(CURDIR)/libNewFLAC/include/libNewFLAC.h
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
BUILD_LIB           := $(BUILD_DIR)/libNewFLAC
BUILD_EXE	    := $(BUILD_DIR)/NewFLAC

.PHONY: all distclean CheckVer release debug utility install clean uninstall

all: release
	$(release)
	$(utility)
distclean: clean
	$(clean)
CheckVer:
	$(shell echo ${VERSION})
release:
	mkdir -p   $(BUILD_DIR)
	mkdir -p   $(BUILD_LIB)
	$(CC)      $(REL_FLAGS) -c $(CURDIR)/libNewFLAC/src/DecodeFLAC.c -o $(BUILD_LIB)/DecodeFLAC.o
	$(CC)      $(REL_FLAGS) -c $(CURDIR)/libNewFLAC/src/EncodeFLAC.c -o $(BUILD_LIB)/EncodeFLAC.o
	ar -crsu   $(BUILD_LIB)/libNewFLAC.a $(BUILD_LIB)/DecodeFLAC.o $(BUILD_LIB)/EncodeFLAC.o
	ranlib -sf $(BUILD_LIB)/libNewFLAC.a
debug: 
	mkdir -p   $(BUILD_DIR)
	mkdir -p   $(BUILD_LIB)
	$(CC)      $(DEB_FLAGS) -c $(CURDIR)/libNewFLAC/src/DecodeFLAC.c -o $(BUILD_LIB)/DecodeFLAC.o
	$(CC)      $(DEB_FLAGS) -c $(CURDIR)/libNewFLAC/src/EncodeFLAC.c -o $(BUILD_LIB)/EncodeFLAC.o
	ar -crsu   $(BUILD_LIB)/libNewFLAC.a $(BUILD_LIB)/DecodeFLAC.o $(BUILD_LIB)/EncodeFLAC.o
	ranlib -sf $(BUILD_LIB)/libNewFLAC.a
utility: 
	mkdir -p   $(BUILD_EXE)
	$(CC)      $(REL_FLAGS) -c $(CURDIR)/NewFLAC/NewFLAC.c -o $(BUILD_EXE)/NewFLAC/NewFLAC.o
	$(CC)      $(BUILD_DIR)/NewFLAC/NewFLAC.o $(BUILD_LIB)/libNewFLAC.a -o $(BUILD_DIR)/NewFLAC/NewFLAC
install:
	install -d -m 777 $(DESTINATION)/lib
	install -d -m 777 $(DESTINATION)/bin
	install -d -m 777 $(DESTINATION)/include
	install -C -v -m 444 $(BUILD_LIB)/libNewFLAC.a $(DESTINATION)/lib/libNewFLAC.a
	install -C -v -m 444 $(BUILD_DIR)/../libNewFLAC/include/libNewFLAC.h $(DESTINATION)/include/libNewFLAC.h
	install -C -v -m 444 $(BUILD_DIR)/../libNewFLAC/include/DecodeFLAC.h $(DESTINATION)/include/DecodeFLAC.h
	install -C -v -m 444 $(BUILD_DIR)/../libNewFLAC/include/EncodeFLAC.h $(DESTINATION)/include/EncodeFLAC.h
	install -C -v -m 444 $(BUILD_DIR)/NewFLAC/NewFLAC $(DESTINATION)/bin/NewFLAC
	chmod +x $(DESTINATION)/bin/NewFLAC
	ln -i $(DESTINATION)/bin/NewFLAC /usr/bin/NewFLAC
	chmod +x /usr/bin/NewFLAC
	install -C -v -m 444 $(BUILD_DIR)/../NewFLAC.pc /usr/share/pkgconfig/NewFLAC.pc
clean:
	cd $(BUILD_LIB)/
	rm -f -v -r DecodeFLAC.o
	rm -f -v -r EncodeFLAC.o
	rm -f -v -r libNewFLAC.a
	rm -f -v -r .DS_Store
	rm -f -v -r Thumbs.db
	rm -f -v -r desktop.ini
	cd ../NewFLAC/
	rm -f -v -r NewFLAC.o
	rm -f -v -r NewFLAC
	rm -f -v -r .DS_Store
	rm -f -v -r Thumbs.db
	rm -f -v -r desktop.ini
	rmdir $(BUILD_DIR)
uninstall:
	rm -d -i $(DESTINATION)
