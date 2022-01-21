all: build

IMAGE_TAG?=scylladb:focal.00

build:
	docker build --rm --pull --no-cache -t $(IMAGE_TAG) \
		--build-arg IMAGE_REVISION=$(IMAGE_REVISION) \
		./
