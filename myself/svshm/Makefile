all:
	gcc svshm_xfr_writer.c ../svsem/binary_sems.c -o svshm_xfr_writer
	gcc svshm_xfr_reader.c ../svsem/binary_sems.c -o svshm_xfr_reader

clean:
	-rm svshm_xfr_writer svshm_xfr_reader *.o
