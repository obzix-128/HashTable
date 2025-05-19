FLAGS := -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal \
         -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs \
         -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions \
         -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security \
         -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers \
         -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 \
         -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla \
         -D_DEBUG -D_EJUDGE_CLIENT_SIDE -DRELEASE_MODE_D -O3 -mavx2 -mfma -I./include

#DEBUG_MODE: -D_DEBUG -fsanitize=address -O1; Для Дебага списка нужно убрать -DRELEASE_MODE_D
#RELEASE_MODE: -DNDEBUG 

CC := g++
ASM := nasm
ASMFLAGS := -f elf64

objects := $(patsubst source/%.cpp, objects/%.o, $(wildcard source/*.cpp))
asm_objects := $(patsubst source/%.asm, objects/%.o, $(wildcard source/*.asm))
list_objects := $(patsubst libs/List/source/%.cpp, libs/List/objects/%.o, $(wildcard libs/List/source/*.cpp))

all: do

do: $(objects) $(asm_objects) $(list_objects)
	@$(CC) $^ $(FLAGS) -o $@

objects/%.o: source/%.cpp
	@mkdir -p objects
	@$(CC) -c $< $(FLAGS) -o $@

objects/%.o: source/%.asm
	@mkdir -p objects
	@$(ASM) $(ASMFLAGS) $< -o $@

libs/List/objects/%.o: libs/List/source/%.cpp
	@mkdir -p libs/List/objects
	@$(CC) -c $< $(FLAGS) -o $@

clean:
	@rm -rf objects/*.o do
	@rm -rf libs/List/objects/*.o
	@rm -rf libs/List/image/*.png
	@rm -rf libs/List/image/*.txt