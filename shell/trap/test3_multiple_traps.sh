#!/bin/bash

trap "echo 'TRAPPED 1 : ce trap ne sera en fait jamais appelé'" EXIT
trap "echo 'TRAPPED 2 : ce trap ne sera en fait jamais appelé'" EXIT
trap "echo 'TRAPPED 3 : ce trap va être appelé et pas les précédents'" EXIT

echo "On laisse le script se terminer de lui-même, pour voir dans quel ordre les traps sont appelés..."
echo "En pratique, malheureusement (?), seul le dernier trap défini est appelé"
