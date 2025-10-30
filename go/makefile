.PHONY: all clean

ALL_FILES := bu.amocli
ALL_SRC := bu/bu.go

all: ${ALL_FILES}

%.amocli:
	@echo "Building $@ from $*/$*.go"
	@GOOS=darwin GOARCH=arm64 go build -o $@ $*/$*.go
	@echo "Done"

clean:
	rm -f *.amocli

fmt:
	@go fmt ${ALL_SRC}
