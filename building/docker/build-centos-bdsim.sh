docker build . -f Dockerfile-centos-bdsim -t bdsim

# Need following flag for apple silicon
# softwareupdate --install-rosetta
# docker build . -f Dockerfile-centos-bdsim -t bdsim --platform linux/amd64