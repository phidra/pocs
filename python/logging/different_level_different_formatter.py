#!/usr/bin/python
# -*- coding: utf-8 -*-
################################################################################################
#
# Démonstration du moyen d'attribuer des formatters différents en fonction du niveau.
# cf. http://stackoverflow.com/questions/1343227/can-pythons-logging-format-be-modified-depending-on-the-message-log-level
#
# Pour la partie sur les ContextManager, cf.  http://sametmax.com/les-context-managers-et-le-mot-cle-with-en-python/
#
################################################################################################

import logging

logger = logging.getLogger()
logger.setLevel(logging.DEBUG)


# Formatter se comportant différement selon les niveaux :
class MySpecificFormatter(logging.Formatter):
    """ Formatter spécifique (pour afficher différemment les erreurs)."""

    # Context Manager pour conserver le format du formatter :
    class FormatKeeper:
        def __init__(me, formatter):
            me.__formatter = formatter
            me.__format_at_entrance = me.__formatter._fmt
        def __enter__(me):
            me.__format_at_entrance = me.__formatter._fmt
        def __exit__(me, type, value, traceback):
            me.__formatter._fmt = me.__format_at_entrance
    
    def __init__(self, common_format, error_format):
        self.__common_format = common_format
        self.__error_format = error_format
        logging.Formatter.__init__(self, self.__common_format)

    def format(self, record):
        to_return = ""
        with self.FormatKeeper(self):
            if record.levelno == logging.ERROR:
                self._fmt = self.__error_format
            to_return = logging.Formatter.format(self, record)
        return to_return

# Handler = StreamHandler
stream_handler = logging.StreamHandler()
stream_handler.setLevel(logging.DEBUG)
stream_handler.setFormatter(MySpecificFormatter(common_format = "%(message)s", error_format = "ERROR - [%(message)s]"))
logger.addHandler(stream_handler)

# Code de test :
logger.info('Hello')
logger.warning('Testing %s', 'foo')
logger.error('IO error !')
logger.info('Back to normality...')


################################################################################################
# FIN DU FICHIER
################################################################################################


