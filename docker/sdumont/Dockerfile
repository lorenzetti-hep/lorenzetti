FROM lorenzetti/lorenzetti:latest

LABEL maintainer "Gabriel Gazola Milan <gabriel.gazola@poli.ufrj.br>"

RUN mkdir -p /output

COPY lorenzetti_run.sh /lorenzetti_run.sh
COPY parser.py /parser.py

ENTRYPOINT ["/lorenzetti_run.sh"]
