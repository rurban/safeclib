SUBDIRS = src . tests

# Additional things to distribute
EXTRA_DIST = safec.pc.in

## Install the generated pkg-config file (.pc) into the expected location for
## architecture-dependent package configuration information.  Occasionally,
## pkg-config files are also used for architecture-independent data packages,
## in which case the correct install location would be $(datadir)/pkgconfig.
#pkgconfigdir = $(libdir)/pkgconfig
#pkgconfig_DATA = safec-$(SAFEC_API_VERSION).pc
# think we need to be architecture-independent
pcdatadir = $(libdir)/pkgconfig
pcdata_DATA = safec-$(SAFEC_API_VERSION).pc

ACLOCAL_AMFLAGS = -I m4
