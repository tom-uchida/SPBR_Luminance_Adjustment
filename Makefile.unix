###########################################
#####        Makefile for SPBR        #####
#####        2017/03/12  S. Tanaka    #####
###########################################

# Installation directory
INSTALL_DIR=$(HOME)/local/bin


##### Build all executable files #####
build:
# Create Makefile.kvs
	kvsmake -g sopbr_tmp
# SPBR
	kvsmake clean
	cp ./MAIN/spbr_main.cpp ./main.cpp
	kvsmake	
	mv sopbr_tmp spbr
# OPBR
	kvsmake clean
	cp ./MAIN/opbr_main.cpp ./main.cpp
	kvsmake
	mv sopbr_tmp opbr

##### Install the Built executable files #####
install:
	@if [ ! -d $(INSTALL_DIR) ]; \
		then echo "mkdir -p $(INSTALL_DIR)"; mkdir -p $(INSTALL_DIR); \
	fi
	cp spbr $(INSTALL_DIR)
	cp opbr $(INSTALL_DIR)

##### Full automatic installation (Build + Install) #####
autoinstall:
# Create Makefile.kvs
	kvsmake -g sopbr_tmp
# SPBR
	kvsmake clean
	cp ./MAIN/spbr_main.cpp ./main.cpp
	kvsmake	
	mv sopbr_tmp spbr
# OPBR
	kvsmake clean
	cp ./MAIN/opbr_main.cpp ./main.cpp
	kvsmake
	mv sopbr_tmp opbr
# install
	@if [ ! -d $(INSTALL_DIR) ]; \
		then echo "mkdir -p $(INSTALL_DIR)"; mkdir -p $(INSTALL_DIR); \
	fi
	cp spbr $(INSTALL_DIR)
	cp opbr $(INSTALL_DIR)
#cleaning
	kvsmake clean
	rm -f sopbr_tmp
	rm -f spbr opbr
	rm -f pbr2014 pbr 
	rm -f pbr2014bin 
	rm -f ptbr 
	rm -f pbr_cpu ptbr_cpu 
	rm -f *.o
	rm -f *~
	rm -f \#*
	cd CONVERT2BINARY; make clean
	cd PLY_SUPPORT/PLYTHIN; make clean


########################################
# Building executable files one by one #
########################################

# SPBR
spbr:
	kvsmake -g sopbr_tmp
	kvsmake clean
	cp ./MAIN/spbr_main.cpp ./main.cpp
	kvsmake	
	mv sopbr_tmp spbr

spbr_install:
	@if [ ! -d $(INSTALL_DIR) ]; \
		then echo "mkdir -p $(INSTALL_DIR)"; mkdir -p $(INSTALL_DIR); \
	fi
	cp spbr $(INSTALL_DIR)

# OPBR
opbr:
	kvsmake -g sopbr_tmp
	kvsmake clean
	cp ./MAIN/opbr_main.cpp ./main.cpp
	kvsmake
	mv sopbr_tmp opbr

opbr_install:
	@if [ ! -d $(INSTALL_DIR) ]; \
		then echo "mkdir -p $(INSTALL_DIR)"; mkdir -p $(INSTALL_DIR); \
	fi
	cp opbr $(INSTALL_DIR)


# TOOL (Convert2Binary)
tool:
	cd CONVERT2BINARY; make 
	cd PLY_SUPPORT/PLYTHIN; make 

tool_install:
	cd CONVERT2BINARY; make install 
	cd PLY_SUPPORT/PLYTHIN; make install

############
# Cleaning #
############

# Clean temporary files in this directory
clean:
	kvsmake clean
	rm -f sopbr_tmp
	rm -f spbr opbr
	rm -f pbr2014 pbr 
	rm -f pbr2014bin 
	rm -f ptbr 
	rm -f pbr_cpu ptbr_cpu 
	rm -f *.o
	rm -f *~
	rm -f \#*

# Clean 3d data files in this directory
dataclean:
	rm -f *.pbr *.spbr
	rm -f *.ply

# Clean image files in this directory
imgclean:
	rm -f  *.bmp *.ppm *.pgm

# clean all, including temporary files in the sub-directories
cleanall:
	kvsmake clean
	rm -f sopbr_tmp
	rm -f spbr opbr
	rm -f pbr2014 pbr 
	rm -f pbr2014bin 
	rm -f ptbr 
	rm -f pbr_cpu ptbr_cpu 
	rm -f *.o
	rm -f *~
	rm -f \#*
	rm -f .param.spbr
#
	cd CONVERT2BINARY; make clean
	cd PLY_SUPPORT/PLYTHIN; make clean
#
	cd MAIN; make clean
#
	cd DOC; make clean
#
	cd SPBR_DATA; make clean
	cd PLY_DATA; make clean
#
	rm -f *.pbr *.spbr
#
	rm -f *.ply
#
	rm -f  *.bmp *.ppm *.pgm


# Automatic uninstallation
uninstall:
	cd $(INSTALL_DIR); rm -f spbr opbr
	cd $(INSTALL_DIR); rm -f pbr2014 pbr
	cd $(INSTALL_DIR); rm -f pbr2014bin 
	cd $(INSTALL_DIR); rm -f ptbr 
	cd $(INSTALL_DIR); rm -f pbr_cpu ptbr_cpu 
	cd CONVERT2BINARY; make uninstall
	cd PLY_SUPPORT/PLYTHIN; make uninstall

edit:
	emacs spbr_ply_ascii.cpp
 