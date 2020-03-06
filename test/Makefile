include ../Makefile.in

CURBUILDDIR = $(TSTBUILDDIR)


MTESTSRC   = mtest.c 

MTESTOBJS  = $(patsubst %.c, $(CURBUILDDIR)/%$(OBJEXT), $(MTESTSRC))
EXTRAOBJS  = $(PRGBUILDDIR)/io$(OBJEXT)

ALLOBJS  = $(patsubst %.c, $(CURBUILDDIR)/%$(OBJEXT), $(wildcard *.c))
HEADERS  = $(wildcard *.h) $(TOPDIR)/include/metis.h 

LIBRARIES = $(BUILDDIR)/libmetis$(LIBEXT)

INCLUDES += -I$(TOPDIR)/libmetis

TARGETS = $(BUILDDIR)/mtest$(VERNUM)$(EXEEXT) 


default: $(TARGETS)


$(BUILDDIR)/mtest$(VERNUM)$(EXEEXT): $(MTESTOBJS) $(LIBRARIES)
	$(LD) $(LDOPTIONS) $(EXEOUTPUTFILE) $(MTESTOBJS) $(EXTRAOBJS) $(LIBSDIR) $(LIBS)
	chmod 744 $@
	@if [ "$(BINDIR)" ]; then cp $@* $(BINDIR); fi


clean:
	rm -f $(ALLOBJS) 

realclean:
	rm -f $(ALLOBJS) $(TARGETS)
	rm -fr $(BUILDDIR) 



$(ALLOBJS) : $(HEADERS) ../Makefile.in Makefile

$(CURBUILDDIR)/%$(OBJEXT) : %.c
	$(CC) $(CFLAGS) $(SOURCEFILE) $(OUTPUTFILE)


