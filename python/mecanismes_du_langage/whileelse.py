#!/usr/bin/env python
# coding: utf-8

# OBJECTIF DE LA POC = tester la syntaxe while...else
# En résumé, la clause else, si elle est exécutée, l'est APRÈS la boucle while.
# Elle EST EXÉCUTÉE si on sort de la boucle while parce que la condition est devenue fausse (cf. cas 1)
# Elle N'EST PAS EXÉCUTÉE si on sort de la boucle while à cause d'un break (cf. cas 2)
# cf. https://docs.python.org/2/reference/compound_stmts.html#while


# CAS 1 = sortir de la boucle while à cause de la condition invalide EXÉCUTE la clause else :
print ""
print u"CAS 1 = sortir de la boucle while à cause de la condition invalide EXÉCUTE la clause else :"
i = 0
while i < 5:
    i += 1
    print "Incrementing i, which is now", i
else:
    print "EXECUTING ELSE CLAUSE !"


# CAS 2 = sortir de la boucle while à cause d'une instruction break N'EXÉCUTE PAS la clause else :
print ""
print u"CAS 2 = sortir de la boucle while à cause d'une instruction break N'EXÉCUTE PAS la clause else :"
j = 0
while True:
    j += 1
    print "Incrementing j, which is now", j
    if j >= 5:
        break
else:
    print "EXECUTING ELSE CLAUSE !"
