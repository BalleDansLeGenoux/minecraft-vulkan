# Variables de compilation et de liens
CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

# Cible par défaut : affichage de l'usage du Makefile
.DEFAULT_GOAL := usage

# Cible principale de compilation
Project: src/*.cpp shaders
	g++ $(CFLAGS) -o bin/Project src/*.cpp $(LDFLAGS)

# Cible test : exécuter le projet compilé
test: Project shaders
	./bin/Project

# Cible de nettoyage : supprimer les fichiers binaires générés
clean:
	rm -f bin/Project
	rm -rf bin/tmp/Project

# Cible de compilation des shaders
shaders:
	glslc shaders/shader.vert -o shaders/vert.spv
	glslc shaders/shader.frag -o shaders/frag.spv

# Cible exemple : compilation d'un exemple spécifique
example:
	g++ $(CFLAGS) -o bin/example example/example.cpp $(LDFLAGS)
	./bin/example

# Cible de debug : compilation avec les symboles de debug
debug:
	g++ $(CFLAGS) -g -o bin/debug src/*.cpp $(LDFLAGS)
	gdb bin/debug

# Cible d'usage : afficher les instructions d'utilisation du Makefile
usage:
	@echo "Utilisation du Makefile :"
	@echo "  make                - Renvoie un usage"
	@echo "  make test           - Compile et exécute le projet principal"
	@echo "  make clean          - Supprime les fichiers générés"
	@echo "  make shaders        - Compile les shaders"
	@echo "  make example        - Compile et exécute un exemple"
	@echo "  make debug          - Compile avec debug et lance gdb"
