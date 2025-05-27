DESCRIPTION = "The library to communicate with Kandou kb900x retimer."
SUMMARY = "kb900x Retimer Library"
HOMEPAGE = "https://www.kandou.ai/"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/files/common-licenses/Apache-2.0;md5=89aea4e17d99a7cacdbeed46a0096b10"
PV = "0.3.0"

# Set default value (can be overridden in a .bbappend or local.conf)
KB900X_BIC_COMMUNICATION ??= "True"

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

DEPENDS += ""

RDEPENDS:${PN} += ""

python __anonymous() {
    bic_comm = d.getVar('KB900X_BIC_COMMUNICATION')
    if bic_comm == "True":
        deps = d.getVar('DEPENDS') or ""
        rdeps = d.getVar('RDEPENDS') or ""
        meson_opts = d.getVar("EXTRA_OEMESON") or ""

        deps += " libbic"
        rdeps += " libbic"
        meson_opts += " -Dbic_communication=true"

        d.setVar('DEPENDS', deps)
        d.setVar('RDEPENDS', rdeps)
        d.setVar("EXTRA_OEMESON", meson_opts)
}
