FROM gabrielmilan/lorenzett:latest

LABEL maintainer "Gabriel Gazola Milan <gabriel.gazola@poli.ufrj.br>"

RUN mkdir -p /output
COPY lorenzett_run.sh /lorenzett_run.sh

ENTRYPOINT ["/lorenzett_run.sh"]
