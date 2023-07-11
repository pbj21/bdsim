#docker build . -f Dockerfile-centos7-bdsim -t bdsim
# Need following flag for apple silicon
# softwareupdate --install-rosetta
# docker build . -f Dockerfile-centos-bdsim -t bdsim --platform linux/amd64

# unncomment as appropriate
# first one for host architecture x86_64
#SUFFIX=""
# second one for host architecture arm64
SUFFIX=" --platform linux/amd64"

docker build . -f Dockerfile-centos7-bdsim-geant4v10.1.3-environment -t centos7-geant4v10.1.3-environment ${SUFFIX}
docker build . -f Dockerfile-centos7-bdsim-geant4v10.2.3-environment -t centos7-geant4v10.2.3-environment ${SUFFIX}
docker build . -f Dockerfile-centos7-bdsim-geant4v10.3.3-environment -t centos7-geant4v10.3.3-environment ${SUFFIX}
docker build . -f Dockerfile-centos7-bdsim-geant4v10.4.3-environment -t centos7-geant4v10.4.3-environment ${SUFFIX}
docker build . -f Dockerfile-centos7-bdsim-geant4v10.5.1-environment -t centos7-geant4v10.5.1-environment ${SUFFIX}
docker build . -f Dockerfile-centos7-bdsim-geant4v10.6.3-environment -t centos7-geant4v10.6.3-environment ${SUFFIX}
docker build . -f Dockerfile-centos7-bdsim-geant4v10.7.4-environment -t centos7-geant4v10.7.4-environment ${SUFFIX}
docker build . -f Dockerfile-centos7-bdsim-geant4v10.7.2.3-jai-environment -t centos7-geant4v10.7.2.3-jai-environment ${SUFFIX}
docker build . -f Dockerfile-centos7-bdsim-geant4v11.0.4-environment -t centos7-geant4v11.0.4-environment ${SUFFIX}
docker build . -f Dockerfile-centos7-bdsim-geant4v11.1.1-environment -t centos7-geant4v11.1.1-environment ${SUFFIX}

