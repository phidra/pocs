#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
# POC de compression de fichiers avec le module tarfile (et de lecture avec tar -xvzf).
########################################################################################################################

import tarfile

# Création d'une tarball compressée :
with tarfile.open("dst_compressed.tar.gz", "w:gz") as dst:
    dst.add("file_to_compress_1")
    dst.add("file_to_compress_2")
    dst.add("file_to_compress_3")

# Création d'une tarball non-compressée :
with tarfile.open("dst_uncompressed.tar", "w") as dst:
    dst.add("file_to_compress_1")
    dst.add("file_to_compress_2")
    dst.add("file_to_compress_3")

print "Ok pour la création des tarballs..."

########################################################################################################################
#
########################################################################################################################
