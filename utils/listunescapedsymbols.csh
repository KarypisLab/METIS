#!/bin/sh

nm *.o | grep " T " | egrep -v '(libmetis|METIS|__)' | awk '{printf("#define %s libmetis__%s\n",$3, $3)}'

