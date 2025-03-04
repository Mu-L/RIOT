RIOTBASE=$(shell git rev-parse --show-toplevel)
# Generate list of quoted absolute include paths. Evaluated in riot.doxyfile.
export STRIP_FROM_INC_PATH_LIST=$(shell \
    git ls-tree -dr --full-tree --name-only HEAD core drivers sys |\
    grep '/include$$' |\
    sed 's/.*/\"$(subst /,\/,$(RIOTBASE))\/\0\"/')

# use lessc (http://lesscss.org/#using-less) for compiling CSS
# It can also be installed in ubuntu with the `node-less` package
LESSC ?= lessc

DOCUMENTATION_FORMAT ?= html

.PHONY: doc
doc: $(DOCUMENTATION_FORMAT)

# by marking html as phony we force make to re-run Doxygen even if the directory exists.
.PHONY: html
html: src/changelog.md src/coc.md src/governance.md
	( cat riot.doxyfile ; echo "GENERATE_HTML = yes" ) | doxygen -
	@echo ""
	@echo "RIOT documentation successfully generated at file://$(RIOTBASE)/doc/doxygen/html/index.html"

.PHONY: check
check: src/changelog.md src/coc.md src/governance.md
	( cat riot.doxyfile) | doxygen -

.PHONY: man
man: src/changelog.md src/coc.md src/governance.md
	( cat riot.doxyfile ; echo "GENERATE_MAN = yes" ) | doxygen -

src/css/riot.css: src/css/riot.less src/css/variables.less
	@$(LESSC) $< $@

src/css/variables.less: src/config.json
	@grep "^\s*\"@" $< | sed -e 's/^\s*"//g' -e 's/":\s*"/: /g' \
	  -e 's/",\?$$/;/g' -e 's/\\"/"/g' > $@

src/changelog.md: src/changelog.md.tmp ../../release-notes.txt
	@./generate-changelog.py $+ $@

src/coc.md: ../../CODE_OF_CONDUCT.md
	awk 'NR == 1 {print $$0,"{#coc}"} NR > 1 {print $$0}' $< > $@

src/governance.md: ../../GOVERNANCE.md
	@sed 's/<!-- TOC start -->/\[TOC\]\n\0/' $< | sed '/<!-- TOC start -->/,/<!-- TOC end -->/d' > $@

.PHONY:
latex: src/changelog.md src/coc.md src/governance.md
	( cat riot.doxyfile ; echo "GENERATE_LATEX= yes" ) | doxygen -

clean:
	-@rm -rf latex man html doxygen_objdb_*.tmp doxygen_entrydb_*.tmp src/changelog.md src/coc.md src/governance.md
