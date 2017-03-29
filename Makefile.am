ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}

# Distribution
# ------------
EXTRA_DIST =
BUILT_SOURCES =
CLEANFILES =
DISTCLEANFILES =
MAINTAINERCLEANFILES = \
    $(GITIGNORE_MAINTAINERCLEANFILES_TOPLEVEL) \
    $(GITIGNORE_MAINTAINERCLEANFILES_M4_LIBTOOL) \
    $(GITIGNORE_MAINTAINERCLEANFILES_MAKEFILE_IN) \
    $(srcdir)/ChangeLog \
    $(srcdir)/INSTALL

# We're running CI in docker, which reports host architecture as host,
# not the docker image architecture as we expected, so specifying the
# host/build information is necessary here.
DISTCHECK_CONFIGURE_FLAGS = \
    --host=$(host_alias) \
    --build=$(build_alias) \
    --disable-dependency-tracking \
    --disable-maintainer-mode
if WITH_STAGE_2
DISTCHECK_CONFIGURE_FLAGS += \
    --with-stage=2
endif

GITIGNOREFILES = \
    build \
    debian \
    $(DIST_ARCHIVES)

# Common/global flags
# -------------------
AM_CPPFLAGS = \
    -include config.h \
    -DNDEBUG

AM_CFLAGS = \
    -Wall -Werror \
    -Wa,--noexecstack \
    -std=gnu11

AM_CXXFLAGS = \
    -Wall -Werror \
    -Wa,--noexecstack \
    -std=gnu++11

AM_LDFLAGS = \
    -Wl,-z,noexecstack \
    -Wl,--as-needed \
    -Wl,--no-undefined \
    -no-undefined

libtool_opts = \
    -version-info $(LT_VERSION_INFO) \
    -export-dynamic \
    -rpath $(libdir)

lib_LTLIBRARIES =
pkgconfig_DATA =

if WITH_STAGE_1
lib_LTLIBRARIES += \
    libandroid-gralloc-drm.la

libandroid_gralloc_drm_la_CPPFLAGS = \
    $(AM_CPPFLAGS) \
    $(LIBDRM_CFLAGS) \
    $(LOG_CFLAGS) \
    $(CUTILS_CFLAGS) \
    $(HARDWARE_CFLAGS) \
    $(HARDWARE_LEGACY_CFLAGS)
libandroid_gralloc_drm_la_CFLAGS = \
    $(AM_CFLAGS) \
    -std=c99 \
    -Wno-unused-variable \
    -Wno-unused-but-set-variable \
    $(PTHREAD_CFLAGS)
libandroid_gralloc_drm_la_SOURCES = \
    gralloc_drm.c \
    gralloc_drm_kms.c
libandroid_gralloc_drm_la_LDFLAGS = \
    $(AM_LDFLAGS) \
    $(libtool_opts)
libandroid_gralloc_drm_la_LIBADD = \
    -lm \
    $(PTHREAD_LIBS) \
    $(LIBDRM_LIBS) \
    $(LOG_LIBS) \
    $(CUTILS_LIBS) \
    $(HARDWARE_LEGACY_LIBS)

android_gralloc_drm_incdir = $(includedir)/android/gralloc-drm-$(LAARID_API_VERSION)
android_gralloc_drm_inc_HEADERS = \
    gralloc_drm.h \
    gralloc_drm_formats.h \
    gralloc_drm_handle.h

pkgconfig_DATA += \
    android-gralloc-drm-0.0.pc
else
if WITH_STAGE_2
hwlibdir = $(libdir)/hw
hwlib_LTLIBRARIES = gralloc.drm.la

gralloc_drm_la_CPPFLAGS = \
    $(AM_CPPFLAGS) \
    $(LIBDRM_CFLAGS) \
    $(LOG_CFLAGS) \
    $(CUTILS_CFLAGS) \
    $(HARDWARE_CFLAGS) \
    $(GRALLOC_DRM_CFLAGS) \
    $(GLESV1_CM_CFLAGS)
gralloc_drm_la_CFLAGS = \
    $(AM_CFLAGS) \
    $(PTHREAD_CFLAGS) \
    -Wno-int-conversion \
    -Wno-unused-variable
gralloc_drm_la_SOURCES = \
    gralloc.c \
    gralloc_drm_drivers.c
gralloc_drm_la_LDFLAGS = \
    $(AM_LDFLAGS) \
    -shared \
    -module \
    -avoid-version
gralloc_drm_la_LIBADD = \
    $(PTHREAD_LIBS) \
    $(LIBDRM_LIBS) \
    $(LOG_LIBS) \
    $(GRALLOC_DRM_LIBS) \
    $(GLESV1_CM_LIBS)

if ENABLE_FREEDRENO
gralloc_drm_la_SOURCES += gralloc_drm_freedreno.c
gralloc_drm_la_CPPFLAGS += -DENABLE_FREEDRENO $(FREEDRENO_CFLAGS)
gralloc_drm_la_LIBADD += $(FREEDRENO_LIBS)
endif
if ENABLE_INTEL
gralloc_drm_la_SOURCES += gralloc_drm_intel.c
gralloc_drm_la_CPPFLAGS += -DENABLE_INTEL $(INTEL_CFLAGS)
gralloc_drm_la_LIBADD += $(INTEL_LIBS)
endif
if ENABLE_NOUVEAU
gralloc_drm_la_SOURCES += gralloc_drm_nouveau.c
gralloc_drm_la_CPPFLAGS += -DENABLE_NOUVEAU $(NOUVEAU_CFLAGS)
gralloc_drm_la_LIBADD += $(NOUVEAU_LIBS)
endif
if ENABLE_RADEON
gralloc_drm_la_SOURCES += gralloc_drm_radeon.c
gralloc_drm_la_CPPFLAGS += -DENABLE_RADEON $(RADEON_CFLAGS)
gralloc_drm_la_LIBADD += $(RADEON_LIBS)
endif

endif # WITH_STAGE_2
endif # WITH_STAGE_1

# PKG-CONFIG data files
# ---------------------
pkgconfigdir = $(libdir)/pkgconfig
$(pkgconfig_DATA): config.status

EXTRA_DIST += $(pkgconfig_DATA:.pc=.pc.in)
DISTCLEANFILES += $(pkgconfig_DATA)

CLEANFILES += $(BUILT_SOURCES)
GITIGNOREFILES += $(BUILT_SOURCES)

-include $(top_srcdir)/git.mk
