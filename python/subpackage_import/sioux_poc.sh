#!/bin/bash
########################################################################################################################
# Faisant suite à la standard poc, on montre qu'on peut tout de même rendre accessible les sous-packages depuis un package.
# Pour cela, l'astuce est que le __init__.py du package importé importe le sous-package.
# (et que le __init__.py du sous-package importé importe le module)
########################################################################################################################
#
# Arborescence de la POC :
#         .
#         ├── package_sioux
#         │   ├── subpackage
#         │   │   ├── __init__.py
#         │   │   └── module.py      # contient une classe "MyClass"
#         │   └── __init__.py
#         └── poc.sh
#
########################################################################################################################

declare -a commands
commands[0]="import package_sioux.subpackage.module              ; instance = package_sioux.subpackage.module.MyClass()"
commands[1]="from package_sioux.subpackage import module         ; instance = module.MyClass()"
commands[2]="from package_sioux.subpackage.module import MyClass ; instance = MyClass()"
commands[3]="import package_sioux                                ; instance = package_sioux.subpackage.module.MyClass()"
commands[4]="import package_sioux.subpackage                     ; instance = package_sioux.subpackage.module.MyClass()"
commands[5]="from package_sioux import subpackage                ; instance = subpackage.module.MyClass()"

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
