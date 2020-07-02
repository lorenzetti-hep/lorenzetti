FROM lorenzetti/lorenzetti:base
SHELL ["/bin/bash", "-c"]
LABEL maintainer "Joao Victor Pinto <jodafons@lps.ufrj.br>"
COPY setup_here.sh /setup_here.sh
COPY parser.py /parser.py
ENTRYPOINT ["/parser.py"]
