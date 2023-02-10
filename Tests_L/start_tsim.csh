PROJECT=ProjetSE
BIN=$HOME/sysemb

export PROJECT
export BIN

$BIN/tsim-eval/tsim/linux-x64/tsim-leon3 -c $HOME/eclipse-workspace/$PROJECT/Tests_L/tsim_init &

$BIN/bcc-2.2.0-gcc/bin/sparc-gaisler-elf-gdb $HOME/eclipse-workspace/$PROJECT/Debug/$PROJECT -d $HOME/eclipse-workspace/$PROJECT/src -batch -x $HOME/eclipse-workspace/$PROJECT/Tests_L/gdb_batch_001.txt


