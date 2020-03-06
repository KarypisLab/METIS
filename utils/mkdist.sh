rm -rf $1
svn up
svn export http://dminers.dtc.umn.edu/svn/programs/karypis/metis/trunk $1
mkdir $1/manual
svn export http://dminers.dtc.umn.edu/svn/programs/karypis/metis/manual/r5.0/manual.pdf $1/manual/manual.pdf
svn export http://dminers.dtc.umn.edu/svn/libs/GKlib/trunk $1/GKlib
rm -rf $1/TODO
rm -rf $1/test
rm -rf $1/utils
rm -rf $1/doxygen
rm -rf $1/CHANGES.v4
tar -czf $1.tar.gz $1
rm -rf $1
