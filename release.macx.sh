#!/bin/bash

# Executables
QMAKE="/usr/bin/qmake"
MACDEPLOYQT="/usr/bin/macdeployqt"
DMGUTIL="`pwd`/thirdparty/dmgutil/dmgutil.pl"
DSYMUTIL=/usr/bin/dsymutil
STRIP=/usr/bin/strip
PLISTBUDDY=/usr/libexec/PlistBuddy

# Directories
BUILD_DIR="build/macx/release"
PRODUCT_DIR="$BUILD_DIR/product"

function notify {
    GROWLNOTIFY=`which growlnotify`
    if [ -x "$GROWLNOTIFY" ]; then
        $GROWLNOTIFY --name sankoreapptoolkit-build --iconpath /Developer/Applications/Xcode.app --message "$1" "SankoreAppToolkit"
    fi
    printf "\033[32m--->\033[0m $1\n"
}

function abort {
    printf "\033[31merror:\033[0m $1\n"
    exit 1
}

function warn {
    abort "$1"
}

function checkExecutable {
    if [ ! -x "$1" ]; then
        abort "$1 not found"
    fi
}

trap "defaults write com.mnemis.SankoreAppToolkit.release Running -bool NO" EXIT

notify "Running SankoreAppToolkit release script (`date`)"

script_is_running=`defaults read com.mnemis.SankoreAppToolkit.release Running 2>/dev/null`
if [[ $? -eq 0 ]] && [[ "$script_is_running" = "1" ]]; then
    trap EXIT
    abort "another release script already running"
fi
defaults write com.mnemis.SankoreAppToolkit.release Running -bool YES

# Check for executables
checkExecutable "$QMAKE"
checkExecutable "$MACDEPLOYQT"
checkExecutable "$DMGUTIL"
checkExecutable "$DSYMUTIL"
checkExecutable "$STRIP"
checkExecutable "$PLISTBUDDY"


# delete the build directory
notify "Cleaning ..."
rm -rf "$BUILD_DIR"

# generate Makefiles
notify "Generating Makefile ..."

QMAKE_CMD="$QMAKE -spec macx-g++"

$QMAKE_CMD

# build
notify "Compiling ..."
make -j2 release

NAME="SankoreAppToolkit"

DMG="$NAME.dmg"
VOLUME="/Volumes/$NAME"
APP="$PRODUCT_DIR/SankoreAppToolkit.app"
DSYM_NAME="$NAME (r$SVN_REVISION).dSYM"
DSYM="$PRODUCT_DIR/$DSYM_NAME"
GSYM_i386="$PRODUCT_DIR/$NAME i386.sym"
INFO_PLIST="$APP/Contents/Info.plist"

rm -f "$APP/Contents/Resources/empty.lproj"

# set various version infomration in Info.plist
#$PLISTBUDDY -c "Set :CFBundleVersion $SVN_REVISION" "$INFO_PLIST"
#$PLISTBUDDY -c "Set :CFBundleShortVersionString $VERSION" "$INFO_PLIST"
$PLISTBUDDY -c "Set :CFBundleGetInfoString $NAME" "$INFO_PLIST"

# bundle Qt Frameworks into the app bundle
notify "Bulding frameworks ..."
$MACDEPLOYQT "$APP"

notify "Extracting debug information ..."
$DSYMUTIL "$APP/Contents/MacOS/SankoreAppToolkit" -o "$DSYM"
$STRIP -S "$APP/Contents/MacOS/SankoreAppToolkit"

notify "Creating dmg ..."
umount "$VOLUME" 2> /dev/null
$DMGUTIL --open --volume="$NAME" "$DMG"

cp -R "$APP" "$VOLUME"
ln -s /Applications "$VOLUME"

# add the next line to the first DMGUTIL command down here (after the icon). Don't forget to remove 'uniboard' from the bg picture
#--background=resources/macx/UniboardDmgBackground.png
$DMGUTIL --set --iconsize=96 --toolbar=false --icon=resources/macx/SankoreAppToolkitDmg.icns --background=resources/macx/UniboardDmgBackground.png "$VOLUME"
$DMGUTIL --set --x=20 --y=60 --width=580 --height=312 "$VOLUME"
$DMGUTIL --set --x=180 --y=160 "$VOLUME/`basename \"$APP\"`"
$DMGUTIL --set --x=400 --y=160 "$VOLUME/Applications"

$DMGUTIL --close --volume="$NAME" "$DMG"

notify "$NAME is built"

PRODUCT_DIR="install/mac/"

if [ ! -d "${PRODUCT_DIR}" ]; then
    mkdir -p "${PRODUCT_DIR}"
fi

mv "$DMG" "${PRODUCT_DIR}"

exit 0

