param(
    [string]$MINGW_ENV
)

#########
# MyGUI #
#########

Write-Output "--- Installing MyGUI ---"

$DIR = Split-Path $MyInvocation.MyCommand.Path

#################
# Include utils #
#################

. (Join-Path "$DIR\.." "utils.ps1")


############################
# Create working directory #
############################

$WORKING_DIR = Join-Path $MINGW_ENV temp\MyGUI

mkdir $WORKING_DIR -force | out-null


###################
# Check for 7-Zip #
###################

$7z = Join-Path $MINGW_ENV "temp\7zip\7za.exe"

if (-Not (Get-Command $7z -errorAction SilentlyContinue))
{
    return $false
}


####################
# Download archive #
####################

$REMOTE_DIR="http://sourceforge.net/projects/my-gui/files/MyGUI/MyGUI_3.2.0/"

$ARCHIVE="MyGUI_3.2.0.zip"

$DESTINATION = Join-Path $WORKING_DIR $ARCHIVE

if (-Not (Test-Path $DESTINATION)) {
    Write-Output "Downloading archive..."
    $CLIENT = New-Object System.Net.WebClient
    $CLIENT.DownloadFile("$REMOTE_DIR/$ARCHIVE", $DESTINATION)
}
else {
    Write-Output "Found archive file, skipping download."
}

##########
# Unpack #
##########

Write-Output "Unpacking archive..."

$ARGUMENTS = "x",
             "-y",
             "-o$WORKING_DIR",
             $DESTINATION
             
& $7z $ARGUMENTS


###########
# Compile #
###########

Write-Output "Compiling..."

$TOOLCHAIN_FILE="$MINGW_ENV/cmake/toolchain.cmake"

$BUILD_DIR = Join-Path $WORKING_DIR "build"

mkdir $BUILD_DIR -force

pushd $WORKING_DIR/build

$env:Path += (Join-Path $MINGW_ENV bin) + ";"

$ARGUMENTS =
    "-DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN_FILE",
    "-DOGRE_SOURCE=$MINGW_ENV/OgreSDK",
    "-DBoost_DIR=$MINGW_ENV/install",
    "-DBOOST_ROOT=$MINGW_ENV/install",
    "-DCMAKE_INSTALL_PREFIX=$MINGW_ENV/install",
    "-DMYGUI_BUILD_DEMOS=false",
    "-DMYGUI_BUILD_TOOLS=false",
    "-DMYGUI_DEPENDENCIES_DIR=$MINGW_ENV/install",
    "$WORKING_DIR/MyGUI_3.2.0"

& cmake -G "MinGW Makefiles" $ARGUMENTS

& $MINGW_ENV/bin/mingw32-make -j4 install

popd
