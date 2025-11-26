.PHONY: all clean

ALL_FILES := bu.amocli fcomb.amocli
ALL_SRC := bu/bu.go fcomb/fcomb.go

OS := darwin
ARCH := arm64

all: ${ALL_FILES}

%.amocli:
	@echo "Building $@ from $*/$*.go"
	@GOOS=${OS} GOARCH=${ARCH} go build -o $@ $*/$*.go
	@echo "Done"

clean:
	rm -f *.amocli

fmt:
	@go fmt ./...
