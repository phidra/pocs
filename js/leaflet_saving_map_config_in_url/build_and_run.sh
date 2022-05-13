#!/bin/bash

echo ""
echo "Ce script lance un serveur pour servir la POC leaflet (nécessaire, car j'utilise des modules, ce qui pose des problèmes CORS)"
echo "Derrière, il faut charger l'URL suivante dans un browser :"
echo ""
echo "     http://localhost:8000/index.html"
echo ""
python -m SimpleHTTPServer
