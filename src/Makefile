
.PHONY: outputdir clean dest_clean purge install install_headers dev_install \
    uninstall

CC = gcc

MAJOR_VERSION = $(shell grep MAJOR_VERSION ../include/libgrc.h | awk '{print $$4}')
MINOR_VERSION = $(shell grep MINOR_VERSION ../include/libgrc.h | awk '{print $$4}')
BUILD = $(shell grep BUILD ../include/libgrc.h | awk '{print $$4}')

USR_DIR = /usr/local/lib
PREFIX = libgrc
LIBNAME = $(PREFIX).so
SONAME = $(LIBNAME)
DEST_LIBNAME = $(LIBNAME).$(MAJOR_VERSION).$(MINOR_VERSION).$(BUILD)

TARGET_DIR = ../bin
TARGET = $(TARGET_DIR)/$(SONAME)
TARGET_DEST = $(TARGET_DIR)/$(DEST_LIBNAME)

INCLUDEDIR = -I../include
INCLUDEDIR += -I/usr/local/include

LIBDIR = -L/usr/local/lib
ALLEG_LIBS = -lalleg -lX11 -ldl -lXpm -lXxf86vm -lXcursor
LIBS = -lcollections -lpthread -lrt $(ALLEG_LIBS)

CFLAGS = -Wall -Wextra -fPIC -ggdb -O0 -g3 \
	 -D_GNU_SOURCE -DLIBGRC_COMPILE $(INCLUDEDIR)

GUI_PATH = ./gui
VPATH = .:../include:.:$(GUI_PATH)

GUI_OBJS =					\
	gui_bitmap.o			\
	gui_button.o			\
	gui_check.o				\
	gui_clock.o				\
	gui_edit.o				\
	gui_icon.o				\
	gui_keyboard.o			\
	gui_list.o				\
	gui_messages_log_box.o	\
	gui_radio.o				\
	gui_slider.o			\
	gui_textbox.o			\
	gui_vt_keyboard.o

OBJS =						\
	callback.o				\
	api.o					\
	colors.o				\
	error.o					\
	grc.o					\
	grc_generic.o			\
	grc_object.o			\
	info.o					\
	gui.o					\
	object_properties.o		\
	parser.o				\
	utils.o					\
	writer.o				\
	$(GUI_OBJS)

$(TARGET): outputdir $(OBJS)
	$(CC) -shared -Wl,-soname,$(SONAME),--version-script,$(PREFIX).sym -o $(TARGET_DEST) $(OBJS) $(LIBDIR) $(LIBS)

clean:
	rm -f ../bin/$(LIBNAME)*
	rm -rf $(OBJS) $(TARGET) $(TARGET_DEST) *~ ../include/*~ ../bin/*.so

dest_clean:
	rm -f $(USR_DIR)/$(LIBNAME)*

purge: clean $(TARGET)

install:
	cp -f $(TARGET_DEST) $(USR_DIR)
	rm -rf $(USR_DIR)/$(LIBNAME) $(USR_DIR)/$(SONAME)
	ln -sf $(USR_DIR)/$(DEST_LIBNAME) $(USR_DIR)/$(LIBNAME)
	ln -sf $(USR_DIR)/$(DEST_LIBNAME) $(USR_DIR)/$(SONAME)

install_headers:
	$(shell [ ! -d /usr/local/include/grc ] && mkdir -p /usr/local/include/grc)
	$(shell for f in `ls ../include/grc | egrep -v '^grc_internal.h'`; do cp -f ../include/grc/$$f /usr/local/include/grc; done)
	cp -f ../include/libgrc.h /usr/local/include

dev_install: install_headers install

uninstall: dest_clean
	rm -rf /usr/local/include/grc
	rm -f /usr/local/include/libgrc.h

outputdir: $(TARGET_DIR)

$(TARGET_DIR):
	mkdir -p $(TARGET_DIR)

