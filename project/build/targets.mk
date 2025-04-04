MakeClean:
	$(RM) $(RM_FLAGS) $(BUILD_DIR)/

MakeBuildDir:
	@$(MD) $(MD_FLAGS) {$(BUILD_DIR),$(BUILD_BIN_DIR),$(BUILD_OBJ_DIR)}

RemoveTokenFile:
	@$(RM) $(RM_FLAGS) $(TOKENFILE)

MakeTokenFile: RemoveTokenFile
	@printf "%s = %s\n" $(foreach token,$(TOKENS),$(token) $(if $(strip $(value $(token))),$(value $(token)),\(null\))) > $(TOKENFILE)

MakeCompilation: MakeCompilationPrerequisites


MakeCompilationDebug:


MakeCompilationRelease:


ifeq ($(BUILD), debug)
MakeCompilationPrerequisites:
	$(CC_TOOL) $(CC_FLAGS_DEBUG) $(CC_ASAN) $(CC_CMOCKA) $(CC_SOURCES) $(TARGET_CMOCKA_CC_SOURCES) -o $(BUILD_BIN_DIR)/$(OUTPUT)-ASAN-cmocka.out.$(VERSION)
	$(CC_TOOL) $(CC_FLAGS_DEBUG) $(CC_MSAN) $(CC_CMOCKA) $(CC_SOURCES) $(TARGET_CMOCKA_CC_SOURCES) -o $(BUILD_BIN_DIR)/$(OUTPUT)-MSAN-cmocka.out.$(VERSION)

else ifeq ($(BUILD), release)
MakeCompilationPrerequisites: $(CC_OBJECTS)
	$(AR) $(AR_FLAGS) $(BUILD_BIN_DIR)/lib$(OUTPUT).a.$(VERSION) $(FIND_ALL_OBJECTS)
	$(CC) -shared -Wl,-soname,lib$(OUTPUT).so.$(VERSION_MAJOR) -o $(BUILD_BIN_DIR)/lib$(OUTPUT).so.$(VERSION) $(FIND_ALL_OBJECTS)

else
MakeCompilationPrerequisites:
	@$(ECHO) "Unknown target"
endif

MakeTestAsan:
	./$(BUILD_BIN_DIR)/$(OUTPUT)-ASAN-cmocka.out.$(VERSION)
	@$(ECHO) Test Asan Cmocka finished...
	@$(SLEEP)

MakeTestSleep:


MakeTestMsan:
	./$(BUILD_BIN_DIR)/$(OUTPUT)-MSAN-cmocka.out.$(VERSION)
	@$(ECHO) Test Msan Cmocka finished...
	@$(SLEEP)

$(BUILD_OBJ_DIR)/%.o: %.c | $(BUILD_OBJ_DIR)
	@$(MD) $(MD_FLAGS) $(dir $(BUILD_OBJ_DIR)/$<)
	$(CC_TOOL) $(CC_FLAGS_RELEASE) -c $< -o $@.$(VERSION)

MakeInstallLinuxSO:
	@$(ECHO) "# $(MD) $(MD_FLAGS) $(INSTALL_LIBDIR)"
	@$(ECHO) "# $(CP) $(BUILD_BIN_DIR)/$(LIB_FULLNAME) $(INSTALL_LIBDIR)"
	@$(ECHO) "# $(LN) -sf $(INSTALL_LIBDIR)/$(LIB_FULLNAME) $(INSTALL_LIBDIR)/$(LIB_SONAME)"
	@$(ECHO) "# $(LN) -sf $(INSTALL_LIBDIR)/$(LIB_SONAME) $(INSTALL_LIBDIR)/$(LIB_LINK)"
	@$(ECHO) "# $(ECHO) Adding $(INSTALL_LIBDIR) to /etc/ld.so.conf.d/local.conf"
	@$(ECHO) "# $(ECHO) $(INSTALL_LIBDIR) | sudo tee -a /etc/ld.so.conf.d/local.conf > /dev/null"
	@$(ECHO) "# sudo ldconfig"
	@$(ECHO)
	@$(ECHO) For the installation to be successful, make sure that you have called:
	@$(ECHO) sudo make install
	@$(ECHO) See build/targets.mk
	@$(ECHO) $(CONFIRM_INSTALL)
	@$(READ) -r answer; \
		if [[ "$$answer" =~ ^[Yy]$$ ]]; then \
		$(ECHO) "Checking root permissions..."; \
		if [[ $$(id -u) -eq 0 ]]; then \
			$(ECHO) "Installing..." ; \
			$(MD) $(MD_FLAGS) "$(INSTALL_LIBDIR)" ; \
			$(CP) "$(BUILD_BIN_DIR)/$(LIB_FULLNAME)" "$(INSTALL_LIBDIR)" ; \
			$(LN) -sf "$(INSTALL_LIBDIR)/$(LIB_FULLNAME)" "$(INSTALL_LIBDIR)/$(LIB_SONAME)" ; \
			$(LN) -sf "$(INSTALL_LIBDIR)/$(LIB_SONAME)" "$(INSTALL_LIBDIR)/$(LIB_LINK)" ; \
			if ! grep -q "$(INSTALL_LIBDIR)" /etc/ld.so.conf.d/local.conf; then \
				$(ECHO) "Adding $(INSTALL_LIBDIR) to /etc/ld.so.conf.d/local.conf" ; \
				$(ECHO) "$(INSTALL_LIBDIR)" | sudo tee -a /etc/ld.so.conf.d/local.conf > /dev/null; \
			else \
				$(ECHO) "$(INSTALL_LIBDIR) already in /etc/ld.so.conf.d/local.conf"; \
			fi ; \
			sudo ldconfig ; \
			$(ECHO) "Installation complete."; \
		else \
			$(ECHO) "This operation requires root privileges. Use sudo." ; \
			$(ECHO) "Installation canceled."; \
		fi ; \
	else \
		$(ECHO) "Installation canceled."; \
	fi
	@$(ECHO) "Continue...(1/2)"
	@$(ECHO)
	@$(ECHO)

MakeInstallLinuxHeader:
	@$(ECHO) "# Installing headers to $(INSTALL_INCLUDE_DIR)"
	@$(ECHO) "# sudo mkdir -p $(INSTALL_INCLUDE_DIR)/{kithdrs,itrlib}"
	@$(ECHO) "# sudo cp -v stdkit.h /usr/include"
	@$(ECHO) "# sudo cp -v stdkit/kithdrs/*.h $(INSTALL_INCLUDE_DIR)/kithdrs"
	@$(ECHO) "# sudo cp -v stdkit/itrlib/*.h $(INSTALL_INCLUDE_DIR)/itrlib"
	@$(ECHO)
	@$(ECHO) $(CONFIRM_INSTALL_HEADER)
	@$(READ) -r answer; \
		if [[ "$$answer" =~ ^[Yy]$$ ]]; then \
		$(ECHO) "Checking root permissions..."; \
		if [[ $$(id -u) -eq 0 ]]; then \
			$(ECHO) "Installing headers..." ; \
			sudo mkdir -p $(INSTALL_INCLUDE_DIR)/{kithdrs,itrlib} ; \
			sudo cp -v stdkit.h /usr/include ; \
			sudo cp -v stdkit/kithdrs/*.h $(INSTALL_INCLUDE_DIR)/kithdrs ; \
			sudo cp -v stdkit/itrlib/*.h $(INSTALL_INCLUDE_DIR)/itrlib ; \
			$(ECHO) "Header installation complete."; \
		else \
			$(ECHO) "This operation requires root privileges. Use sudo." ; \
			$(ECHO) "Header installation canceled."; \
		fi ; \
	else \
		$(ECHO) "Header installation canceled."; \
	fi
	@$(ECHO) $(CONFIRM_INSTALL_HEADER)
	@$(ECHO) "Installation complete(2/2)."