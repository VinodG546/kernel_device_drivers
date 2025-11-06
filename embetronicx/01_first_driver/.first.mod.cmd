savedcmd_first.mod := printf '%s\n'   first.o | awk '!x[$$0]++ { print("./"$$0) }' > first.mod
