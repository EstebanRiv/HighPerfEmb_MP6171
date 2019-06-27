SUMMARY = "bitbake-layers recipe"
DESCRIPTION = "Recipe created by bitbake-layers"
LICENSE = "MIT"

# Inherit/require console-image
require ../meta-rpi/images/console-image.bb

# Custom image name
export IMAGE_BASENAME = "tec-image"

# Add to tec-image the following recipes
IMAGE_INSTALL += "rgb2yuv-c rgb2yuv-intrinsics"