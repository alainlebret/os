SUBDIRS = 1_process 3_files 4_memory 5_synchronization 6_interprocess 7_sockets 8_multiplexing 9_threads 10_various

all:
	for dir in $(SUBDIRS) ; do \
	make -C  $$dir ; \
	done

clean:
	for dir in $(SUBDIRS) ; do \
	make -C  $$dir clean; \
	done

distclean:
	for dir in $(SUBDIRS) ; do \
	make -C  $$dir distclean; \
	done

.PHONY: distclean clean all

