SUMMARY = "Utility to convert .rgb files to .yuv, optimized with NEON"
# Disable license checks
LICENSE = "CLOSED"

# List of source files to build the package
SRC_URI += "file://rgb2yuv_intrinsics-1.0/autogen.sh \
            file://rgb2yuv_intrinsics-1.0/configure.ac \
            file://rgb2yuv_intrinsics-1.0/Makefile.am \
            file://rgb2yuv_intrinsics-1.0/rgb2yuv_intrinsics.c "

# Extracted folder name
S = "${WORKDIR}/rgb2yuv_intrinsics-1.0"

# Execute the external recipe to install the documentation
DEPENDS += "intrinsics-doc"

# Use autotools to configure/compile/install/deploy the package
inherit autotools