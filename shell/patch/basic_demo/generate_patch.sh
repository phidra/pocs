#!/bin/bash
########################################################################################################################
# POC sur le patching :
# Dans une première partie, on génère le patch entre deux fichiers du répertoire de la POC.
# Dans une seconde partie, on applique le patch à une copie du fichier du répertoire de la POC.
########################################################################################################################


here=$(pwd)
poc_tmp="poc_execution_dir"
rm -rf "${poc_tmp}"

# Fichiers à comparer :
container_dir="files_for_the_poc"
with_errors_basename="liste_AVEC_erreurs"
without_errors_basename="liste_SANS_erreurs"

# Fichier de destination du patch :
patch_file="${here}/correction.patch"
rm -f "${patch_file}"

########################################################################################################################
# PREMIÈRE PARTIE : génération du patch :
########################################################################################################################

echo ""
echo "===== On va créer un patch entre les fichiers :"
echo -e "\t${container_dir}/${with_errors_basename}"
echo -e "\t${container_dir}/${without_errors_basename}"
echo ""
echo "===== Le fichier de patch créé va être :"
echo -e "\t$(basename "${patch_file}" )"
echo ""

########################################################################################################################

# Attention à se placer à un endroit indépendant de la localisation des fichiers à patcher :
pushd "files_for_the_poc" &> /dev/null || exit

echo "====== Génération du patch :"
echo -e "\tdiff -ruN liste_AVEC_erreurs liste_SANS_erreurs > \"${patch_file}\""
diff -ruN liste_AVEC_erreurs liste_SANS_erreurs > "${patch_file}"
status=$?

popd &> /dev/null || exit

########################################################################################################################

# En cas d'erreur, le statut de diff est supérieur à 1 :
if [ "$status" == "0" ] || [ "$status" == "1" ]
then
    echo "===== [SUCCESS] Contenu du patch généré :"
    echo ""
    cat "${patch_file}"
else
    echo "===== [FAILURE] diff returncode = $status"
    exit
fi

########################################################################################################################
# DEUXIÈME PARTIE : application du patch :
########################################################################################################################

# On part du fichier avec des erreurs :
mkdir "${poc_tmp}"
pushd "${poc_tmp}" &> /dev/null || exit
cp "../files_for_the_poc/liste_AVEC_erreurs" . # Le fichier à patcher doit avoir le même nom !

echo ""
echo "===== Contenu du fichier AVANT application du patch :"
cat "${with_errors_basename}"
echo ""

echo "ICI = $(pwd)"

echo "===== Application du patch :"
echo -e "\tpatch -p0 -N -i ../correction.patch"
patch -p0 -N -i "${patch_file}"
echo ""

echo "===== Contenu du fichier APRÈS application du patch :"
cat "${with_errors_basename}"
echo ""

popd &> /dev/null || exit

########################################################################################################################
# RÉSUMÉ :
########################################################################################################################
echo ""
echo "===== En résumé, et en faisant bien attention aux chemins (qui doivent rester les mêmes entre la création du patch et son application)"
echo -e "\tdiff  -ruN  AVANT_PATCH  APRES_PATCH   >   PATCH"
echo -e "\tpatch  -p0  -N  -i  PATCH"


########################################################################################################################
# Fin du fichier.
########################################################################################################################
