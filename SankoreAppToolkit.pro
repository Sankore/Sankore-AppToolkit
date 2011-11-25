#-------------------------------------------------
#
# Project created by QtCreator 2011-06-17T15:52:59
#
#-------------------------------------------------
TARGET = SankoreAppToolkit
TEMPLATE = app

CONFIG += debug_and_release

#Version
VERSION_MAJ = 1
VERSION_MIN = 00
VERSION_TYPE = r
VERSION_PATCH = 00
VERSION = "$${VERSION_MAJ}.$${VERSION_MIN}.$${VERSION_TYPE}"
VERSION = $$replace(VERSION, "\\.r", "")

VERSION_RC = $$VERSION_MAJ,$$VERSION_MIN,$$VERSION_TYPE,$$VERSION_PATCH
VERSION_RC = $$replace(VERSION_RC, "a", "160") # 0xA0
VERSION_RC = $$replace(VERSION_RC, "b", "176") # 0xB0
VERSION_RC = $$replace(VERSION_RC, "r", "240") # 0xF0

macx:OSX_VERSION = "$${VERSION} (r$${SVN_VERSION})"

#Qt modules
QT += core gui webkit xml

INCLUDEPATH += src

include(src/api/api.pri)
include(src/core/core.pri)
include(src/gui/gui.pri)

#Build directory
BUILD_DIR = build
macx:BUILD_DIR = $$BUILD_DIR/macx
win32:BUILD_DIR = $$BUILD_DIR/win32
linux-g++:BUILD_DIR = $$BUILD_DIR/linux
linux-g++-64:BUILD_DIR = $$BUILD_DIR/linux

CONFIG(debug, debug|release):BUILD_DIR = $$BUILD_DIR/debug
CONFIG(release, debug|release):BUILD_DIR = $$BUILD_DIR/release

DESTDIR = $$BUILD_DIR/product
OBJECTS_DIR = $$BUILD_DIR/objects
MOC_DIR = $$BUILD_DIR/moc
RCC_DIR = $$BUILD_DIR/rcc
UI_DIR = $$BUILD_DIR/ui

#Configuration

win32 {
#   RC_FILE = resources/win/sankore.rc
   CONFIG += qaxcontainer
   exists(console):CONFIG += console
   QMAKE_CXXFLAGS += /MP
   QMAKE_CXXFLAGS_RELEASE += /Od \
       /Zi
   QMAKE_LFLAGS_RELEASE += /DEBUG
   
   UB_LIBRARY.path = $$DESTDIR
   UB_I18N.path = $$DESTDIR/i18n
   UB_ETC.path = $$DESTDIR
   UB_THIRDPARTY_INTERACTIVE.path = $$DESTDIR/library
   
   system(md $$replace(BUILD_DIR, /, \\))
}

macx {
    CONFIG(release, debug|release):CONFIG += x86
    CONFIG(debug, debug|release):CONFIG += x86

    QMAKE_MAC_SDK = "/Developer/SDKs/MacOSX10.5.sdk"
    QMAKE_MACOSX_DEPLOYMENT_TARGET = "10.4"
    VERSION_RC_PATH = "$$BUILD_DIR/version_rc"

    QMAKE_CXX_FLAGS_RELEASE += -gdwarf -2 \
        -mdynamic-no-pic

    QMAKE_CFLAGS += -fopenmp
    QMAKE_CXXFLAGS += -fopenmp
    QMAKE_LFLAGS += -fopenmp

    CONTENTS_DIR = "Contents"
    RESOURCES_DIR = "$$CONTENTS_DIR/Resources"
    FRAMEWORK_DIR = "$$CONTENTS_DIR/Frameworks"

    # Build the package content here
    MSG_FILE.files = "msg.config"
    MSG_FILE.path = "$$RESOURCES_DIR"
    DOC.files = "doc"
    DOC.path = "$$RESOURCES_DIR"
    EXAMPLE_WIDGET.files = "example.wgt"
    EXAMPLE_WIDGET.path = "$$RESOURCES_DIR"
    SAMPLE_WIDGET.files = "sample.wgt"
    SAMPLE_WIDGET.path = "$$RESOURCES_DIR"

    QMAKE_BUNDLE_DATA += MSG_FILE
    QMAKE_BUNDLE_DATA += DOC
    QMAKE_BUNDLE_DATA += EXAMPLE_WIDGET
    QMAKE_BUNDLE_DATA += SAMPLE_WIDGET

    # Generate some files related to versions
    system(mkdir -p $$BUILD_DIR)
    system(printf \""$$OSX_VERSION"\" > $$BUILD_DIR/osx_version)
    system(printf "%02x%02x%02x%02x" `printf $$VERSION_RC | cut -d ',' -f 1` `printf $$VERSION_RC | cut -d ',' -f 2` `printf $$VERSION_RC | cut -d ',' -f 3` `printf $$VERSION_RC | cut -d ',' -f 4` | xxd -r -p > "$$VERSION_RC_PATH")

    ICON = resources/macx/Uniboard.icns
}

linux-g++ {
    QMAKE_CFLAGS += -fopenmp
    QMAKE_CXXFLAGS += -fopenmp
    QMAKE_LFLAGS += -fopenmp

    MSG_FILE.files = "msg.config"
    MSG_FILE.path = $$DESTDIR
    DOC.files = "doc"
    DOC.path = $$DESTDIR
    EXAMPLE_WIDGET.files = "example.wgt"
    EXAMPLE_WIDGET.path = $$DESTDIR
    SAMPLE_WIDGET.files = "sample.wgt"
    SAMPLE_WIDGET.path = $$DESTDIR

    system(mkdir -p $$BUILD_DIR)
    system(echo "$$VERSION" > $$BUILD_DIR/version)
}

linux-g++-64 {
    QMAKE_CFLAGS += -fopenmp
    QMAKE_CXXFLAGS += -fopenmp
    QMAKE_LFLAGS += -fopenmp

    MSG_FILE.files = "msg.config"
    MSG_FILE.path = $$DESTDIR
    DOC.files = "doc"
    DOC.path = $$DESTDIR
    EXAMPLE_WIDGET.files = "example.wgt"
    EXAMPLE_WIDGET.path = $$DESTDIR
    SAMPLE_WIDGET.files = "sample.wgt"
    SAMPLE_WIDGET.path = $$DESTDIR

    system(mkdir -p $$BUILD_DIR)
    system(echo "$$VERSION" > $$BUILD_DIR/version)
}

RESOURCES += \
    res.qrc
