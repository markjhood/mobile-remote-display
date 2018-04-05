# Makefile for the JBlend remote display demo.  The project uses GTK+-2.0
# development libraries.  The Makefile supports building under at least Ubuntu
# 8.04 or Mac OS X 10.6, but a Mac OS X build requires the GTK-OSX jhbuild
# system.  See BUILDING.txt.

PROJECT = MWC-2010-multidisplay-demo
PROGNAMES = remote-jblend demotest
PROGS = $(PROGNAMES:%=$(BINDIR)/%)

CFILES = demotest.c pixbufs.c fb.c remote.c stream.c cmd.c
HFILES = protocol.h stream.h fb.h pixbufs.h cmd.h
OFILES = $(CFILES:%.c=$(OBJDIR)/%.o)

IMGDIR = images
SCPDIR = scripts/$(OS)
BLDDIR = ../build/$(OS)
BINDIR = $(BLDDIR)/bin
OBJDIR = $(BLDDIR)/obj
TOPDIR = ../..

CC = gcc
OPT = -O3
CFLAGS = -Wall -DPERF_OUTPUT $(OPT) $(CFLAGS_EXTRA)
GTK_CFLAGS = $(ARCH) `$(PKG_CONFIG) gtk+-2.0 $(MAC_INTEGRATION) --cflags`
LDFLAGS = $(ARCH) `$(PKG_CONFIG) gtk+-2.0 gthread-2.0 $(MAC_INTEGRATION) --libs`

OS = $(shell uname)
ifeq ($(OS),Darwin)
  # Mac OS X.
  ARCH = -arch i386
  JHBUILD_PREFIX = $(HOME)/gtk/inst
  PKG_CONFIG = $(JHBUILD_PREFIX)/bin/pkg-config

  SETUP.APP = $(BLDDIR)/Remote\ Demo\ Setup.app
  REMOTE.APP = $(BLDDIR)/remote-jblend.app
  PROXIMITY.APP = $(BLDDIR)/Proximity.app

  APPS = $(SETUP.APP) $(REMOTE.APP) $(PROXIMITY.APP)
  DMG = $(BLDDIR)/Remote\ Display\ Demo.dmg

  # Next are for the GTK-OSX integration of the remote-jblend executable.
  BUNDLE_CONFIG = bundler/remote-jblend.bundle
  BUNDLE_LAUNCHER = bundler/launcher.sh
  MAC_INTEGRATION = ige-mac-integration

  # Proximity.app installation root is relative to its src directory.
  PROXIMITY_SRC = proximity/Darwin
  PROXIMITY_ROOT = ../../../build

  # Following AppleScript is for Remote Demo Setup.app.
  SCRIPTNAMES = start-remote.osa kill-remote.osa
  SCRIPTRSRC = $(SETUP.APP)/Contents/Resources
  SCRIPTBUNDLEDIR = $(SCRIPTRSRC)/Scripts
  SCRIPTDESCDIR = $(SCRIPTRSRC)/description.rtfd
  SCRIPTDESC = $(SCRIPTDESCDIR)/TXT.rtf
  SCRIPTMAIN = $(SCRIPTBUNDLEDIR)/main.scpt
  SCRIPTBUNDLE = $(SCRIPTNAMES:%.osa=$(SCRIPTBUNDLEDIR)/%.scpt) $(SCRIPTDESC)
else
  # Assume Linux for now.
  PKG_CONFIG = /usr/bin/pkg-config

  INSTALL_DIR = /usr/local
  INSTALL_BINDIR = $(INSTALL_DIR)/bin
  INSTALL_IMGDIR = $(INSTALL_DIR)/share/remote-jblend
  CFLAGS_EXTRA = -DINSTALL_IMGDIR=\"$(INSTALL_IMGDIR)\"

  LAUNCHER_DIR = scripts/Linux/desktop
  SCRIPTNAMES = remote-demo-setup.sh wifi-adhoc.sh wifi-managed.sh
  SCRIPTS = $(SCRIPTNAMES:%.sh=$(BINDIR)/%)
endif

#
# Targets and rules.
#
bin: $(BINDIR) $(OBJDIR) $(PROGS) $(SCRIPTS)
apps: $(BINDIR) $(OBJDIR) $(APPS)

debug: OPT = -g
debug: bin

$(OFILES): $(HFILES)

$(OBJDIR)/%.o: %.c
	$(CC) $(GTK_CFLAGS) $(CFLAGS) -c $< -o $@

$(BINDIR)/remote-jblend: $(OBJDIR)/remote.o $(OBJDIR)/fb.o \
  $(OBJDIR)/pixbufs.o $(OBJDIR)/stream.o $(OBJDIR)/cmd.o
	$(CC) $^ $(LDFLAGS) -o $@

$(BINDIR)/demotest: $(OBJDIR)/demotest.o $(OBJDIR)/fb.o \
  $(OBJDIR)/pixbufs.o $(OBJDIR)/stream.o $(OBJDIR)/cmd.o
	$(CC) $^ $(LDFLAGS) -o $@

# Create a Mac OS X .app bundle for remote-jblend.  Any existing .app bundle
# has to be removed and the executable has to be copied to JHBUILD_PREFIX/bin.
remote.app: $(BINDIR) $(OBJDIR) $(REMOTE.APP)
$(REMOTE.APP): $(BINDIR)/remote-jblend $(BUNDLE_CONFIG) $(BUNDLE_LAUNCHER)
	@echo make: creating $(REMOTE.APP)
	rm -rf $(REMOTE.APP)
	cp $(BINDIR)/remote-jblend $(JHBUILD_PREFIX)/bin
	jhbuild run ige-mac-bundler $(BUNDLE_CONFIG)

# Proximity.app is maintained as an Xcode project.  This rule only builds
# and installs the app if it doesn't exist in the build directory.
proximity.app: $(BLDDIR) $(PROXIMITY.APP)
$(PROXIMITY.APP):
	@echo make: installing Xcode project $(PROXIMITY.APP)
	(cd $(PROXIMITY_SRC); \
           xcodebuild DSTROOT=$(PROXIMITY_ROOT) INSTALL_PATH=/Darwin \
             -sdk macosx10.5 install)

# Remote Demo Setup.app is written in AppleScript.
setup.app: $(BLDDIR) $(SETUP.APP)
$(SETUP.APP): $(SCRIPTMAIN) $(SCRIPTBUNDLE)

# Compile $(SETUP.APP)'s main script with -u to get a startup dialog.  Using
# -o with a .app extension creates an entire app bundle hierarchy.  The script
# is compiled to main.scpt in the Contents/Resources/Scripts folder in the app
# bundle.
$(SCRIPTMAIN): $(SCPDIR)/remote-demo-setup.osa
	@echo make: creating $(SETUP.APP)
	osacompile -o $(SETUP.APP) -u $<

# Copy $(SETUP.APP)'s description text. The -u option to osacompile only
# displays a generic confirmation dialog if this text isn't copied.
$(SCRIPTDESC): $(SCPDIR)/TXT.rtf $(SCRIPTDESCDIR)
	cp -f $< $(SCRIPTDESCDIR)

# Compile $(SETUP.APP)'s bundled AppleScripts.
$(SCRIPTBUNDLEDIR)/%.scpt: $(SCPDIR)/%.osa
	osacompile -o "$@" $<

# Copy demo setup scripts for Linux.
$(BINDIR)/%: $(SCPDIR)/%.sh
	cp -f $< $@

clean: 
	rm -f $(OBJDIR)/*.o

clean-all: 
	rm -rf $(BLDDIR)

$(INSTALL_BINDIR) $(INSTALL_IMGDIR) \
  $(BLDDIR) $(BINDIR) $(OBJDIR) $(SCRIPTDESCDIR):
	mkdir -p "$@"

install: install-$(OS)

install-Linux: bin $(INSTALL_BINDIR) $(INSTALL_IMGDIR)
	install -m 644 $(IMGDIR)/* $(INSTALL_IMGDIR)
	install $(PROGS) $(SCRIPTS) $(INSTALL_BINDIR)
	install -m 777 $(LAUNCHER_DIR)/*.desktop $(HOME)/Desktop

install-Darwin: bin $(APPS)
	cp -fR $(APPS) /Applications

# Create a Mac OS X zlib compressed disk image holding all the apps we need.
dmg: bin $(DMG)
$(DMG): $(REMOTE.APP) $(PROXIMITY.APP) $(SCRIPTMAIN) $(SCRIPTBUNDLE)
	@echo make: creating $(DMG)
	hdiutil create $(DMG) -ov -format UDZO -srcfolder $(REMOTE.APP) \
	  -srcfolder $(PROXIMITY.APP) -srcfolder $(SETUP.APP) \
          -srcfolder ../src -srcfolder ../doc -volname Remote\ Display\ Demo

dist: 
	(cd ../build/Linux; rm -rf obj)
	(cd ../build/Darwin; rm -rf obj bin *.app)
	(cd $(TOPDIR); tar cvf - $(PROJECT) | gzip -c > $(PROJECT).tar.gz)
