
all: build

build:
	docker build --network host --compress -t lorenzetti/lorenzetti:latest .
	docker build --network host --compress -t lorenzetti/lorenzetti:base .
push:
	docker push lorenzetti/lorenzetti:latest
	docker push lorenzetti/lorenzetti:base
run:
	docker run --network host -v ${PWD}:${HOME} -it lorenzetti/lorenzetti:base
clean:
	docker system prune -a
	