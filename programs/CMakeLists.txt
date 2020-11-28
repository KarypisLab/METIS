# These programs use internal metis data structures.
include_directories(../libmetis)

# Build program.
add_executable(gpmetis gpmetis.c cmdline_gpmetis.c io.c stat.c)
add_executable(ndmetis ndmetis.c cmdline_ndmetis.c io.c smbfactor.c)
add_executable(mpmetis mpmetis.c cmdline_mpmetis.c io.c stat.c)
add_executable(m2gmetis m2gmetis.c cmdline_m2gmetis.c io.c)
add_executable(graphchk graphchk.c io.c)
add_executable(cmpfillin cmpfillin.c io.c smbfactor.c)

# Link with the required libraries
foreach(prog gpmetis ndmetis mpmetis m2gmetis graphchk cmpfillin)
  target_link_libraries(${prog} metis GKlib m)
endforeach(prog)

if(METIS_INSTALL)
  install(TARGETS gpmetis ndmetis mpmetis m2gmetis graphchk cmpfillin
    RUNTIME DESTINATION bin)
endif()
