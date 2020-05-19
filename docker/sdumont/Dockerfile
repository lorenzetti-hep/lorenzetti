FROM lorenzett/lorenzett:latest

LABEL maintainer "Gabriel Gazola Milan <gabriel.gazola@poli.ufrj.br>"

RUN mkdir -p /output

COPY lorenzett_run.sh /lorenzett_run.sh
COPY parser.py /parser.py

ENTRYPOINT ["/lorenzett_run.sh"]
