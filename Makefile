
all: bin/bsv-parser-cpp classes/Main.class

JARS = jars/antlr-4.7.1-complete.jar

test: classes/Main.class bin/bsv-parser-cpp connectal
	./bsvparse tests/*.bsv
	./bsvparse connectal/bsv/*.bsv

classes/Main.class: java/Main.java generated/BSVParser.java generated/BSVLexer.java java/BSVToKami.java java/StaticAnalysis.java java/SymbolTable.java $(JARS)
	mkdir -p classes
	javac -d classes -classpath classes:$(JARS) java/*.java generated/*.java

generated/BSVParser.java: BSV.g4 $(JARS)
	java -jar $(JARS) -listener -visitor -o generated BSV.g4

generated/BSVParser.cpp: BSV.g4 $(JARS)
	java -jar $(JARS)  -Dlanguage=Cpp -listener -visitor -o generated BSV.g4

bin/bsv-parser-cpp: cpp/main.cpp generated/BSVParser.cpp antlr4-cpp-runtime/dist/libantlr4-runtime.a
	mkdir -p bin
	$(CXX) -O -Wall -std=c++11 -Igenerated -Iantlr4-cpp-runtime/runtime/src/ -o bin/bsv-parser-cpp cpp/main.cpp generated/*.cpp antlr4-cpp-runtime/dist/libantlr4-runtime.a

antlr4-cpp-runtime: antlr4-cpp-runtime-4.7.1-source.zip
	rm -fr antlr4-cpp-runtime/*
	mkdir -p antlr4-cpp-runtime; cd antlr4-cpp-runtime; unzip ../antlr4-cpp-runtime-4.7.1-source.zip

antlr4-cpp-runtime/dist/libantlr4-runtime.a: antlr4-cpp-runtime
	mkdir -p build-antlr4; cd build-antlr4; cmake ../antlr4-cpp-runtime; make -j4

antlr4-cpp-runtime-4.7.1-source.zip:
	curl http://www.antlr.org/download/antlr4-cpp-runtime-4.7.1-source.zip > antlr4-cpp-runtime-4.7.1-source.zip

$(JARS):
	mkdir -p jars
	curl http://www.antlr.org/download/antlr-4.7.1-complete.jar > jars/antlr-4.7.1-complete.jar

connectal:
	git clone --depth=1 git://github.com/cambridgehackers/connectal

clean:
	rm -fr classes bin
