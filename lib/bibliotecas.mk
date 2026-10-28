# Infraestrutura fornecida com o trabalho (Linux).
# Este arquivo é incluído pelo Makefile; não é necessário editá-lo.

INCLUDES_BIBLIOTECAS = -Ilib/apoio -Ilib/interface -Ilib/linux/include \
                       -Ilib/raylib/src -Ilib/raylib/src/external/glfw/include

BIBLIOTECAS = -l:libGL.so.1 -l:libX11.so.6 -lm -lpthread -ldl -lrt

# A Raylib é escrita em C: usamos gcc e geramos um .o para cada módulo.
# Esses objetos são reaproveitados nas próximas compilações do jogo.
OPCOES_RAYLIB = -std=c99 -O2 -D_DEFAULT_SOURCE -DPLATFORM_DESKTOP \
                -DGRAPHICS_API_OPENGL_33 -D_GLFW_X11

# Módulos da Raylib: somente os .c diretamente em src/.
# Os arquivos de external/ e platforms/ são incluídos pelos próprios módulos.
FONTES_RAYLIB = $(wildcard lib/raylib/src/*.c)

# Converte, por exemplo, lib/raylib/src/rcore.c em lib/.build/raylib/rcore.o.
OBJETOS_RAYLIB = $(patsubst lib/raylib/src/%.c,lib/.build/raylib/%.o,$(FONTES_RAYLIB))

# Mudanças nos arquivos fornecidos da Raylib também provocam recompilação.
DEPENDENCIAS_RAYLIB = $(shell find lib/raylib/src -type f)

.PHONY: raylib
raylib: $(OBJETOS_RAYLIB)

# Uma regra para todos os módulos: % representa o nome do módulo.
# $< é o arquivo .c; $@ é o arquivo .o que será gerado.
lib/.build/raylib/%.o: lib/raylib/src/%.c $(DEPENDENCIAS_RAYLIB)
	mkdir -p lib/.build/raylib
	gcc $(OPCOES_RAYLIB) $(INCLUDES_BIBLIOTECAS) -c $< -o $@
