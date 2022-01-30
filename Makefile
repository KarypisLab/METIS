# Configuration options.
i64        = not-set
r64        = not-set
gdb        = not-set
assert     = not-set
assert2    = not-set
debug      = not-set
gprof      = not-set
valgrind   = not-set
openmp     = not-set
shared     = not-set
cc         = not-set
prefix     = ~/local
gklib_path = ~/local


# Basically proxies everything to the builddir cmake.

BUILDDIR = build

IDXWIDTH  = "\#define IDXTYPEWIDTH 32"
REALWIDTH = "\#define REALTYPEWIDTH 32"

# Process configuration options.
CONFIG_FLAGS = -DCMAKE_VERBOSE_MAKEFILE=1
ifneq ($(gklib_path), not-set)
    CONFIG_FLAGS += -DGKLIB_PATH=$(abspath $(gklib_path))
endif
ifneq ($(prefix), not-set)
    CONFIG_FLAGS += -DCMAKE_INSTALL_PREFIX=$(prefix)
endif
ifneq ($(i64), not-set)
    IDXWIDTH  = "\#define IDXTYPEWIDTH 64"
endif
ifneq ($(r64), not-set)
    REALWIDTH = "\#define REALTYPEWIDTH 64"
endif
ifneq ($(gdb), not-set)
    CONFIG_FLAGS += -DGDB=$(gdb)
endif
ifneq ($(assert), not-set)
    CONFIG_FLAGS += -DASSERT=$(assert)
endif
ifneq ($(assert2), not-set)
    CONFIG_FLAGS += -DASSERT2=$(assert2)
endif
ifneq ($(debug), not-set)
    CONFIG_FLAGS += -DDEBUG=$(debug)
endif
ifneq ($(gprof), not-set)
    CONFIG_FLAGS += -DGPROF=$(gprof)
endif
ifneq ($(valgrind), not-set)
    CONFIG_FLAGS += -DVALGRIND=$(valgrind)
endif
ifneq ($(openmp), not-set)
    CONFIG_FLAGS += -DOPENMP=$(openmp)
endif
ifneq ($(shared), not-set)
    CONFIG_FLAGS += -DSHARED=1
endif
ifneq ($(cc), not-set)
    CONFIG_FLAGS += -DCMAKE_C_COMPILER=$(cc)
endif

VERNUM=5.1.0
PKGNAME=metis-$(VERNUM)

define run-config
mkdir -p $(BUILDDIR)
mkdir -p $(BUILDDIR)/xinclude
echo $(IDXWIDTH) > $(BUILDDIR)/xinclude/metis.h
echo $(REALWIDTH) >> $(BUILDDIR)/xinclude/metis.h
cat include/metis.h >> $(BUILDDIR)/xinclude/metis.h
cp include/CMakeLists.txt $(BUILDDIR)/xinclude
cd $(BUILDDIR) && cmake $(CURDIR) $(CONFIG_FLAGS)
endef

all clean install:
	@if [ ! -f $(BUILDDIR)/Makefile ]; then \
		more BUILD.txt; \
	else \
	  	make -C $(BUILDDIR) $@ $(MAKEFLAGS); \
	fi

uninstall:
	xargs rm < $(BUILDDIR)/install_manifest.txt

config: distclean
	$(run-config)

distclean:
	rm -rf $(BUILDDIR)

remake:
	find . -name CMakeLists.txt -exec touch {} ';'

dist:
	utils/mkdist.sh $(PKGNAME)

.PHONY: config distclean all clean install uninstall remake dist
