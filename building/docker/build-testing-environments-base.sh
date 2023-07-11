#docker build . -f Dockerfile-centos7-bdsim -t bdsim
# Need following flag for apple silicon
# softwareupdate --install-rosetta
# docker build . -f Dockerfile-centos-bdsim -t bdsim --platform linux/amd64

# unncomment as appropriate
# first one for host architecture x86_64
#SUFFIX=""
# second one for host architecture arm64
SUFFIX=" --platform linux/amd64"

docker build . -f Dockerfile-centos7-bdsim-base-environment -t centos7-base-environment ${SUFFIX}
docker build . -f Dockerfile-centos7-bdsim-base-environment-dt9 -t centos7-base-environment-dt9 ${SUFFIX}
docker build . -f Dockerfile-centos7-bdsim-base-environment-dt9 -t centos7-base-environment-dt9-no-root ${SUFFIX}
