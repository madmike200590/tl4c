PROJECT_NAME=tl4c
PROJECT_VERSION=1.0.0-SNAPSHOT

ARTIFACT_NAME=$(PROJECT_NAME)-$(PROJECT_VERSION)

#########################################################################################################################
                                                      # VARIABLES #
#########################################################################################################################

SRC_DIR=src
INCLUDE_DIR=include

TEST_SRC_DIR=src_test

BUILD_DIR=build

TEST_BUILD_DIR=build_test
TEST_DRIVER=$(TEST_BUILD_DIR)/test

RELEASE_DIR=release
RELEASE_HEADERS=$(INCLUDE_DIR)/logging.h
SRC_RELEASE_DIR=$(RELEASE_DIR)-src
RELEASE_SOURCES=$(SRC_DIR) $(INCLUDE_DIR) $(TEST_SRC_DIR) makefile

CC=gcc

# gcc options
#	-std=c99              use C99 standard
#	-pedantic             enforce strict compliance to standard
#	-Wall                 show all warnings
#       -g                    produce debug information  
#	-c                    compile and assemble, but don't link (output is an object file
CFLAGS=-std=c99 -pedantic -Wall -Werror -g -c -I$(INCLUDE_DIR)
CFLAGS_TEST=$(CFLAGS)

# wildcard function creates a space-separated list of matching files,
# i.e. all .c files in SRC_DIR
CFILES=$(wildcard $(SRC_DIR)/*.c)
TEST_CFILES=$(wildcard $(TEST_SRC_DIR)/*.c)

# create object file list out of source file listing
OBJFILES=$(subst .c,.o,$(subst $(SRC_DIR),$(BUILD_DIR),$(CFILES)))
TEST_OBJFILES=$(subst .c,.o,$(subst $(TEST_SRC_DIR),$(TEST_BUILD_DIR),$(TEST_CFILES)))

#########################################################################################################################
                                                       # TARGETS #                                                      
#########################################################################################################################

.PHONY: all clean compile test-compile test package package-sources

all : clean $(BUILD_DIR) $(TEST_BUILD_DIR) compile test-compile test package package-sources

# delete build output folder if it exists
clean :
	if [ -e $(BUILD_DIR) ]; then rm -r $(BUILD_DIR); fi;
	if [ -e $(TEST_BUILD_DIR) ]; then rm -r $(TEST_BUILD_DIR); fi;
	if [ -e $(RELEASE_DIR) ]; then rm -r $(RELEASE_DIR); fi;
	if [ -e $(SRC_RELEASE_DIR) ]; then rm -r $(SRC_RELEASE_DIR); fi;

$(BUILD_DIR) :
	mkdir $(BUILD_DIR)

$(TEST_BUILD_DIR) :
	mkdir $(TEST_BUILD_DIR)

$(RELEASE_DIR) :
	mkdir $(RELEASE_DIR)
	mkdir $(RELEASE_DIR)/include

$(SRC_RELEASE_DIR) :
	mkdir $(SRC_RELEASE_DIR)

compile : $(BUILD_DIR) $(OBJFILES)

test-compile : compile $(TEST_BUILD_DIR) $(TEST_OBJFILES)

test : test-compile $(TEST_DRIVER)
	./$(TEST_DRIVER)

package : test $(RELEASE_DIR)
	ar -cvq $(BUILD_DIR)/$(ARTIFACT_NAME).a $(BUILD_DIR)/*.o
	cp $(BUILD_DIR)/$(ARTIFACT_NAME).a $(RELEASE_DIR)/$(ARTIFACT_NAME).a
	cp $(RELEASE_HEADERS) $(RELEASE_DIR)/include/

package-sources : test $(SRC_RELEASE_DIR)
	cp -r $(RELEASE_SOURCES) $(SRC_RELEASE_DIR)

$(TEST_DRIVER) : $(TEST_OBJFILES)
	$(CC) $(OBJFILES) $(TEST_OBJFILES) -o $(TEST_DRIVER)

# expression "$<" matches first element of prerequisites list
# "$@" = first element of target list
$(BUILD_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $< -o $@

# expression "$<" matches first element of prerequisites list
# "$@" = first element of target list
$(TEST_BUILD_DIR)/%.o : $(TEST_SRC_DIR)/%.c
	$(CC) $(CFLAGS) $< -o $@
