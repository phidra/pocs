#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

cat << EOF

Cette POC est simplement le support du suivi de ce tutorial sur typescript :
    https://www.typescripttutorial.net/typescript-tutorial/typescript-hello-world/

Ici, on se borne à compiler puis exécuter une app typescript, de deux façons :
    avec tsc = le compilo typescript
    avec ts-node = une extension node qui fait la compilation puis l'exécution

EOF

rm -f NOGIT_app.js

set -o xtrace

# STEP 0 = content of app.ts :
cat app.ts

# STEP 1 = compile from typescript to javascript :
tsc app.ts --outFile NOGIT_app.js

# STEP 2 = run app with node :
node NOGIT_app.js

# STEP 3 = all-in-one with ts-node :
ts-node app.ts
