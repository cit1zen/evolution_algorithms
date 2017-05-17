.PHONY: all
all: bp

# Create devel experiments
.PHONY: exp
exp:
	./tools/create_exp.py --config sets/config_exp.cfg

# Create experiments used in my thesis
.PHONY: bp
bp:
	./tools/create_exp.py --config sets/config_bp.cfg

.PHONY: delete
delete:
	rm -r _results experiments/*

.PHONY: remove
remove:
	rm -r experiments/*

# Build and run experiments
.PHONY: run
run:
	./tools/make_exp.sh
	./tools/run_exp.sh

# Stop all experiments
.PHONY: stop
stop:
	./tools/stop_exp.sh

# Resutls and statistics
.PHONY: results
result:
	./tools/get_res.sh

# Show status of my IT4I queue
.PHONY: status
status:
	./tools/xorman.sh