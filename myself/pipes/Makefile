
GEN_EXE = simple_pipe

GEN_EXE_OBJS = ${GEN_EXE:=.o}
GEN_EXE_SRCS = ${GEN_EXE:=.c}

EXE = ${GEN_EXE}

all : ${EXE} pipe_sync

pipe_sync:
	gcc pipe_sync.c ../time/curr_time.c -I../lib/ -I../time -o pipe_sync

allgen : ${GEN_EXE}

.PHONY:clean
clean:
	-rm -rf ./*.o ${EXE} pipe_sync
