
GEN_EXE = svsem_demo svsem_mon svsem_semget svsem_setall \
		  svsem_good_init svsem_op

GEN_EXE_OBJS = ${GEN_EXE:=.o}
GEN_EXE_SRCS = ${GEN_EXE:=.c}

EXE = ${GEN_EXE}

all : ${EXE}

allgen : ${GEN_EXE}

.PHONY:clean
clean:
	-rm -rf ./*.o ${EXE}
