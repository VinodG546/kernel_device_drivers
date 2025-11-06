savedcmd_dynamic_create.mod := printf '%s\n'   dynamic_create.o | awk '!x[$$0]++ { print("./"$$0) }' > dynamic_create.mod
