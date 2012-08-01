makefile:

DEBUG_FLAG = -g
ALL_EXECS = 
TEST_EXECS = md5reader_test skeleton_test
OBJ_FILES = md5reader.o skeleton.o model.o mesh.o
GLLIBS = -lGL -lglut -lGLU
EDITOR_MESS =

all: $(ALL_EXECS) $(TEST_EXECS)

objs: $(OBJ_FILES)

skeleton.o: skeleton.c skeleton.h
	gcc $(DEBUG_FLAG) -c skeleton.c 

skeleton_test: skeleton.o skeleton.c skeleton.h
	gcc $(DEBUG_FLAG) -o skeleton_test skeleton_test.c skeleton.o

model.o: model.c model.h
	gcc $(DEBUG_FLAG) -c model.c

mesh.o: mesh.c mesh.h
	gcc $(DEBUG_FLAG) -c mesh.c

md5reader.o: md5reader.c md5reader.h model.h model_formats.h mesh.h
	gcc $(DEBUG_FLAG) -c md5reader.c

md5reader_test: md5reader.o skeleton.o md5reader_test.c 
	gcc $(DEBUG_FLAG) -o md5reader_test md5reader_test.c md5reader.o skeleton.o

modelrender: modelrender.c md5reader.o skeleton.o model.o mesh.o
	gcc $(DEBUG_FLAG) -I. $(LIBS) $(GLLIBS) -o modelrender modelrender.c md5reader.o skeleton.o model.o mesh.o

tests: $(TEST_EXECS)

clean: 
	rm -f $(TEST_EXECS) $(OBJ_FILES) $(EDITOR_MESS)
