BDSIM="bdsim"

# use the loop one so we can test recreation also with a loop
$BDSIM --file=userfile-loop.gmad --outfile=userfile-sample --batch --ngenerate=10 --seed=123
