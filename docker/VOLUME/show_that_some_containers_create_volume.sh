#!/bin/bash

BASH="bash:latest"
POSTGIS="mdillon/postgis:9.5"

function call()
{
    echo "===== CALLING :   $@"
    eval "$@"
}


echo ""
echo "This POC shows that running some container (e.g. postgis) creates an implicit volume, but"
echo "not all containers (counter-example = bash) behave that way."
echo ""
echo "WARNING : this pocs leaves some garbage : stopped bash/postgis container, and a dangling volume"
echo "          please confirm you want to run POC by hitting ENTER, or leave with Ctrl+C"
read
# NOTE : can't use --rm option, or volume will be cleaned up after run...

echo ""
echo "STEP 1 : show that some images have volume, some don't :"
call "docker history $BASH | grep VOLUME"
call "docker history $POSTGIS | grep VOLUME"


echo ""
echo "STEP 2 : number of volumes before POCS :"
nb_volumes_before="$(docker volume list --quiet|wc -l)"
call "docker volume list --quiet|wc -l"


echo ""
echo "STEP 3 : showing that running a bash container don't add a volume :"
call "docker run $BASH"
call "docker volume list --quiet|wc -l"


echo ""
echo "STEP 4 : showing that running a postgis container adds a new volume :"
call "docker run  $POSTGIS 2> /dev/null"
call "docker volume list --quiet|wc -l"

echo ""
echo "You might want to cleanup unused containers and volumes"
