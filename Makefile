.PHONY: all
all: exp

.PHONY: exp
exp:
	./tools/create_exp.py --config sets/config_exp.cfg

.PHONY: prod
prod:
	./tools/create_exp.py --config sets/config_prod.cfg
	./tools/create_exp.py --config sets/config_patterns.cfg
	./tools/create_exp.py --config sets/config_letters.cfg

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
	./tools/make_exp.sh
	./tools/run_exp.sh

.PHONY: stop
stop:
	./tools/stop_exp.sh

.PHONY: result
result:
	./tools/get_res.sh

.PHONY: status
status:
	./tools/xorman.sh