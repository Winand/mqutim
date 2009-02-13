IMAGE?=/opt/Qtopia/x86/image
DIMAGE?=/opt/Qtopia/x86/sdk/dimage
SDKROOT?=/opt/Qtopia/x86/sdk
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
	@/opt/Qtopia/x86/sdk/src/build/bin/runwithvars.sh DEVICE_CONFIG_PATH=/opt/Qtopia/x86/sdk/devices/gcc411 DEFAULT_DEVICE_PATH=/opt/Qtopia/x86/sdk/devices/default DEVICE_BIN=/opt/Qtopia/x86/sdk/devices/gcc411/device_bin QMAKEFEATURES=/opt/Qtopia/x86/sdk/devices/gcc411/features:/opt/Qtopia/x86/sdk/src/build QTDIR=/opt/Qtopia/x86/sdk/qtopiacore/target MKSPECS=/opt/Qtopia/x86/sdk/qtopiacore/qt/mkspecs INSTALL_ROOT=$(IMAGE) IMAGE=$(IMAGE) DIMAGE=$(DIMAGE) SDKROOT=$(SDKROOT) PROJECT_ROOT=/home/andray/dev/qutim/qutim_0.1.99 MAKE=$(MAKE) /opt/Qtopia/x86/sdk/bin/qtopiamake -makemakefile "/home/andray/dev/qutim/qutim_0.1.99/plugins/icq/icq.pro"
	@/opt/Qtopia/x86/sdk/src/build/bin/runwithvars.sh DEVICE_CONFIG_PATH=/opt/Qtopia/x86/sdk/devices/gcc411 DEFAULT_DEVICE_PATH=/opt/Qtopia/x86/sdk/devices/default DEVICE_BIN=/opt/Qtopia/x86/sdk/devices/gcc411/device_bin QMAKEFEATURES=/opt/Qtopia/x86/sdk/devices/gcc411/features:/opt/Qtopia/x86/sdk/src/build QTDIR=/opt/Qtopia/x86/sdk/qtopiacore/target MKSPECS=/opt/Qtopia/x86/sdk/qtopiacore/qt/mkspecs INSTALL_ROOT=$(IMAGE) IMAGE=$(IMAGE) DIMAGE=$(DIMAGE) SDKROOT=$(SDKROOT) PROJECT_ROOT=/home/andray/dev/qutim/qutim_0.1.99 $(MAKE) -f Makefile.target $@ QTDIR=/opt/Qtopia/x86/sdk/qtopiacore/target INSTALL_ROOT=$(IMAGE) MAKEFILE=Makefile.target 'D=$(D)'
regenerate_all: regenerate
	@$(MAKE) $@
qmake: regenerate_all
qmake-debug:
	@/opt/Qtopia/x86/sdk/src/build/bin/runwithvars.sh DEVICE_CONFIG_PATH=/opt/Qtopia/x86/sdk/devices/gcc411 DEFAULT_DEVICE_PATH=/opt/Qtopia/x86/sdk/devices/default DEVICE_BIN=/opt/Qtopia/x86/sdk/devices/gcc411/device_bin QMAKEFEATURES=/opt/Qtopia/x86/sdk/devices/gcc411/features:/opt/Qtopia/x86/sdk/src/build QTDIR=/opt/Qtopia/x86/sdk/qtopiacore/target MKSPECS=/opt/Qtopia/x86/sdk/qtopiacore/qt/mkspecs INSTALL_ROOT=$(IMAGE) IMAGE=$(IMAGE) DIMAGE=$(DIMAGE) SDKROOT=$(SDKROOT) PROJECT_ROOT=/home/andray/dev/qutim/qutim_0.1.99 QMAKEPATH=/opt/Qtopia/x86/sdk/qtopiacore/qt /opt/Qtopia/x86/sdk/qtopiacore/host/bin/qmake $(D) -o /home/andray/dev/qutim/qutim_0.1.99/plugins/icq/Makefile.target -spec /opt/Qtopia/x86/sdk/devices/gcc411/mkspecs/qws/linux-generic-g++ /home/andray/dev/qutim/qutim_0.1.99/plugins/icq/icq.pro SRCDIR=/home/andray/dev/qutim/qutim_0.1.99/plugins/icq QTOPIA_ID=plugins/icq 2>&1 /home/andray/dev/qutim/qutim_0.1.99/plugins/icq/icq.pro
.PHONY: all install install_target packages lupdate sdk cleaninstall cinstall syncqtopia first_syncqtopia checkp4 remove_target relink printdependrules qmake_check test install_tests listtests qtopia_first first qtopia_all qtopia_clean clean qtopia_distclean distclean qtopia_install qmake qmake-debug regenerate regenerate_all nodeps loud silent
