savedcmd_mutex.mod := printf '%s\n'   mutex.o | awk '!x[$$0]++ { print("./"$$0) }' > mutex.mod
