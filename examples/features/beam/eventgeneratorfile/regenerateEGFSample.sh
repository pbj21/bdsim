


crmc -o hepmc -s 123 -n 4 -m 6 -p 100 -P -100 -f egf-hepmc2-4pp-100gev-evts.dat


# on lxplus

# source /cvmfs/beam-physics.cern.ch/bdsim/x86_64-centos7-gcc11-opt/bdsim-env-_prod_20221105-g4v11.0.3-boost.sh
# crmc -o hepmc -s 123 -n 4 -m 6 -p 100 -P -100 -f egf-hepmc2-4pp-100gev-evts.dat --config $CRMC__HOME/etc/crmc.param
# seed=123, 4 events, model 6, 100gev -100gev, output file, general config file as it doesn't find it normally
