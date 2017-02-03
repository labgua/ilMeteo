# Installazione lato client

La cartella "MAIN" va posta nella seguente cartella:
/home/pi/


il file "startsht71" gestisce l'autoavvio dello script in python e si trova in :
/etc/init.d/

Qui sono presenti i programmi bash che si avviano al boot del sistema operativo

 - Questo deve essere registrato in update.rc nel seguente modo : 

```bash
#sudo update-rc.d startsht71 defaults
```

 - Deve avere assolutamente avere i privilegi di amministratore poichè avvia il programma in C
   che gestisce la GPIO per l'acquisizione

```bash
#sudo chmod 755 /etc/init.d/startsht71
#sudo chmod 755 /home/pi/MAIN/main.py
#sudo chmod 755 /home/pi/MAIN/sht71
```

Il programma "startsht71" segue le specifiche dei classici servizi del sistema e rimane in background;

 - Può essere chiuso in qualsiasi istante con il seguente comando:
```bash
$sudo /etc/init.d/startsht71 stop
```

 - Può essere avviato con il seguente comando:
```bash
$sudo /etc/init.d/startsht71 start
```