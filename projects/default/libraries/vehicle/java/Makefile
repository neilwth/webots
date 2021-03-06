# Copyright 1996-2018 Cyberbotics Ltd.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

space :=
space +=
WEBOTS_HOME_PATH=$(subst $(space),\ ,$(strip $(subst \,/,$(WEBOTS_HOME))))
include $(WEBOTS_HOME_PATH)/resources/Makefile.os.include

WEBOTS_LIBRARY = 1
MODULE_NAME = vehicle

INTERFACE = $(MODULE_NAME).i
SOURCE_HEADERS = $(wildcard $(WEBOTS_HOME_PATH)/include/controller/cpp/webots/*.hpp)
JAVA_FILES = Driver.java Car.java vehicle.java vehicleJNI.java

WRAPPER = $(MODULE_NAME).cpp
WRAPPER_OBJECT = $(WRAPPER:.cpp=.o)
JAR = $(WEBOTS_HOME_PATH)/lib/java/$(MODULE_NAME).jar
INCLUDES = -I"$(WEBOTS_HOME_PATH)/include/controller/c/" -I"$(WEBOTS_HOME_PATH)/include/controller/cpp/"

SWIG_OUTPUT  = output
MODULE_PATH  = com/cyberbotics/webots/controller/vehicle
JAVA_SOURCES = $(addprefix $(SWIG_OUTPUT)/,$(JAVA_FILES))

SWIG_EXISTS = $(shell which swig 2> /dev/null)

ifeq ($(OSTYPE),linux)
JAVA_INCLUDES = -I"$(JAVA_HOME)/include" -I"$(JAVA_HOME)/include/linux"
CFLAGS2       = -c -Wall -Wno-unused-function -fPIC
LIBRARY       = $(WEBOTS_HOME_PATH)/lib/java/libvehicle.so
LIB           = -L"$(WEBOTS_HOME)/lib"
endif

ifeq ($(OSTYPE),windows)
JAVA_INCLUDES = -I"$(JAVA_HOME)/include" -I"$(JAVA_HOME)/include/win32"
CFLAGS2       = -c -Wall -Wno-unused-function -O
LIBRARY       = $(WEBOTS_HOME_PATH)/lib/java/vehicle.dll
LIB           = -L"$(WEBOTS_HOME_PATH)/msys64/mingw64/bin"
endif

ifeq ($(OSTYPE),darwin)
JAVA_INCLUDES = -I"$(MACOSX_SDK_PATH)/System/Library/Frameworks/JavaVM.framework/Versions/Current/Headers"
CFLAGS1 = -isysroot $(MACOSX_SDK_PATH) -mmacosx-version-min=$(MACOSX_MIN_SDK_VERSION) -stdlib=libc++ -Wl,-rpath,@loader_path/../..
CFLAGS2 = -c -Wall -isysroot $(MACOSX_SDK_PATH) -mmacosx-version-min=$(MACOSX_MIN_SDK_VERSION) -stdlib=libc++ -Wno-unused-function
LIBRARY = $(WEBOTS_HOME_PATH)/lib/java/libvehicle.jnilib
LIB     = -L"$(WEBOTS_HOME)/lib"
endif

LIB += -lController -lCppController -lCppCar -lCppDriver

.PHONY: JAVA_CLASSES all

# rules
ifeq ($(SWIG_EXISTS),)
all release debug profile:
	@echo -e "# \033[0;33mSWIG not installed, skipping Java API\033[0m"
else ifeq ($(JAVA_HOME),)
all release debug profile:
	@echo -e "# \033[0;33mJava not installed or 'JAVA_HOME' not set, skipping Java API\033[0m"
else
all release debug profile: $(JAR) $(LIBRARY)

$(JAR): JAVA_CLASSES
	@mkdir -p $(WEBOTS_HOME_PATH)/lib/java
	@jar cfv $@ $(MODULE_PATH)/*.class > /dev/null

JAVA_CLASSES: $(JAVA_SOURCES)
	@javac -target 1.8 -classpath "$(WEBOTS_HOME_PATH)/lib/java/Controller.jar" $(JAVA_SOURCES) -d .

$(JAVA_SOURCES): $(WRAPPER)

$(WRAPPER): $(INTERFACE) $(SOURCE_HEADERS)
	@mkdir -p $(SWIG_OUTPUT)
	@swig -c++ -java -package $(subst /,.,$(MODULE_PATH)) -outdir $(SWIG_OUTPUT) -I$(WEBOTS_HOME_PATH)/resources/languages/java/ $(INCLUDES) -o $@ $<

$(WRAPPER_OBJECT): $(WRAPPER)
	@$(CXX) $(CFLAGS2) $(INCLUDES) $(JAVA_INCLUDES) $< -o $@

%.so: $(WRAPPER_OBJECT)
	@mkdir -p $(WEBOTS_HOME_PATH)/lib/java
	@$(CXX) -shared $< $(LIB) -o $@

%.dll: $(WRAPPER_OBJECT)
	@mkdir -p $(WEBOTS_HOME_PATH)/lib/java
	@$(CXX)  -shared -mwindows -Wl,--add-stdcall-alias -Wl,--enable-auto-import -O -lm $< $(LIB) -o $@

%.jnilib: $(WRAPPER_OBJECT)
	@mkdir -p $(WEBOTS_HOME_PATH)/lib/java
	@$(CXX) $(CFLAGS1) -bundle $< $(LIB) -o $@
endif

clean:
	@rm -fr com $(SWIG_OUTPUT) $(JAR) $(WRAPPER) $(LIBRARY) $(WRAPPER_OBJECT)
	@rm -d $(WEBOTS_HOME_PATH)/lib/java > /dev/null 2>&1 || :
