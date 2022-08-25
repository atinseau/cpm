all:
	@cd cpm && make > /dev/null
	@./cpm/cpm 

DEFAULT_GOAL := all