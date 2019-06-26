# This is a workaround to force install the documentation.
# Autotools understood the command to install the documentation, but for unknown
# reasons it cannot create folder /usr/share/doc in image rootfs.

SUMMARY = "Recipe to copy the rgb2yuv-c binary documentation to /usr/share folder"
# Disable license checks
LICENSE = "CLOSED"

# List of source files to build the package
SRC_URI += "file://rgb2yuv_c_doc-1.0/c_results.pdf \
            file://rgb2yuv_c_doc-1.0/image.bgr \
            file://rgb2yuv_c_doc-1.0/output.yuv "

# Extracted folder name
S = "${WORKDIR}/rgb2yuv_c_doc-1.0"

# Install the documentation
do_install () {
    install -d ${D}/usr/share/rgb2yuv_c
    install -m 0755 ${S}/c_results.pdf ${D}/usr/share/rgb2yuv_c
    install -m 0755 ${S}/image.bgr ${D}/usr/share/rgb2yuv_c
    install -m 0755 ${S}/output.yuv ${D}/usr/share/rgb2yuv_c
}

# Add documentation to image file
FILES_${PN} += "/usr/share/rgb2yuv_c"