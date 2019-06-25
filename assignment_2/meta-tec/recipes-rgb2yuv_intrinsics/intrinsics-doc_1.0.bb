# This is a workaround to force install the documentation.
# Autotools understood the command to install the documentation, but for unknown
# reasons it cannot create folder /usr/share/doc in image rootfs.

# Disable license checks
LICENSE = "CLOSED"

# List of source files to build the package
SRC_URI += "file://rgb2yuv_intrinsics_doc-1.0/rgb2yuv_intrinsics.pdf \
            file://rgb2yuv_intrinsics_doc-1.0/image.bgr \
            file://rgb2yuv_intrinsics_doc-1.0/output.yuv "

# Extracted folder name
S = "${WORKDIR}/rgb2yuv_intrinsics_doc-1.0"

# Install the documentation
do_install () {
    install -d ${D}/usr/share/rgb2yuv_intrinsics
    install -m 0755 ${S}/rgb2yuv_intrinsics.pdf ${D}/usr/share/rgb2yuv_intrinsics
    install -m 0755 ${S}/image.bgr ${D}/usr/share/rgb2yuv_intrinsics
    install -m 0755 ${S}/output.yuv ${D}/usr/share/rgb2yuv_intrinsics
}

# Add documentation to image file
FILES_${PN} += "/usr/share/rgb2yuv_intrinsics"