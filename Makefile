makefile:

DEBUG_FLAG = -g
ALL_EXECS = 
TEST_EXECS = md5reader_test skeleton_test
OBJ_FILES = md5reader.o skeleton.o 
EDITOR_MESS = *~ \#*

all: $(ALL_EXECS) $(TEST_EXECS)

objs: $(OBJ_FILES)


skeleton.o: skeleton.c skeleton.h
	gcc $(DEBUG_FLAG) -c skeleton.c 
skeleton_test: skeleton.o
	gcc $(DEBUG_FLAG) -o skeleton_test skeleton_test.c skeleton.o
md5reader.o: md5reader.c md5reader.h 
	gcc $(DEBUG_FLAG) -c md5reader.c
md5reader_test: md5reader.o skeleton.o
	gcc $(DEBUG_FLAG) -o md5reader_test md5reader_test.c md5reader.o skeleton.o
tests: $(TEST_EXECS)

clean: 
	rm -f $(TEST_EXECS) $(OBJ_FILES) $(EDITOR_MESS)
