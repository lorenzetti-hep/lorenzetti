build:
	docker build --compress --no-cache -t lorenzett/lorenzett:latest .
	docker build --compress  -t lorenzett/lorenzett:base .
push:
	docker push lorenzett/lorenzett:latest
	docker push lorenzett/lorenzett:base
