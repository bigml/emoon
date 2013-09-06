BASEDIR = ../
include $(BASEDIR)Make.env

UNAME := $(shell uname)
CFLAGS += -fPIC -shared

SOURCES = $(wildcard clearsilver/util/*.c)
SOURCES += $(wildcard mnl/*.c)
OBJS = $(patsubst %.c, %.o, $(SOURCES))
DEPEND = .depend
SOURCE1 = clearsilver/util/missing.c

all: libemnl.a

libemnl.a:$(OBJS) $(DEPEND)
	@rm -rf $@
	$(AR) rcs $@ $(OBJS)

libemnl.so: $(OBJS) $(DEPEND)
ifeq ($(UNAME), Darwin)
	$(CC) $(CFLAGS) -dynamiclib -undefined suppress -flat_namespace -o $@ $(OBJS) $(LIB_MOON)
else
	@$(CC) $(CFLAGS) -o $@ $(OBJS)
endif

$(DEPEND): $(SOURCES)
	@$(CC) $(CFLAGS) -MM $^ $(INC_MOON) > $@

include $(DEPEND)
%.o:%.c
	@if [ "$<" = "$(SOURCE1)" ]; then \
		echo "=== "$(CFLAGS); \
		echo "=== "$(INC_MOON); \
        echo "=== "$(LIB_MOON); fi
	@echo "$(CC) -c $<"
	@$(CC) $(CFLAGS) -c $< -o $@ $(INC_MOON)

install:
	install -m 0644 libemnl.so /usr/local/lib

uninstall:
	rm -f /usr/local/lib/libemnl.so

clean:
	rm -f $(OBJS) *.a *.so
