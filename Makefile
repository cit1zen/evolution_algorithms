.PHONY: all
all:
	./tools/make_exp.sh

.PHONY: delete
delete:
	rm -r _results experiments/*

.PHONY: remove
remove:
	rm -r experiments/*

.PHONY: clean
clean:
	./tools/clr_res.sh

.PHONY: run
run:
	./tools/run_exp.sh

.PHONY: stop
stop:
	./tools/stop_exp.sh

.PHONY: result
result:
	./tools/get_res.sh

.PHONY: status
status:
	./tools/xorman00