#!/bin/bash

trap "echo 'TRAPPED : cette ligne a été exécutée suite à un exit prématuré'" EXIT

echo "On s'apprête à exit 0, ce qui triggera le trap."
exit 0
echo "Cette ligne ne sera jamais exécutée."
