CC := gcc
SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)
IN_DIR := /home/wstcsie/OS2020/OS_PJ1_Test/
TASK_IN := $(sort $(wildcard $(IN_DIR)*.txt))
TASK := $(patsubst $(IN_DIR)%.txt, %, $(TASK_IN))
TASK_OUT := $(TASK_IN:$(IN_DIR)%.txt=output/%_stdout.txt)
DEMO = TIME_MEASUREMENT.txt FIFO_1.txt PSJF_2.txt RR_3.txt SJF_4.txt
CFLAG := 

.PHONY: clean run $(TASK) clean_out demo demo_each

all: $(OBJS)
	$(CC) $^ -o main $(CFLAG)

%.o: %.c
	$(CC) -c $< $(CFLAG)

run: clean_out $(TASK)
	@echo "Done!"

demo:
	@for i in $(DEMO); \
	do make --no-print-directory demo_each INPUT=OS_PJ1_Test/$$i; \
	done

demo_each:
	@echo
	sudo dmesg --clear
	sudo ./main <$(INPUT)
	dmesg | grep Project1

$(TASK): 
	@[ -d ./output/ ] || mkdir ./output/
	sudo dmesg --clear
	sudo ./main <$(IN_DIR)$@.txt >output/$@_stdout.txt
	dmesg | grep Project1 >output/$@_dmesg.txt
	@echo "================================================================"

veri: demo
	for i in $(patsubst %.txt, %_dmesg.txt, $(DEMO)); \
	do ../pj1_ver<output/$$i; \
	echo ""; \
	done;

clean:
	-rm -rf main *.o
clean_out:
	@-rm -rf $(TASK_OUT)
