#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail


cat << EOF

POC pour démontrer l'utilisation de puppeteer pour tester une application web.

HOWTO :
    dans un terminal 1 :
        npm install
        npm run dev
    dans un terminal 2 :
        npm run test

NOTE : j'ai parfois un bug quand je lance puppeteer :
    reject(new Error([
           ^
    Error: Failed to launch the browser process! undefined
    [33935:33935:0426/213934.421624:ERROR:ozone_platform_x11.cc(239)] Missing X server or \$DISPLAY
    [33935:33935:0426/213934.421659:ERROR:env.cc(255)] The platform failed to initialize.  Exiting.
    ----------------------------------------
    TROUBLESHOOTING: https://pptr.dev/troubleshooting
        at ChildProcess.onClose (/path/to/node_modules/@puppeteer/browsers/lib/cjs/launch.js:262:24)
        at ChildProcess.emit (node:events:525:35)
        at ChildProcess._handle.onexit (node:internal/child_process:291:12)
    Node.js v18.16.0


SOLUTION = forcer l'envvar DISPLAY (qui disparaît mystérieusement) :
    Ouvrir un autre terminal, pour regarder la valeur de DISPLAY :
        echo \$DISPLAY
        :0.0
    Dans le terminal qui bugge, setter le display :
        export DISPLAY=:0.0
EOF
