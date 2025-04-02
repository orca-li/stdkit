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
	$(CC_TOOL) $(CC_MSAN) $(CC_CMOCKA) $(TARGET_CMOCKA_CC_SOURCES) -o $(BUILD)/$(OUTPUT)-MSAN-cmocka.out

else
MakeCompilationPrerequisites:
	$(ECHO) "Unknown target"
endif

MakeTestAsan:
	./$(BUILD)/$(OUTPUT)-ASAN-cmocka.out
	@$(ECHO) Test Asan Cmocka finished...
	@$(SLEEP)

MakeTestSleep:


MakeTestMsan:
	./$(BUILD)/$(OUTPUT)-MSAN-cmocka.out
	@$(ECHO) Test Msan Cmocka finished...
	@$(SLEEP)