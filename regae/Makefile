LOCALBASE = ./
include $(LOCALBASE)Make.env
include $(BASEDIR)Make.env

SUBDIR = walk lib run

all: $(SUBDIR)
	@$(MULTIMAKE) $(SUBDIR)

clean:
	@$(MULTIMAKE) -m clean $(SUBDIR)

install:
	@$(MULTIMAKE) -m install $(SUBDIR)
