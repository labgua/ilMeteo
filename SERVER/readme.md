# Installazione lato server

I file contenuti in questa cartella devono essere copiati nella cartella principale del server

Prima di mandarlo in esecuzione, bisogna :

 - sostituire l'indirizzo del file "end-polling.php"
   presente nella funzione "update" della pagina "index.html" 
   con quello in questione
   
(vedi riga 51, index.html)
   
   es.
   
```javascript
//////////////
///	MODULI AJAX per il polling al server
function update( ) {
		var risp;
		$.get("http://www.labpacinotti.altervista.org/end-polling2.php", { nazione: nazione , regione: regione , paese: paese }, function(rsp) { ...
	
```



```javascript
//////////////
///	MODULI AJAX per il polling al server
function update( ) {
		var risp;
		$.get("http://SITOWEB.COM/end-polling2.php", { nazione: nazione , regione: regione , paese: paese }, function(rsp) { ...
	
```