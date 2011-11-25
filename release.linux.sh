make clean
rm -rf build/linux/release/
rm -rf install

QT_PATH="/usr/local/Trolltech/Qt-4.7.3"
PLUGINS_PATH="$QT_PATH/plugins"
QMAKE_PATH="$QT_PATH/bin/qmake"

if [ ! -e "$QMAKE_PATH" ]; then
	echo "qmake command not found at $QMAKE_PATH"
	exit 1
fi

if [ ! -e "$PLUGINS_PATH" ]; then
	echo "plugins path not found at $PLUGINS_PATH"
	exit 1
fi
	
$QMAKE_PATH -spec linux-g++

make -j 2 release

VERSION=`cat build/linux/release/version`
if [ ! -f build/linux/release/version ]; then
	echo "version not found"
	exit 1
fi

cp resources/linux/run.sh build/linux/release/product
chmod +x build/linux/release/product/run.sh

#copying plugins
cp -R $PLUGINS_PATH build/linux/release/product/
#removing debug version
find build/linux/release/product/ -name *.debug -exec rm {} \;

#copying custom qt libraries
QT_LIBRARY_DEST_PATH="build/linux/release/product/qtlib"
mkdir $QT_LIBRARY_DEST_PATH
QT_LIBRARY_SOURCE_PATH="$QT_PATH/lib"

copyQtLibrary(){
    if [ ! -e "$QT_LIBRARY_SOURCE_PATH/$1.so.4" ]; then
        echo "library not found: $QT_LIBRARY_SOURCE_PATH"
        exit 1;
    fi
    cp "$QT_LIBRARY_SOURCE_PATH/$1.so.4" "$QT_LIBRARY_DEST_PATH/"
    cp "$QT_LIBRARY_SOURCE_PATH/$1.so.4.7.3" "$QT_LIBRARY_DEST_PATH/"
}

copyQtLibrary libQtWebKit
copyQtLibrary libphonon
copyQtLibrary libQtDBus
copyQtLibrary libQtScript
copyQtLibrary libQtSvg
copyQtLibrary libQtXmlPatterns
copyQtLibrary libQtNetwork
copyQtLibrary libQtXml
copyQtLibrary libQtGui
copyQtLibrary libQtCore

rm -rf install/linux
mkdir -p install/linux

#add some dependencies
cp msg.config build/linux/release/product/msg.config
cp -r doc build/linux/release/product/doc
cp -r example.wgt build/linux/release/product/example.wgt
cp -r sample.wgt build/linux/release/product/sample.wgt

mv build/linux/release/product build/linux/release/SankoreAppToolkit.$VERSION
cd build/linux/release

tar cvzf ../../../install/linux/SankoreAppToolkit.tar.gz SankoreAppToolkit.$VERSION -C . 
echo "Build Finished";

