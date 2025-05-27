DESCRIPTION = "The library to communicate with Kandou kb900x retimer."
SUMMARY = "kb900x Retimer Library"
HOMEPAGE = "https://www.kandou.ai/"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/files/common-licenses/Apache-2.0;md5=89aea4e17d99a7cacdbeed46a0096b10"

S = "${WORKDIR}/sources"
UNPACKDIR="${S}"

SRC_URI = "\
    file://meson.build \
    file://meson_options.txt \
    file://kb900x_addresses.h \
    file://kb900x_bic_comm.c \
    file://kb900x_bic_comm.h \
    file://kb900x_eeprom.c \
    file://kb900x_eeprom.h \
    file://kb900x_i2c_comm.c \
    file://kb900x_i2c_comm.h \
    file://kb900x_i2c_master.c \
    file://kb900x_i2c_master.h \
    file://kb900x_log.h \
    file://kb900x_log.c \
    file://kb900x_reg_dump.h \
    file://kb900x_smbus_comm.c \
    file://kb900x_smbus_comm.h \
    file://kb900x_utils.c \
    file://kb900x_utils.h \
    file://kb900x.c \
    file://kb900x.h \
    "

inherit meson pkgconfig

DEPENDS += "libbic"

RDEPENDS:${PN} += "libbic"

EXTRA_OEMESON += "-Dbic_communication=true"
