
GEN_EXE = calendar_time strtime

GEN_EXE_OBJS = ${GEN_EXE:=.o}
GEN_EXE_SRCS = ${GEN_EXE:=.c}

EXE = ${GEN_EXE}

all : ${EXE}

allgen : ${GEN_EXE}

.PHONY:clean
clean:
	-rm -rf ./*.o ${EXE}
