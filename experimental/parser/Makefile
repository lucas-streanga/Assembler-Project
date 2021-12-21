ASM_PARSER = asm_parser

LEX    = flex
YACC   = bison

INCDIR = -I ../include

all : $(ASM_PARSER).tab.o $(ASM_PARSER).yy.o

$(ASM_PARSER).tab.c $(ASM_PARSER).tab.h : $(ASM_PARSER).y $(ASM_PARSER).yy.h
	bison --report=all --xml $(ASM_PARSER).y
	xsltproc /usr/share/bison/xslt/xml2xhtml.xsl $(ASM_PARSER).xml > $(ASM_PARSER).html
	rm -f $*.xml

$(ASM_PARSER).yy.c $(ASM_PARSER).yy.h : $(ASM_PARSER).l
	flex -o $(ASM_PARSER).yy.c $(ASM_PARSER).l

$(ASM_PARSER).tab.o: $(ASM_PARSER).tab.c $(ASM_PARSER).tab.h
	g++ $(CFLAGS) -c $<

$(ASM_PARSER).yy.o: $(ASM_PARSER).yy.c $(ASM_PARSER).yy.h
	g++ $(CFLAGS) -c $<

cleanup:
	rm -f *.yy.*
	rm -f *.tab.*
	rm -f *.output
	#rm -f *.hh
	rm -f *.o
	rm -f *xml
