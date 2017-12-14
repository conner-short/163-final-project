OBJDIR = .obj
CFLAGS = -g -Wall
CXXFLAGS = -g -Wall

LIBS = -lstdc++ `pkg-config --libs opencv` -lGL -lglut -lm
OBJ = $(addprefix $(OBJDIR)/,camera.o main.o sim.o vector.o view.o faces.o)
BIN = project

all: $(BIN)
.PHONY: all

$(BIN): $(OBJ)
	$(CC) -o $(BIN) $(OBJ) $(LIBS)

$(OBJ): | $(OBJDIR)

$(OBJDIR):
	mkdir $(OBJDIR)

$(OBJDIR)/%.o: src/%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -Iinclude/ -o $@
	
$(OBJDIR)/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -Iinclude/ -o $@

$(OBJDIR)/camera.o: include/camera.h
$(OBJDIR)/main.o: include/camera.h include/sim.h include/view.h
$(OBJDIR)/sim.o: include/sim.h
$(OBJDIR)/vector.o: include/vector.h
$(OBJDIR)/view.o: include/camera.h include/vector.h include/view.h \
include/faces.h
$(OBJDIR)/faces.o: include/faces.h include/vector.h

.PHONY: clean
clean:
	-rm $(BIN) $(OBJ)
	-rmdir $(OBJDIR)
