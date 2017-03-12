# Author:  Tan Menglong <tanmenglong@gmail.com>
# Date:    Sun Sep 23 23:39:46 2012
#
# Make Target:
# ------------
# The Makefile provides the following targets to make:
#   $ make           compile and link
#   $ make clean     clean objects and the executable file
#
#===========================================================================

all :
	ino build

upload : all
	sudo ino upload

clean :
	rm -rf .build

serial :
	sudo ino serial
