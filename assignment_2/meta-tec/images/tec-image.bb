SUMMARY = "bitbake-layers recipe"
DESCRIPTION = "Recipe created by bitbake-layers"
LICENSE = "MIT"

#require ${HOME}/rpi/meta-rpi/images/console-image.bb
require ../meta-rpi/images/console-image.bb

export IMAGE_BASENAME = "tec-image"
