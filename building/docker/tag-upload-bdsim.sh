
docker image tag centos7-bdsim-geant4v10.7.2.3-jai-v1.7.0 jairhul/centos7-bdsim-geant4v10.7.2.3-jai:v1.7.0
docker image tag centos7-bdsim-geant4v10.7.2.3-jai-develop jairhul/centos7-bdsim-geant4v10.7.2.3-jai-develop:latest

#docker image tag centos7-bdsim-geant4v10.7.2.3-jai-v1.7.0 jairhul/bdsim:v1.7.0-rc

docker image push jairhul/centos7-bdsim-geant4v10.7.2.3-jai:v1.7.0
docker image push jairhul/centos7-bdsim-geant4v10.7.2.3-jai-develop:latest

docker image push jairhul/bdsim:v1.7.0
