#!/bin/bash
########################################################################################################################
# POC pour montrer qu'on ne peut pas importer un sous-package depuis un package importé.
#
# Dit autrement : lorsqu'on importe un objet (module, package), on ne peut accéder qu'à ce qu'il contient directement.
# (on ne peut pas utiliser l'un de ses sous-packages)
#
# L'idée est donc de montrer qu'on NE PEUT PAS FAIRE :
#
#       from package import subpackage
#       instance = subpackage.module.MyClass()   # module n'est pas un élément direct de subpackage
#
# cf. aussi : http://stackoverflow.com/questions/12229580/python-importing-a-sub-package-or-sub-module
########################################################################################################################
#
# Arborescence de la POC :
#         .
#         ├── package
#         │   ├── subpackage
#         │   │   ├── __init__.py
#         │   │   └── module.py      # contient une classe "MyClass"
#         │   └── __init__.py
#         └── poc.sh
#
########################################################################################################################

declare -a commands
commands[0]="import package.subpackage.module              ; instance = package.subpackage.module.MyClass()"
commands[1]="from package.subpackage import module         ; instance = module.MyClass()"
commands[2]="from package.subpackage.module import MyClass ; instance = MyClass()"
commands[3]="import package                                ; instance = package.subpackage.module.MyClass()"
commands[4]="import package.subpackage                     ; instance = package.subpackage.module.MyClass()"
commands[5]="from package import subpackage                ; instance = subpackage.module.MyClass()"

length="${#commands[@]}"

for i in $(seq 0 $((length-1)) )
do
    command_=${commands[$i]}
    echo ""
    echo ""
    echo "========================================================================================================================"
    echo "${command_}"
    echo "========================================================================================================================"
    python -c "${command_}"
    echo "========================================================================================================================"
done

########################################################################################################################
# Fin du fichier.
########################################################################################################################
