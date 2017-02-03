#-------------------------------------------------------------------------------
# Name:        module1
# Purpose:
#
# Author:      Guastaferro
#
# Created:     10/01/2013
# Copyright:   (c) Guastaferro 2013
# Licence:     <your licence>
#-------------------------------------------------------------------------------
#!/usr/bin/env python

#				DEFINIZIONE DEI MODULI	
import subprocess      # MODULO PER RICHIAMARE IL PROGRAMMA IN C
import sys             # MODULO ACCESSO LIBRERIE DI SISTEMA
import time            # MODULO LIBRERIA DI TEMPORIZZAZIONE

import re              # MODULO REGULAR EXPRESSION (ricerca in stringa)
import string
	
import urllib
	
id = "italia#campania#scafati"
while(True):
	output = subprocess.Popen(["/home/pi/MAIN/sht71"], stdout=subprocess.PIPE).communicate()[0]
	#output = "48.07&20.62"
	
	print output

	
	buffer = output.split("&")
	
	tc = buffer[1]
	rh = buffer[0]
		
		
	values=urllib.urlencode({'id':id, 't':tc, 'u':rh})
	path='http://www.labpacinotti.altervista.org/upload.php'
	url = path + '?' + values

	
	try:
		result = urllib.urlopen(url)
		print result.read()
		pass
	except IOError:
		print('Errore di Rete')
		pass
	#time.sleep(1)
	
