# Credit for the Auto-dependecy generation portion
# of this file goes to Tom Tromey <tromey@cygnus.com>.
# This file is based on a post that focused on
# this method. The post can be found at:
# http://make.mad-scientist.net/papers/advanced-auto-dependency-generation/
# Adapted for SDL et al. from Zackary Sullivan by Xin Cheng

CXX = g++

DEPDIR := .deps
OBJDIR := build
SRCDIR := src
INCLUDE := include
MAIN := main

DEPFLAGS = -MT $@ -MMD -MP -MF
INCLUDEFLAGS = -I./$(INCLUDE)

CXXFLAGS = `sdl2-config --cflags` -g -W -Wall -std=c++11 -Weffc++ -Wextra -pedantic -O0 -I `sdl2-config --prefix`/include/

LDFLAGS = `sdl2-config --libs` -lm -lexpat -lSDL2_image -lSDL2_ttf -lSDL2_mixer


$(shell mkdir -p $(DEPDIR) >/dev/null)
$(shell mkdir -p $(OBJDIR) >/dev/null)

# OBJS = $(shell find . -regex ".*\.cpp" -printf "%P " | sed 's/.cpp/.o/g')
SRCPTH := $(shell find . -regex ".*\.cpp" | tr '\n' ' ')
# SRCPTH += $(shell find $(SRCDIR) -regex ".*\.cpp" | tr '\n' ' ')
SRCFLS = $(notdir $(SRCPTH))
OBJFLS = $(patsubst %.cpp, $(OBJDIR)/%.o, $(SRCFLS))
DEPFLS = $(DEPDIR)/$*.Td


EXEC = run

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

all: $(EXEC)
	@true

$(OBJDIR)/$(MAIN).o: $(MAIN).cpp $(DEPDIR)/$(MAIN).d
	@echo "Creating Object $(notdir $@)"
	@$(CXX) $(DEPFLAGS) $(DEPDIR)/$(MAIN).Td $(CXXFLAGS) $(INCLUDEFLAGS) -c $< -o $@
	@mv -f $(DEPDIR)/$(MAIN).Td $(DEPDIR)/$(MAIN).d && touch $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(INCLUDE)/%.h $(DEPDIR)/%.d
	@echo "Creating Object $(notdir $@)"
	@$(CXX) $(DEPFLAGS) $(DEPFLS) $(CXXFLAGS) $(INCLUDEFLAGS) -c $< -o $@
	@mv -f $(DEPFLS) $(DEPDIR)/$*.d && touch $@

# Link
$(EXEC): $(OBJFLS)
	@echo "Creating Executable $@"
	@$(CXX) $(CXXFLAGS) $(INCLUDEFLAGS) $(OBJFLS) $(LDFLAGS) -o $@

# @echo $(OBJFLS)
# @$(CXX) $(CXXFLAGS) $(INCLUDEFLAGS) -o $@ $(OBJFLS) $(LDFLAGS)

clean:
	@echo "Deleting Objects"
	@rm -rf $(OBJDIR)
	@echo "Deleting Executable"
	@rm -rf $(EXEC)
	@echo "Deleting Dependecy List"
	@rm -rf $(DEPDIR)
	@echo "Deleting Images"
	@rm -rf frames/*.bmp

include $(wildcard $(patsubst %,$(DEPDIR)/%.d,$(basename $(OBJS))))
