
CFLAGS  += -Wall
#CFLAGS  += -ggdb
CFLAGS  += -O3
LDFLAGS += -lm

all: main test_case main2

main: main.o transformation/bwt.o transformation/mtf.o transformation/rle.o transformation/huffman.o transformation/rle2.o \
	transformation/dict.o transformation/rle0.o ext/rle.o ext/dict.o ext/rle0.o ext/utils.o util/my_io.o

test_case: test_case.o transformation/bwt.o transformation/mtf.o transformation/rle.o transformation/huffman.o \
	util/my_io.o util/common.o

main2: main2.o transformation/bwt.o transformation/mtf.o transformation/rle.o transformation/huffman.o transformation/rle2.o\
	transformation/dict.o transformation/rle0.o ext/rle.o ext/dict.o ext/rle0.o ext/utils.o util/my_io.o util/common.o \
	data_model.o

clean:
	$(RM) main test_case main2 *.o ext/*.o util/*.o transformation/*.o

.PHONY: clean
