IMAGE?=/opt/toolchains/arm-iwmmxt-linux-gnu/qtopia/image
DIMAGE?=/opt/toolchains/arm-iwmmxt-linux-gnu/qtopia/dimage
SDKROOT?=/opt/toolchains/arm-iwmmxt-linux-gnu/qtopia
D="-d"

all: regenerate
	@$(MAKE) $@
install: regenerate
	@$(MAKE) $@
install_target: regenerate
	@$(MAKE) $@
packages: regenerate
	@$(MAKE) $@
lupdate: regenerate
	@$(MAKE) $@
sdk: regenerate
	@$(MAKE) check_$@
cleaninstall: regenerate
	@$(MAKE) $@
cinstall: regenerate
	@$(MAKE) $@
syncqtopia: regenerate
	@$(MAKE) $@
first_syncqtopia: regenerate
	@$(MAKE) $@
checkp4: regenerate
	@$(MAKE) $@
remove_target: regenerate
	@$(MAKE) $@
relink: regenerate
	@$(MAKE) $@
printdependrules: regenerate
	@$(MAKE) $@
qmake_check: regenerate
	@$(MAKE) $@
test: regenerate
	@$(MAKE) $@
install_tests: regenerate
	@$(MAKE) $@
listtests: regenerate
	@$(MAKE) $@
qtopia_first: regenerate
	@$(MAKE) $@
first: regenerate
	@$(MAKE) $@
qtopia_all: regenerate
	@$(MAKE) $@
qtopia_clean: regenerate
	@$(MAKE) $@
qtopia_distclean: regenerate
	@$(MAKE) $@
qtopia_install: regenerate
	@$(MAKE) $@
clean:
	@echo ""
	@echo "You cannot run make clean immediately after running configure."
	@echo "If you want to perform a clean build you should pass -clean to configure."
	@echo ""
distclean:
	@echo ""
	@echo "You cannot run make distclean immediately after running configure."
	@echo "If you want to perform a clean build you should pass -clean to configure."
	@echo ""
loud: regenerate
	@$(MAKE) all 
silent: regenerate
	@$(MAKE) all 
nodeps: all
Makefile.target: 
	@$(MAKE) regenerate
regenerate:
	@/opt/toolchains/arm-iwmmxt-linux-gnu/qtopia/src/build/bin/runwithvars.sh DEVICE_CONFIG_PATH=/opt/toolchains/arm-iwmmxt-linux-gnu/qtopia/devices/ezx DEFAULT_DEVICE_PATH=/opt/toolchains/arm-iwmmxt-linux-gnu/qtopia/devices/default DEVICE_BIN=/opt/toolchains/arm-iwmmxt-linux-gnu/qtopia/devices/ezx/device_bin QMAKEFEATURES=/opt/toolchains/arm-iwmmxt-linux-gnu/qtopia/devices/ezx/features:/opt/toolchains/arm-iwmmxt-linux-gnu/qtopia/src/build QTDIR=/opt/toolchains/arm-iwmmxt-linux-gnu/qtopia/qtopiacore/target MKSPECS=/opt/toolchains/arm-iwmmxt-linux-gnu/qtopia/qtopiacore/qt/mkspecs INSTALL_ROOT=$(IMAGE) IMAGE=$(IMAGE) DIMAGE=$(DIMAGE) SDKROOT=$(SDKROOT) PROJECT_ROOT=/opt/projects/mqutim/plugins/jabber/libs MAKE=$(MAKE) /opt/toolchains/arm-iwmmxt-linux-gnu/qtopia/bin/qtopiamake -makemakefile "/opt/projects/mqutim/plugins/jabber/libs/gloox.pro"
	@/opt/toolchains/arm-iwmmxt-linux-gnu/qtopia/src/build/bin/runwithvars.sh DEVICE_CONFIG_PATH=/opt/toolchains/arm-iwmmxt-linux-gnu/qtopia/devices/ezx DEFAULT_DEVICE_PATH=/opt/toolchains/arm-iwmmxt-linux-gnu/qtopia/devices/default DEVICE_BIN=/opt/toolchains/arm-iwmmxt-linux-gnu/qtopia/devices/ezx/device_bin QMAKEFEATURES=/opt/toolchains/arm-iwmmxt-linux-gnu/qtopia/devices/ezx/features:/opt/toolchains/arm-iwmmxt-linux-gnu/qtopia/src/build QTDIR=/opt/toolchains/arm-iwmmxt-linux-gnu/qtopia/qtopiacore/target MKSPECS=/opt/toolchains/arm-iwmmxt-linux-gnu/qtopia/qtopiacore/qt/mkspecs INSTALL_ROOT=$(IMAGE) IMAGE=$(IMAGE) DIMAGE=$(DIMAGE) SDKROOT=$(SDKROOT) PROJECT_ROOT=/opt/projects/mqutim/plugins/jabber/libs $(MAKE) -f Makefile.target $@ QTDIR=/opt/toolchains/arm-iwmmxt-linux-gnu/qtopia/qtopiacore/target INSTALL_ROOT=$(IMAGE) MAKEFILE=Makefile.target 'D=$(D)'
regenerate_all: regenerate
	@$(MAKE) $@
qmake: regenerate_all
qmake-debug:
	@/opt/toolchains/arm-iwmmxt-linux-gnu/qtopia/src/build/bin/runwithvars.sh DEVICE_CONFIG_PATH=/opt/toolchains/arm-iwmmxt-linux-gnu/qtopia/devices/ezx DEFAULT_DEVICE_PATH=/opt/toolchains/arm-iwmmxt-linux-gnu/qtopia/devices/default DEVICE_BIN=/opt/toolchains/arm-iwmmxt-linux-gnu/qtopia/devices/ezx/device_bin QMAKEFEATURES=/opt/toolchains/arm-iwmmxt-linux-gnu/qtopia/devices/ezx/features:/opt/toolchains/arm-iwmmxt-linux-gnu/qtopia/src/build QTDIR=/opt/toolchains/arm-iwmmxt-linux-gnu/qtopia/qtopiacore/target MKSPECS=/opt/toolchains/arm-iwmmxt-linux-gnu/qtopia/qtopiacore/qt/mkspecs INSTALL_ROOT=$(IMAGE) IMAGE=$(IMAGE) DIMAGE=$(DIMAGE) SDKROOT=$(SDKROOT) PROJECT_ROOT=/opt/projects/mqutim/plugins/jabber/libs QMAKEPATH=/opt/toolchains/arm-iwmmxt-linux-gnu/qtopia/qtopiacore/qt /opt/toolchains/arm-iwmmxt-linux-gnu/qtopia/qtopiacore/host/bin/qmake $(D) -o /opt/projects/mqutim/plugins/jabber/libs/Makefile.target -spec /opt/toolchains/arm-iwmmxt-linux-gnu/qtopia/devices/ezx/mkspecs/qws/linux-ezx-g++ /opt/projects/mqutim/plugins/jabber/libs/gloox.pro SRCDIR=/opt/projects/mqutim/plugins/jabber/libs QTOPIA_ID= 2>&1 /opt/projects/mqutim/plugins/jabber/libs/gloox.pro
.PHONY: all install install_target packages lupdate sdk cleaninstall cinstall syncqtopia first_syncqtopia checkp4 remove_target relink printdependrules qmake_check test install_tests listtests qtopia_first first qtopia_all qtopia_clean clean qtopia_distclean distclean qtopia_install qmake qmake-debug regenerate regenerate_all nodeps loud silent
