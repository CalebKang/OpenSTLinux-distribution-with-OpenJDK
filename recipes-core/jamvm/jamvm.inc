SUMMARY = "A compact Java Virtual Machine which conforms to the JVM specification version 2."
HOMEPAGE = "http://jamvm.sourceforge.net/"
LICENSE = "GPL-2.0"
LIC_FILES_CHKSUM = "file://COPYING;md5=0636e73ff0215e8d672dc4c32c317bb3"

DEPENDS = "zlib classpath virtual/javac-native libffi virtual/java-initial-native"
DEPENDS_class-native = "virtual/java-initial-native classpath-initial-native zlib-native classpath-native ecj-initial-native libffi-native"

RDEPENDS_${PN} = "classpath"
RDEPENDS_${PN}_class-native = ""

PROVIDES_class-native = "virtual/java-native"
RPROVIDES_${PN} = "java2-runtime"
RPROVIDES_${PN}_class-native = ""

BBCLASSEXTEND = "native"

inherit java autotools update-alternatives pkgconfig relative_symlinks distro_features_check

REQUIRED_DISTRO_FEATURES_class-target = "x11"

# This uses 32 bit arm, so force the instruction set to arm, not thumb
ARM_INSTRUCTION_SET = "arm"

do_configure_append_class-native() {
	sed -i -e"s|STAGING_LIBDIR_NATIVE|${STAGING_LIBDIR_NATIVE}|g" \
		-e "s|STAGING_DATADIR_NATIVE|${STAGING_DATADIR_NATIVE}|g" \
	${WORKDIR}/java
}


EXTRA_OECONF = "--with-classpath-install-dir=${prefix} --libdir=${libdir}/jamvm --enable-ffi --disable-int-inlining"

# Needed for big compilation targets like OpenJDK
CFLAGS_append_class-native = " -DDEFAULT_MAX_HEAP=1024*MB"

# Enforce usage of ecj-initial.
EXTRA_OEMAKE = "JAVAC=${STAGING_BINDIR_NATIVE}/ecj-initial \
                GLIBJ_ZIP=${STAGING_DATADIR_NATIVE}/classpath/glibj.zip \
               "

do_install_append_class-native() {
	install -d ${D}${bindir}
	install -m 0755 ${WORKDIR}/java ${D}${bindir}/java

}

PRIVATE_LIBS = "libjvm.so"
INSANE_SKIP_${PN} = "dev-so"

FILES_${PN} += "${libdir}/jamvm/lib*.so"

ALTERNATIVE_${PN} = "java"
ALTERNATIVE_TARGET = "${bindir}/jamvm"
ALTERNATIVE_PRIORITY = "4"
# shared state for jamvm-native does not work
# since the paths are hardcoded
#SSTATE_MIRRORS_class-native = ""
export LD_LIBRARY_PATH
export BOOTCLASSPATH

