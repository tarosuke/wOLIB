
target = libwO.a

all: $(target)

.PHONY : clean test watch uninstall
.DELETE_ON_ERROR : $(wildcard objs/*)


####################################################################### VALUES

COPTS ?= -I/usr/local/include/toolbox -Iinclude
LIBOPTS ?= -L/usr/local/lib/

COPTS += -Wall -Werror -g -O2 -fno-strict-aliasing -IX11
CCOPTS += $(COPTS) -std=c++11


ssfc = .c .cc .glsl
spth = * */* */*/*
srcs = $(foreach p, $(spth), $(foreach s, $(ssfc), $(wildcard $(p)$(s))))
hdrs = $(foreach p, * $(spth), $(foreach s, .h, $(wildcard $(p)$(s))))

dirs = $(sort $(dir $(srcs)))
mods = $(basename $(notdir $(srcs)))
dmds= $(addprefix objs/, $(mods))
objs = $(addsuffix .o, $(dmds)) $(exobjs)
deps = $(addsuffix .d, $(dmds))

exobjs =



######################################################################## RULES

$(target): makefile $(objs) $(exdeps)
	@echo " AR $@"
	@ar rc $@ $(objs)

install: $(target)
	@sudo cp $(target) /usr/local/lib
	@sudo cp -a include/wOLIB /usr/local/include

uninstall:
	@sudo rm -f /usr/local/lib/$(target)
	@sudo rm -rf /usr/local/include/wOLIB

clean:
	rm -f libwO* objs/*


################################################################### EXTRA RULES

../toolbox/libtoolbox.a:
	@make -C ../toolbox


################################################################# COMMON RULES


-include $(deps)

vpath %.o objs
vpath % $(dirs)


objs/%.o : %.cc makefile
	@echo " CC $@"
	@$(CC) $(CCOPTS) -c -o $@ $<

objs/%.o : %.c makefile
	@echo " CC $@"
	@${CC} $(COPTS) -c -o $@ $<

objs/%.o : %.glsl makefile
	@echo " OBJCOPY $@"
	@objcopy -I binary -O elf64-x86-64 -B i386 $< $@

objs/%.d : %.cc
	@echo " CPP $@"
	@echo -n objs/ > $@
	@$(CPP) $(CCOPTS) -MM $< >> $@

objs/%.d : %.c
	@echo " CPP $@"
	@echo -n objs/ > $@
	@$(CPP) $(COPTS) -MM $< >> $@
