CC = gcc
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
IN_DIR = ../OS_PJ1_Test/
OUT_DIR = output/
TASK_IN = $(sort $(wildcard $(IN_DIR)*.txt))
TASK_OUT = $(TASK_IN:$(IN_DIR)%.txt=$(OUT_DIR)%_stdout.txt)
TASK = $(patsubst $(IN_DIR)%.txt, %, $(TASK_IN))
DEMO = TIME_MEASUREMENT.txt FIFO_1.txt PSJF_2.txt RR_3.txt SJF_4.txt
CFLAG := 

.PHONY: clean run $(TASK) clean_out demo demo_each

all: $(OBJS)
	$(CC) $^ -o main $(CFLAG)

%.o: %.c
	$(CC) -c $< $(CFLAG)

demo:
	@for i in $(DEMO); \
	do make --no-print-directory $(patsubst %.txt, %, $(DEMO)) IN=$(IN_DIR)$$i; \
	done

$(TASK): 
	sudo dmesg --clear
	sudo ./main < $(patsubst %, $(IN_DIR)%.txt, $@)
	dmesg | grep Project1
	@echo

$(TASK_OUT):
	sudo dmesg --clear
	sudo ./main < $(patsubst $(OUT_DIR)%_stdout.txt, $(IN_DIR)%.txt, $@) > $@
	dmesg | grep Project1 > $(patsubst %_stdout.txt, %_dmesg.txt, $@)
	@echo

run:
	@for task in $(TASK); \
	do make --no-print-directory $$task; \
	done;
	@echo "Done!"

out2file: clean_out
	@[ -d $(OUT_DIR) ] || mkdir $(OUT_DIR)
	@for task in $(TASK_OUT); \
	do make --no-print-directory $$task; \
	done
	@echo "Done!"

check: $(patsubst %.txt, $(OUT_DIR)%_dmesg.txt, $(DEMO))
	@for i in $(patsubst %.txt, %_dmesg.txt, $(DEMO)); \
	do ../pj1checker<$(OUT_DIR)$$i; \
	echo ""; \
	done;

clean:
	-rm -rf main *.o
clean_out:
	@-rm -rf $(TASK_OUT)
