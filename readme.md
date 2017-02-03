# SHT71 : Acquisizione e Trasmissione Real-Time
![Image of Yaktocat](http://labpacinotti.altervista.org/css/logo.png)
## Descrizione Software del progetto
Anno di progettazione : 2012/13 
[Link alla relazione tecnica](http://www.itipacinotti.gov.it/pagine/download/Acquisizione_a.pdf)

## Scopo del progetto
Si vuole realizzare un sistema metereologico su scala nazionale che si in grado di misurare la temperatura e l'umidità. Per fare ciò è necessario poter costruire delle unità che possano essere posizionarte in punti differenti.
Tali informazioni dovranno essere reperibili in tempo reale da qualunque punto ci si trovi e poter essere analizzate per scopi futuri.

## Il Client, il Server e gli Utenti
Il sistema si compone di piu unità, ciascuna di esse deve essere in grado di poter acquisire i parametri salienti e di trasmetterli in rete ad un servrr.
Quindi tutte le unità devono comunicare al client i parametri : tali dati devo essere registrati e reperibili da terzi per poter essere visionati in tempo reale

## Il client : Raspberry-Pi + SHT71
Il client è stato con la scheda RPI attraverso una serie di GPIO comunica con il sensore SHT71, il quale è fornito di una interfaccia I2C con cui è possibile comunicare; i dati acquisiti sono poi normalizzati ed inviati al server grazie alla interfaccia Ethernet ( RJ45 )
Insieme ai dati è spedito anche un identificativo che identifica la scheda, quindi anche la sua posizione.

## Il server : Ricezione ed Accesso
Sul server è presente un endpoint sul quale possono essere spediti i dati, essi vengono registrati su un database associato ad una particolare locazione/paese.
In più attraverso index del server è possibile accedere ad una pagina che permette di selezionare la locazione desiderata e visionare i dati in tempo reale che arrivano dai client.
