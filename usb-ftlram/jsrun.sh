#!/usr/bin/env sh

#jsmon -batch $1
# or use this
printf "reset\nload $1\nrun\nquit\n" | jsmon
