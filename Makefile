.PHONY: all
all: exp

.PHONY: exp
exp:
	./tools/create_exp.py --config sets/config_exp.cfg
	./tools/make_exp.sh

.PHONY: prod
prod:
	./tools/create_exp.py --config sets/config_prod.cfg
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