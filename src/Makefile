
CC = gcc

MAJOR_VERSION = $(shell grep MAJOR_VERSION ../include/libalex.h | awk '{print $$4}')
MINOR_VERSION = $(shell grep MINOR_VERSION ../include/libalex.h | awk '{print $$4}')
BUILD = $(shell grep BUILD ../include/libalex.h | awk '{print $$4}')

USR_DIR = /usr/local/lib
PREFIX = libalex
LIBNAME = $(PREFIX).so
SONAME = $(LIBNAME)
DEST_LIBNAME = $(LIBNAME).$(MAJOR_VERSION).$(MINOR_VERSION).$(BUILD)

TARGET = ../bin/$(SONAME)
TARGET_DEST = ../bin/$(DEST_LIBNAME)

INCLUDEDIR = -I../include
INCLUDEDIR += -I/usr/local/include
INCLUDEDIR += -I../../../libimg/include
INCLUDEDIR += -I../../../libframe_grab/include
INCLUDEDIR += -I/usr/include/freetype2

LIBDIR = -L/usr/local/lib
ALLEG_LIBS = -lalleg -lX11 -ldl -lXpm -lXxf86vm -lXcursor
LIBS = -lcollections -lpthread -lrt $(ALLEG_LIBS)

CFLAGS = -Wall -Wextra -fPIC -ggdb -O0 -g3 -fvisibility=hidden \
	 -D_GNU_SOURCE -DLIBALEX_COMPILE $(INCLUDEDIR)

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

OBJS =				\
	api.o			\
	common.o 		\
	error.o			\
	gui.o			\
	parser.o		\
	writer.o		\
	$(GUI_OBJS)

$(TARGET): $(OBJS)
	rm -f $(LIBNAME)*
	$(CC) -shared -Wl,-soname,$(SONAME) -o $(TARGET_DEST) $(OBJS) $(LIBDIR) $(LIBS)

clean:
	rm -f ../bin/$(LIBNAME)*
	rm -rf $(OBJS) $(TARGET) $(TARGET_DEST) *~ ../include/*~ ../bin/*.so

dest_clean:
	rm -f $(USR_DIR)/$(LIBNAME)*

purge: clean $(TARGET)

