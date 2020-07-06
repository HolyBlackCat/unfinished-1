# Sources
SOURCE_DIRS := src lib

# Object directory
OBJECT_DIR := obj

# Resulting binary
OUTPUT_FILE := bin/unknown
LINKER_MODE := CXX

# Dependency set name
LIBRARY_PACK_NAME := imp-re_deps_2020-07-06
USED_PACKAGES := openal freetype2 ogg vorbis vorbisfile zlib fmt double-conversion
USED_EXTERNAL_PACKAGES :=
ifeq ($(TARGET_OS),windows)
USED_PACKAGES += sdl2
else
USED_EXTERNAL_PACKAGES += sdl2
endif


# Flags
CXXFLAGS := -std=c++2a -pedantic-errors -Wall -Wextra -Wdeprecated -Wextra-semi
LDFLAGS :=
# Important flags
override CXXFLAGS += -include src/program/common_macros.h -include src/program/parachute.h -Isrc -Ilib/include $(subst -Dmain,-DENTRY_POINT_OVERRIDE,$(sort $(deps_compiler_flags)))
override CXXFLAGS += -Ilib/include/cglfl_gl3.2_core # OpenGL version
override LDFLAGS += $(filter-out -mwindows,$(deps_linker_flags))

# Build modes
$(call new_mode,debug)
$(mode_flags) CXXFLAGS += -g -D_GLIBCXX_ASSERTIONS

$(call new_mode,debug_hard)
$(mode_flags) CXXFLAGS += -g -D_GLIBCXX_DEBUG

$(call new_mode,release)
$(mode_flags) CXXFLAGS += -DNDEBUG -O3
$(mode_flags) LDFLAGS += -O3 -s
ifeq ($(TARGET_OS),windows)
$(mode_flags) LDFLAGS += -mwindows
endif

$(call new_mode,release_profiled)
$(mode_flags) CXXFLAGS += -DNDEBUG -O3 -pg
$(mode_flags) LDFLAGS += -O3 -pg
ifeq ($(TARGET_OS),windows)
$(mode_flags) LDFLAGS += -mwindows
endif

# File-specific flags
FILE_SPECIFIC_FLAGS := lib/implementation.cpp lib/cglfl.cpp > -g0 -O3

# Precompiled headers
PRECOMPILED_HEADERS := src/game/*.cpp > src/game/master.hpp

# Code generation
GEN_CXXFLAGS := -std=c++2a -Wall -Wextra -pedantic-errors
override generators_dir := gen
override generated_headers := math:src/utils/mat.h macros:src/macros/generated.h
override generate_file = $(call host_native_path,$2) : $(generators_dir)/make_$1.cpp ; \
	@+$(MAKE) -f gen/Makefile _gen_dir=$(generators_dir) _gen_source_file=make_$1 _gen_target_file=$2 --no-print-directory
$(foreach f,$(generated_headers),$(eval $(call generate_file,$(word 1,$(subst :, ,$f)),$(word 2,$(subst :, ,$f)))))
