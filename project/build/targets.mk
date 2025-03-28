MakeClean:
	$(RM) $(RM_FLAGS) $(BUILD)

MakeBuildDir:
	$(MD) $(MD_FLAGS) $(BUILD)

RemoveTokenFile:
	@$(RM) $(RM_FLAGS) $(TOKENFILE)

MakeTokenFile: RemoveTokenFile
	@printf "%s = %s\n" $(foreach token,$(TOKENS),$(token) $(if $(strip $(value $(token))),$(value $(token)),\(null\))) > $(TOKENFILE)

MakeCompilation: MakeCompilationPrerequisites


MakeCompilationDebug:


MakeCompilationRelease:


ifeq ($(PROJECT), tests)
MakeCompilationPrerequisites:
	$(CC_TOOL) $(CC_ASAN) $(CC_CMOCKA) $(TARGET_CMOCKA_CC_SOURCES) -o $(BUILD)/$(OUTPUT)-ASAN-cmocka.out

else
MakeCompilationPrerequisites:
	$(ECHO) "Unknown target"
endif