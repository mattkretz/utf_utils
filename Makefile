CXX ?= c++
build_dir := $(shell which $(CXX))
tmp := "case $$(readlink -f $(build_dir)) in *icecc) which $${ICECC_CXX:-g++};; *) echo $(build_dir);; esac"
build_dir := $(shell sh -c $(tmp))
build_dir := $(realpath $(build_dir))
build_dir := build-$(subst /,-,$(build_dir:/%=%)$(CXXFLAGS))

all:
%:: $(build_dir)/CMakeCache.txt
	$(MAKE) --no-print-directory -C "$(build_dir)" $*

$(build_dir)/CMakeCache.txt:
	@test -n "$(build_dir)"
	@mkdir -p "$(build_dir)"
	@test -e "$(build_dir)/CMakeCache.txt" || cmake -DCMAKE_BUILD_TYPE=Release -H. -B"$(build_dir)"

print_build_dir:
	@echo "$(PWD)/$(build_dir)"

clean_builddir:
	rm -rf "$(build_dir)"

# the following rule works around %:: grabbing the Makefile rule and thus stops it from running every time
Makefile:
	@true

$(build_dir)/utf_utils_test: utf_utils_test

$(build_dir)/utf-32.dat: $(build_dir)/utf_utils_test
	cd "$(build_dir)" && \
		 ./utf_utils_test -dd ../test_data

bench: $(build_dir)/utf-32.dat
	cd "$(build_dir)" && \
		 gnuplot -e "t=\"$(CXXFLAGS)\"" ../pdf.gnuplot && \
		 gnuplot -e "t=\"$(CXXFLAGS)\"" ../svg.gnuplot && \
		 mv utf-16.pdf "../utf-16$(CXXFLAGS).pdf" && \
		 mv utf-32.pdf "../utf-32$(CXXFLAGS).pdf" && \
		 mv utf-16.svg "../utf-16$(CXXFLAGS).svg" && \
		 mv utf-32.svg "../utf-32$(CXXFLAGS).svg"

bench-%:
	CFLAGS="-march=$*" CXXFLAGS="-march=$*" $(MAKE) --no-print-directory -C . bench

bench-native-%:
	CFLAGS="-march=native -m$*" CXXFLAGS="-march=native -m$*" $(MAKE) --no-print-directory -C . bench

bench-all: bench-native-no-sse3 bench-native-no-avx bench-native-no-avx2 bench-native-no-avx512f

.PHONY: print_build_dir clean_builddir bench bench-all
