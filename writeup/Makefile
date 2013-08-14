PDF=pdflatex
BIB=bibtex
INPUT=main
OUTPUT=WheresWallyDissertation-CianBooth
OUTDIR=pdf
SUBFOLDER=tex
SUBFILES=$(patsubst $(SUBFOLDER)/%.tex,%,$(wildcard $(SUBFOLDER)/*.tex))
ENV=TEXINPUTS=.:./tex:..:
HASBIB=1
main:
	$(ENV) $(PDF) -draftmode --output-directory $(OUTDIR) -jobname $(OUTPUT) $(INPUT)
	$(ENV) $(PDF) -draftmode --output-directory $(OUTDIR) -jobname $(OUTPUT) $(INPUT)
	$(ENV) $(BIB) $(OUTDIR)/$(OUTPUT)
	$(ENV) $(PDF) --output-directory $(OUTDIR) -jobname $(OUTPUT) $(INPUT)

quick_%: $(SUBFOLDER)/%.tex
	$(ENV) $(PDF) --output-directory $(SUBFOLDER)/$(OUTDIR) $(patsubst quick_%,%,$@)

%: $(SUBFOLDER)/%.tex
	$(ENV) $(PDF) -draftmode --output-directory $(OUTDIR) -jobname $@ $(SUBFOLDER)/$@
	grep -q biblio $(SUBFOLDER)/$@.tex && \
	  $(ENV) $(PDF) -draftmode --output-directory $(OUTDIR) -jobname $@ $(SUBFOLDER)/$@ &&\
    $(ENV) $(BIB) $(OUTDIR)/$@ || true
	$(ENV) $(PDF) --output-directory $(OUTDIR) -jobname $@ $(SUBFOLDER)/$@


subfiles: $(SUBFILES)

clean:
	@rm $(OUTPUT).{pdf,aux,log,blg,bbl}
