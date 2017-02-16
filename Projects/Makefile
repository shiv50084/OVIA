PACKAGE_NAME        := ModernPNG
FILE                := $(CURDIR)/include/libModernPNG.h
VERSION             := $(shell cat ${FILE} | grep -e "@version")
CC                  := cc
DESTINATION         := /usr/local/Packages/$(PACKAGE_NAME)
BUILD_DIR           := $(CURDIR)/BUILD
CFLAGS              := -std=c11 -march=native -funroll-loops -lmath `pkg-config --libs libBitIO`
LDFLAGS             := -flto=thin
DEB_ERROR_OPTIONS   := -Wno-unused-parameter -Wno-unused-variable -Wno-int-conversion
REL_ERROR_OPTIONS   := -Weverything -Wunreachable-code -Wno-conversion -Wno-padded -Wnounused-variable
DEB_FLAGS           := $(CFLAGS) -g -o0 $(DEB_ERROR_OPTIONS) $(LDFLAGS)
REL_FLAGS           := $(CFLAGS) -ofast $(REL_ERROR_OPTIONS) $(LDFLAGS)

.PHONY: all detect_platform Release Debug Install Uninstall Clean distclean utility

all: release
	$(release)
	$(utility)
	$(install)
check: 
	$(test)
distclean: 
	$(clean)

CHECK_VERS:
	$(shell echo ${VERSION})

release:
	mkdir -p   $(BUILD_DIR)/libModernPNG
	$(CC)      $(REL_FLAGS) -c $(CURDIR)/libModernPNG/src/libModernPNG.c -o $(BUILD_DIR)/libModernPNG/libModernPNG.o
	$(CC)      $(REL_FLAGS) -c $(CURDIR)/libModernPNG/src//DecodePNG.c -o $(BUILD_DIR)/libModernPNG/DecodePNG.o
	$(CC)      $(REL_FLAGS) -c $(CURDIR)/libModernPNG/src//EncodePNG.c -o $(BUILD_DIR)/libModernPNG/EncodePNG.o
	ar -crsu   $(BUILD_DIR)/libModernPNG/libModernPNG.a $(BUILD_DIR)/libModernPNG/*.o
	ranlib -sf $(BUILD_DIR)/libModernPNG/libModernPNG.a
debug:
	mkdir -p   $(BUILD_DIR)/libModernPNG
	$(CC)      $(DEB_FLAGS) -c $(CURDIR)/libModernPNG/src/libModernPNG.c -o $(BUILD_DIR)/libModernPNG/libModernPNG.o
	$(CC)      $(DEB_FLAGS) -c $(CURDIR)/libModernPNG/src//DecodePNG.c -o $(BUILD_DIR)/libModernPNG/DecodePNG.o
	$(CC)      $(DEB_FLAGS) -c $(CURDIR)/libModernPNG/src//EncodePNG.c -o $(BUILD_DIR)/libModernPNG/EncodePNG.o
	ar -crsu   $(BUILD_DIR)/libModernPNG/libModernPNG.a $(BUILD_DIR)/libModernPNG/*.o
	ranlib -sf $(BUILD_DIR)/libModernPNG/libModernPNG.a
utility:
	$(CC)	   $(REL_FLAGS) -c $(CURDIR)/ModernPNG/ModernPNG.c -o $(BUILD_DIR)/ModernPNG/ModernPNG -l$(BUILD_DIR)/libModernPNG/libModernPNG.a
install:
	install -d -m 777 $(DESTINATION)/bin
	install -d -m 777 $(DESTINATION)/lib
	install -d -m 777 $(DESTINATION)/include
	install -C -v -m 444 $(BUILD_DIR)/libModernPNG/libModernPNG.a $(DESTINATION)/lib/libModernPNG.a
	install -C -v -m 444 $(CURDIR)/libModernPNG/include/libModernPNG.h $(DESTINATION)/include/libModernPNG.h
	install -C -v -m 444 $(CURDIR)/libModernPNG.pc /usr/share/pkgconfig/libModernPNG.pc

uninstall:
	rm -d -i $(DESTINATION)

clean:
	cd $(BUILD_DIR)/libModernPNG/
	rm -f -v -r *.o
	rm -f -v -r *.a
	rm -f -v -r .DS_Store
	rm -f -v -r Thumbs.db
	rm -f -v -r desktop.ini
	cd $(BUILD_DIR)/ModernPNG/
	rm -f -v -r *.o
	rm -f -v -r *.a
	rm -f -v -r .DS_Store
	rm -f -v -r Thumbs.db
	rm -f -v -r desktop.ini
	rmdir $(BUILD_DIR)
