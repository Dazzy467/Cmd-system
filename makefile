DEBUG_FLAG = -g
RELEASE_FLAG = -s -O3
PATH_D = build/debug
PATH_R = build/release

all: release debug

debug: $(PATH_D)/debug.exe

release: $(PATH_R)/release.exe

$(PATH_D)/debug.exe: src/main.cpp include/*.h
	g++ $(DEBUG_FLAG) $< -I include -o $@

$(PATH_R)/release.exe: src/main.cpp include/*.h
	g++ $(RELEASE_FLAG) $< -I include -o $@

