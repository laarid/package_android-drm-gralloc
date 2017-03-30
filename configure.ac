# android-drm-gralloc versioning
# -------------------------
# To make new release:
#
#   laarid_micro_version += 1
#   laarid_interface_revision += 1
#
# if any functions have been added:
#
#   laarid_micro_version += 1
#   laarid_interface_revision = 0
#
#   or
#
#   laarid_minor_version += 1
#   laarid_micro_version = 0
#   laarid_interface_revision = 0
#
# if backwards compatibility has been broken:
#
#   laarid_major_version += 1
#   laarid_minor_version = 0
#   laarid_micro_version = 0
#   laarid_interface_revision = 0
m4_define([laarid_major_version], [0])
m4_define([laarid_minor_version], [0])
m4_define([laarid_micro_version], [3])
m4_define([laarid_interface_revision], [0])
m4_define([laarid_api_version], [laarid_major_version.0])
m4_define([laarid_release_number], [laarid_major_version])
m4_define([laarid_version],
  [laarid_major_version.laarid_minor_version.laarid_micro_version])

# Require autoconf 2.69 for aarch64
AC_PREREQ(2.69)

AC_INIT([android-drm-gralloc], [laarid_version],
  [https://github.com/laarid/package_android-drm-gralloc/issues], [],
  [https://github.com/laarid/package_android-drm-gralloc])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_SRCDIR([gralloc_drm.h])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])

AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects no-define tar-ustar])
AM_MAINTAINER_MODE([enable])
AM_SILENT_RULES([yes])

LAARID_MAJOR_VERSION=laarid_major_version
LAARID_MINOR_VERSION=laarid_minor_version
LAARID_MICRO_VERSION=laarid_micro_version
LAARID_API_VERSION=laarid_api_version
LAARID_RELEASE_NUMBER=laarid_release_number
AC_SUBST([LAARID_MAJOR_VERSION])
AC_SUBST([LAARID_MINOR_VERSION])
AC_SUBST([LAARID_MICRO_VERSION])
AC_SUBST([LAARID_API_VERSION])
AC_SUBST([LAARID_RELEASE_NUMBER])

# Define _GNU_SOURCE etc. where appropriate, e.g. for strptime() on glibc
AC_USE_SYSTEM_EXTENSIONS

# Large file support
# ------------------
AC_SYS_LARGEFILE

# Host environment
# ----------------
AC_CANONICAL_HOST

AM_PROG_AR
AM_PROG_AS
AC_PROG_CC
AC_PROG_CXX

# libtool versioning
# ------------------
LT_INIT

LT_RELEASE="laarid_api_version"
AC_SUBST(LT_RELEASE)

m4_define([laarid_binary_age], [m4_eval(100 * laarid_minor_version + laarid_micro_version)])
m4_define([lt_current], [m4_eval(laarid_binary_age - laarid_interface_revision)])
m4_define([lt_revision], [laarid_interface_revision])
# Whenever backward compatibility is broken, we generate libraries of different
# release number. That follows the library files of the same release are always
# compatible, and they have their lt_age equal to lt_current.
m4_define([lt_age], [lt_current])
LT_VERSION_INFO="lt_current:lt_revision:lt_age"
AC_SUBST(LT_VERSION_INFO)

# System dlloader
# ---------------
LT_LIB_DLLOAD

LT_OUTPUT

# Thread support
# --------------
AX_PTHREAD

# Build stages
# ------------
# Android source code might have circular dependency between two or more
# source repositories, so spliting them into some stages is necessary. Here
# we have currently stage 1 and 2 only.
AC_ARG_WITH([stage],
  [AS_HELP_STRING([--with-stage], [Stage to build with @<:@default=1@:>@])],
  [AS_CASE([${with_stage}],
    [1|2], [AC_MSG_RESULT([Building for stage ${with_stage}])],
    [AC_MSG_ERROR([Unknown stage "${with_stage}"])])],
  [with_stage=1])
AM_CONDITIONAL([WITH_STAGE_1], [test "x${with_stage}" = x1])
AM_CONDITIONAL([WITH_STAGE_2], [test "x${with_stage}" = x2])

# Required libraries
# ------------------

LIBDRM_REQUIRED_VERSION=2.4.66
LIBDRM_FREEDRENO_REQUIRED_VERSION=2.4.68
LIBDRM_INTEL_REQUIRED_VERSION=2.4.61
LIBDRM_NOUVEAU_REQUIRED_VERSION=2.4.66
LIBDRM_RADEON_REQUIRED_VERSION=2.4.56
SYSTEMCORE_API_VERSION=0.0
SYSTEMCORE_REQUIRED_VERSION=0.0.16
HARDWARE_API_VERSION=0.0
HARDWARE_REQUIRED_VERSION=0.0.1
HARDWARE_LEGACY_API_VERSION=0.0
HARDWARE_LEGACY_REQUIRED_VERSION=0.0.1
AC_SUBST([LIBDRM_REQUIRED_VERSION])
AC_SUBST([LIBDRM_FREEDRENO_REQUIRED_VERSION])
AC_SUBST([LIBDRM_INTEL_REQUIRED_VERSION])
AC_SUBST([LIBDRM_NOUVEAU_REQUIRED_VERSION])
AC_SUBST([LIBDRM_RADEON_REQUIRED_VERSION])
AC_SUBST([SYSTEMCORE_API_VERSION])
AC_SUBST([SYSTEMCORE_REQUIRED_VERSION])
AC_SUBST([HARDWARE_API_VERSION])
AC_SUBST([HARDWARE_REQUIRED_VERSION])
AC_SUBST([HARDWARE_LEGACY_API_VERSION])
AC_SUBST([HARDWARE_LEGACY_REQUIRED_VERSION])

PKG_CHECK_MODULES([LIBDRM], [libdrm >= $LIBDRM_REQUIRED_VERSION])
PKG_CHECK_MODULES([LOG], [android-log-$SYSTEMCORE_API_VERSION >= $SYSTEMCORE_REQUIRED_VERSION])
PKG_CHECK_MODULES([CUTILS], [android-cutils-$SYSTEMCORE_API_VERSION >= $SYSTEMCORE_REQUIRED_VERSION])
PKG_CHECK_MODULES([HARDWARE], [android-hardware-$HARDWARE_API_VERSION >= $HARDWARE_REQUIRED_VERSION])

AS_IF([test "x${with_stage}" = x1],
  [PKG_CHECK_MODULES([HARDWARE_LEGACY],
    [android-hardware-legacy-$HARDWARE_LEGACY_API_VERSION >= $HARDWARE_LEGACY_REQUIRED_VERSION])],

  [PKG_CHECK_MODULES([GRALLOC_DRM], [android-gralloc-drm-$LAARID_API_VERSION])
  PKG_CHECK_MODULES([GLESV1_CM], [glesv1_cm])

  PKG_CHECK_MODULES([FREEDRENO], [libdrm_freedreno >= $LIBDRM_FREEDRENO_REQUIRED_VERSION],,[# no op])
  PKG_CHECK_MODULES([INTEL], [libdrm_intel >= $LIBDRM_INTEL_REQUIRED_VERSION],,[# no op])
  PKG_CHECK_MODULES([NOUVEAU], [libdrm_nouveau >= $LIBDRM_NOUVEAU_REQUIRED_VERSION],,[# no op])
  PKG_CHECK_MODULES([RADEON], [libdrm_radeon >= $LIBDRM_RADEON_REQUIRED_VERSION],,[# no op])
  ])
AM_CONDITIONAL([ENABLE_FREEDRENO], [test -n "${pkg_cv_FREEDRENO_LIBS}"])
AM_CONDITIONAL([ENABLE_INTEL], [test -n "${pkg_cv_INTEL_LIBS}"])
AM_CONDITIONAL([ENABLE_NOUVEAU], [test -n "${pkg_cv_NOUVEAU_LIBS}"])
AM_CONDITIONAL([ENABLE_RADEON], [test -n "${pkg_cv_RADEON_LIBS}"])

AC_CONFIG_FILES([
  Makefile
])
AS_IF([test "x${with_stage}" = x1],
  [AC_CONFIG_FILES(
    [android-gralloc-drm-$LAARID_API_VERSION.pc])
  ])
AC_OUTPUT
