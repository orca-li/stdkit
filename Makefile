include project/project.mk
include project/build/version.mk
include project/build/tokens.mk
include project/build/source.mk
include project/build/tools.mk
include project/build/prerequisites.mk

main: $(MAIN)
	@$(ECHO)

test: $(TEST)
	@$(ECHO)

install: $(INSTALL)
	@$(ECHO)

clean: $(CLEAN)
	@$(ECHO)

include project/build/targets.mk