SUMMARY = "rgb2yuv-c recipe"
DESCRIPTION = "Utility to convert .rgb files to .yuv"
# Disable license checks
LICENSE = "CLOSED"

# List of source files to build the package
SRC_URI += "file://rgb2yuv_c-1.0/autogen.sh \
            file://rgb2yuv_c-1.0/configure.ac \
            file://rgb2yuv_c-1.0/Makefile.am \
            file://rgb2yuv_c-1.0/rgb2yuv_c.c \
            file://rgb2yuv_c_doc-1.0/c_results.pdf \
            file://rgb2yuv_c_doc-1.0/image.bgr \
            file://rgb2yuv_c_doc-1.0/output.yuv "

# Extracted folders names
S = "${WORKDIR}/rgb2yuv_c-1.0"
DOCS = "${WORKDIR}/rgb2yuv_c_doc-1.0"

# Install the documentation
do_install_append () {
    install -d ${D}/usr/share/rgb2yuv_c
    install -m 0755 ${DOCS}/c_results.pdf ${D}/usr/share/rgb2yuv_c
    install -m 0755 ${DOCS}/image.bgr ${D}/usr/share/rgb2yuv_c
    install -m 0755 ${DOCS}/output.yuv ${D}/usr/share/rgb2yuv_c
}

# Add documentation to image file
FILES_${PN} += "/usr/share/rgb2yuv_c"

# Use autotools to configure/compile/install/deploy the package
inherit autotools