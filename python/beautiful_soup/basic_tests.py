#!/usr/bin/env python
# -*- coding: utf-8 -*-
########################################################################################################################
#
########################################################################################################################

import bs4
 

# Recherche de tous les paragraphes (ATTENTION : il n'est pas robuste aux espaces dans les balises, et ne trouvera pas < p >) :
soup1 = bs4.BeautifulSoup(open("sample1.html"))
paragraphs = soup1.find_all("p")
print "PARAGRAPHES :"
for p in paragraphs:
    print "\t{p}".format(p=p)

# Recherche de tous les objets d'une classe donnée :
soup2 = bs4.BeautifulSoup(open("sample2.html"))
class_A_objects = soup2.find_all(class_="A")
print "OBJETS de classe A :"
for p in class_A_objects:
    print "\t{p}".format(p=p)


# Recherche de tous les objets d'une classe donnée, et affichage de leur id :
soup3 = bs4.BeautifulSoup(open("sample3.html"))
class_A_objects = soup3.find_all(class_="A")
print "OBJETS de classe A, et leur ID :"
for p in class_A_objects:
    print "\t[ID={id:10}] {p}".format(id=p.get("id"), p=p)

# ticket:405  =  affichage de l'identifiant de chaque jarvis widget d'un template django :
soup4 = bs4.BeautifulSoup(open("sample4.html"))
jarviswidgets = soup4.find_all(class_="jarviswidget")
print "IDENTIFIANTS des jarviswidgets :"
for j in jarviswidgets:
        print "\t[{id}]".format(id=j.get("id"))

# Récupération d'un élément par son ID :
soup5 = bs4.BeautifulSoup(open("sample5.html"))
myparagraph = soup5.find(id="pouet")
print "ELEMENT D'ID pouet :"
print "\t[{}]".format(myparagraph)

# Affichage du contenu HTML d'un élément :
print ".contents DE L'ELEMENT D'ID pouet :"
print "\t[{}]".format(myparagraph.contents)

# Affichage du contenu HTML d'un élément :
print ".children DE L'ELEMENT D'ID pouet :"
for i in myparagraph.children:
    print "\t[{}]".format(i)

# Affichage du contenu textuel d'un élément :
print "CONTENU TEXTUEL DE L'ELEMENT D'ID pouet :"
print "\t[{}]".format(myparagraph.get_text())

# Lorsqu'on recherche un élément inexistant, il renvoie None :
inexisting = soup5.find(id="inexistingid")
print "ELEMENT INEXISTANT :"
print "\t[{}]".format(inexisting)
assert(inexisting is None)


########################################################################################################################
# Fin du fichier.
########################################################################################################################


