makefile:

DEBUG_FLAG = -g
TEST_EXECS = md5reader_test skeleton_test
ALL_EXECS = $(TEST_EXECS) modelrender
OBJ_FILES = md5reader.o skeleton.o model.o mesh.o
GLLIBS = -lGL -lglut -lGLU
EDITOR_MESS =

all: $(ALL_EXECS) $(OBJ_FILES)

objs: $(OBJ_FILES)

skeleton.o: skeleton.c skeleton.h
	$(CC) $(DEBUG_FLAG) -c skeleton.c 

skeleton_test: skeleton.o skeleton.c skeleton.h
	$(CC) $(DEBUG_FLAG) -o skeleton_test skeleton_test.c skeleton.o

model.o: model.c model.h
	$(CC) $(DEBUG_FLAG) -c model.c

mesh.o: mesh.c mesh.h
	$(CC) $(DEBUG_FLAG) -c mesh.c

md5reader.o: md5reader.c md5reader.h model.h model_formats.h mesh.h
	$(CC) $(DEBUG_FLAG) -c md5reader.c

md5reader_test: md5reader.o skeleton.o mesh.o md5reader_test.c 
	$(CC) $(DEBUG_FLAG) -o md5reader_test md5reader_test.c md5reader.o skeleton.o mesh.o

modelrender: modelrender.c md5reader.o skeleton.o model.o mesh.o
	$(CC) $(DEBUG_FLAG) -I. $(LIBS) $(GLLIBS) -o modelrender modelrender.c md5reader.o skeleton.o model.o mesh.o

tests: $(TEST_EXECS)

clean: 
	rm -f $(ALL_EXECS) $(OBJ_FILES) $(EDITOR_MESS)
