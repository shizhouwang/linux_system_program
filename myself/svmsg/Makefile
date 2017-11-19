
GEN_EXE = svmsg_create svmsg_send svmsg_receive \
		  svmsg_rm svmsg_chqbytes svmsg_ls \
		  svmsg_file_server svmsg_file_client

GEN_EXE_OBJS = ${GEN_EXE:=.o}
GEN_EXE_SRCS = ${GEN_EXE:=.c}

EXE = ${GEN_EXE}

all : ${EXE}

allgen : ${GEN_EXE}

.PHONY:clean
clean:
	-rm -rf ./*.o ${EXE}
