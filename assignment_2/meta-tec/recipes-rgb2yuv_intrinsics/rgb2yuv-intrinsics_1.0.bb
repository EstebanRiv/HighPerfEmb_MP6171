# Disable license checks
LICENSE = "CLOSED"

# List of source files to build the package
SRC_URI += "file://rgb2yuv_intrinsics-1.0.tar.gz"

# Extracted folder name
S = "${WORKDIR}/rgb2yuv_intrinsics-1.0"

# Execute the external recipe to install the documentation
DEPENDS += "intrinsics-doc"

# Use autotools to configure/compile/install/deploy the package
inherit autotools