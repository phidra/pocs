#!/bin/bash

trap "echo 'TRAPPED : cette ligne a été exécutée suite à un exit implicite'" EXIT

echo "On laisse le script se terminer de lui-même, et on vérifie que le trap est appelé."
