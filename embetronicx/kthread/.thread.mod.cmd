savedcmd_thread.mod := printf '%s\n'   thread.o | awk '!x[$$0]++ { print("./"$$0) }' > thread.mod
