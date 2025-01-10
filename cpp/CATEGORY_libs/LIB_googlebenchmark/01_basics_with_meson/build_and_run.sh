#!/bin/bash

set -o errexit
set -o nounset
set -o pipefail

rm -rf _build_dir
rm -rf subprojects  # le nom est imposé par le package-manager de meson (=wrap)

echo ""
echo "=============== SETUP GOOGLE BENCHMARK DEPENDENCY BEGIN"
mkdir -p subprojects/packagefiles

# fichier meson-wrap pour google-benchmark, identique à celui obtenu par :
#      meson wrap install google-benchmark
# à l'exception de la ligne diff_files utilisant le patch pour désactiver les tests
cat << EOF > subprojects/google-benchmark.wrap
[wrap-file]
directory = benchmark-1.7.1
source_url = https://github.com/google/benchmark/archive/refs/tags/v1.7.1.tar.gz
source_filename = benchmark-1.7.1.tar.gz
source_hash = 6430e4092653380d9dc4ccb45a1e2dc9259d581f4866dc0759713126056bc1d7
patch_filename = google-benchmark_1.7.1-2_patch.zip
patch_url = https://wrapdb.mesonbuild.com/v2/google-benchmark_1.7.1-2/get_patch
patch_hash = 169304b7095ed39f2a260c84fe3fe368124a5a9ab66e529f11c71c9907fc4085
source_fallback_url = https://github.com/mesonbuild/wrapdb/releases/download/google-benchmark_1.7.1-2/benchmark-1.7.1.tar.gz
wrapdb_version = 1.7.1-2

# NDM : cette ligne a été ajoutée par moi, pour ne pas exécuter les tests meson quand je teste mon application :
diff_files = disable_google_benchmark_tests.patch

[provide]
benchmark = google_benchmark_dep
EOF

# patch pour désactiver les tests :
cat << EOF > subprojects/packagefiles/disable_google_benchmark_tests.patch
diff --color -ruN benchmark-1.7.1/meson.build benchmark-1.7.1_BIS/meson.build
--- benchmark-1.7.1/meson.build	2024-01-09 12:12:35.810861437 +0100
+++ benchmark-1.7.1_BIS/meson.build	2024-01-09 12:14:05.973690466 +0100
@@ -81,5 +81,3 @@
   link_with: lib_benchmark_main,
   include_directories: inc,
 )
-
-subdir('test')
EOF

echo "=============== SETUP GOOGLE BENCHMARK DEPENDENCY END"
echo ""

echo ""
echo "=============== CONFIGURE BEGIN"
meson setup _build_dir/ .
echo "=============== CONFIGURE END"
echo ""

echo ""
echo "=============== BUILD BEGIN"
meson compile -C _build_dir/
echo "=============== BUILD END"
echo ""


echo ""
echo "=============== TEST EXECUTION THROUGH MESON BEGIN"
meson test -C _build_dir/
echo "=============== TEST EXECUTION THROUGH MESON END"
echo ""

echo ""
echo "=============== MANUAL TEST EXECUTION BEGIN"
_build_dir/my_super_test
echo "=============== MANUAL TEST EXECUTION END"
echo ""




cat << EOF

Cette POC montre l'utilisation de google-benchmark avec meson.
Elle est aussi une démonstration de comment patcher une dépendance installée avec meson.
Ici, je modifier google-benchmark pour ne pas exécuter les tests de la lib quand j'exécute mes propres tests.

EOF

