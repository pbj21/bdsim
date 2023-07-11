
docker image tag centos7-base-environment jairhul/centos7-base-environment:latest
docker image tag centos7-base-environment-dt9 jairhul/centos7-base-environment-dt9:latest
docker image tag centos7-base-environment-dt9 jairhul/centos7-base-environment-dt9-no-root:latest

docker image push jairhul/centos7-base-environment:latest
docker image push jairhul/centos7-base-environment-dt9:latest
docker image push jairhul/centos7-base-environment-dt9-no-root:latest
