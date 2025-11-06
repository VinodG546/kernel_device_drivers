savedcmd_major_minor.mod := printf '%s\n'   major_minor.o | awk '!x[$$0]++ { print("./"$$0) }' > major_minor.mod
