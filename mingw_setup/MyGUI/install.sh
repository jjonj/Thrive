#!/bin/bash

. utils.sh

MINGW_ENV=$1

WORKING_DIR=$MINGW_ENV/temp/MyGUI 

REMOTE_DIR="http://sourceforge.net/projects/my-gui/files/MyGUI/MyGUI_3.2.0/"

ARCHIVE="MyGUI_3.2.0.zip"

TOOLCHAIN_FILE=$MINGW_ENV/cmake/toolchain.cmake

mkdir -p $WORKING_DIR

################################################################################
# Download
################################################################################

if [ -e $WORKING_DIR/$ARCHIVE ]; then
    echo "OGRE archive file found, skipping download. If you want to redownload it, please delete ${WORKING_DIR}/${ARCHIVE}."
else
    echo "Downloading MyGUI"
    wget -O $WORKING_DIR/$ARCHIVE $REMOTE_DIR/$ARCHIVE
    if [ $? -gt 0 ]; then
        echo "Error downloading MyGUI archive. Aborting."
        rm $WORKING_DIR/$ARCHIVE
        exit 1
    fi
fi


################################################################################
# Unpack, compile, install
################################################################################

# untar the sources
7za x -y -o$WORKING_DIR $WORKING_DIR/$ARCHIVE
if [ $? -ne 0 ]; then
    echo "Could not unpack MyGUI If the archive is corrupted, delete 

       $WORKING_DIR/$ARCHIVE 

to force a new download.
    "
fi

mkdir -p $WORKING_DIR/build
cd $WORKING_DIR/build
cmake \
    -DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN_FILE \
    -DOGRE_SOURCE=$MINGW_ENV/OgreSDK \
    -DBoost_DIR=$MINGW_ENV/install \
    -DBOOST_ROOT=$MINGW_ENV/install \
    -DCMAKE_INSTALL_PREFIX=$MINGW_ENV/install \
    -DMYGUI_BUILD_DEMOS=false \
    -DMYGUI_BUILD_TOOLS=false \
    -DMYGUI_DEPENDENCIES_DIR=$MINGW_ENV/install \
    $WORKING_DIR/MyGUI_3.2.0
make -j4 && make install

